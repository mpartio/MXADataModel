///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include "MXA/MXA.h"
#include <MXA/Common/MXAEndian.h>
#include <MXA/Common/IO/MXAFileReader64.h>
#include <MXA/Common/IO/MXAFileWriter64.h>
#include <MXAUnitTestDataFileLocations.h>
#include <MXA/Utilities/MXADir.h>
#include "UnitTestSupport.hpp"


typedef MXAFILEWRITER_CLASS_NAME FileWriter64;
typedef MXAFILEREADER_CLASS_NAME FileReader64;

//-- C++ Includes
#include <iostream>

typedef union
{
  int32_t i;
  float f;

} _FourByteData;

typedef union
{
  int64_t ll;
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
  ok = writer.writeArray<type>(type##Array, (int64_t)(ARRAY_SIZE) );\
  MXA_REQUIRE_EQUAL(ok, true);\
  delete type##Array;}\
  /*writer.write( &newLine, 1);*/

#define WriteCharArray(type, initValue) \
  type* type##CharArray = new type[ARRAY_SIZE];\
  for (int index = 0; index < ARRAY_SIZE; ++index) { type##CharArray[index] = initValue; }\
  numBytes = sizeof(type) * ARRAY_SIZE;\
  ok = writer.write(reinterpret_cast<char*>(type##CharArray), (int64_t)(numBytes) );\
  MXA_REQUIRE_EQUAL(ok, true);\
  delete type##CharArray;\
  /*writer.write( &newLine, 1);*/

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
 // std::cout << "   Removing Test files" << std::endl;
  MXADir::remove(MXAUnitTest::MXAFileReaderWriterTest::OutputFile);
#else
  std::cout << "   Test files NOT removed." << std::endl;
  #endif
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int TestMXAFileWriter()
{
 // std::cout << "   Testing MXAFileWriter" << std::endl;
  int err = 0;
  bool ok = false;
  //char newLine = '\n';
  FileWriter64 writer(MXAUnitTest::MXAFileReaderWriterTest::OutputFile);
  bool isReady = writer.initWriter();
  MXA_REQUIRE( isReady == true )

  int64_t numBytes = 0;
  //char ascii[4] = { 'A', 'S', 'C', 'I'};
  char c = 0x63;
  int16_t s = 0x7368;
  int32_t i = 0x696E745F;
  int64_t ll = 0x6C6F6E676C6F6E67ll;

  FourByteData fd;
  EightByteData ed;
  fd.i = 0x666C6F74;
  ed.ll = 0x5F646F75626C655Fll;
  float f = fd.f;
  double d = ed.d;

  // numBytes = sizeof(ascii);
 // writer.write(ascii, numBytes);
  writer.writeValue<char>(&c);
  writer.writeValue<int16_t>(&s);
  writer.writeValue<int32_t>(&i);
  writer.writeValue<int64_t>(&ll);
  writer.writeValue<float>(&fd.f);
  writer.writeValue<double>(&ed.d);

  WriteCharArray(int8_t, c);
  WriteCharArray(int16_t, s);
  WriteCharArray(int32_t, i);
  WriteCharArray(int64_t, ll);
  WriteCharArray(float, f);
  WriteCharArray(double, d);
//std::cout << "Pos: " << writer.getFilePointer64() << std::endl;
  // Should be at byte 31 at this point

 // WriteArray(int8_t, c);
{
  int8_t* int8_tArray = new int8_t[ARRAY_SIZE];
  for (int index = 0; index < ARRAY_SIZE; ++index) { int8_tArray[index] = c; }
  ok = writer.writeArray<int8_t>(int8_tArray, (int64_t)(ARRAY_SIZE) );
  MXA_REQUIRE_EQUAL(ok, true);
  delete int8_tArray;
}

//  std::cout << "Pos: " << writer.getFilePointer64() << std::endl;
  WriteArray(int16_t, s);
//  std::cout << "Pos: " << writer.getFilePointer64() << std::endl;
  WriteArray(int32_t, i);
  WriteArray(int64_t, ll);
  WriteArray(float, f);
  WriteArray(double, d);
//std::cout << "Pos: " << writer.getFilePointer64() << std::endl;
  int64_t pos = writer.getFilePointer64();
  MXA_REQUIRE_EQUAL(pos, 243);

  return err;
}

#define ReadValue(type, compare_value)\
  { type type##Read;\
  ok = reader.readValue<type>(type##Read);\
  MXA_REQUIRE_EQUAL(ok, true);\
  MXA_REQUIRE_EQUAL(compare_value, type##Read);}\

#define RawRead(type, compare_value)\
  {type* type##RawArray = new type[ARRAY_SIZE];\
  ok = reader.rawRead(reinterpret_cast<char*>(type##RawArray), sizeof(type)*ARRAY_SIZE );\
  MXA_REQUIRE_EQUAL(ok, true);\
  for (int index = 0; index < ARRAY_SIZE; ++index) {\
    MXA_REQUIRE_EQUAL(type##RawArray[index], compare_value);\
  }\
  delete type##RawArray;}\

#define ReadArray(type, compare_value)\
  { type* type##Array = new type[ARRAY_SIZE];\
  ok = reader.readArray<type>( type##Array, ARRAY_SIZE );\
  MXA_REQUIRE_EQUAL(ok, true);\
  for (int index = 0; index < ARRAY_SIZE; ++index) {\
    MXA_REQUIRE_EQUAL(type##Array[index], compare_value);\
  }\
  delete type##Array;}\

#define ReadSwappedValue(type, compare_value)\
  {type type##SwapRead;\
  ok = reader.read<EndianPolicy, type>(type##SwapRead);\
  MXA_REQUIRE_EQUAL(ok, true);\
  MXA_REQUIRE_EQUAL(type##SwapRead, compare_value);}\

#define ReadSwappedArray(type, compare_value)\
  { type* type##Array = new type[ARRAY_SIZE];\
  ok = reader.readArrayWithSwap<EndianPolicy, type>( type##Array, ARRAY_SIZE );\
  MXA_REQUIRE_EQUAL(ok, true);\
  for (int index = 0; index < ARRAY_SIZE; ++index) {\
    MXA_REQUIRE_EQUAL(type##Array[index], compare_value);\
  }\
  delete type##Array;}\


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int TestMXAFileReader()
{
 // std::cout << "   Testing MXAFileReader" << std::endl;
  int err = 0;
  bool ok = false;
  char c = 0x63;
  int16_t s = 0x7368;
  int32_t i = 0x696E745F;
  int64_t ll = 0x6C6F6E676C6F6E67ll;

  FourByteData fd;
  EightByteData ed;
  fd.i = 0x666C6F74;
  ed.ll = 0x5F646F75626C655Fll;
  float f = fd.f;
  double d = ed.d;

  FileReader64 reader(MXAUnitTest::MXAFileReaderWriterTest::OutputFile);
  MXA_REQUIRE( reader.initReader() == true )

  ReadValue(int8_t, c);
  ReadValue(int16_t, s);
  ReadValue(int32_t, i);
  ReadValue(int64_t, ll);
  ReadValue(float, f);
  ReadValue(double, d);

  RawRead(int8_t, c);
  RawRead(int16_t, s);
  RawRead(int32_t, i);
  RawRead(int64_t, ll);
  RawRead(float, f);
  RawRead(double, d);

  ReadArray(int8_t, c);
  ReadArray(int16_t, s);
  ReadArray(int32_t, i);
  ReadArray(int64_t, ll);
  ReadArray(float, f);
  ReadArray(double, d);

  // Now test the Byte Swapping functionality
  reader.setFilePointer64(0ll);
  int8_t int8_tRead;
  reader.readValue<int8_t>(int8_tRead);
  EndianPolicy::convert(s);
  EndianPolicy::convert(i);
  EndianPolicy::convert(ll);
  EndianPolicy::convert(f);
  EndianPolicy::convert(d);

  // Read and test the Swapped Values
  ReadSwappedValue(int16_t, s);
  ReadSwappedValue(int32_t, i);
  ReadSwappedValue(int64_t, ll);
  ReadSwappedValue(float, f);
  ReadSwappedValue(double, d);

  ReadArray(int8_t, c);
  ReadSwappedArray(int16_t, s);
  ReadSwappedArray(int32_t, i);
  ReadSwappedArray(int64_t, ll);
  ReadSwappedArray(float, f);
  ReadSwappedArray(double, d);



  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  int err = EXIT_SUCCESS;
  MXA_REGISTER_TEST( TestMXAFileWriter() );
  MXA_REGISTER_TEST( TestMXAFileReader() );
  MXA_REGISTER_TEST( RemoveTestFiles() );
  PRINT_TEST_SUMMARY();
  return err;
}


