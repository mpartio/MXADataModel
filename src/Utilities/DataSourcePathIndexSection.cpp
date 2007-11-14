#include "DataSourcePathIndexSection.h"

#include <iomanip>

#include <sstream>
#include <iostream>


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataSourcePathIndexSection::DataSourcePathIndexSection(  int32 dimIndex, int8 fillChar, int32 width, const std::string &numericType) :
  _index(dimIndex),
  _fillChar (fillChar),
  _width (width),
  _numericType (numericType)
 {}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataSourcePathIndexSection::~DataSourcePathIndexSection()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
//std::string DataSourcePathIndexSection::toString( std::vector<uint32> &dims, int8 &ok)
std::string DataSourcePathIndexSection::toString (int value, int8 &ok)
{ 
//  if (_index > dims.size() -1 )
//  {
//    ok = false;
//    return std::string(); // Return an empty string
//  }
//  
  std::stringstream strStream;
  strStream.clear();
  strStream.setf(std::ios::fixed);
  strStream.fill(_fillChar);
  
  strStream << _preText << std::setw(_width) << value << this->_postText;
  ok = true;
  return strStream.str();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataSourcePathIndexSection::setPreText(const std::string &preText)
{
  this->_preText = preText;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataSourcePathIndexSection::setPostText(const std::string &postText)
{
  this->_postText = postText;
}


