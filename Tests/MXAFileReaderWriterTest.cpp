///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include <MXA/Common/MXATypes.h>
#include <MXA/Common/MXAEndian.h>
#include <MXA/Common/IO/MXAFileReader64.h>
#include <MXA/Common/IO/MXAFileWriter64.h>
#include <MXAUnitTestDataFileLocations.h>

typedef MXAFILEWRITER_CLASS_NAME FileWriter64;
typedef MXAFILEREADER_CLASS_NAME FileReader64;

//-- C++ Includes
#include <iostream>

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


//-- Boost Filesystem Headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

typedef union
{
  int32 i;
  float f;

} _FourByteData;

typedef union
{
  int64 ll;
  double d;
} _EightByteData;

typedef _EightByteData EightByteData;
typedef _FourByteData  FourByteData;

#ifdef MXA_BIG_ENDIAN
typedef MXA::Endian::FromLittleToSystem EndianPolicy;
#else
typedef MXA::Endian::FromBigToSystem    EndianPolicy;
#endif

#define ARRAY_SIZE 4

#define WriteArray(type, initValue) \
{type* type##Array = new type[ARRAY_SIZE];\
  for (int index = 0; index < ARRAY_SIZE; ++index) { type##Array[index] = initValue; }\
  ok = writer.writeArray<type>(type##Array, (int64)(ARRAY_SIZE) );\
  BOOST_REQUIRE_EQUAL(ok, true);\
  delete type##Array;}\
  /*writer.write( &newLine, 1);*/

#define WriteCharArray(type, initValue) \
  type* type##CharArray = new type[ARRAY_SIZE];\
  for (int index = 0; index < ARRAY_SIZE; ++index) { type##CharArray[index] = initValue; }\
  numBytes = sizeof(type) * ARRAY_SIZE;\
  ok = writer.write(reinterpret_cast<char*>(type##CharArray), (int64)(numBytes) );\
  BOOST_REQUIRE_EQUAL(ok, true);\
  delete type##CharArray;\
  /*writer.write( &newLine, 1);*/

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
  std::cout << "   Removing Test files" << std::endl;
  boost::filesystem::remove(Testing::MXAFileReaderWriterTest::OutputFile);
#else
  std::cout << "   Test files NOT removed." << std::endl;
  #endif
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int TestMXAFileWriter()
{
  std::cout << "   Testing MXAFileWriter" << std::endl;
  int err = 0;
  bool ok = false;
  //char newLine = '\n';
  FileWriter64 writer(Testing::MXAFileReaderWriterTest::OutputFile);
  if (writer.initWriter() == false)
  {
    BOOST_ASSERT("Could not initialize the file writer");
  }

  int64 numBytes = 0;
  //char ascii[4] = { 'A', 'S', 'C', 'I'};
  char c = 0x63;
  int16 s = 0x7368;
  int32 i = 0x696E745F;
  int64 ll = 0x6C6F6E676C6F6E67ll;

  FourByteData fd;
  EightByteData ed;
  fd.i = 0x666C6F74;
  ed.ll = 0x5F646F75626C655Fll;
  float f = fd.f;
  double d = ed.d;

  // numBytes = sizeof(ascii);
 // writer.write(ascii, numBytes);
  writer.writeValue<char>(&c);
  writer.writeValue<int16>(&s);
  writer.writeValue<int32>(&i);
  writer.writeValue<int64>(&ll);
  writer.writeValue<float>(&fd.f);
  writer.writeValue<double>(&ed.d);

  WriteCharArray(int8, c);
  WriteCharArray(int16, s);
  WriteCharArray(int32, i);
  WriteCharArray(int64, ll);
  WriteCharArray(float, f);
  WriteCharArray(double, d);
std::cout << "Pos: " << writer.getFilePointer64() << std::endl;
  // Should be at byte 31 at this point

 // WriteArray(int8, c);
{
  int8* int8Array = new int8[ARRAY_SIZE];
  for (int index = 0; index < ARRAY_SIZE; ++index) { int8Array[index] = c; }
  ok = writer.writeArray<int8>(int8Array, (int64)(ARRAY_SIZE) );
  BOOST_REQUIRE_EQUAL(ok, true);
  delete int8Array;
}

  std::cout << "Pos: " << writer.getFilePointer64() << std::endl;
  WriteArray(int16, s);
  std::cout << "Pos: " << writer.getFilePointer64() << std::endl;
  WriteArray(int32, i);
  WriteArray(int64, ll);
  WriteArray(float, f);
  WriteArray(double, d);
std::cout << "Pos: " << writer.getFilePointer64() << std::endl;
  int64 pos = writer.getFilePointer64();
  BOOST_REQUIRE_EQUAL(pos, 243);

  return err;
}

#define ReadValue(type, compare_value)\
  { type type##Read;\
  ok = reader.readValue<type>(type##Read);\
  BOOST_REQUIRE_EQUAL(ok, true);\
  BOOST_REQUIRE_EQUAL(compare_value, type##Read);}\

#define RawRead(type, compare_value)\
  {type* type##RawArray = new type[ARRAY_SIZE];\
  ok = reader.rawRead(reinterpret_cast<char*>(type##RawArray), sizeof(type)*ARRAY_SIZE );\
  BOOST_REQUIRE_EQUAL(ok, true);\
  for (int index = 0; index < ARRAY_SIZE; ++index) {\
    BOOST_REQUIRE_EQUAL(type##RawArray[index], compare_value);\
  }\
  delete type##RawArray;}\

#define ReadArray(type, compare_value)\
  { type* type##Array = new type[ARRAY_SIZE];\
  ok = reader.readArray<type>( type##Array, ARRAY_SIZE );\
  BOOST_REQUIRE_EQUAL(ok, true);\
  for (int index = 0; index < ARRAY_SIZE; ++index) {\
    BOOST_REQUIRE_EQUAL(type##Array[index], compare_value);\
  }\
  delete type##Array;}\

#define ReadSwappedValue(type, compare_value)\
  {type type##SwapRead;\
  ok = reader.read<EndianPolicy, type>(type##SwapRead);\
  BOOST_REQUIRE_EQUAL(ok, true);\
  BOOST_REQUIRE_EQUAL(type##SwapRead, compare_value);}\

#define ReadSwappedArray(type, compare_value)\
  { type* type##Array = new type[ARRAY_SIZE];\
  ok = reader.readArrayWithSwap<EndianPolicy, type>( type##Array, ARRAY_SIZE );\
  BOOST_REQUIRE_EQUAL(ok, true);\
  for (int index = 0; index < ARRAY_SIZE; ++index) {\
    BOOST_REQUIRE_EQUAL(type##Array[index], compare_value);\
  }\
  delete type##Array;}\


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int TestMXAFileReader()
{
  std::cout << "   Testing MXAFileReader" << std::endl;
  int err = 0;
  bool ok = false;
  char c = 0x63;
  int16 s = 0x7368;
  int32 i = 0x696E745F;
  int64 ll = 0x6C6F6E676C6F6E67ll;

  FourByteData fd;
  EightByteData ed;
  fd.i = 0x666C6F74;
  ed.ll = 0x5F646F75626C655Fll;
  float f = fd.f;
  double d = ed.d;

  FileReader64 reader(Testing::MXAFileReaderWriterTest::OutputFile);
  if (reader.initReader() == false)
  {
    BOOST_ASSERT("Could not initialize the file reader");
  }

  ReadValue(int8, c);
  ReadValue(int16, s);
  ReadValue(int32, i);
  ReadValue(int64, ll);
  ReadValue(float, f);
  ReadValue(double, d);

  RawRead(int8, c);
  RawRead(int16, s);
  RawRead(int32, i);
  RawRead(int64, ll);
  RawRead(float, f);
  RawRead(double, d);

  ReadArray(int8, c);
  ReadArray(int16, s);
  ReadArray(int32, i);
  ReadArray(int64, ll);
  ReadArray(float, f);
  ReadArray(double, d);

  // Now test the Byte Swapping functionality
  reader.setFilePointer64(0ll);
  int8 int8Read;
  reader.readValue<int8>(int8Read);
  EndianPolicy::convert(s);
  EndianPolicy::convert(i);
  EndianPolicy::convert(ll);
  EndianPolicy::convert(f);
  EndianPolicy::convert(d);

  // Read and test the Swapped Values
  ReadSwappedValue(int16, s);
  ReadSwappedValue(int32, i);
  ReadSwappedValue(int64, ll);
  ReadSwappedValue(float, f);
  ReadSwappedValue(double, d);

  ReadArray(int8, c);
  ReadSwappedArray(int16, s);
  ReadSwappedArray(int32, i);
  ReadSwappedArray(int64, ll);
  ReadSwappedArray(float, f);
  ReadSwappedArray(double, d);



  return err;
}


// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "MXAFileReaderWriterTest");
  test->add( BOOST_TEST_CASE( &TestMXAFileWriter), 0);
  test->add( BOOST_TEST_CASE( &TestMXAFileReader), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}


