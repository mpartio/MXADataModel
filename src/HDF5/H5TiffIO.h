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
#include "Common/DLLExport.h"
#include "Base/ITiffTagExtractor.h"

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
  virtual ~H5TiffIO();
  
  void setFileId(hid_t fileId) { this->_fileId = fileId; }
  /**
   * @brief imports a Tiff image into the MHDataModel
   * 
   * @param filename The absolute path to the Tiff file
   * @param groupdId The HDF Group/FileId to store the imported data
   * @param datasetName The Name of the HDF5 dataset to store the data
   * @param asGrayscale Should the tiff be stored as a grayscale image
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

  void extractTiffTagContents(ITiffTagExtractor* extractor);
  
 protected:
   
   hid_t _fileId; 
   TIFF* _tiff;
   
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
  int32 _determineTiffImageClass(TIFF *in);
  
  
  /**
   * @brief Determines the value of the "Class" attribute for the image
   * @param hid_t The HDF5 file id
   * @param string The path to the dataset
   * @return One of the enumerated types from tiffImageClasses. 
   */
  int32 _determineTiffOutputImageClass(hid_t fileId, const string &img_dataset_name);
  
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
  herr_t _importGrayscaleTiffImage(TIFF *, hid_t, string &);
  
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
  herr_t _importPaletteColorTiff( TIFF *, hid_t, string &);
  
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
  herr_t _importRGBFullColorTiff(TIFF *, hid_t, string &);
  
#if 0
  We are not currently supporting the export of Tiff images with palettes. You 
  should be exporting the image as RGBA instead
  /**
   * @brief Writes an 8bit tiff file into an HDF5 dataset
   * @param image The TIFF image pointer
   * @param fileId The HDF5 File Id
   * @param img_dataset_name The name of the HDF5 dataset
   * @param data The tiff data to be written
   * @param width The width of the image
   * @param height The height of the image
   * @param numpalettes The number of palettes for the iamge
   * @return Standard HDF5 error condition
   */
  herr_t _export8BitTiff(TIFF *image, hid_t fileId, 
        string img_dataset_name,
        unsigned char *data, hsize_t width,
        hsize_t height,
        hssize_t numpalettes);

#endif
  


  
  /**
   * @brief Writes a tiff image as a grayscale image HDF5 dataset 
   * @param image The TIFF image pointer
   * @param fileId The HDF5 File Id
   * @param img_dataset_name The name of the HDF5 dataset
   * @param data The tiff data to be written
   * @param width The width of the image
   * @param height The height of the image
   * @return Standard HDF5 error condition
   */
  herr_t _exportGrayScaleTiff(TIFF *image,
                             unsigned char *data, 
                             hsize_t width, 
                             hsize_t height);
  
  

  herr_t _exportRGBFullColorTiff(TIFF* out, 
                                hid_t fileId,
                                const std::string &img_dataset_name, 
                                hsize_t width,
                                hsize_t height,
                                hsize_t planes, 
                                hssize_t numpalettes, 
                                const std::string &interlace);

    
 private:
  int32 _findColorMapIndex(int, int, int, int, unsigned char *);
  void _closePaletteCreatedDataset(hid_t, hid_t, string, int);
    
};



#endif /* _MHDTIFFIO_H_ */
