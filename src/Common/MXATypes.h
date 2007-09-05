///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, Perry Miller IV
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef MXA_BASIC_TYPES_H_
#define MXA_BASIC_TYPES_H_

/**
 * @file MXATypes.h
 * @brief Contains the typedefs for all the common integer and floating point
 * prinitives that are used in the MXA Code Base
 * @version $Revision: 1.3 $
 */


// C++ Includes
#include <vector>

// -----------------------------------------------------------------------------
//  MXA Standard Types
// -----------------------------------------------------------------------------

#if defined (_MSC_VER) // Visual C++

  typedef __int64             int64;
  typedef int                 int32;
  typedef short               int16;
  typedef signed char         int8;

  typedef unsigned __int64    uint64;
  typedef unsigned int        uint32;
  typedef unsigned short      uint16;
  typedef unsigned char       uint8;

  typedef float               float32;
  typedef double              float64;

#elif defined( __SGI )
  
  typedef long long           int64;
  typedef int                 int32;
  typedef short               int16;
  typedef char                int8;

  typedef unsigned long long  uint64;
  typedef unsigned int        uint32;
  typedef unsigned short      uint16;
  typedef unsigned char       uint8;

  typedef float               float32;
  typedef double              float64;

#elif defined (__APPLE__) 


  typedef u_int8_t       uint8;
  typedef int8_t         int8;
  
  typedef u_int16_t      uint16;
  typedef int16_t        int16;

  typedef u_int32_t      uint32;
  typedef int32_t        int32;
  
  typedef int64_t        int64;
  typedef u_int64_t      uint64;
  
  typedef float          float32;
  typedef double         float64;  

#elif defined(__linux) 
  #include <bits/wordsize.h>
  typedef signed short            int16;
  typedef signed char             int8;
  
  typedef unsigned short          uint16;
  typedef unsigned char           uint8;
  
  typedef float                   float32;
  typedef double                  float64;

  #if __WORDSIZE == 64
  //64 bit system
    typedef unsigned int            uint32;
    typedef signed int              int32;
    typedef signed long int       int64;
    typedef unsigned long int       uint64;
  #else
  // 32 Bit system
    typedef signed long long int    int64;
    typedef signed long int         int32;
  
    typedef unsigned long long int  uint64;
    typedef unsigned long int       uint32;
  #endif

  
#else 
  #if defined(_SGI_PLATFORM) 
      #include <inttypes.h> 
  #elif defined(_COMPAQ_PLATFORM) 
      #include <inttypes.h> 
  #else 
      #include <stdint.h> 
  #endif 

  typedef uint64_t            uint64; 
  typedef uint32_t            uint32; 
  typedef uint16_t            uint16; 

  typedef int64_t             int64; 
  typedef int32_t             int32; 
  typedef int16_t             int16; 

  typedef float               float32;
  typedef double              float64;
#endif


#define MXA_UINT8_TYPE 1
#define MXA_INT8_TYPE  2
#define MXA_UINT16_TYPE 4
#define MXA_INT16_TYPE  8
#define MXA_UINT32_TYPE 16
#define MXA_INT32_TYPE  32
#define MXA_UINT64_TYPE 64
#define MXA_INT64_TYPE  128
#define MXA_FLOAT32_TYPE 256
#define MXA_FLOAT64_TYPE  512

namespace MXATypes {

  const int8 Int8Type = 0x0F;
  const uint8 Uint8Type = 0x0F;
  const int16 Int16Type = 0x0F0F;
  const uint16 Uint16Type = 0x0F0F;
  const int32 Int32Type = 0x0F0F0F0F;
  const uint32 Uint32Type = 0x0F0F0F0F;
  const int64 Int64Type = 0x0F0F0F0F0F0F0F0Fll;
  const uint64 Uint64Type = 0x0F0F0F0F0F0F0F0Full;
  const float32 Float32Type = 0.0f;
  const float64 Float64Type = 0.0;

  typedef std::vector<int8> Int8Vector;
  typedef std::vector<uint8> Uint8Vector;
  typedef std::vector<int16> Int16Vector;
  typedef std::vector<uint16> Uint16Vector;
  typedef std::vector<int32> Int32Vector;
  typedef std::vector<uint32> Uint32Vector;
  typedef std::vector<int64> Int64Vector;
  typedef std::vector<uint64> Uint64Vector;
  typedef std::vector<float32> Float32Vector;
  typedef std::vector<float64> Float64Vector;

  typedef std::vector<uint64> H5Dimensions;

  typedef int32 MXAError;

} // namespace Types



#endif // MXA_BASIC_TYPES_H_
