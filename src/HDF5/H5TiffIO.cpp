
//-- MXA Headers
#include <HDF5/H5Lite.h>
#include <HDF5/H5Image.h>
#include <HDF5/H5TiffIO.h>
#include <Common/LogTime.h>

//-- STL Headers
#include <iostream>
#include <math.h>


H5TiffIO::H5TiffIO(hid_t fileId) :
  _fileId(fileId)
{
  _tiff = NULL;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5TiffIO::~H5TiffIO()
{
  if ( NULL != _tiff)
  {
    (void) TIFFClose(_tiff);
  }
}

// -------------------------------------------------------------------------
// Read the tiff file
// -------------------------------------------------------------------------
herr_t H5TiffIO::importTiff(string filename, hid_t groupId, 
			     string datasetName, bool asGrayscale)   
{
  if (NULL != _tiff)
  {
    (void) TIFFClose(_tiff); // Close any open tiff file first
  }
  
  herr_t err = 0;
  
  _tiff = TIFFOpen(filename.c_str(), "r");
  if (_tiff == NULL) {
    std::cout << "Error Opening Tiff file with Absolute Path:\n    " 
	      << filename << std::endl;
    return (herr_t) -1;
  }
  
  if (asGrayscale) {
    err = _importGrayscaleTiffImage(_tiff, groupId, datasetName);
  } else {
    int32 imageClass = _determineTiffImageClass(_tiff);

    switch (imageClass) {
      case GrayscaleTiffImage:
        err = _importGrayscaleTiffImage( _tiff, groupId, datasetName);
        break;
      case PaletteColorTiffImage: 
        err = _importPaletteColorTiff( _tiff, groupId, datasetName);
        break;
      case RGBFullColorTiffImage:
        err = _importRGBFullColorTiff(_tiff, groupId, datasetName);
        break;
      default:
        std::cout << "importTiff ERROR: Unsupported TIFF image type:" << imageClass << std::endl;
        err = -1;
    }
  }

  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5TiffIO::extractTiffTagContents(ITiffTagExtractor* extractor)
{
  extractor->extractTagContents(_tiff);
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

//  std::cout << "TIFF Image Field Values: " << std::endl;
//  std::cout << "  SamplesPerPixel: " << samplesperpixel << std::endl;
//  std::cout << "  BitsPerSample:   " << bitspersample << std::endl;
//  std::cout << "  Compression:     " << compression << std::endl;
//  std::cout << "  Photometric:     " << photometric << std::endl;

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
herr_t H5TiffIO::_importGrayscaleTiffImage(TIFF *in, hid_t groupId, string &datasetName)
{
  uint32* raster;     /* retrieve RGBA image */
  uint32  width, height;    /* image width & height */
  herr_t err = 0;
  
  //std::cout << "Importing grayscale tiff image" << std::endl;
  
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
  if (!TIFFReadRGBAImageOriented(in, width, height, raster, ORIENTATION_TOPLEFT, 0)) {
    _TIFFfree(raster);
    return (-1);
  }
  
  // Collapse the data down to a single channel, that will end up
  //  being the grayscale values
  int32 pixel_count = width * height;
  unsigned char *src, *dst;
  
  src = (unsigned char *) raster;
  dst = (unsigned char *) raster;
  while( pixel_count > 0 ) {
    *(dst++) = *(src);        
    src += 4; //skip ahead 4 bytes
    pixel_count--;
  }
  
  // Store byte array to HDF5 File
  err = H5Image::makeGrayScaleImage(groupId, datasetName, width, height, (unsigned char *)raster );
  if (err<0) {
    std::cout << "Error storing Image data with H5IM API:  datasetName: "
	      << datasetName << std::endl; 
  }
  
  // Need to update the attributes to be correct for a grayscale image
  H5Lite::writeStringAttribute(groupId, 
                         datasetName, 
                         const_cast<std::string&>(H5ImageConst::ImageSubclass), 
                         const_cast<std::string&>(H5ImageConst::ImageGrayScale) );
 // H5LTset_attribute_string(groupId, datasetName.c_str(), H5ImageConst::ImageSubclass, H5ImageConst::ImageGrayScale);
  uint32 white_is_zero = 0;
  H5Lite::writeScalarAttribute(groupId, 
                         datasetName, 
                         const_cast<std::string&>(H5ImageConst::ImageWhiteIsZero), 
                         white_is_zero);
  
  // set the display origin 
  //    "UL": (0,0) is at the upper left.
  //    "LL": (0,0) is at the lower left.
  //    "UR": (0,0) is at the upper right.
  //    "LR": (0,0) is at the lower right.
  err = H5Lite::writeStringAttribute(groupId, 
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
herr_t H5TiffIO::_importPaletteColorTiff( TIFF *in, hid_t groupId, string &datasetName) 
{
  herr_t err = 0;
  
  if (false)
  {
    std::cout << DEBUG_OUT(logTime) << "Indexed Color Images are not supported." << std::endl;
    return -1;
  }

  uint32* raster;     /* retrieve RGBA image */
  uint32  width, height;    /* image width & height */
  uint16  bitspersample;

 // std::cout << "Importing 8bit color palette tiff image" << std::endl;
  
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
  if (!TIFFReadRGBAImageOriented(in, width, height, raster, ORIENTATION_TOPLEFT, 0)) 
  {
    _TIFFfree(raster);
    return (-1);
  }

  // Strip out the Alpha Components
  int32 pixel_count = width * height;
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
  err = H5Image::H5IMmake_image_24bit(groupId, datasetName, width, height, H5IM_INTERLACE_PIXEL.c_str() , (unsigned char *)raster);
  if (err<0) {
    std::cout << "_importPaletteColorTiff: Error storing 24 bit true color Image data with H5IM API. datasetName: " << datasetName << std::endl; 
  }
  _TIFFfree( raster );
  return err;
}

// ---------------------------------------------------------------------
// Reads a True Color image into an HDF5 24Bit Image data set
// ---------------------------------------------------------------------
herr_t H5TiffIO::_importRGBFullColorTiff(TIFF *in, hid_t groupId, string &datasetName)
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
  int32 pixel_count = width * height;
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
int H5TiffIO::_determineTiffOutputImageClass(hid_t fileId, const string &img_dataset_name)
{
  herr_t err = 0;
  int32 dimRank;

  err = H5Lite::getAttributeNDims(fileId, img_dataset_name, const_cast<std::string&>(H5ImageConst::ImageSubclass), dimRank);
  if (err < 0) {
    return UnknownTiffImage;
  }

  size_t type_size;
  H5T_class_t class_type;
  hid_t attrType = -1;
  std::vector<uint64> dimensions;
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
    err = H5Lite::readStringAttribute(fileId, img_dataset_name, H5ImageConst::ImageSubclass, atStr);

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
// While linking the palette to the dataset referred to by
//   groupId and datasetName, the HDF code opened an hid_t
//   to that dataset but didn't close it.  This is an 
//  Attribute dataset - search through the list of open attr Ids
//   and close the one matching.
void H5TiffIO::_closePaletteCreatedDataset(hid_t fileId, hid_t groupId, 
              string datasetName, 
              int32 num_attrs)
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
int H5TiffIO::_findColorMapIndex(int max, int32 imgR, int32 imgG, int32 imgB,
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
  std::string interlace("");
  err = H5Image::H5IMget_image_info(fileId, img_dataset_name, &width, &height, &planes, interlace, &numpalettes);
  if (err < 0) {
    std::cout << "Error getting Image information from hdf file" << std::endl;
    return err;
  }

  std::vector<uint8>::size_type dSize = static_cast<std::vector<uint8>::size_type> (width * height);
  std::vector<uint8> data(dSize);
  err = H5Image::H5IMread_image(fileId, img_dataset_name, &(data.front() ) );
  if (err < 0) {
    std::cout << "Error reading image data" << std::endl;
    return err;
  }

  out = TIFFOpen(filename.c_str(), "w");
  if (out == NULL) {
    std::cout << "Could not open output tiff for writing" << std::endl;
    return -1;
  }

  int32 imageClass = _determineTiffOutputImageClass(fileId, img_dataset_name);
  
  switch (imageClass) {
    case GrayscaleTiffImage:
      err = _exportGrayScaleTiff(out, &(data.front() ), width, height);
      break;
    case PaletteColorTiffImage:
      //err = _export8BitTiff(out, fileId, img_dataset_name, &(data.front() ), width, height, numpalettes);
      err = -1;
      std::cout << "PaletteColorTiffImage currently not supported for Export" << std::endl;
      break;
    case RGBFullColorTiffImage:
      err = _exportRGBFullColorTiff(out, fileId, img_dataset_name, width, height, planes, numpalettes, interlace);
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
herr_t H5TiffIO::_exportRGBFullColorTiff(TIFF* out, 
                              hid_t fileId,
                              const std::string &img_dataset_name, 
                              hsize_t width,
                              hsize_t height,
                              hsize_t planes, 
                              hssize_t numpalettes, 
                              const std::string &interlace)
{
  herr_t err = -1;
  std::cout << "_exportRGBFullColorTiff NOT Implemented." << std::endl;
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5TiffIO::_exportGrayScaleTiff(TIFF *image,
                                     uint8* data, 
                                     hsize_t width, 
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
  tsize_t area = static_cast<tsize_t>(width* height);
  err = TIFFWriteEncodedStrip(image, 0, data, area);
  return err;
}

#if 0
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5TiffIO::_export8BitTiff(TIFF *image, 
                                hid_t fileId, 
                                std::string img_dataset_name,
                                unsigned char *data, 
                                hsize_t width,
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
  hsize_t numRows = pal_dims[0];
  std::vector<uint8>::size_type palRank = static_cast<std::vector<uint8>::size_type> (numRows * pal_dims[1]);
  //unsigned char colorMap[palRank];
  std::vector<uint8> colorMap(palRank);
  err = H5Image::H5IMget_palette(fileId, img_dataset_name, 0, &(colorMap.front() ) );
  if (err < 0) {
    std::cout << "Error getting color palette" << std::endl;
    return err;
  }
  int32 cRank = (int) palRank / 3;
  //uint16 dRed[cRank], dGreen[cRank], dBlue[cRank];
  std::vector<uint16> dRed(cRank);
  std::vector<uint16> dGreen(cRank);
  std::vector<uint16> dBlue(cRank);

  int32 index;
  for (unsigned int i=0; i<palRank; i=i+3) {
    index = (unsigned int)( i/3 );
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
  TIFFSetField(image, TIFFTAG_COLORMAP, &(dRed.front()), &(dGreen.front() ), &(dBlue.front() ) );
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);  

  // Write the information to the file
  err = TIFFWriteEncodedStrip(image, 0, data, static_cast<tsize_t>(width * height) );

  return err;

}

#endif



