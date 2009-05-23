///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _MXALogger_H_
#define _MXALogger_H_

#ifdef LOGGER_NAMESPACE
#error REMOVE THE DEFINITION YOU DONT NEED IT ANYMORE
#endif

#include <MXA/Common/MXATypes.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Common/MXASetGetMacros.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/noncopyable.hpp>

#define DECLARE_MXA_LOGGER(var)\
	MXALogger::Pointer var;

#define MXA_LOGGER_INSTANCE(var)\
	var = MXALogger::instance();


#define LOGGER_INSTANCE()\
	MXALogger::Pointer MXA_Global_Logger = MXALogger::instance();

#define DECLARE_MXA_DEFAULT_LOGGER MXALogger::Pointer MXA_Global_Logger;
#define MXA_DEFAULT_INSTANCE       MXA_Global_Logger = MXALogger::instance();
#define mxa_log                    MXA_Global_Logger->mxaLogger


#define CHECK_PRECONDITION( stuff )\
  if(_isFileBased == false){\
    std::cout << stuff; return std::cout;}\
    else {\
      _out << (stuff);  return _out; }


/**
* @class MXALogger MXALogger.h MXA/Utilities/MXALogger.h
* @brief
* @author Michael A. Jackson for BlueQuartz Software
* @date May 22, 2009
* @version $Revision$
*/
class MXA_EXPORT MXALogger_Implementation : private boost::noncopyable
{

  public:
	  MXALogger_Implementation();
    virtual ~MXALogger_Implementation();

    MXA_INSTANCE_PROPERTY(bool, IsFileBased, _isFileBased)
    MXA_INSTANCE_STRING_PROPERTY(FileName, _fileName)

    bool open(const std::string &fn, std::ios::openmode mode = std::ios::trunc);
    bool close();

    std::ostream& dateTime() { CHECK_PRECONDITION(logTime()) }
    std::ostream& warn() { CHECK_PRECONDITION("[Warning] ")}
    std::ostream& error() { CHECK_PRECONDITION("[Error] ")}
    std::ostream& operator<<(uint8 v) { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(int8 v) { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(uint16 v) { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(uint32 v) { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(int32 v) { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(uint64 v) { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(int64 v) { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(double v)  { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(const std::string &v) { CHECK_PRECONDITION(v) }
    std::ostream& operator<<(const char* v) { CHECK_PRECONDITION(v) }

  private:
    std::ofstream _out;

   // MXALogger_Implementation(const MXALogger_Implementation&);    // Copy Constructor Not Implemented
    void operator=(const MXALogger_Implementation&);  // Operator '=' Not Implemented

};

class MXA_EXPORT MXALogger
{
  public:
	MXA_SHARED_POINTERS(MXALogger);
	virtual ~MXALogger();

    static MXALogger::Pointer instance();

    MXALogger_Implementation mxaLogger;

  private:
    MXALogger();

};

#endif
