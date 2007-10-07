///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _IMXAATTRIBUTE_H_
#define _IMXAATTRIBUTE_H_

#include <Common/DLLExport.h>


/**
 * @brief Base class for attributes. This is used becase subclasses are usually
 * templated.
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.3 $
 *  
 */
class MXA_EXPORT IAttribute 
{
  public:
    IAttribute() {};
    virtual ~IAttribute() {};
     
  private:
    IAttribute(const IAttribute&);    //Not Implemented
    void operator=(const IAttribute&); //Not Implemented
};

#endif /*_IMXAATTRIBUTE_H_*/
