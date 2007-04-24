
//-- MHD Headers
#include "H5Lite.h"
#include "H5Image.h"
#include "H5TiffIO.h"


//-- STL Headers
#include <iostream>
#include <math.h>


H5TiffIO::H5TiffIO(hid_t fileId) :
  _fileId(fileId)
{
}


// -------------------------------------------------------------------------
// Read the tiff file
// -------------------------------------------------------------------------
herr_t H5TiffIO::importTiff(string filename, hid_t groupId, 
			     string datasetName, bool asGrayscale)   
{
  TIFF *in;
  herr_t err = 0;
  
  in = TIFFOpen(filename.c_str(), "r");
  if (in == NULL) {
    std::cout << "Error Opening Tiff file with Absolute Path:\n    " 
	      << filename << std::endl;
    return (herr_t) -1;
  }

  if (asGrayscale) {
    err = _readGrayscaleTiff(in, groupId, datasetName);
  } else {
    int imageClass = _determineTiffImageClass(in);

    switch (imageClass) {
    case GrayscaleTiffImage:
      err = _readGrayscaleTiff(in, groupId, datasetName);
      break;
    case PaletteColorTiffImage:  
      err = _read8BitTiff( in, groupId, datasetName);
      break;
    case RGBFullColorTiffImage:
      err = _read24BitTiff(in, groupId, datasetName);
      break;
    default:
      std::cout << "importTiff ERROR: Unsupported TIFF image type:" 
		<< imageClass << std::endl;
      err = -1;
    }
  }

  (void) TIFFClose(in);
  return err;
}
    


// ---------------------------------------------------------------------
//  determines the TIFF Image Class type
// ---------------------------------------------------------------------
int H5TiffIO::_determineTiffImageClass(TIFF *in)
{
  uint16 samplesperpixel = 0;
  uint16 bitspersample = 0;
  uint16 compression = 0;
  uint16 photometric = 0;
    
  // Get determining fields
  TIFFGetField(in, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
  TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bitspersample);
  TIFFGetField(in, TIFFTAG_COMPRESSION, &compression);
  TIFFGetField(in, TIFFTAG_PHOTOMETRIC, &photometric);

  std::cout << "TIFF Image Field Values: " << std::endl;
  std::cout << "  SamplesPerPixel: " << samplesperpixel << std::endl;
  std::cout << "  BitsPerSample:   " << bitspersample << std::endl;
  std::cout << "  Compression:     " << compression << std::endl;
  std::cout << "  Photometric:     " << photometric << std::endl;

  if (photometric == 6) {
    return YCbCrTiffImage;
  } else if (photometric == 5) {
    return SeparatedTiffImage;
  } else if (compression == 3 || compression == 4) {
    return FacsimileTiffImage;
  } else if (photometric == 3) {
    return PaletteColorTiffImage;
  } else if (photometric == 2) {
    return RGBFullColorTiffImage;
  } else if (bitspersample == 1) {
    return BilevelTiffImage;
  } else if (samplesperpixel == 1) {
    return GrayscaleTiffImage;
  }
  return UnknownTiffImage;
}



