///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) Mike Jackson, Herb Mullens
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef MXA_BMP_IO_H
#define MXA_BMP_IO_H

//MXA Includes
#include <MXA/Common/MXATypes.h>
#include <MXA/Common/DLLExport.h>
#include <MXA/BMPIO/MXABmpHeaders.h>

//STL includes
#include <vector>

//--Boost Includes
#include <boost/shared_ptr.hpp>

class MXAFILEREADER_CLASS_NAME;
typedef boost::shared_ptr<MXAFILEREADER_CLASS_NAME>    Reader64Ptr;

// The following is the function return type. Use this to
// get information about how the loading operation went.

enum LOAD_TEXTUREBMP_RESULT {
  // The texture loading operation succeeded.
  LOAD_TEXTUREBMP_SUCCESS=0,
  // The file could not be found or it could not be opened for reading.
  LOAD_TEXTUREBMP_COULD_NOT_FIND_OR_READ_FILE,
  // The file does not comply with the specification.
  LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT,
  // OpenGL could not accept the texture. You proably used a internal
  // format not accepted by your OpenGL implementation or you may have run
  // out of available texture names.
  LOAD_TEXTUREBMP_OPENGL_ERROR,
  // The system ran out of heap memory during the texture load.
  LOAD_TEXTUREBMP_OUT_OF_MEMORY
};

/**
* @class MXABmpIO MXABmpIO.h BMPIO/MXABmpIO.h
* @brief This class Handles the Reading of Windows Bitmap files (.bmp)
* @author Herb Mullens
* @date Oct 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT MXABmpIO
{
public:
  MXABmpIO();
  virtual ~MXABmpIO();

  /**
   * @brief Returns height of image
   */
  int32 getHeight();

  /**
   * @brief returns width of image
   */
  int32 getWidth();

  /**
   * @brief Returns the number of color channels in the Image
   */
  int32 getNumberOfChannels();

  /**
   * @brief Loads the bmp from file into memory
   * @param fName The file name of the image to load
   * @param readAsGrayScale Read the file as a grayscale image
   * @return Error code.
   */
  LOAD_TEXTUREBMP_RESULT loadBMPData(const char* fName, bool readAsGrayScale = false);
  /**
   * @brief Converts the image to grayscale
   */
  void convertToGrayscale();

  /**
   * @brief Is the image a grayscale image
   */
  bool isGrayscaleImage();

  /**
   * @brief Copies the BMP data into a supplied buffer. The supplied buffer
   * will be resized to fit all the data
   * @param buffer A buffer to copy the bmp data into
   */
  void copyDataArray(std::vector<uint8> &buffer);

protected:

private:
  std::vector<uint8> bitmapDataVec;
  int32 width;
  int32 height;
  int32 numChannels;

  bool isGrayscale;
  bool _imageFlipped;
  bool _imageConvertedToGrayScale;
  bool _convertToGrayScale;

  int32 bytesRead;

  MXABMPFileHeader fileHeader;
  MXABMPDIBHeader dibHeader;
  Reader64Ptr _reader64Ptr;

  // Palette used for paletted images during load.
  uint8 palette[3][256];

  /**
   * @brief Reads a 4 byte integer from the file */
  int32 read32BitValue();

  /**
   * @brief Reads a 2 byte integer from the file */
  int16 read16BitValue();

  /**
   * @brief Reads a 1 byte integer from the file */
  uint8 read8BitValue();

  /**
   * @brief Reads the file header from the bmp file
   * @return LOAD_TEXTUREBMP_RESULT
   */
  LOAD_TEXTUREBMP_RESULT readFileHeader();

  /**
   * @brief Reads the info header from the bmp file
   * @return LOAD_TEXTUREBMP_RESULT
   */
  LOAD_TEXTUREBMP_RESULT readInfoHeader();

  /**
   * @brief Reads the color palette from the bmp file
   * @return LOAD_TEXTUREBMP_RESULT
   */
  LOAD_TEXTUREBMP_RESULT readPalette();

  /**
   * @brief Reads the bitmap data from the bmp file
   * @return LOAD_TEXTUREBMP_RESULT
   */
  LOAD_TEXTUREBMP_RESULT readBitmapData();

  /**
   * @brief Reads 1 bit encoded bitmap data from the bmp file
   * @return LOAD_TEXTUREBMP_RESULT
   */
  LOAD_TEXTUREBMP_RESULT readBitmapData1Bit();

  /**
   * @brief Reads 4 bit encoded bitmap data from the bmp file
   * @return LOAD_TEXTUREBMP_RESULT
   */
  LOAD_TEXTUREBMP_RESULT readBitmapData4Bit();

  /**
   * @brief Reads 8 bit encoded bitmap data from the bmp file
   * @return LOAD_TEXTUREBMP_RESULT
   */
  LOAD_TEXTUREBMP_RESULT readBitmapData8Bit();

  /**
   * @brief Reads 24 bit encoded bitmap data from the bmp file
   * @return LOAD_TEXTUREBMP_RESULT
   */
  LOAD_TEXTUREBMP_RESULT readBitmapData24Bit();

  /**
   * @brief Parser for escape codes
   * @param secondByte
   * @param x
   * @param y
   * @param res
   * @return
   */
  bool handleEscapeCode(int secondByte, int* x, int* y,
                        LOAD_TEXTUREBMP_RESULT* res);

  /**
   * @brief Flips the bitmap image so the (0,0) pixel is in the upper left.
   */
  void flipBitmap();

  MXABmpIO(const MXABmpIO&);    //Not Implemented
  void operator=(const MXABmpIO&);  //Not Implemented


};


#endif
