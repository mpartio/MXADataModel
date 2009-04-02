///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#include "MXALogger.h"
#include <MXA/Common/LogTime.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger::MXALogger()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger::MXALogger(const std::string &logfile, std::ios::openmode mode)
{
  this->_logFile = logfile;
  _out.open(_logFile.c_str(),  std::ios::out | mode);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXALogger::~MXALogger()
{
   if (_out.is_open() == true)
   {
     _out.close();
   }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 MXALogger::open(const std::string &logFile, std::ios::openmode mode)
{
   this->setLogFile(logFile);
   // Close any existing log file now
   if (_out.is_open() == true)
   {
     _out.close();
   }

   // Open a new file
   _out.open(_logFile.c_str(), std::ios::out | mode);
  if (_out.is_open() == false)
  {
    std::cerr << logTime() << "Error - Could not open output file" << std::endl;
    return -1;
  }
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXALogger::close()
{
  if (_out.is_open() == true)
  {
    _out.close();
  }
}

#if 0
std::ofstream csvFile(csvFileName.c_str(), std::ios::out);
if (csvFile.is_open() == false)
{
  std::cout << logTime() << "Error - Could not open output file" << std::endl;
  return -1;
}
this->printTable(csvFile, ',');
csvFile.close();

#endif
