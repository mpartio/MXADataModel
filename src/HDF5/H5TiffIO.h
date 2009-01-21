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
#include <Common/DLLExport.h>
#include <Base/ITiffTagExtractor.h>

//-- HDF Headers
#include <hdf5.h>

//-- TIFF Headers
#include <tiffio.h>

//-- STL Headers
#include <string>

using namespace std;

namespace MXA
{
  namespace Tiff
  {
    const std::string FileExtension(".tif");
    const std::string TiffExtension(".tiff");
  }
}


const std::string H5IM_INTERLACE_PIXEL("INTERLACE_PIXEL");

/**
* @brief General IO class for TIFF images
* @author Mike Jackson/Shawn Nicholson
* @date March 2007
* @version $Revision: 1.14 $
*/
class MXA_EXPORT H5TiffIO
{
 public:
  explicit H5TiffIO(hid_t fileId);
  virtual ~H5TiffIO();

  void setFileId(hid_t fileId) { this->_fileId = fileId; }
  /**
   * @brief imports a Tiff image into the MXADataModel
   *
   * @param filename The absolute path to the Tiff file
   * @param groupId The HDF Group/FileId to store the imported data
   * @param datasetName The Name of the HDF5 dataset to store the data
   * @param asGrayscale Should the tiff be stored as a grayscale image
   * @return Error < 0 Error
   */
  herr_t importTiff(const std::string &filename,
                    hid_t groupId,
                    const std::string &datasetName,
                    bool asGrayscale = true);


  /**
   * @brief exports a TIFF image from the MXADataModel
   *
   * @param fileId - the hdf fileID
   * @param filename - the output file to write the tiff to
   * @param img_dataset_name - the image dataset in the XMData file
   */
  herr_t exportTiff(hid_t fileId,
                    const std::string &filename,
                    const std::string &img_dataset_name);

  /**
   * @brief Extracts the contents of a tag using an instance of ITiffTagExtractor
   * @param extractor An Instance of an ITiffTagExtractor class
   */
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
   * @param fileId The HDF5 file id
   * @param img_dataset_name The path to the dataset
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
  herr_t _importGrayscaleTiffImage(TIFF *in, hid_t groupId, const std::string &datasetName);

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
  herr_t _importPaletteColorTiff(TIFF *in, hid_t groupId, const std::string &datasetName);

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
  herr_t _importRGBFullColorTiff(TIFF *in, hid_t groupId, const std::string &datasetName);

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
   * @param data The tiff data to be written
   * @param width The width of the image
   * @param height The height of the image
   * @return Standard HDF5 error condition
   */
  herr_t _exportGrayScaleTiff(TIFF *image,
                                       uint8* data,
                                       hsize_t width,
                                       hsize_t height,
                                       const std::string &documentName,
                                       const std::string &imageDescription);

  /**
   * @brief Exports an Image data set as a true color tiff file
   * @param out The Tiff file to write the data into
   * @param fileId The HDF5 file_id value
   * @param img_dataset_name The Name of the data set
   * @param width The width of the image
   * @param height The heigth of the image
   * @param planes The number of planes in the image
   * @param numpalettes The number of palettes from the image
   * @param interlace The interlace mode of the image
   * @return Negative value on error
   */
  herr_t _exportRGBFullColorTiff(TIFF* out,
                                hid_t fileId,
                                const std::string &img_dataset_name,
                                hsize_t width,
                                hsize_t height,
                                hsize_t planes,
                                hssize_t numpalettes,
                                const std::string &interlace);


 private:
  int32 _findColorMapIndex(int max, int32 imgR, int32 imgG, int32 imgB,
      unsigned char *colorMap);

  void _closePaletteCreatedDataset(hid_t fileId, hid_t groupId,
                string datasetName,
                int32 num_attrs);

};



#endif /* _MHDTIFFIO_H_ */