// ---------------------------------------------------------------------
// Reads a grayscale image into an HDF5 Image Dataset
// ---------------------------------------------------------------------
herr_t H5TiffIO::_readGrayscaleTiff(TIFF *in, hid_t groupId, 
				    string &datasetName)
{
  uint32* raster;     /* retrieve RGBA image */
  uint32  width, height;    /* image width & height */
  herr_t err = 0;
  
  std::cout << "Importing grayscale tiff image" << std::endl;
  
  TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);

  raster = (uint32*)_TIFFmalloc(width * height * sizeof (uint32));
  if (raster == 0) {
    TIFFError(TIFFFileName(in), "No space for raster buffer");
    return (-1);
  }
  
  // TIFFReadRGBAImageOriented converts non 8bit images including:
  //  Grayscale, bilevel, CMYK, and YCbCR transparently into 32bit RGBA
  //  samples
  
  /* Read the image in one chunk into an RGBA array */
  if (!TIFFReadRGBAImageOriented(in, width, height, raster,
				 ORIENTATION_TOPLEFT, 0)) {
    _TIFFfree(raster);
    return (-1);
  }
  
  // Collapse the data down to a single channel, that will end up
  //  being the grayscale values
  int pixel_count = width * height;
  unsigned char *src, *dst;
  
  src = (unsigned char *) raster;
  dst = (unsigned char *) raster;
  while( pixel_count > 0 ) {
    *(dst++) = *(src);        
    src += 4; //skip ahead 4 bytes
    pixel_count--;
  }
  
  // Store byte array to HDF5 File
  err = H5Image::H5IMmake_image_8bit(groupId, datasetName, width, height, (unsigned char *)raster );
  if (err<0) {
    std::cout << "Error storing Image data with H5IM API:  datasetName: "
	      << datasetName << std::endl; 
  }
  
  // Need to update the attributes to be correct for a grayscale image
  H5Lite::writeAttribute(groupId, 
                         datasetName, 
                         const_cast<std::string&>(H5ImageConst::ImageSubclass), 
                         const_cast<std::string&>(H5ImageConst::ImageGrayScale) );
 // H5LTset_attribute_string(groupId, datasetName.c_str(), H5ImageConst::ImageSubclass, H5ImageConst::ImageGrayScale);
  uint32 white_is_zero = 0;
  H5Lite::writeAttribute(groupId, 
                         datasetName, 
                         const_cast<std::string&>(H5ImageConst::ImageWhiteIsZero), 
                         white_is_zero);
  
  // set the display origin 
  //    "UL": (0,0) is at the upper left.
  //    "LL": (0,0) is at the lower left.
  //    "UR": (0,0) is at the upper right.
  //    "LR": (0,0) is at the lower right.
  err = H5Lite::writeAttribute(groupId, 
                               datasetName, 
                               const_cast<std::string&>(H5ImageConst::DisplayOrigin), 
                               const_cast<std::string&>(H5ImageConst::UpperLeft) );
  if (err < 0) {
    std::cout << "Error setting display origin" << std::endl;
    return -1;
  }
  
  
  _TIFFfree( raster );
  return err;
}



// ---------------------------------------------------------------------
// Reads an 8bit color palette image into an HDF5 Image Dataset
// ---------------------------------------------------------------------
herr_t H5TiffIO::_read8BitTiff( TIFF *in, hid_t groupId, string &datasetName) 
{
  uint32* raster;     /* retrieve RGBA image */
  uint32  width, height;    /* image width & height */
  uint16  bitspersample;
  herr_t err = 0;
  
  std::cout << "Importing 8bit color palette tiff image" << std::endl;
  
  TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bitspersample);
  
  raster = (uint32*)_TIFFmalloc(width * height * sizeof (uint32));
  if (raster == 0) {
    TIFFError(TIFFFileName(in), "No space for raster buffer");
    return (-1);
  }
  

  // TIFFReadRGBAImageOriented converts non 8bit images including:
  //  Grayscale, bilevel, CMYK, and YCbCR transparently into 32bit RGBA
  //  samples
  /* Read the image in one chunk into an RGBA array */
  if (!TIFFReadRGBAImageOriented(in, width, height, raster,
				 ORIENTATION_TOPLEFT, 0)) {
    _TIFFfree(raster);
    return (-1);
  }


  // Read the Color Map
  uint16 cmapRank = 1 << bitspersample;
  uint16* dRed =   (uint16*)_TIFFmalloc(cmapRank * sizeof (uint16));
  uint16* dGreen = (uint16*)_TIFFmalloc(cmapRank * sizeof (uint16));
  uint16* dBlue =  (uint16*)_TIFFmalloc(cmapRank * sizeof (uint16));
  TIFFGetField(in, TIFFTAG_COLORMAP, &dRed, &dGreen, &dBlue);
  
  unsigned char colorMap[cmapRank * 3];
  int j=0;
  for (int i=0; i<cmapRank; i++) {
    colorMap[j++] = ((int) dRed[i] / 256);
    colorMap[j++] = ((int) dGreen[i] / 256);
    colorMap[j++] = ((int) dBlue[i] / 256);
  }

  // Need to go back and turn the RGBA values back into index entries
  //  into the colormap
  int pixel_count = width * height;
  unsigned char idxRaster[pixel_count];
  int index;

  for (int i=0; i<pixel_count; i++) {
    index = _findColorMapIndex(pixel_count,(int) TIFFGetR(raster[i]), 
			       (int) TIFFGetG(raster[i]),
			       (int) TIFFGetB(raster[i]),
			       colorMap);
    if (index < 0) {
      std::cout << "Error looking up colormap index for indexed image import" 
		<< std::endl;
      _TIFFfree(raster);
      return -1;
    }
    idxRaster[i] = (unsigned char) index;
  }

  _TIFFfree(raster);


  // Store byte array to HDF5 File
  err = H5Image::H5IMmake_image_8bit(groupId, datasetName, width, height, idxRaster);
  if (err<0) {
    std::cout << "Error storing Image data with H5IM API:  datasetName: "
	      << datasetName << std::endl; 
    return -1;
  }


  // Create the color palette  (HDF5 palette)
  hsize_t pal_dims[] = {cmapRank, 3};
  char pname[strlen(datasetName.c_str()) + 8];
  strcpy(pname, datasetName.c_str());
  strcat(pname, "-palette");

  err = H5Image::H5IMmake_palette(groupId, pname, pal_dims, colorMap);
  if (err < 0) {
    std::cout << "Error creating palette" << std::endl;
    return -1;
  }


