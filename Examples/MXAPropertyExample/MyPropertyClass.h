///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, Michael A. Jackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _MYPROPERTYCLASS_H_
#define _MYPROPERTYCLASS_H_


#include "MXA/MXA.h"
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IImportProperty.h>

//C++ includes
#include <string>


// Put our property keys into some constants inside the MyPropertyClass namespace
namespace MyPropertyNamespace {
  MXA_PROPERTY_CONSTANT(BoolProperty)
  MXA_PROPERTY_CONSTANT(Int8Property)
  MXA_PROPERTY_CONSTANT(Int16Property)
  MXA_PROPERTY_CONSTANT(Int32Property)
  MXA_PROPERTY_CONSTANT(Int64Property)
  MXA_PROPERTY_CONSTANT(UInt8Property)
  MXA_PROPERTY_CONSTANT(UInt16Property)
  MXA_PROPERTY_CONSTANT(UInt32Property)
  MXA_PROPERTY_CONSTANT(UInt64Property)

  MXA_PROPERTY_CONSTANT(StrProperty)

  MXA_PROPERTY_CONSTANT(FloatProperty)
  MXA_PROPERTY_CONSTANT(DoubleProperty)

}; // End Namespace

/**
* @brief
* @version $Revision: 1.2 $
* @author Michale A. Jackson
*/
class MyPropertyClass
{
public:
  MyPropertyClass();
  virtual ~MyPropertyClass();

// -----------------------------------------------------------------------------
//  Start the Properties Declarations using our macros from Common/MXAMacros.h
//  These macros will create the named get/set methods for each property
// -----------------------------------------------------------------------------
  // Boolean Property
  MXA_INSTANCE_PROPERTY(bool, BoolProperty)

  // Character type properties - These are essentially single character string properties
  // Attempting to use either int8_t or uint8_t types to store actual integer values has undefined
  // results. Try using int16_t or uint16_t to store these types of values instead.
  MXA_INSTANCE_PROPERTY(uint8_t, UInt8Property)
  MXA_INSTANCE_PROPERTY(int8_t,  Int8Property)

  // Signed integer Properties
  MXA_INSTANCE_PROPERTY(int16_t, Int16Property)
  MXA_INSTANCE_PROPERTY(int32_t, Int32Property)
  MXA_INSTANCE_PROPERTY(int64_t, Int64Property)

  // Unsigned Integer properties
  MXA_INSTANCE_PROPERTY(uint16_t, UInt16Property)
  MXA_INSTANCE_PROPERTY(uint32_t, UInt32Property)
  MXA_INSTANCE_PROPERTY(uint64_t, UInt64Property)

  // String properties
  MXA_INSTANCE_STRING_PROPERTY( StrProperty)
  // Floating point properties
  MXA_INSTANCE_PROPERTY(float, FloatProperty)
  MXA_INSTANCE_PROPERTY(double, DoubleProperty)

  // -----------------------------------------------------------------------------
  //  If you want to add a generic 'set' method that takes a string key and string
  //   value then implement the following method
  // -----------------------------------------------------------------------------
  virtual int32_t setProperty(const std::string &key, const std::string &value);


  // -----------------------------------------------------------------------------
  //  If you want to implement a generic "get" method that will retrieve the proper
  //  type of value from the object then you can use a C++ template function.
  // -----------------------------------------------------------------------------
  /**
  * @brief returns the value of the property through the argument 'value' which is passed
  *  by reference.
  * @param key The property key
  * @param value The variable to store the property into
  * @return Success Code. Zero means there was a problem with the retrieval. 1 Means the
  * retrieval was successful. There may also be error messages printed to the std::cout device
  */
    template<typename T>
    int32_t getProperty(const std::string &key, T &value)
    {
      GET_PROPERTY_BODY(MyPropertyNamespace, T, BoolProperty, key, value);

      // Signed integer Properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int8Property, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int16Property, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int32Property, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int64Property, key, value);

      // Unsigned Integer properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt8Property,  key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt16Property, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt32Property, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt64Property, key, value);

      // String properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, StrProperty, key, value);
      // Floating point properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, FloatProperty, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, DoubleProperty, key, value);
      return 0;
    }

  private:
    bool _boolProp;
    int8_t _int8_tProp;
    int16_t _int16_tProp;
    int32_t _int32_tProp;
    int64_t _int64_tProp;
    uint8_t _uint8_tProp;
    uint16_t _uint16_tProp;
    uint32_t _uint32_tProp;
    uint64_t _uint64_tProp;
    std::string _strProp;
    float _floatProp;
    double _doubleProp;


      MyPropertyClass(const MyPropertyClass&);   //Copy Constructor Not Implemented
      void operator=(const MyPropertyClass&); //Copy Assignment Not Implemented

};


#endif /* _MYPROPERTYCLASS_H_ */

