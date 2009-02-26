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
#ifndef ISOURCEPATHSECTION_H_
#define ISOURCEPATHSECTION_H_

#include <MXA/Common/DLLExport.h>
#include <MXA/Common/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>

#include <vector>

namespace DataSourcePathSection
{
   const std::string IntegerType("Integer");
}

/**
* @class IStringSection IStringSection.h Utilities/IStringSection.h
* @brief Abstract superclass that defines the methods to generate strings from the formatting information
* @author Mike Jackson
* @date Nov 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT IStringSection
{
  
  
  public:
    IStringSection() {};
    virtual ~IStringSection() {};

    virtual void setPreText(const std::string &preText) = 0;
    virtual void setPostText(const std::string &postText)= 0;
    
    
  //  virtual std::string toString( std::vector<uint32> &dims, int8 &ok) = 0;
    virtual std::string toString(int value, int8 &ok) = 0;
    
  private:
    
    IStringSection(const IStringSection&);    // Copy Constructor Not Implemented
    void operator=(const IStringSection&);  // Operator '=' Not Implemented
    
    
};


#endif /*ISOURCEPATHSECTION_H_*/
