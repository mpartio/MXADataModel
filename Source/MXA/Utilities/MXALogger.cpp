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
MXALogger::MXALogger()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger::~MXALogger()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger::Pointer MXALogger::instance()
{
	static MXALogger::Pointer singleton;
	if (singleton.get() == NULL)
	  {
		std::cout << "MXALogger constructing singleton instance" << std::endl;
	    singleton.reset (new MXALogger() );
	  }
	  return singleton;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger_Implementation::MXALogger_Implementation() :
  _isFileBased(false),
  _fileName("NO FILE SET")
{
	std::cout << "MXALogger_Implementation constructing" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger_Implementation::~MXALogger_Implementation()
{
  this->_out.flush();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXALogger_Implementation::open(const std::string &fn, std::ios::openmode mode)
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
bool MXALogger_Implementation::close()
{
  _out.close();
  _isFileBased = false;
  return !_out.is_open();
}
