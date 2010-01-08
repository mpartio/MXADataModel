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
  int8_t _int8_tProp = 'A';
  int16_t _int16_tProp = -34;
  int32_t _int32_tProp = -300;  
  int64_t _int64_tProp = -9832498342ll;
  uint8_t _uint8_tProp = 'Z';
  uint16_t _uint16_tProp = 234;
  uint32_t _uint32_tProp = 2342342;
  uint64_t _uint64_tProp = 908324923490324ull;
  std::string _strProp("The String Property");
  float _floatProp = 3.1415927f;
  double _doubleProp = 234234.234234234;
  
  
  // Set all the properties using their named methods
  pclass.setBoolProperty(_boolProp);
  pclass.setInt8Property(_int8_tProp);
  pclass.setInt16Property(_int16_tProp);
  pclass.setInt32Property(_int32_tProp);
  pclass.setInt64Property(_int64_tProp);
  
  pclass.setUInt8Property(_uint8_tProp);
  pclass.setUInt16Property(_uint16_tProp);
  pclass.setUInt32Property(_uint32_tProp);
  pclass.setUInt64Property(_uint64_tProp);
  
  pclass.setFloatProperty(_floatProp);
  pclass.setDoubleProperty(_doubleProp);
  
  pclass.setStrProperty(_strProp);
  
  
  // Get all the properties using their named methods
  _boolProp = pclass.getBoolProperty();
  _int8_tProp = pclass.getInt8Property();
  _int16_tProp = pclass.getInt16Property();
  _int32_tProp = pclass.getInt32Property();  
  _int64_tProp = pclass.getInt64Property();
  _uint8_tProp = pclass.getUInt8Property();
  _uint16_tProp = pclass.getUInt16Property();
  _uint32_tProp = pclass.getUInt32Property();
  _uint64_tProp = pclass.getUInt64Property();
  _strProp = pclass.getStrProperty();
  _floatProp = pclass.getFloatProperty();
  _doubleProp = pclass.getDoubleProperty();
  PRINT_RESULTS(BoolProperty, _boolProp)
  PRINT_RESULTS(Int8Property, _int8_tProp)
  PRINT_RESULTS(Int16Property, _int16_tProp)
  PRINT_RESULTS(Int32Property, _int32_tProp)
  PRINT_RESULTS(Int64Property, _int64_tProp)
  PRINT_RESULTS(UInt8Property, _uint8_tProp)
  PRINT_RESULTS(UInt16Property, _uint16_tProp)
  PRINT_RESULTS(UInt32Property, _uint32_tProp)
  PRINT_RESULTS(UInt64Property, _uint64_tProp)
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
  
  int32_t success = 0;
  success = pclass.getProperty<bool>(MyPropertyNamespace::BoolProperty, _boolProp);
  PRINT_RESULTS(BoolProperty, _boolProp)
  success = pclass.getProperty<int8_t>(MyPropertyNamespace::Int8Property, _int8_tProp);
  PRINT_RESULTS(Int8Property, _int8_tProp)
  success = pclass.getProperty<int16_t>(MyPropertyNamespace::Int16Property, _int16_tProp);
  PRINT_RESULTS(Int16Property, _int16_tProp)
  success = pclass.getProperty<int32_t>(MyPropertyNamespace::Int32Property, _int32_tProp);
  PRINT_RESULTS(Int32Property, _int32_tProp)
  success = pclass.getProperty<int64_t>(MyPropertyNamespace::Int64Property, _int64_tProp);
  PRINT_RESULTS(Int64Property, _int64_tProp)
  
  success = pclass.getProperty<uint8_t>(MyPropertyNamespace::UInt8Property, _uint8_tProp);
  PRINT_RESULTS(UInt8Property, _uint8_tProp)
  success = pclass.getProperty<uint16_t>(MyPropertyNamespace::UInt16Property, _uint16_tProp);
  PRINT_RESULTS(UInt16Property, _uint16_tProp)
  success = pclass.getProperty<uint32_t>(MyPropertyNamespace::UInt32Property, _uint32_tProp);
  PRINT_RESULTS(UInt32Property, _uint32_tProp)
  success = pclass.getProperty<uint64_t>(MyPropertyNamespace::UInt64Property, _uint64_tProp);
  PRINT_RESULTS(UInt64Property, _uint64_tProp)
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



