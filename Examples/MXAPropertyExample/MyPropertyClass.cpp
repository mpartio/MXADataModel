///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, Michael A. Jackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#include <Examples/MXAPropertyExample/MyPropertyClass.h>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MyPropertyClass::MyPropertyClass()
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MyPropertyClass::~MyPropertyClass()
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32_t MyPropertyClass::setProperty(const std::string &key, const std::string &value)
{
  SET_PROPERTY_BODY(MyPropertyNamespace, bool, BoolProperty, key, value);

  // Signed integer Properties
  SET_PROPERTY_BODY(MyPropertyNamespace, int8_t,  Int8Property,  key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, int16_t, Int16Property, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, int32_t, Int32Property, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, int64_t, Int64Property, key, value);
  
  // Unsigned Integer properties
  SET_PROPERTY_BODY(MyPropertyNamespace, uint8_t,  UInt8Property,  key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, uint16_t, UInt16Property, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, uint32_t, UInt32Property, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, uint64_t, UInt64Property, key, value);
  
  // String properties
  SET_STRING_PROPERTY_BODY(MyPropertyNamespace, std::string, StrProperty, key, value);
  // Floating point properties
  SET_PROPERTY_BODY(MyPropertyNamespace, float, FloatProperty, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, double, DoubleProperty, key, value);
  
  return 0;
}
