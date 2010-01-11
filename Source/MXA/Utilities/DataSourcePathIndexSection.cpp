#include "DataSourcePathIndexSection.h"

#include <iomanip>

#include <sstream>
#include <iostream>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataSourcePathIndexSection::DataSourcePathIndexSection(  int32_t dimIndex,
                                                         const std::string  &fillChar,
                                                         int32_t width,
                                                         const std::string &numericType) :
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
std::string DataSourcePathIndexSection::toString (int value, int8_t &ok)
{
//  if (_index > dims.size() -1 )
//  {
//    ok = false;
//    return std::string(); // Return an empty string
//  }
//

  // Sanity check first.
  if (this->_fillChar.empty() == true && this->_width != -1)
  {
    ok = 0;
    std::string str;
    return str;
  }

  if (this->_width == -1 && this->_fillChar.empty() == false)
  {
    ok = 0;
    std::string str;
    return str;
  }

  std::stringstream strStream;
  strStream.clear();

  if (this->_fillChar.empty() == false
      && this->_fillChar.size() == 1
      && this->_width != -1)
  {
    strStream.setf(std::ios::fixed);
    strStream.fill(_fillChar[0]);
    strStream << _preText << std::setw(_width) << value << this->_postText;
    ok = true;
  }
  else if (this->_fillChar.empty() == true
           && this->_width == -1)
  {
    strStream << _preText << value << this->_postText;
    ok = true;
  }
  else
  {
    ok = false;
    std::string str;
    return str;
  }
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


