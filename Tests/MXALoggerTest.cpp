///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include <MXA/Utilities/MXALogger.h>
#include <MXA/Utilities/MXAFileSystemPath.h>
#include "MXAUnitTestDataFileLocations.h"

//-- C++
#include <iostream>

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
  std::cout << "   Removing Test files" << std::endl;
#ifdef REMOVE_TEST_FILES
  MXAFileSystemPath::remove(MXAUnitTest::MXALoggerTest::TestFile);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int MXALoggerTest_EntryPoint()
{
	int err = 0;

	uint8 ui8 = 50;
	int8 i8 = 51;
	uint16 ui16 = 53;
	int16 i16 = 54;
	uint32 ui32 = 55;
	int32  i32 = 56;
	uint64 ui64 = 57;
	int64 i64 = 58;
	float f = 59.59f;
	double d = 60.60;

	std::string str("String");

	std::cout << logTime() << "Logging To File: " << MXAUnitTest::MXALoggerTest::TestFile << std::endl;

  MXALogger::Pointer logger = MXALogger::New();
  logger->open(MXAUnitTest::MXALoggerTest::TestFile);
	logger->dateTime()->log(ui8)->log(i8)->log(ui16)->log(i16)->log(ui32)->log(i32)->log(ui64)->log(i64)->log(f)->log(d)->endl();
	logger->dateTime()->log(str)->endl();

	logger->close();
	BOOST_REQUIRE(logger->is_open() == false);

  logger->open(MXAUnitTest::MXALoggerTest::TestFile, std::ios::app);
  BOOST_REQUIRE(logger->is_open() == true);
	std::stringstream ss;
	ss << "This is a test of the stringstream";
	logger->dateTime()->log(ss)->endl();
	logger->close();
	BOOST_REQUIRE(logger->is_open() == false);


	std::cout << logTime() << "Logging to std::cout" << std::endl;
	logger = MXALogger::New();
	BOOST_REQUIRE(logger.get() != NULL);
  logger->dateTime()->log(ui8)->log(i8)->log(ui16)->log(i16)->log(ui32)->log(i32)->log(ui64)->log(i64)->log(f)->log(d)->endl();
  logger->dateTime()->log(str)->endl();

  logger->close();
  BOOST_REQUIRE(logger->is_open() == false);

  logger->open(MXAUnitTest::MXALoggerTest::TestFile, std::ios::app);
  BOOST_REQUIRE(logger->is_open() == true);
  ss.str();
  ss << "This is a test of the stringstream";
  logger->dateTime()->log(ss)->endl();
  logger->close();
  BOOST_REQUIRE(logger->is_open() == false);

  std::string empty;
  logger = MXALogger::New(empty);
  BOOST_REQUIRE(logger.get() == NULL);

	return err;
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "MXALoggerTest Test Running");
  test->add( BOOST_TEST_CASE( &MXALoggerTest_EntryPoint), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}


