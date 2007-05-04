///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, Shawn Nicholson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _MHDTIFFIO_H_
#define _MHDTIFFIO_H_

//-- MXA Headers
#include "Headers/DLLExport.h"

//-- HDF Headers
#include "hdf5.h"

//-- TIFF Headers
#include "tiffio.h"

//-- STL Headers
#include <string>

using namespace std;

namespace TiffIOConst {
  const std::string TifExtension(".tif");
  const std::string TiffExtension(".tiff");
}

//#define H5IM_INTERLACE_PIXEL "INTERLACE_PIXEL"
const std::string H5IM_INTERLACE_PIXEL("INTERLACE_PIXEL");

/**
* @brief General IO class for TIFF images
* @author Mike Jackson/Shawn Nicholson
* @date March 2007
* @version 1.0
*/
class MXA_EXPORT H5TiffIO 
{
 public:
  explicit H5TiffIO(hid_t fileId);
  virtual ~H5TiffIO() {};
  
  void setFileId(hid_t fileId) { this->_fileId = fileId; }
  /**
   * @brief imports a Tiff image into the MHDataModel
   * 
   * @param filename
   * @param groupdId
   * @param datasetName
   * @param asGrayscale
   * @return Error < 0 Error 
   */
  herr_t importTiff(string filename, hid_t groupId, 
                   string datasetName, bool asGrayscale = true);
  

  /**
   * @brief exports a TIFF image from the MHDataModel
   *
   * @param fileId - the hdf fileID
   * @param filename - the output file to write the tiff to
   * @param img_dataset_name - the image dataset in the XMData file
   */
  herr_t exportTiff(hid_t, string, string);

    
 protected:
   
   hid_t _fileId; 
   
  enum tiffImageClasses {
    UnknownTiffImage = -1,
    BilevelTiffImage = 0,
    GrayscaleTiffImage,
    PaletteColorTiffImage,
    RGBFullColorTiffImage,
    FacsimileTiffImage,
    SeparatedTiffImage,
    YCbCrTiffImage
  };

  /**
   * @brief Determines the image class for a given tiff image
   *
   * @param in The Tiff Image
   * @return element from tiffImageClasses enum
   */
  int32 _determineTiffImageClass(TIFF *);
  
  
  /**
   * @brief
   * @param hid_t
   * @param string
   * @return
   */
  int32 _determineTiffOutputImageClass(hid_t, string);
  
  
  /**
   * @brief Reads a grayscale tiff file and stores the image as raw 
   *  8 bit values in the given dataset inside the give group.
   * 
   * @param in The Tiff Image
   * @param groupId The HDF location identifier of the group into 
   *      which to store the image data
   * @param datasetName The name to store the data under in the HDF5 file.
   * @return Error < 0 Error 
   */
  herr_t _read8BitTiff( TIFF *, hid_t, string &);
  
  /**
   * @brief
   * @param image
   * @param fileId
   * @param img_dataset_name
   * @param data
   * @param width
   * @param height
   * @param numpalettes
   * @return
   */
  herr_t _write8BitTiff(TIFF *image, hid_t fileId, 
        string img_dataset_name,
        unsigned char *data, hsize_t width,
        hsize_t height,
        hssize_t numpalettes);
  
  

  /**
   * @brief Reads a grayscale tiff file and stores the image as raw 
   *  8 bit values in the given dataset inside the give group.
   * 
   * @param in The Tiff Image
   * @param groupId The HDF location identifier of the group into which
   *       to store the image data
   * @param datasetName The name to store the data under in the HDF5 file.
   * @return Error < 0 Error 
   */
  herr_t _readGrayscaleTiff(TIFF *, hid_t, string &);
  
  /**
   * @brief
   * @param image
   * @param fileId
   * @param img_dataset_name
   * @param data
   * @param width
   * @param height
   * @return
   */
  herr_t _writeGrayscaleTiff(TIFF *image,
                             unsigned char *data, 
                             hsize_t width, 
                             hsize_t height);
  
  
  /**
   * @brief Reads a true color tiff file and stores the image as raw 
   *  rgb values in the given dataset inside the give group.
   * 
   * @param in The Tiff Image
   * @param groupId The HDF location identifier of the group into which
   *     to store the image data
   * @param datasetName The name to store the data under in the HDF5 file.
   * @return Error < 0 Error 
   */
  herr_t _read24BitTiff(TIFF *, hid_t, string &);
  

    
 private:
  int32 _findColorMapIndex(int, int, int, int, unsigned char *);
  void _closePaletteCreatedDataset(hid_t, hid_t, string, int);
    
};



#endif /* _MHDTIFFIO_H_ */