//  err = H5LTset_attribute_string(groupId, pname, H5ImageConst::PalColorMap, H5ImageConst::RGB);
  std::string strPname(pname);
  err = H5Lite::writeAttribute(groupId, 
                               strPname, 
                               const_cast<std::string&>(H5ImageConst::PalColorMap), 
                               const_cast<std::string&>(H5ImageConst::RGB) );
  if (err < 0) {
    std::cout << "Error setting palette colormap" << std::endl;
    return -1;
  }


  err = H5Lite::writeAttribute(groupId, 
                               strPname, 
                               const_cast<std::string&>(H5ImageConst::PalType), 
                               const_cast<std::string&>(H5ImageConst::Standard8) );
  if (err < 0) {
    std::cout << "Error setting palette type" << std::endl;
    return -1;
  }

  // record the # of open attribute ids for error checking after
  //  the next call to H5IMlink_palette
  int num_attrs = H5Fget_obj_count(this->_fileId, H5F_OBJ_ATTR);

  // Attach the color palette to the image
  err = H5Image::H5IMlink_palette(groupId, datasetName, pname);

  int new_num_attrs = H5Fget_obj_count(this->_fileId, H5F_OBJ_ATTR);
  if (new_num_attrs > num_attrs) {
    std::cout << "NUMBER OF ATTRIBUTES DON'T MATCH: " 
	      << new_num_attrs << std::endl;
    _closePaletteCreatedDataset(this->_fileId, groupId, datasetName, new_num_attrs);
  }

  // Set the image colorset (redundant information from pal_colormap
  err = H5Lite::writeAttribute(groupId, 
                               datasetName, 
                               const_cast<std::string&>(H5ImageConst::ImageColorModel),
                               const_cast<std::string&>(H5ImageConst::RGB) );
  if (err < 0) {
    std::cout << "Error setting image colormap" << std::endl;
    return -1;
  }

  // set the display origin 
  //    "UL": (0,0) is at the upper left.
  //    "LL": (0,0) is at the lower left.
  //    "UR": (0,0) is at the upper right.
  //    "LR": (0,0) is at the lower right.
  err = H5Lite::writeAttribute(groupId,
                                 datasetName, 
                                 const_cast<std::string&>(H5ImageConst::DisplayOrigin),
                                 const_cast<std::string&>(H5ImageConst::UpperLeft) );
  if (err < 0) {
    std::cout << "Error setting display origin" << std::endl;
    return -1;
  }

  return err;
}


