///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MXALOGGER_H_
#define MXALOGGER_H_


#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Common/LogTime.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/shared_ptr.hpp>

#define CHECK_PRECONDITION( stuff )\
  if(_out.is_open() == false){\
    std::cout << stuff; }\
    else {\
      _out << stuff; } return this;


class MXA_EXPORT MXALogger
{
  public:

    MXA_SHARED_POINTERS(MXALogger);
    MXA_NEW_MACRO(MXALogger);
    MXA_TYPE_MACRO(MXALogger);
    virtual ~MXALogger();

    /**
     * @brief Creates a new log file and opens the file for logging. If there was
     * an error creating or opening the file then a NULL shared_pointer is returned.
     *
     */
    static Pointer New(const std::string &logfile, std::ios::openmode mode = std::ios::app)
    {
      Pointer sharedPtr (new MXALogger(logfile));
      if (sharedPtr->is_open()== false)
      {
        sharedPtr.reset(static_cast<MXALogger*>(NULL));
      }
      return sharedPtr;
    }

    MXA_STRING_PROPERTY(LogFile, _logFile);

    /**
     * @brief Opens the given log file deleting any existing file
     * @param logFile The name of the log file to open
     * @param mode The mode to open the file. Default is to truncate any existing file
     * @return 0 On success;
     */
    int32 open(const std::string &logFile, std::ios::openmode mode = std::ios::trunc);

    /**
     * @brief Closes the log file to any more logging.
     */
    void close();

    bool is_open() { return _out.is_open(); }

    MXALogger* dateTime() { CHECK_PRECONDITION(logTime()) }
    MXALogger* warn() { CHECK_PRECONDITION("[Warning] ")}
    MXALogger* error() { CHECK_PRECONDITION("[Error] ")}
    MXALogger* log(uint8 v) { CHECK_PRECONDITION(v) }
    MXALogger* log(int8 v) { CHECK_PRECONDITION(v) }
    MXALogger* log(uint16 v) { CHECK_PRECONDITION(v) }
    MXALogger* log(uint32 v) { CHECK_PRECONDITION(v) }
    MXALogger* log(int32 v) { CHECK_PRECONDITION(v) }
    MXALogger* log(uint64 v) { CHECK_PRECONDITION(v) }
    MXALogger* log(int64 v) { CHECK_PRECONDITION(v) }
    MXALogger* log(double v)  { CHECK_PRECONDITION(v) }
    MXALogger* log(const std::string &v) { CHECK_PRECONDITION(v) }
    MXALogger* log(char* v) { CHECK_PRECONDITION(v) }
    MXALogger* log(std::stringstream &v) { CHECK_PRECONDITION(v.str()) }
    MXALogger* endl() { CHECK_PRECONDITION(std::endl)}

  protected:
    MXALogger();
    explicit MXALogger(const std::string &logfile, std::ios::openmode mode = std::ios::app);



  private:
    std::ofstream _out;
    std::string _logFile;


    MXALogger(const MXALogger&);    // Copy Constructor Not Implemented
    void operator=(const MXALogger&);  // Operator '=' Not Implemented
};

#endif /* MXALOGGER_H_ */
