///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _IAbstractDataset_h_
#define _IAbstractDataset_h_

#include <Experimental/IDataFileIO.h>
#include <Experimental/MXAAbstractData.hpp>

typedef boost::shared_ptr<MXAAbstractData>    MXAAbstractDataPtr;

#include <Experimental/MXAAbstractAttribute.hpp>

typedef boost::shared_ptr<MXAAbstractAttribute>    MXAAbstractAttributePtr;
/**
* @class IAbstractDataset IAbstractDataset.h PathToHeader/IAbstractDataset.h
* @brief 
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.1 $
*/
class IAbstractDataset : public IDataFileIO 
{
  public:
    IAbstractDataset() {}
    virtual ~IAbstractDataset() {}
    
    virtual setData(MXAAbstractDataPtr dataPtr) = 0;
    virtual MXAAbstractDataPtr getData() = 0;
    
    virtual addAttribute(MXAAbstractAttributePtr attribute) = 0;
    virtual removeAttribute(const std::string &attributeKey) = 0;
    virtual getAttribute (const std::string &attributeKey) = 0;
        
  protected:  
  
  private:
      IAbstractDataset(const IAbstractDataset&);    //Not Implemented
      void operator=(const IAbstractDataset&); //Not Implemented
  
};

#endif //_IAbstractDataset_h_


