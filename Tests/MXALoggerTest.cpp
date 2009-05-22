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
	{
    MXALogger logger;
    std::stringstream ss;
    ss << "This is a test of the stringstream";
    logger.open(MXAUnitTest::MXALoggerTest::TestFile);
    BOOST_REQUIRE(logger.getIsFileBased() == true);
    logger << logTime() << (ui8) << (i8) << (ui16) << (i16) << (ui32) << (i32) << (ui64) << (i64) << (f) << (d) << std::endl;
    logger << logTime() << (str) << std::endl;
    logger << logTime() << ss.str() << std::endl;
    logger.close();
    BOOST_REQUIRE(logger.getIsFileBased() == false);

    logger << logTime() << ss.str() << std::endl;
    logger << "+++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    logger.open(MXAUnitTest::MXALoggerTest::TestFile, std::ios::app);
    BOOST_REQUIRE(logger.getIsFileBased() == true);

    logger << logTime() << (ss.str()) << std::endl;
    logger.close();;
    BOOST_REQUIRE(logger.getIsFileBased() == false);
	}

	std::cout << logTime() << "Logging to std::cout" << std::endl;
	{
    MXALogger logger;
    logger << logTime() << (ui8) << (i8) << (ui16) << (i16) << (ui32) << (i32) << (ui64) << (i64) << (f) << (d) << std::endl;
    logger << logTime() << (str) << std::endl;

    logger.close();;
    BOOST_REQUIRE(logger.getIsFileBased() == false);

    logger.open(MXAUnitTest::MXALoggerTest::TestFile, std::ios::app);
    BOOST_REQUIRE(logger.getIsFileBased() == true);
    std::stringstream ss;
    ss.str();
    ss << "This is a test of the stringstream";
    logger << logTime() << (ss.str()) << std::endl;
    logger.close();;
    BOOST_REQUIRE(logger.getIsFileBased() == false);


	}
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