// While linking the palette to the dataset referred to by
//   groupId and datasetName, the HDF code opened an hid_t
//   to that dataset but didn't close it.  This is an 
//  Attribute dataset - search through the list of open attr Ids
//   and close the one matching.
void H5TiffIO::_closePaletteCreatedDataset(hid_t fileId, hid_t groupId, 
					    string datasetName, 
					    int num_attrs)
{
  hid_t *attr_ids;
  attr_ids = (hid_t *)malloc(num_attrs * sizeof(hid_t));
  H5Fget_obj_ids(fileId, H5F_OBJ_ATTR, num_attrs, attr_ids);
  ssize_t numchars;
  char *attrname;
  
  numchars = 1 + H5Iget_name(groupId, NULL, 0);
  attrname = (char *)malloc(numchars * sizeof(char));
  H5Iget_name(groupId, attrname, numchars);
  string gname = attrname;
  gname += "/" + datasetName;
  string aname;
  for (int i=0; i<num_attrs; i++) {
    if (attr_ids[i] > 0) {
      numchars = 1+ H5Iget_name(attr_ids[i], NULL, 0);
      attrname = (char *)malloc(numchars * sizeof(char));
      H5Iget_name(attr_ids[i], attrname, numchars);
      aname = attrname;
      free(attrname);
      if (aname == gname) {
	H5Aclose(attr_ids[i]);
	break;
      }
    }
  }
  free(attr_ids);
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int H5TiffIO::_findColorMapIndex(int max, int imgR, int imgG, int imgB,
				 unsigned char *colorMap)
{
  // note - colorMap is in the format for HDF5 which means it's a flat
  //  1dimensional array where the entries are:  [R1, G1, B1, R2, G2, B2,...]
  for (int i=0; i<max; i=i+3) {
    if (imgR == colorMap[i] && 
	imgG == colorMap[i+1] && 
	imgB == colorMap[i+2]) {
      return (int)i/3;
    }
  }
  return -1;
}



// ---------------------------------------------------------------------
// Reads a True Color image into an HDF5 24Bit Image data set
// ---------------------------------------------------------------------
herr_t H5TiffIO::_read24BitTiff(TIFF *in, hid_t groupId, string &datasetName)
{
  uint32* raster;     /* retrieve RGBA image */
  uint32  width, height;    /* image width & height */
  herr_t err = -1;
  
  std::cout << "Importing rgb full color tiff image" << std::endl;
  
  TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
  
  raster = (uint32*)_TIFFmalloc(width * height * sizeof (uint32));
  if (raster == 0) {
    TIFFError(TIFFFileName(in), "No space for raster buffer");
    return (0);
  }
  
  /* Read the image in one chunk into an RGBA array */
  if (!TIFFReadRGBAImageOriented(in, width, height, raster,
				 ORIENTATION_TOPLEFT, 0)) {
    _TIFFfree(raster);
    return (0);
  }
  
  /*
  ** Strip out the Alpha Components
  */
  int pixel_count = width * height;
  unsigned char *src, *dst;
  
  src = (unsigned char *) raster;
  dst = (unsigned char *) raster;
  while( pixel_count > 0 )
    {
      *(dst++) = *(src++);
      *(dst++) = *(src++);
      *(dst++) = *(src++);
      src++;
      pixel_count--;
    }
  
  // Store byte array to HDF5 File
  err = H5Image::H5IMmake_image_24bit(groupId, datasetName, width, height,
			     H5IM_INTERLACE_PIXEL.c_str() , (unsigned char *)raster);
  if (err<0) {
    std::cout << "Error storing 24 bit true color Image data with H5IM API. datasetName: " << datasetName << std::endl; 
  }
  _TIFFfree( raster );
  return err;
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int H5TiffIO::_determineTiffOutputImageClass(hid_t fileId, 
					     string img_dataset_name)
{
  herr_t err = 0;
  int dimRank;

  err = H5Lite::getAttributeNDims(fileId, img_dataset_name, const_cast<std::string&>(H5ImageConst::ImageSubclass), dimRank);
  if (err < 0) {
    return UnknownTiffImage;
  }

  size_t type_size;
  H5T_class_t class_type;
  hid_t attrType = -1;
  std::vector<hsize_t> dimensions;
  err = H5Lite::getAttributeInfo(fileId,
                                 img_dataset_name, 
                                 const_cast<std::string&>(H5ImageConst::ImageSubclass), 
                                 dimensions, class_type, type_size, attrType );
  err = H5Tclose(attrType); 
  if (err < 0) {
    return UnknownTiffImage;
  }

  if (class_type == H5T_STRING) {
    //char atStr[type_size];
    std::string atStr;
    err = H5Lite::readAttribute(fileId, img_dataset_name, H5ImageConst::ImageSubclass, atStr);

    if (err < 0) {
      return UnknownTiffImage;
    }

    if (atStr == H5ImageConst::ImageGrayScale) {
      return GrayscaleTiffImage;
    } else if (atStr == H5ImageConst::ImageIndexed ) {
      return PaletteColorTiffImage;
    }
  }

  return UnknownTiffImage;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5TiffIO::exportTiff(hid_t fileId, string filename, 
			    string img_dataset_name)
{
  TIFF *out;
  herr_t err = 0;

  hsize_t width, height, planes;
  hssize_t numpalettes;
  char interlace[16];
  err = H5Image::H5IMget_image_info(fileId, img_dataset_name, &width, &height,
			   &planes, interlace, &numpalettes);
  if (err < 0) {
    std::cout << "Error getting Image information from hdf file" << std::endl;
    return err;
  }

  hsize_t dSize = width * height;
  unsigned char data[dSize];
  err = H5Image::H5IMread_image(fileId, img_dataset_name, data);
  if (err < 0) {
    std::cout << "Error reading image data" << std::endl;
    return err;
  }

  out = TIFFOpen(filename.c_str(), "w");
  if (out == NULL) {
    std::cout << "Could not open output tiff for writing" << std::endl;
    return -1;
  }

  int imageClass = _determineTiffOutputImageClass(fileId, img_dataset_name);
  
  switch (imageClass) {
  case GrayscaleTiffImage:
    err = _writeGrayscaleTiff(out, fileId, img_dataset_name, data,
			      width, height);
    break;
  case PaletteColorTiffImage:
    err = _write8BitTiff(out, fileId, img_dataset_name, data, width, 
			 height, numpalettes);
    break;
  case RGBFullColorTiffImage:
    //    err = _write24BitTiff(out, fileId, img_dataset_name, width, height,
    //			  planes, numpalettes, interlace);
    break;
  default:
    std::cout << "importTiff ERROR: Unsupported TIFF image type:" 
	      << imageClass << std::endl;
    err = -1;
  }

  (void) TIFFClose(out);
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5TiffIO::_writeGrayscaleTiff(TIFF *image, hid_t fileId, 
				     string img_dataset_name,
				     unsigned char *data, hsize_t width, 
				     hsize_t height)
{
  herr_t err = 0;

  // set the basic values
  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, (int) width);
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, (int) height);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, (int) height); // 1 strip

  TIFFSetField(image, TIFFTAG_COMPRESSION, 1);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, 1);

  // Write the information to the file
  err = TIFFWriteEncodedStrip(image, 0, data, width * height);
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5TiffIO::_write8BitTiff(TIFF *image, hid_t fileId, 
				string img_dataset_name,
				unsigned char *data, hsize_t width,
				hsize_t height,
				hssize_t numpalettes)
{
  herr_t err = 0;

  // Get the color palette dimensions
  hsize_t pal_dims[2];
  err = H5Image::H5IMget_palette_info(fileId, img_dataset_name, 0, pal_dims);
  if (err < 0) {
    std::cout << "Error getting palette dimenssions" << std::endl;
    return err;
  }

  // Read the color map
  int numRows = pal_dims[0];
  int palRank = numRows * pal_dims[1];
  unsigned char colorMap[palRank];
  err = H5Image::H5IMget_palette(fileId, img_dataset_name, 0, colorMap);
  if (err < 0) {
    std::cout << "Error getting color palette" << std::endl;
    return err;
  }
  int cRank = (int) palRank / 3;
  uint16 dRed[cRank], dGreen[cRank], dBlue[cRank];
  
  int index;
  for (int i=0; i<palRank; i=i+3) {
    index = (int) i/3;
    dRed[index] = (uint16) colorMap[i] * 256;
    dGreen[index] = (uint16) colorMap[i+1] * 256;
    dBlue[index] = (uint16) colorMap[i+2] * 256;
  }
  
  std::cout << "Computed Color map - setting values" << std::endl;

  // set the basic values
  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, (int) width); 
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, (int) height);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, (int) height); // 1 strip

  TIFFSetField(image, TIFFTAG_COMPRESSION, 1);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, 3);
  TIFFSetField(image, TIFFTAG_COLORMAP, dRed, dGreen, dBlue);
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);  

  // Write the information to the file
  err = TIFFWriteEncodedStrip(image, 0, data, width * height);

  return err;

}
