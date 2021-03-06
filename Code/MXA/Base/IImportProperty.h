///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) Mike Jackson,
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _IIMPORT_PROPERTY_H_
#define _IIMPORT_PROPERTY_H_

///////////////////////////////////////////////////////////////////////////////

#include "MXA/MXA.h"

/**
 * @brief
 * @author Mike Jackson
 * @date Septermber 2008
 * @version Revision: 1.2
 *
 */
class MXA_EXPORT IImportProperty
{

public:
    MXA_TYPE_MACRO(IImportProperty)
  IImportProperty(){};
  virtual ~IImportProperty(){};

  virtual int32_t setProperty(const std::string &key, const std::string &value) = 0;

  virtual int32_t getProperty(const std::string &key, std::string &value) = 0;


};

#endif /*_IIMPORT_PROPERTY_H_*/
