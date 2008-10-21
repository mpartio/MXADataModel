///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _WRITER64_H_
#define _WRITER64_H_



#if defined (_MSC_VER)
#include "Common/Win32Defines.h"
#endif
// PVDislocation Headers
#include "Common/MXATypes.h"
#include "Common/MXAEndian.h"

// Conditional Includes/Defines for Large File Support on Windows
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
/*
"It's a known, long-standing bug in the compiler system's headers.  For
some reason the manufacturer, in its infinite wisdom, chose to #define
macros min() and max() in violation of the upper-case convention and so
break any legitimate functions with those names, including those in the
standard C++ library."
*/
#define NOMINMAX
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#define WRITER64_OUTPUT_STREAM HANDLE

#else  // Non Windows Platform
#include <fstream>
#define WRITER64_OUTPUT_STREAM std::ofstream
#endif

// Hard set our stream buffer to 4096 bytes
#define BUFF_SIZE 4096

// C++ Includes
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

/**
 * @class Writer64 Writer64.h Common/IO/Writer64.h
 * @brief This class is a wrapper around platform specific native streams to make
 * sure that we can write files larger than 2GB
 * @author Mike Jackson @ IMTS.us
 * @date August 2007
 * @version $Revision: 1.4 $
 */
class Writer64
{


public:
  explicit Writer64(const std::string &filename);

  virtual ~Writer64();

  /**
   * @brief Initializes our stream object and opens the file
   * @return True on success. False on any failure
   */
  bool initWriter();

  /**
 * @brief Reads a number of bytes from the underlying stream
 * @param data The char pointer to read the data into
 * @param numBytes The number of bytes to read
 */
  void write(char* data, int64 numBytes)
  {
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
  DWORD nBytesToWrite = static_cast<DWORD>(numBytes);
  DWORD nBytesWritten = 0;
  int error = WriteFile(_outStream,  // Which is really an HANDLE hFile on Windows
                   data,
                   nBytesToWrite,
                   &nBytesWritten,
                   NULL) ;
  if (nBytesToWrite != nBytesWritten)
  {
    throw std::runtime_error ( "Error: Number of bytes written did not match number of bytes asked." );
  }
#else
    _outStream.write(data, numBytes);
#endif
  //  std::cout << "Done Writing" << std::endl;
  }

  /**
 * @brief Sets the filepointer of the underlying stream
 * @param position The position to set the file pointer to
 */
  void setFilePointer64( int64 position )
  {
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
    LARGE_INTEGER posOut;
    LARGE_INTEGER offset;
    offset.QuadPart = position;
    SetFilePointerEx(_outStream, offset, &posOut , FILE_BEGIN); //Seek from the beginning of file
#else
    _outStream.seekp(position);
#endif
  }

/**
 * @brief Returns the current position of the file pointer
 * @return the Current position of the file pointer
 */
  int64 getFilePointer64()
  {
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
    LARGE_INTEGER posOut;
    LARGE_INTEGER offset;
    offset.QuadPart = 0;
    SetFilePointerEx(_outStream, offset, &posOut , FILE_CURRENT); //From where we are now
    return posOut.QuadPart;
#else
    return _outStream.tellp();
#endif
  }


private:
  std::string _filename;
  WRITER64_OUTPUT_STREAM _outStream;
  std::vector<char> _buffer;

};

#endif // _WRITER64_H_
