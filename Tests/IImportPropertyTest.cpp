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

#include <MXA/MXATypes.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IImportProperty.h>
#include "UnitTestSupport.hpp"

#define PRINT_RESULTS(prpty, var)\
std::cout << #prpty << ": " << var << std::endl;

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


class MyPropertyClass : public IImportProperty
{
public:
  MyPropertyClass() {};
  virtual ~MyPropertyClass() {};

// -----------------------------------------------------------------------------
//  Start the Properties Declarations using our macros from Common/MXAMacros.h
//  These macros will create the named get/set methods for each property
// -----------------------------------------------------------------------------
  // Boolean Property
  MXA_PROPERTY(bool, BoolProperty, _boolProp)

  // Character type properties - These are essentially single character string properties
  // Attempting to use either int8_t or uint8_t types to store actual integer values has undefined
  // results. Try using int16_t or uint16_t to store these types of values instead.
  MXA_PROPERTY(uint8_t, UInt8Property,  _uint8_tProp)
  MXA_PROPERTY(int8_t,  Int8Property,  _int8_tProp)

  // Signed integer Properties
  MXA_PROPERTY(int16_t, Int16Property, _int16_tProp)
  MXA_PROPERTY(int32_t, Int32Property, _int32_tProp)
  MXA_PROPERTY(int64_t, Int64Property, _int64_tProp)

  // Unsigned Integer properties
  MXA_PROPERTY(uint16_t, UInt16Property, _uint16_tProp)
  MXA_PROPERTY(uint32_t, UInt32Property, _uint32_tProp)
  MXA_PROPERTY(uint64_t, UInt64Property, _uint64_tProp)

  // String properties
  MXA_PROPERTY(std::string, StrProperty, _strProp)
  // Floating point properties
  MXA_PROPERTY(float, FloatProperty, _floatProp)
  MXA_PROPERTY(double, DoubleProperty, _doubleProp)

  // -----------------------------------------------------------------------------
  //  If you want to add a generic 'set' method that takes a string key and string
  //   value then implement the following method
  // -----------------------------------------------------------------------------
  virtual int32_t setProperty(const std::string &key, const std::string &value)
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

  virtual int32_t getProperty(const std::string &key, std::string &value)
  {
    return -1;
  }

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
      GET_PROPERTY_BODY(MyPropertyNamespace, T, BoolProperty, _boolProp, key, value);

      // Signed integer Properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int8Property, _int8_tProp, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int16Property, _int16_tProp, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int32Property, _int32_tProp, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, Int64Property, _int64_tProp, key, value);

      // Unsigned Integer properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt8Property, _uint8_tProp,  key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt16Property, _uint16_tProp, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt32Property, _uint32_tProp, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, UInt64Property, _uint64_tProp, key, value);

      // String properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, StrProperty, _strProp, key, value);
      // Floating point properties
      GET_PROPERTY_BODY(MyPropertyNamespace, T, FloatProperty, _floatProp, key, value);
      GET_PROPERTY_BODY(MyPropertyNamespace, T, DoubleProperty, _doubleProp, key, value);

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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t PropertyTest()
{
  int32_t err = 0;

  MyPropertyClass pclass;


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
  MXA_REQUIRE (_boolProp == true);
  _int8_tProp = pclass.getInt8Property();
  MXA_REQUIRE (_int8_tProp == 'A');

  _int16_tProp = pclass.getInt16Property();
  MXA_REQUIRE (_int16_tProp == -34);
  _int32_tProp = pclass.getInt32Property();
  MXA_REQUIRE (_int32_tProp == -300);
  _int64_tProp = pclass.getInt64Property();
  MXA_REQUIRE (_int64_tProp == -9832498342ll);
  _uint8_tProp = pclass.getUInt8Property();
  MXA_REQUIRE (_uint8_tProp == 'Z');
  _uint16_tProp = pclass.getUInt16Property();
  MXA_REQUIRE (_uint16_tProp == 234);
  _uint32_tProp = pclass.getUInt32Property();
  MXA_REQUIRE (_uint32_tProp == 2342342);
  _uint64_tProp = pclass.getUInt64Property();
  MXA_REQUIRE (_uint64_tProp == 908324923490324ull);
  _strProp = pclass.getStrProperty();

  MXA_REQUIRE (_strProp.compare("The String Property") == 0);
  _floatProp = pclass.getFloatProperty();
  MXA_REQUIRE (_floatProp == 3.1415927f);
  _doubleProp = pclass.getDoubleProperty();
  MXA_REQUIRE (_doubleProp == 234234.234234234);

#if 0
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
#endif

 // std::cout << "--------------- Using 'setProperty(string, string)' ---------------------" << std::endl;
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
  success = pclass.getProperty<int8_t>(MyPropertyNamespace::Int8Property, _int8_tProp);
  success = pclass.getProperty<int16_t>(MyPropertyNamespace::Int16Property, _int16_tProp);
  success = pclass.getProperty<int32_t>(MyPropertyNamespace::Int32Property, _int32_tProp);
  success = pclass.getProperty<int64_t>(MyPropertyNamespace::Int64Property, _int64_tProp);
  success = pclass.getProperty<uint8_t>(MyPropertyNamespace::UInt8Property, _uint8_tProp);
  success = pclass.getProperty<uint16_t>(MyPropertyNamespace::UInt16Property, _uint16_tProp);
  success = pclass.getProperty<uint32_t>(MyPropertyNamespace::UInt32Property, _uint32_tProp);
  success = pclass.getProperty<uint64_t>(MyPropertyNamespace::UInt64Property, _uint64_tProp);
  success = pclass.getProperty<float>(MyPropertyNamespace::FloatProperty, _floatProp);
  success = pclass.getProperty<double>(MyPropertyNamespace::DoubleProperty, _doubleProp);
  std::string emptyString;
  success = pclass.getProperty<std::string>(MyPropertyNamespace::StrProperty, emptyString);
  
  MXA_REQUIRE (_boolProp == true);
  MXA_REQUIRE (_int8_tProp == 'G');
  MXA_REQUIRE (_int16_tProp == -2342);
  MXA_REQUIRE (_int32_tProp == -987543);
  MXA_REQUIRE (_int64_tProp == -65478934);
  MXA_REQUIRE (_uint8_tProp == 'P');
  MXA_REQUIRE (_uint16_tProp == 23654);
  MXA_REQUIRE (_uint32_tProp == 41234342);
  MXA_REQUIRE (_uint64_tProp == 9897842323ull);

  MXA_REQUIRE (emptyString.compare("Other String Property") == 0);
  MXA_REQUIRE (_floatProp == 4123.4342f);
  MXA_REQUIRE (_doubleProp == 9897.842323);

#if 0
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
  PRINT_RESULTS(StrProperty, emptyString)
#endif

  return err;
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  int err = EXIT_SUCCESS;
  MXA_REGISTER_TEST( PropertyTest() );
  PRINT_TEST_SUMMARY();
  return err;
}
