///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _IAttribute_H_
#define _IAttribute_H_

#include "MXA/MXADLLExport.h"
#include <MXA/Common/MXATypeDefs.h>

#include <string>
#include <boost/any.hpp>
typedef boost::any Any;

class IAttributeWriter;

/**
 * @brief Base class for attributes. This is used because subclasses are usually
 * templated.
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT IAttribute
{
  public:
    IAttribute() {};
    virtual ~IAttribute() {};

    /**
     * @brief Returns the Key for this attribute
     */
    virtual std::string getKey() = 0;

    /**
     * @brief Returns the data value of the attribute as an Boost Any
     */
    virtual Any getValue() = 0;

    /**
     * @brief Converts the data value to an int32_t value
     */
    virtual int32_t valueToInt32() = 0;

    /**
     * @brief Converts the data value to a float value
     */
    virtual float valueToFloat32() = 0;

    /**
     * @brief Converts the data value to a double value
     */
    virtual double valueToFloat64() = 0;

    /**
     * @brief Converts the data value to a string
     */
    virtual std::string valueToString() = 0;

    /**
     * @brief Writes the Attribute using the IAttributeWriter Object
     */
    virtual int32_t write(int32_t locationId, std::string datasetPath, IAttributeWriter &writer) = 0;

  private:
    IAttribute(const IAttribute&);    //Not Implemented
    void operator=(const IAttribute&); //Not Implemented
};

#endif /*_IAttribute_H_*/
