#include "H5BmpIO.h"


//-- MXA Headers
#include <MXA/Common/LogTime.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Image.h>



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
  MXABmpIO reader;

  LOAD_TEXTUREBMP_RESULT res = reader.loadBMPData(filename.c_str(), asGrayscale);
  
  //std::cout << logTime() << "  H5BmpIO::importBmp  WRITING HDF5" << std::endl;
  if ( res != LOAD_TEXTUREBMP_SUCCESS )
  {
    return -1;
  }
 
  if (true == asGrayscale)
  {
    err =  _importGrayscaleBmpImage(fileId, datasetName, reader);
  } 
  else 
  {
    err =  _importRGBFullColorBmp(fileId, datasetName, reader);
  }
  return err;
}

                         
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5BmpIO::_importGrayscaleBmpImage(hid_t fileId, 
										 const std::string &datasetName,
										 MXABmpIO &reader)
{
	std::vector<uint8> rgbRaster;
	reader.copyDataArray(rgbRaster);
  int32 width = reader.getWidth();
  int32 height = reader.getHeight();
  int32 err = -1;

  //std::cout << "H5BmpIO::_importGrayscaleBmpImage: width and height: " << width << "," << height << std::endl;
  // Store byte array to HDF5 File
  err = H5Image::makeGrayScaleImage(fileId, datasetName, width, height, static_cast<uint8*>( &(rgbRaster.front() ) ) );
  if (err<0) {
    std::cout << "Error storing Image data with H5IM API:  datasetName: "
        << datasetName << std::endl; 
  }
   // Need to update the attributes to be correct for a grayscale image
   err = H5Lite::writeStringAttribute(fileId, 
                          datasetName, 
                          const_cast<std::string&>(MXA::H5Image::ImageSubclass), 
                          const_cast<std::string&>(MXA::H5Image::ImageGrayScale) );
  // H5LTset_attribute_string(groupId, datasetName.c_str(), MXA::H5Image::ImageSubclass, MXA::H5Image::ImageGrayScale);
   uint32 white_is_zero = 0;
   err = H5Lite::writeScalarAttribute(fileId, 
                          datasetName, 
                          const_cast<std::string&>(MXA::H5Image::ImageWhiteIsZero), 
                          white_is_zero);

   // set the display origin 
   //    "UL": (0,0) is at the upper left.
   //    "LL": (0,0) is at the lower left.
   //    "UR": (0,0) is at the upper right.
   //    "LR": (0,0) is at the lower right.
   err = H5Lite::writeStringAttribute(fileId, 
                                datasetName, 
                                const_cast<std::string&>(MXA::H5Image::DisplayOrigin), 
                                const_cast<std::string&>(MXA::H5Image::UpperLeft) );
   if (err < 0) {
     std::cout << "Error setting display origin" << std::endl;
     return -1;
   }
   
   return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t H5BmpIO::_importRGBFullColorBmp(hid_t fileId, 
									   const std::string &datasetName,
									   MXABmpIO &reader)
{
  int32 err = -1;
  int32 width = reader.getWidth();
  int32 height = reader.getHeight();
  
  std::vector<uint8> rgbRaster;
  reader.copyDataArray(rgbRaster);
  
  // Store byte array to HDF5 File
  // SET THE INTERLACE MODE CORRECTLY - EITHER H5IM_INTERLACE_PIXEL OR H5IM_INTERLACE_PLANE
  err = H5Image::H5IMmake_image_24bit(fileId, datasetName, width, height,
                        MXA::H5Image::InterlacePixel,  static_cast<uint8*>( &(rgbRaster.front() ) ) );
  if (err<0) {
    std::cout << "Error storing 24 bit true color Image data with H5IM API. datasetName: " << datasetName << std::endl; 
  }
  
  
  return err;
}

herr_t H5BmpIO::exportBmp(hid_t fileId,
		 const std::string &filename,
		 const std::string &datasetPath)
{
  std::cout << "H5BmpIO::exportBMP is NOT implemented" << std::endl;
  return -1;
}
