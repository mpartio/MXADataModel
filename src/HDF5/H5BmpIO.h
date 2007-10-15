#ifndef _H5BMPIO_H_
#define _H5BMPIO_H_

//-- MXA Headers
#include <Common/DLLExport.h>
#include <Base/ITiffTagExtractor.h>

//-- HDF Headers
#include <hdf5.h>

//-- STL Headers
#include <string>

using namespace std;

namespace MXA
 {
  namespace BMP
  {
     const std::string FileExtension(".bmp");
  }
}


const std::string H5IM_INTERLACE_PIXEL("INTERLACE_PIXEL");

/**
* @brief General IO class for Bmp images
* @author Mike Jackson/Herb Mullens
* @date October 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT H5BmpIO 
{
 public:
  explicit H5BmpIO(hid_t fileId);
  virtual ~H5BmpIO();
  
  /**
   * @brief imports a Bmp image into the MHDataModel
   * 
   * @param filename The absolute path to the Bmp file
   * @param groupId The HDF Group/FileId to store the imported data
   * @param datasetName The Name of the HDF5 dataset to store the data
   * @param asGrayscale Should the Bmp be stored as a grayscale image
   * @return Error < 0 Error 
   */
  herr_t importBmp(const std::string &filename, 
                    hid_t fileId, 
                    const std::string &datasetName, 
                    bool asGrayscale = true);
  

  /**
   * @brief exports a BMP image from the MHDataModel
   *
   * @param fileId - the hdf fileID
   * @param filename - the output file to write the bmp to
   * @param datasetPath - the image dataset in the MXA HDF5 file
   */
  herr_t exportBmp(hid_t fileId, 
                    const std::string &filename, 
                    const std::string &datasetPath);
  

  
 protected:
   
   herr_t _readBmpFile(const std::string &filename,
                                 int32 &width,
                                 int32 &height,
                                 uint8* rgbRaster);

   herr_t _importGrayscaleBmpImage(uint8* rgbRaster, hid_t fileId, const std::string &datasetName);
   herr_t _importRGBFullColorBmp(uint8* rgbRaster, hid_t fileId, const std::string &datasetName);
 private:
   hid_t _fileId; 
   
   
   
  H5BmpIO(const H5BmpIO&);   //Copy Constructor Not Implemented
  void operator=(const H5BmpIO&); //Copy Assignment Not Implemented
    
};



#endif /* _H5BMPIO_H_ */
