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
#ifndef STRINGUTILITIES_H_
#define STRINGUTILITIES_H_

//-- MXA Includes
#include "Headers/DLLExport.h"


//-- STL Includes
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <iostream>
#include <algorithm>


//TODO: Create Test Suite for these Methods
  
class StringUtils {
  
public:

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template<typename T>
static std::string typeForPrimitive(T value)
{
  if (typeid(value) == typeid(char)) return "H5T_NATIVE_INT8";
  if (typeid(value) == typeid(signed char)) return "H5T_NATIVE_INT8";  
  if (typeid(value) == typeid(unsigned char)) return "H5T_NATIVE_UINT8";
  
  if (typeid(value) == typeid(short int)) return "H5T_NATIVE_INT16";
  if (typeid(value) == typeid(signed short int)) return "H5T_NATIVE_INT16";
  if (typeid(value) == typeid(unsigned short int)) return "H5T_NATIVE_UINT16";
  
  if (typeid(value) == typeid(int)) return "H5T_NATIVE_INT32";
  if (typeid(value) == typeid(signed int)) return "H5T_NATIVE_INT32";
  if (typeid(value) == typeid(long int)) return "H5T_NATIVE_INT32";
  if (typeid(value) == typeid(signed long int)) return "H5T_NATIVE_INT32";
  
  if (typeid(value) == typeid(unsigned int)) return "H5T_NATIVE_UINT32";
  if (typeid(value) == typeid(unsigned long int)) return "H5T_NATIVE_UINT32";
  
  if (typeid(value) == typeid(long long int)) return "H5T_NATIVE_INT64";
  if (typeid(value) == typeid(signed long long int)) return "H5T_NATIVE_INT64";
  if (typeid(value) == typeid(unsigned long long int)) return "H5T_NATIVE_UINT64";
  
  
  if (typeid(value) == typeid(float)) return "H5T_NATIVE_FLOAT";
  if (typeid(value) == typeid(double)) return "H5T_NATIVE_DOUBLE";
  
  std::cout << "Error: HDFTypeForPrimitive - Unknown Type: " << typeid(value).name() << std::endl;
  return "";
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template <typename T>
static bool stringToNum(T& t,const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
static MXA_EXPORT std::string numToString(int num) 
{
	std::stringstream converter;
  converter << num;
  return converter.str();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template<typename T>
static std::string numToString(T num) 
{
  std::stringstream converter;
  converter << num;
  return converter.str();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
static MXA_EXPORT std::string numToString(size_t num)
{
  std::stringstream converter;
  converter << num;
  return converter.str();
}
  
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
static MXA_EXPORT void ensureRightSlash(std::string& path)
{
  if (! (path.at(path.length()-1) == '/')) {  
    path += "/";
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
//TODO: Fix this to be more efficient
/* Returns a copy of the list. Not memory efficient at all */
static MXA_EXPORT std::list<std::string> splitString(std::string sep, std::string str)
{
  std::list<std::string> strings;
  int32 sep_length = sep.length();
  std::string::iterator iter = std::search(str.begin(), str.end(), sep.begin(), sep.end());
  strings.push_back(std::string(str.begin(), iter));
  std::string::iterator iter_start;
  while(iter != str.end()) {
    iter_start = iter+sep_length;
    iter = std::search(iter_start, str.end(), sep.begin(), sep.end());
    strings.push_back(std::string(iter_start, iter));
  }
  return strings;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
static MXA_EXPORT std::string joinList(std::string sep, std::list<std::string> strs)
{
  std::string result("");
  std::list<std::string>::const_iterator iter = strs.begin();
  result += *iter;
  if (iter != strs.end()) {
    iter++;
    for (; iter!=strs.end(); iter++) {
        #if DEBUG
        std::cout << "JOINING Current: " << result << " To: / + " << *iter << std::endl;
        #endif
      result += sep + *iter;
    }
  }
  return result;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
static MXA_EXPORT std::string joinList(std::string sep, std::list<int> nums)
{
  std::string result("");
  
  std::list<int>::const_iterator iter = nums.begin();
  result += numToString(*iter);
  if (iter != nums.end()) {
    iter++;
    for (; iter!=nums.end(); iter++) {
      result += sep + numToString(*iter);
    }
  }
  return result;
}
  
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
static MXA_EXPORT std::string makePath(std::string parentPath, std::string name)
{
  std::string result;
  if (parentPath == "/") {
    result = name;
  } else {
    StringUtils::ensureRightSlash(parentPath);
    result = parentPath + name;
  }
  return result;
} 

// -----------------------------------------------------------------------------
//  Determines if a string ends with another string
// -----------------------------------------------------------------------------
static MXA_EXPORT bool endsWith(std::string &base, std::string ending) {
  std::string::size_type len = base.length();
  std::string::size_type eLen = ending.length();
  std::string::size_type npos = len - eLen;
  std::string::size_type pos = base.find(ending, npos);  
  return   ( pos != std::string::npos);
}

};


#endif /*STRINGUTILITIES_H_*/
