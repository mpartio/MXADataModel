///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include "MXALogger.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger::MXALogger() :
  _isFileBased(false),
  _fileName("NO FILE SET")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger::~MXALogger()
{
  this->_out.flush();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXALogger::open(const std::string &fn, std::ios::openmode mode)
{
  this->_fileName = fn;
  _out.open(_fileName.c_str(),  std::ios::out | mode);
  if (_out.is_open() )
  {
    this->_isFileBased = true;
  }
  else
  {
    this->_fileName = "";
    this->_isFileBased = false;
  }
  return _isFileBased;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXALogger::close()
{
  _out.close();
  _isFileBased = false;
  return !_out.is_open();
}
