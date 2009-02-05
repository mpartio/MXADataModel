///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _CTMDREADER64_H_
#define _CTMDREADER64_H_

#if defined (_MSC_VER)
#include <MXA/Common/Win32Defines.h>
// This will define WINDOWS_LARGE_FILE_SUPPORT to true, which is what we want
#endif

#include <MXA/Common/MXATypes.h>


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
#define READER64_INPUT_STREAM HANDLE

#else

#include <fstream>
#define READER64_INPUT_STREAM std::ifstream

#endif


// Hard set our stream buffer to 4096 bytes
#define BUFF_SIZE 4096



// C++ Includes
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>


/**
 * @class Reader64 Reader64.h PVDislocation/Reader64.h
 * @brief This class is a wrapper around platform specific native streams to make
 * sure that we can read files larger than 2GB
 * @author Mike Jackson @ IMTS.us
 * @date August 2007
 * @version $Revision: 1.2 $
 */
class MXA_EXPORT Reader64
{


public:
  Reader64(const std::string &filename);

  virtual ~Reader64();

  /**
   * @brief Initializes our stream object and opens the file
   * @return True on success. False on any failure
   */
  bool initReader();

/**
 * @brief Reads and swaps (if necessary) a single primitive value.
 * @param t The value to read the value into
*/
  template <typename EndianPolicy, typename P>
  void read(P &t)
  {
    this->readPrimitive<P>(t);
    EndianPolicy::convert(t);
  }

/**
 * @brief Reads a number of bytes from the underlying stream
 * @param data The char pointer to read the data into
 * @param numBytes The number of bytes to read
 */
  void rawRead(char* data, std::streamsize numBytes)
  {
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
  DWORD nBytesToRead = static_cast<DWORD>(numBytes);
  DWORD nBytesRead = 0;
  int error = ReadFile(_instream,  // Which is really an HANDLE hFile on Windows
                   data,
                   nBytesToRead,
                   &nBytesRead,
                   NULL) ;
  if (nBytesRead != nBytesToRead)
  {
    throw std::runtime_error ( "Error: Number of bytes read does NOT match size of input." );
  }
#else
   if ( EOF == _instream.peek() )
   {
      throw std::runtime_error ( "Error: Unexpected end of file reached" );
   }
   _instream.read ( data, numBytes );
   if (_instream.gcount() != numBytes)
   {
     throw std::runtime_error ( "Error: Number of bytes read does NOT match size of input." );
   }
#endif
  }

/**
 * @brief Reads a single primitive value from the underlying stream without any
 * byte swapping or conversions
 * @param t The value to read the primitive into
 */
  template <  typename P >
  void readPrimitive ( P &t )
  {
#if defined (WINDOWS_LARGE_FILE_SUPPORT)
  DWORD nBytesToRead = sizeof(P);
  DWORD nBytesRead = 0;
  int error = ReadFile(_instream,  // Which is really an HANDLE hFile on Windows
                   &t,
                   nBytesToRead,
                   &nBytesRead,
                   NULL) ;
  if (nBytesRead != nBytesToRead)
  {
    throw std::runtime_error ( "Error: Number of bytes read does NOT match size of data type." );
  }
#else
   if ( EOF == _instream.peek() )
   {
      throw std::runtime_error ( "Error: Unexpected end of file reached" );
   }
   _instream.read ( reinterpret_cast<char*>( &t ), sizeof ( P ) );
#endif
  }


  /**
   * @brief This method will read an array of values and swap each value
   * @param t Pointer to the front of the array where the values will be stored.
   * The array MUST already be preallocated.
   * @param size The number of elements in the array
   */
  template < typename EndianPolicy, typename P>
  void readArrayWithSwap(P* t, int32 size)
  {
    for (int32 i = 0; i < size; ++i)
    {
      read<EndianPolicy>(t[i]);
    }

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
    SetFilePointerEx(_instream, offset, &posOut , FILE_BEGIN); //Seek from the beginning of file
#else
    _instream.seekg(position);
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
    SetFilePointerEx(_instream, offset, &posOut , FILE_CURRENT); //From where we are now
    return posOut.QuadPart;
#else
    return _instream.tellg();
#endif
  }


private:

  std::string _filename;
  READER64_INPUT_STREAM _instream;
  std::vector<char> _buffer;

};

#endif /*_CTMDREADER64_H_*/
