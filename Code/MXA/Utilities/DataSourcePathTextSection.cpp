#include "DataSourcePathTextSection.h"

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataSourcePathTextSection::DataSourcePathTextSection(const std::string &text) :
  _text(text)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataSourcePathTextSection::~DataSourcePathTextSection()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string DataSourcePathTextSection::toString (int value, int8_t &ok)
{
  ok = true;
  std::string path = _preText;
  path.append(_text).append(_postText);
  return path;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataSourcePathTextSection::setPreText(const std::string &preText)
{
  this->_preText = preText;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataSourcePathTextSection::setPostText(const std::string &postText)
{
  this->_postText = postText;
}

