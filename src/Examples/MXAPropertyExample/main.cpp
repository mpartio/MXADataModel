///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, Michael A. Jackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include <Examples/MXAPropertyExample/MyPropertyClass.h>

#define PRINT_RESULTS(prpty, var)\
std::cout << #prpty << ": " << var << std::endl;


int main (int argc, char const *argv[])
{
  std::cout << "MXAProperty Example Starting" << std::endl;
  MyPropertyClass pclass;
    std::cout << "--------------- Using named set methods ---------------------" << std::endl;
  bool _boolProp = true;
  int8 _int8Prop = 'A';
  int16 _int16Prop = -34;
  int32 _int32Prop = -300;  
  int64 _int64Prop = -9832498342ll;
  uint8 _uint8Prop = 'Z';
  uint16 _uint16Prop = 234;
  uint32 _uint32Prop = 2342342;
  uint64 _uint64Prop = 908324923490324llu;
  std::string _strProp("The String Property");
  float _floatProp = 3.1415927f;
  double _doubleProp = 234234.234234234;
  
  
  // Set all the properties using their named methods
  pclass.setBoolProperty(_boolProp);
  pclass.setInt8Property(_int8Prop);
  pclass.setInt16Property(_int16Prop);
  pclass.setInt32Property(_int32Prop);
  pclass.setInt64Property(_int64Prop);
  
  pclass.setUInt8Property(_uint8Prop);
  pclass.setUInt16Property(_uint16Prop);
  pclass.setUInt32Property(_uint32Prop);
  pclass.setUInt64Property(_uint64Prop);
  
  pclass.setFloatProperty(_floatProp);
  pclass.setDoubleProperty(_doubleProp);
  
  pclass.setStrProperty(_strProp);
  
  
  // Get all the properties using their named methods
  _boolProp = pclass.getBoolProperty();
  _int8Prop = pclass.getInt8Property();
  _int16Prop = pclass.getInt16Property();
  _int32Prop = pclass.getInt32Property();  
  _int64Prop = pclass.getInt64Property();
  _uint8Prop = pclass.getUInt8Property();
  _uint16Prop = pclass.getUInt16Property();
  _uint32Prop = pclass.getUInt32Property();
  _uint64Prop = pclass.getUInt64Property();
  _strProp = pclass.getStrProperty();
  _floatProp = pclass.getFloatProperty();
  _doubleProp = pclass.getDoubleProperty();
  PRINT_RESULTS(BoolProperty, _boolProp)
  PRINT_RESULTS(Int8Property, _int8Prop)
  PRINT_RESULTS(Int16Property, _int16Prop)
  PRINT_RESULTS(Int32Property, _int32Prop)
  PRINT_RESULTS(Int64Property, _int64Prop)
  PRINT_RESULTS(UInt8Property, _uint8Prop)
  PRINT_RESULTS(UInt16Property, _uint16Prop)
  PRINT_RESULTS(UInt32Property, _uint32Prop)
  PRINT_RESULTS(UInt64Property, _uint64Prop)
  PRINT_RESULTS(FloatProperty, _floatProp)
  PRINT_RESULTS(DoubleProperty, _doubleProp)
  PRINT_RESULTS(StrProperty, _strProp)
  
  std::cout << "--------------- Using 'setProperty(string, string)' ---------------------" << std::endl;
  // Set the properties using the generic 'set' method
  pclass.setProperty(MyPropertyNamespace::BoolProperty, "1");
  pclass.setProperty(MyPropertyNamespace::Int8Property, "G");
  pclass.setProperty(MyPropertyNamespace::Int16Property, "-2342");
  pclass.setProperty(MyPropertyNamespace::Int32Property, "-987543");
  pclass.setProperty(MyPropertyNamespace::Int64Property, "-65478934");
  
  pclass.setProperty(MyPropertyNamespace::UInt8Property, "P");
  pclass.setProperty(MyPropertyNamespace::UInt16Property, "23654");
  pclass.setProperty(MyPropertyNamespace::UInt32Property, "41234342");
  pclass.setProperty(MyPropertyNamespace::UInt64Property, "9897842323");
  
  pclass.setProperty(MyPropertyNamespace::FloatProperty, "4123.4342");
  pclass.setProperty(MyPropertyNamespace::DoubleProperty, "9897.842323");
  
  pclass.setProperty(MyPropertyNamespace::StrProperty, "Other String Property");
  
  int32 success = 0;
  success = pclass.getProperty<bool>(MyPropertyNamespace::BoolProperty, _boolProp);
  PRINT_RESULTS(BoolProperty, _boolProp)
  success = pclass.getProperty<int8>(MyPropertyNamespace::Int8Property, _int8Prop);
  PRINT_RESULTS(Int8Property, _int8Prop)
  success = pclass.getProperty<int16>(MyPropertyNamespace::Int16Property, _int16Prop);
  PRINT_RESULTS(Int16Property, _int16Prop)
  success = pclass.getProperty<int32>(MyPropertyNamespace::Int32Property, _int32Prop);
  PRINT_RESULTS(Int32Property, _int32Prop)
  success = pclass.getProperty<int64>(MyPropertyNamespace::Int64Property, _int64Prop);
  PRINT_RESULTS(Int64Property, _int64Prop)
  
  success = pclass.getProperty<uint8>(MyPropertyNamespace::UInt8Property, _uint8Prop);
  PRINT_RESULTS(UInt8Property, _uint8Prop)
  success = pclass.getProperty<uint16>(MyPropertyNamespace::UInt16Property, _uint16Prop);
  PRINT_RESULTS(UInt16Property, _uint16Prop)
  success = pclass.getProperty<uint32>(MyPropertyNamespace::UInt32Property, _uint32Prop);
  PRINT_RESULTS(UInt32Property, _uint32Prop)
  success = pclass.getProperty<uint64>(MyPropertyNamespace::UInt64Property, _uint64Prop);
  PRINT_RESULTS(UInt64Property, _uint64Prop)
  success = pclass.getProperty<float>(MyPropertyNamespace::FloatProperty, _floatProp);
  PRINT_RESULTS(FloatProperty, _floatProp)
  success = pclass.getProperty<double>(MyPropertyNamespace::DoubleProperty, _doubleProp);
  PRINT_RESULTS(DoubleProperty, _doubleProp)
  std::string emptyString;
  success = pclass.getProperty<std::string>(MyPropertyNamespace::StrProperty, emptyString);
  PRINT_RESULTS(StrProperty, emptyString)
  
  std::cout << "MXAProperty Example Ending" << std::endl;
  
  return 0;
}



