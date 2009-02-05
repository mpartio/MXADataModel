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


#include <Common/MXATypes.h>
#include <Common/MXASetGetMacros.h>
#include <Base/IImportProperty.h>

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
* @version $Revision: 1.1 $
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
  MXA_PROPERTY(bool, BoolProperty, _boolProp)

  // Character type properties - These are essentially single character string properties
  // Attempting to use either int8 or uint8 types to store actual integer values has undefined
  // results. Try using int16 or uint16 to store these types of values instead.
  MXA_PROPERTY(uint8, UInt8Property,  _uint8Prop)
  MXA_PROPERTY(int8,  Int8Property,  _int8Prop)

  // Signed integer Properties
  MXA_PROPERTY(int16, Int16Property, _int16Prop)
  MXA_PROPERTY(int32, Int32Property, _int32Prop)
  MXA_PROPERTY(int64, Int64Property, _int64Prop)

  // Unsigned Integer properties
  MXA_PROPERTY(uint16, UInt16Property, _uint16Prop)
  MXA_PROPERTY(uint32, UInt32Property, _uint32Prop)
  MXA_PROPERTY(uint64, UInt64Property, _uint64Prop)

  // String properties
  MXA_PROPERTY(std::string, StrProperty, _strProp)
  // Floating point properties
  MXA_PROPERTY(float, FloatProperty, _floatProp)
  MXA_PROPERTY(double, DoubleProperty, _doubleProp)

  // -----------------------------------------------------------------------------
  //  If you want to add a generic 'set' method that takes a string key and string
  //   value then implement the following method
  // -----------------------------------------------------------------------------
  virtual int32 setProperty(const std::string &key, const std::string &value);


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
    int32 getProperty(const std::string &key, T &value)
    {
      GET_PROPERTY_BODY(MyPropertyNamespace, T, BoolProperty, _boolProp, key, value);

      // Signed integer Properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int8Property, _int8Prop, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int16Property, _int16Prop, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int32Property, _int32Prop, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int64Property, _int64Prop, key, value);

      // Unsigned Integer properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt8Property, _uint8Prop,  key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt16Property, _uint16Prop, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt32Property, _uint32Prop, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt64Property, _uint64Prop, key, value);

      // String properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, StrProperty, _strProp, key, value);
      // Floating point properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, FloatProperty, _floatProp, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, DoubleProperty, _doubleProp, key, value);
      return 0;
    }

  private:
    bool _boolProp;
    int8 _int8Prop;
    int16 _int16Prop;
    int32 _int32Prop;
    int64 _int64Prop;
    uint8 _uint8Prop;
    uint16 _uint16Prop;
    uint32 _uint32Prop;
    uint64 _uint64Prop;
    std::string _strProp;
    float _floatProp;
    double _doubleProp;


      MyPropertyClass(const MyPropertyClass&);   //Copy Constructor Not Implemented
      void operator=(const MyPropertyClass&); //Copy Assignment Not Implemented

};


#endif /* _MYPROPERTYCLASS_H_ */

