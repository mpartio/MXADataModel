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
* @version $Revision: 1.3 $
*/
class MXA_EXPORT IAbstractDataset : public IDataFileIO 
{
  public:
    IAbstractDataset() {}
    virtual ~IAbstractDataset() {}
    
    /**
     * @brief Sets the AbstractData shared pointer
     * @param dataPtr A MXAAbstractDataPtr object
     */
    virtual setData(MXAAbstractDataPtr dataPtr) = 0;
   
    /**
     * @brief Returns the MXAAbstractDataPtr object
     */
    virtual MXAAbstractDataPtr getData() = 0;
    
    /**
     * @brief Adds an attribute to the data set
     * @param attribute The attribute to add to the data set.
     */
    virtual addAttribute(MXAAbstractAttributePtr attribute) = 0;
    
    /**
     * @brief Removes an attribute from the data set
     * @param attributeKey The Attribute to remove based on its 'key' value
     */
    virtual removeAttribute(const std::string &attributeKey) = 0;
    
    /**
     * @brief Returns an Attribute from the dataset
     * @param attributeKey The Attribute to return based on its 'key' value. If
     * the attribute is not found then a NULL wrapped pointer is returned.
     */
    virtual getAttribute (const std::string &attributeKey) = 0;
        
  protected:  
  
  private:
      IAbstractDataset(const IAbstractDataset&);    //Not Implemented
      void operator=(const IAbstractDataset&); //Not Implemented
  
};

#endif //_IAbstractDataset_h_


