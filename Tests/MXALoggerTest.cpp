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
#include "UnitTestSupport.hpp"

#define logger               MXA_Global_Logger->mxaLogger

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
	  MXALOGGER_METHOD_VARIABLE_INSTANCE;
    std::stringstream ss;
    ss << "This is a test of the stringstream";
    logger.open(MXAUnitTest::MXALoggerTest::TestFile);
    MXA_REQUIRE((logger.getIsFileBased()) == true);
    logger << logTime() << (ui8) << (i8) << (ui16) << (i16) << (ui32) << (i32) << (ui64) << (i64) << (f) << (d) << std::endl;
    logger << logTime() << (str) << std::endl;
    logger << logTime() << ss.str() << std::endl;
    logger.close();
    MXA_REQUIRE(logger.getIsFileBased() == false);

    logger << logTime() << ss.str() << std::endl;
    logger << "+++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    logger.open(MXAUnitTest::MXALoggerTest::TestFile, std::ios::app);
    MXA_REQUIRE(logger.getIsFileBased() == true);

    logger << logTime() << (ss.str()) << std::endl;
    logger.close();;
    MXA_REQUIRE(logger.getIsFileBased() == false);
	}

	std::cout << logTime() << "Logging to std::cout" << std::endl;
	{
	  MXALOGGER_METHOD_VARIABLE_INSTANCE;
    logger << logTime() << (ui8) << (i8) << (ui16) << (i16) << (ui32) << (i32) << (ui64) << (i64) << (f) << (d) << std::endl;
    logger << logTime() << (str) << std::endl;

    logger.close();;
    MXA_REQUIRE(logger.getIsFileBased() == false);

    logger.open(MXAUnitTest::MXALoggerTest::TestFile, std::ios::app);
    MXA_REQUIRE(logger.getIsFileBased() == true);
    std::stringstream ss;
    ss.str();
    ss << "This is a test of the stringstream";
    logger << logTime() << (ss.str()) << std::endl;
    logger.close();;
    MXA_REQUIRE(logger.getIsFileBased() == false);


	}
	return err;
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  int err = EXIT_SUCCESS;

  MXA_REGISTER_TEST( MXALoggerTest_EntryPoint() );
  MXA_REGISTER_TEST( RemoveTestFiles() );
  PRINT_TEST_SUMMARY();
  return err;
}


