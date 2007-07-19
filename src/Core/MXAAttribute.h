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
#ifndef _MXAATTRIBUTE_H
#define _MXAATTRIBUTE_H

//Include MXA Headers
#include "Common/MXATypes.h"
#include "Common/MXATypeDefs.h"
#include "Base/IDataModelWriter.h"
#include "Base/IAttribute.h"
#include "Core/AttributeHelpers.h"

// C++ includes
#include <string>
#include <sstream>
#include <iostream>

//boost includes
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>



typedef boost::any Any;

/**
 * @class MXAAttribute MXAAttribute.h MXADataModel/MXAAttribute.h
 * @brief This class stores Key/Value type attributes usually used for MetaData
 * entries in the data model
 * @author mjackson
 * @date Mar 28, 2007
 * @version 
 */
class MXA_EXPORT MXAAttribute : public IAttribute
{
  public:
    typedef boost::shared_ptr<IAttributeHelper> IAttributeHelperPtr;

    /**
     * @brief Used to create an attribute that has a string as its data value
     * @param key The key for the attribute
     * @param value The data value for the attribute
     * @return A boost::shared_ptr containing an MXAAttribute object
     */
    static MXAAttributePtr createAttribute(const std::string &key, const std::string &value)
    {
      //std::cout << "Creating String Attribute" << std::endl;
      Any val(value);
      MXAAttributePtr attr (new MXAAttribute(key, val) );
      IAttributeHelperPtr conv (new StringAttributeHelper(const_cast<std::string&>(value) ) );
      attr->setAttributeHelper(conv);
      return attr;
    }

    /**
     * @brief Used to create an attribute that has a scalar value for its data value
     * @param key The key for the attribute
     * @param value The data value for the attribute
     * @return A boost::shared_ptr containing an MXAAttribute object
     */
    template<typename T>
    static MXAAttributePtr createAttribute(const std::string &key, T value)
    {
      //std::cout << "Creating " <<  typeid(T).name() << " attribute for " << value << std::endl;
      Any val(value);
      MXAAttributePtr attrPtr (new MXAAttribute(key, val) );
      MXAAttribute* attr = attrPtr.get();
      IAttributeHelperPtr conv (new NumberAttributeHelper<T>(value));
      attr->setAttributeHelper(conv);
      return attrPtr;
    }
    
    /**
     * @brief Used to create an attribute that has a Multi-dimensional array of values
     * @param key The key for the attribute
     * @param value The data value for the attribute
     * @param dims The dimensions of the array
     * @return A boost::shared_ptr containing an MXAAttribute object
     */
    template <typename T>
    static MXAAttributePtr createAttribute(const std::string &key, std::vector<T> value, std::vector<uint64> dims)
    {
      //std::cout << "creating Vector Attribute" << std::endl;
      Any val(value);
      MXAAttributePtr attr (new MXAAttribute( key, val) );
      IAttributeHelperPtr conv (new VectorAttributeHelper<T>( value, dims) );
      attr->setAttributeHelper(conv);
      return attr;  
    }
       
    virtual ~MXAAttribute();
    
    /**
     * @brief Returns the Key for this attribute
     */
    const std::string& getKey();
    
    /**
     * @brief Returns the data value of the attribute as an Boost Any
     */
    const Any& getValue();

    /**
     * @brief Converts the data value to an int32 value
     */
    int32 valueToInt32();

    /**
     * @brief Converts the data value to a float value
     */
    float valueToFloat32();
    
    /**
     * @brief Converts the data value to a string
     */
    std::string valueToString();
    
    /**
     * @brief Is the data value numeric in natur
     */
    bool isNumeric();
    
    /**
     * @brief Is the data value a string
     */
    bool isString();
    
    /**
     * @brief Can the data value be converted to a string
     */
    bool canConvertToString();
    
    /**
     * @brief Can the data value be converted to a scalar number
     */
    bool canConvertToNumber();
    
    /**
     * @brief Sets the AttributeHelper object
     */
    void setAttributeHelper(IAttributeHelperPtr converter) {
      this->_attrHelper = converter;
    }
    
    /**
     * @brief Writes the Attribute using the IAttributeWriter Object
     */
    int32 write(int32 locationId, std::string datasetPath, IAttributeWriter &writer)
    {
      return this->_attrHelper->writeAttribute(locationId, datasetPath, this->_key, writer);
    }
    
  protected:
    MXAAttribute();
    MXAAttribute(const std::string &key, const boost::any &value);
    
    std::string _key;  // The Key for this attribute
    Any _value;       // The data value for this attribute
    IAttributeHelperPtr _attrHelper; // The AttributeHelper Class to use
    
  private:
    MXAAttribute(const MXAAttribute&); //Not Implemented
    void operator=(const MXAAttribute&); //Not Implemented

};


#endif /*_MXAATTRIBUTE_H*/
