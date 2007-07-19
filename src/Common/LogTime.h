///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) Mike Jackson, IMTS 
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _LOGTIME_H_
#define _LOGTIME_H_

#include <time.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _MSC_VER
#define TimeType __time64_t
#define TimeFunc _time64
#define LocalTimeFunc _localtime64
#else
#define TimeType time_t
#define TimeFunc time
#define LocalTimeFunc localtime
#endif

#define DEBUG_OUT(function) \
  function() << "File: " << __FILE__ << "(" << __LINE__ << "): "
/**
* @brief Returns a Formatted String of the current Date/Time for logging
* purpose.
* @return A std:string of the current date/time
*/
inline std::string logTime() {
  TimeType long_time = 0;
  TimeFunc(&long_time);
  tm *t = 0;
  t = LocalTimeFunc(&long_time);
  std::stringstream ss;
  ss.setf(std::ios::fixed);
  ss.fill('0');
  ss  << "[" << std::setw(4) << t->tm_year + 1900 << "/" 
    << std::setw(2) << t->tm_mon + 1 << "/" 
    << std::setw(2) << t->tm_mday << " "
    << std::setw(2) << t->tm_hour << ":"
    << std::setw(2) << t->tm_min << ":"
    << std::setw(2) << t->tm_sec << "] ";
  return ss.str();
}


#endif //_LOGTIME_H_



