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

#include <MXAConfiguration.h>

#if defined (HAVE_SYS_TIME_GETTIMEOFDAY)
#include <sys/time.h>
#endif

#if defined (HAVE_TIME_GETTIMEOFDAY) || defined (_MSC_VER)
#include <time.h>
#endif

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _MSC_VER
#if _MSC_VER < 1400
#define TimeType __time64_t
#define TimeFunc _time64
#else 
#define TimeType __time64_t
#define TimeFunc _time64
#endif
#else
#define TimeType time_t
#define TimeFunc time
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
  tm *t = 0x0;
  
#ifdef _MSC_VER
#if _MSC_VER < 1400
	t = _localtime64(&long_time);
#else 
  tm time;
  t = &time;
  errno_t tError = _localtime64_s(&time, &long_time);
#endif
#else  // Non windows platforms
	t = localtime(&long_time);
#endif

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

namespace MXA {

/**
 * @brief returns the number of milliseconds from a platform specified time.
 */
inline unsigned long long int getMilliSeconds()
{
#ifdef _MSC_VER
  return (unsigned long long int)(::clock());
#else 
  struct timeval t1;
  gettimeofday(&t1, NULL);
  unsigned long long int seconds ( t1.tv_sec );
  unsigned long long int microSec ( t1.tv_usec );
  seconds *= 1000;
  microSec /= 1000;
  return seconds + microSec; // Both in milliseconds at this point.
#endif
}


} // end Namespace MXA

#endif //_LOGTIME_H_
