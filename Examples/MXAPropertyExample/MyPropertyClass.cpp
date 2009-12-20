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
int32 MyPropertyClass::setProperty(const std::string &key, const std::string &value)
{
  SET_PROPERTY_BODY(MyPropertyNamespace, bool, BoolProperty, key, value);

  // Signed integer Properties
  SET_PROPERTY_BODY(MyPropertyNamespace, int8,  Int8Property,  key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, int16, Int16Property, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, int32, Int32Property, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, int64, Int64Property, key, value);
  
  // Unsigned Integer properties
  SET_PROPERTY_BODY(MyPropertyNamespace, uint8,  UInt8Property,  key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, uint16, UInt16Property, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, uint32, UInt32Property, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, uint64, UInt64Property, key, value);
  
  // String properties
  SET_STRING_PROPERTY_BODY(MyPropertyNamespace, std::string, StrProperty, key, value);
  // Floating point properties
  SET_PROPERTY_BODY(MyPropertyNamespace, float, FloatProperty, key, value);
  SET_PROPERTY_BODY(MyPropertyNamespace, double, DoubleProperty, key, value);
  
  return 0;
}
