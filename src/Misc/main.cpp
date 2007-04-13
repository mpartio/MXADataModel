//Include MXA Headers
#include "Headers/MXATypes.h"

// C++ includes
#include <string>
#include <sstream>
#include <iostream>

#include "hdf5.h"

template<typename T>
hid_t HDFTypeForPrimitive(T value)
{
  if (typeid(value) == typeid(char)) return H5T_NATIVE_INT8;
  if (typeid(value) == typeid(signed char)) return H5T_NATIVE_INT8;  
  if (typeid(value) == typeid(unsigned char)) return H5T_NATIVE_UINT8;
  
  if (typeid(value) == typeid(short int)) return H5T_NATIVE_INT16;
  if (typeid(value) == typeid(signed short int)) return H5T_NATIVE_INT16;
  if (typeid(value) == typeid(unsigned short int)) return H5T_NATIVE_UINT16;
  
  if (typeid(value) == typeid(int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(signed int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(long int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(signed long int)) return H5T_NATIVE_INT32;
  
  if (typeid(value) == typeid(unsigned int)) return H5T_NATIVE_UINT32;
  if (typeid(value) == typeid(unsigned long int)) return H5T_NATIVE_UINT32;
  
  if (typeid(value) == typeid(long long int)) return H5T_NATIVE_INT64;
  if (typeid(value) == typeid(signed long long int)) return H5T_NATIVE_INT64;
  if (typeid(value) == typeid(unsigned long long int)) return H5T_NATIVE_UINT64;
  
  
  if (typeid(value) == typeid(float)) return H5T_NATIVE_FLOAT;
  if (typeid(value) == typeid(double)) return H5T_NATIVE_DOUBLE;
  return -1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main (int argc, char const* argv[])
{
  
  char c8 = 0;
  signed char sc8 = 0;
  unsigned char uc8 = 0;
  int8  i8  = 0;
  uint8 ui8 = 1;
  int16 i16 = 0;
  uint16 ui16 = 0;
   signed long int i32 = 0;
  uint32 ui32 = 0;
  signed long long int i64 = 0;
  
  unsigned long long int ui64 = 0;
  float32 f32 = 0.0;
  float64 f64 = 0.0;
  
  unsigned short s = 0;
  
std::cout << "char          : " << HDFTypeForPrimitive(c8) << std::endl;
std::cout << "signed char   : " << HDFTypeForPrimitive(sc8) << std::endl;
std::cout << "unsigned char : " << HDFTypeForPrimitive(uc8) << std::endl;
  std::cout << "int8     : " << HDFTypeForPrimitive(i8) << std::endl;
  std::cout << "uint8    : " << HDFTypeForPrimitive(ui8) << std::endl;
  std::cout << "int16    : " << HDFTypeForPrimitive(i16) << std::endl;
  std::cout << "uint16   : " << HDFTypeForPrimitive(ui16) << std::endl;
  std::cout << "int32    : " << HDFTypeForPrimitive(i32) << std::endl;
  std::cout << "uint32   : " << HDFTypeForPrimitive(ui32) << std::endl;
  std::cout << "int64    : " << HDFTypeForPrimitive(i64) << std::endl;
  std::cout << "uint64   : " << HDFTypeForPrimitive(ui64) << std::endl;
  std::cout << "float32  : " << HDFTypeForPrimitive(f32) << std::endl;
  std::cout << "float64  : " << HDFTypeForPrimitive(f64) << std::endl;

  std::cout << "short    : " << HDFTypeForPrimitive(s) << std::endl;
  
  char ff = 0xFF;
 // std::cout << "Char (FF) : " << (int)ff << std::endl;
  return 0;
}




