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

#include <Common/MXATypes.h>
#include <Base/IImportProperty.h>


//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


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
  virtual int32 setProperty(const std::string &key, const std::string &value)
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
    SET_PROPERTY_BODY(MyPropertyNamespace, std::string, StrProperty, key, value);
    // Floating point properties
    SET_PROPERTY_BODY(MyPropertyNamespace, float, FloatProperty, key, value);
    SET_PROPERTY_BODY(MyPropertyNamespace, double, DoubleProperty, key, value);

    return 0;
  }

  virtual int32 getProperty(const std::string &key, std::string &value)
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 PropertyTest()
{
  int32 err = 0;

  MyPropertyClass pclass;


  bool _boolProp = true;
  int8 _int8Prop = 'A';
  int16 _int16Prop = -34;
  int32 _int32Prop = -300;
  int64 _int64Prop = -9832498342ll;
  uint8 _uint8Prop = 'Z';
  uint16 _uint16Prop = 234;
  uint32 _uint32Prop = 2342342;
  uint64 _uint64Prop = 908324923490324ull;
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
  BOOST_REQUIRE (_boolProp == true);
  _int8Prop = pclass.getInt8Property();
  BOOST_REQUIRE (_int8Prop == 'A');

  _int16Prop = pclass.getInt16Property();
  BOOST_REQUIRE (_int16Prop == -34);
  _int32Prop = pclass.getInt32Property();
  BOOST_REQUIRE (_int32Prop == -300);
  _int64Prop = pclass.getInt64Property();
  BOOST_REQUIRE (_int64Prop == -9832498342ll);
  _uint8Prop = pclass.getUInt8Property();
  BOOST_REQUIRE (_uint8Prop == 'Z');
  _uint16Prop = pclass.getUInt16Property();
  BOOST_REQUIRE (_uint16Prop == 234);
  _uint32Prop = pclass.getUInt32Property();
  BOOST_REQUIRE (_uint32Prop == 2342342);
  _uint64Prop = pclass.getUInt64Property();
  BOOST_REQUIRE (_uint64Prop == 908324923490324ull);
  _strProp = pclass.getStrProperty();

  BOOST_REQUIRE (_strProp.compare("The String Property") == 0);
  _floatProp = pclass.getFloatProperty();
  BOOST_REQUIRE (_floatProp == 3.1415927f);
  _doubleProp = pclass.getDoubleProperty();
  BOOST_REQUIRE (_doubleProp == 234234.234234234);

#if 0
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

  int32 success = 0;
  success = pclass.getProperty<bool>(MyPropertyNamespace::BoolProperty, _boolProp);
  success = pclass.getProperty<int8>(MyPropertyNamespace::Int8Property, _int8Prop);
  success = pclass.getProperty<int16>(MyPropertyNamespace::Int16Property, _int16Prop);
  success = pclass.getProperty<int32>(MyPropertyNamespace::Int32Property, _int32Prop);
  success = pclass.getProperty<int64>(MyPropertyNamespace::Int64Property, _int64Prop);
  success = pclass.getProperty<uint8>(MyPropertyNamespace::UInt8Property, _uint8Prop);
  success = pclass.getProperty<uint16>(MyPropertyNamespace::UInt16Property, _uint16Prop);
  success = pclass.getProperty<uint32>(MyPropertyNamespace::UInt32Property, _uint32Prop);
  success = pclass.getProperty<uint64>(MyPropertyNamespace::UInt64Property, _uint64Prop);
  success = pclass.getProperty<float>(MyPropertyNamespace::FloatProperty, _floatProp);
  success = pclass.getProperty<double>(MyPropertyNamespace::DoubleProperty, _doubleProp);
  std::string emptyString;
  success = pclass.getProperty<std::string>(MyPropertyNamespace::StrProperty, emptyString);

  BOOST_REQUIRE (_boolProp == true);
  BOOST_REQUIRE (_int8Prop == 'G');
  BOOST_REQUIRE (_int16Prop == -2342);
  BOOST_REQUIRE (_int32Prop == -987543);
  BOOST_REQUIRE (_int64Prop == -65478934);
  BOOST_REQUIRE (_uint8Prop == 'P');
  BOOST_REQUIRE (_uint16Prop == 23654);
  BOOST_REQUIRE (_uint32Prop == 41234342);
  BOOST_REQUIRE (_uint64Prop == 9897842323ull);

  BOOST_REQUIRE (emptyString.compare("Other String Property") == 0);
  BOOST_REQUIRE (_floatProp == 4123.4342f);
  BOOST_REQUIRE (_doubleProp == 9897.842323);


#if 0
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
  PRINT_RESULTS(StrProperty, emptyString)
#endif

  return err;
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "IImportProperty Test");
  test->add( BOOST_TEST_CASE (&PropertyTest), 0);

  return test;
}
