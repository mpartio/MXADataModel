#include "H5BmpIO.h"
//-- MXA Headers
#include <HDF5/H5Lite.h>
#include <HDF5/H5Image.h>
#include <Common/LogTime.h>


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5BmpIO::H5BmpIO(hid_t fileId) :
_fileId(fileId)
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5BmpIO::~H5BmpIO()
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5BmpIO::importBmp(const std::string &filename, 
                  hid_t fileId, 
                  const std::string &datasetName, 
                  bool asGrayscale)
{
  // Put the bmp file parsing code here
  int32 err = 0;
  int32 width = 0;
  int32 height = 0;
  uint8* rgbRaster; // This is an RGB array. 
  
  err = _readBmpFile(filename, width, height, rgbRaster);
  if (err < 0)
  {
    return err;
  }
  
  if (true == asGrayscale)
  {
     err =  _importGrayscaleBmpImage(rgbRaster, fileId, datasetName);
  } 
  else 
  {
    err =  _importRGBFullColorBmp(rgbRaster, fileId, datasetName);
  }
  
  return err;
  
}

// -----------------------------------------------------------------------------
//  Width and Height are passed by reference. 
// -----------------------------------------------------------------------------
herr_t H5BmpIO::_readBmpFile(const std::string &filename,
                              int32 &width,
                              int32 &height,
                              uint8* rgbRaster)
{
  int32 err = 1;
  // Read the BMP file into an RGB array.
  // If something goes wrong set err to a negative value and return.
  
  
  //If everything goes correctly, then set err to a positive value
  return err;
}
                              
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5BmpIO::_importGrayscaleBmpImage(uint8* rgbRaster, hid_t fileId, const std::string &datasetName)
{
  int32 err = 0;
  int32 width = 0;
  int32 height = 0;
    
  // Collapse the data down to a single channel, that will end up
  //  being the grayscale values
  int32 pixel_count = width * height;
  unsigned char *src, *dst;
  
  src = (unsigned char *) rgbRaster;
  dst = (unsigned char *) rgbRaster;
  while( pixel_count > 0 ) {
    *(dst++) = *(src);        
    src += 4; //skip ahead 4 bytes - THIS ASSUMES AN RGBA image. If this is RGB IMAGE then only skip 3 bytes
    pixel_count--;
  }

  // Store byte array to HDF5 File
  err = H5Image::makeGrayScaleImage(fileId, datasetName, width, height, (unsigned char *)rgbRaster );
  if (err<0) {
    std::cout << "Error storing Image data with H5IM API:  datasetName: "
	      << datasetName << std::endl; 
  }
   // Need to update the attributes to be correct for a grayscale image
   H5Lite::writeStringAttribute(fileId, 
                          datasetName, 
                          const_cast<std::string&>(H5ImageConst::ImageSubclass), 
                          const_cast<std::string&>(H5ImageConst::ImageGrayScale) );
  // H5LTset_attribute_string(groupId, datasetName.c_str(), H5ImageConst::ImageSubclass, H5ImageConst::ImageGrayScale);
   uint32 white_is_zero = 0;
   H5Lite::writeScalarAttribute(fileId, 
                          datasetName, 
                          const_cast<std::string&>(H5ImageConst::ImageWhiteIsZero), 
                          white_is_zero);

   // set the display origin 
   //    "UL": (0,0) is at the upper left.
   //    "LL": (0,0) is at the lower left.
   //    "UR": (0,0) is at the upper right.
   //    "LR": (0,0) is at the lower right.
   err = H5Lite::writeStringAttribute(fileId, 
                                datasetName, 
                                const_cast<std::string&>(H5ImageConst::DisplayOrigin), 
                                const_cast<std::string&>(H5ImageConst::UpperLeft) );
   if (err < 0) {
     std::cout << "Error setting display origin" << std::endl;
     return -1;
   }
   
   return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5BmpIO::_importRGBFullColorBmp(uint8* rgbRaster, hid_t fileId, const std::string &datasetName)
{
  int32 err = -1;
  int32 width = 0;
  int32 height = 0;
  /*
  ** Strip out the Alpha Components - ONLY DO THIS IF NEEDED... DEPENDS ON HOW YOU READ THE BMP FILE
  */
  int32 pixel_count = width * height;
  unsigned char *src, *dst;
  
  src = (unsigned char *) rgbRaster;
  dst = (unsigned char *) rgbRaster;
  while( pixel_count > 0 )
    {
      *(dst++) = *(src++);
      *(dst++) = *(src++);
      *(dst++) = *(src++);
      src++;
      pixel_count--;
    }
  
  // Store byte array to HDF5 File
  // SET THE INTERLACE MODE CORRECTLY - EITHER H5IM_INTERLACE_PIXEL OR H5IM_INTERLACE_PLANE
  err = H5Image::H5IMmake_image_24bit(fileId, datasetName, width, height,
			                  H5IM_INTERLACE_PIXEL.c_str() , (unsigned char *)rgbRaster);
  if (err<0) {
    std::cout << "Error storing 24 bit true color Image data with H5IM API. datasetName: " << datasetName << std::endl; 
  }
  
  
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t exportBmp(hid_t fileId, 
                  const std::string &filename, 
                  const std::string &datasetPath)
{
  // LEAVE THIS UNIMPLEMENTED FOR NOW
  return -1;
}

