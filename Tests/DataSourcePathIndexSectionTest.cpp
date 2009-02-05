///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include <MXA/Common/MXATypes.h>
#include <MXA/DataImport/DataImportXmlParser.h>
#include <MXA/Utilities/IStringSection.h>
#include <MXA/Utilities/DataSourcePathIndexSection.h>
#include "MXAUnitTestDataFileLocations.h"

//-- STL includes
#include <iostream>
#include <string>
#include <map>


//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


//-- Boost Filesystem Headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace Testing
{
  namespace DataSourcePathIndexSectionTest
  {
    const std::string DataSourcePathIndexSectionTest_OuptutFile ("DataSourcePathIndexSectionTest_OutputFile");
  }

}

// --------------------------------- Typedefs ----------------------------------
typedef std::map<std::string, std::string>   XMLAttributeMap;


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
  std::cout << "   Removing Test files" << std::endl;
#if REMOVE_TEST_FILES
  boost::filesystem::remove(Testing::DataSourcePathIndexSectionTest::DataSourcePathIndexSectionTest_OuptutFile);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int DataSourcePathIndexSectionTest_EntryPoint()
{
	int err = 0;
	int8 ok = 1;
	std::string preText("0-Test_");

	int32 dimIndex = 0;
	std::string paddingChar("");
	int32 width = -1;
	std::string numericType ("Integer");
	{
    IStringSectionPtr section (new DataSourcePathIndexSection(dimIndex, paddingChar, width, numericType) );
    section->setPreText(preText);
    int index = 1;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;

    DataSourcePathIndexSection* ptr = dynamic_cast<DataSourcePathIndexSection*>(section.get());
    std::string s = ptr->toString(index, ok);

    BOOST_REQUIRE_EQUAL(section->toString(index,ok).compare("0-Test_1"), 0);
    index = 10;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    BOOST_REQUIRE_EQUAL(section->toString(index,ok).compare("0-Test_10"), 0);
    index = 100;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    BOOST_REQUIRE_EQUAL(section->toString(index,ok).compare("0-Test_100"), 0);
	}
  //////////////////////////////////////////////////////////////////////////
  paddingChar = "0";
  width = 3;
  preText = "1-Test_";
  {
    IStringSectionPtr section (new DataSourcePathIndexSection(dimIndex, paddingChar, width, numericType) );
    section->setPreText(preText);
    int index = 1;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    BOOST_REQUIRE_EQUAL(section->toString(index,ok).compare("1-Test_001"), 0);
    index = 10;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    BOOST_REQUIRE_EQUAL(section->toString(index,ok).compare("1-Test_010"), 0);
    index = 100;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    BOOST_REQUIRE_EQUAL(section->toString(index,ok).compare("1-Test_100"), 0);
    index = 1000;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    BOOST_REQUIRE_EQUAL(section->toString(index,ok).compare("1-Test_1000"), 0);
  }

  //////////////////////////////////////////////////////////////////////////
  // This test should fail because we have NOT matched the paddingChar and width
  // properly so we use boost to test for "NOT EQUAL (NE)"
  paddingChar = "";
  width = 3;
  preText = "2-Test_";
  {
    IStringSectionPtr section (new DataSourcePathIndexSection(dimIndex, paddingChar, width, numericType) );
    section->setPreText(preText);
    int index = 1;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    BOOST_REQUIRE_NE(section->toString(index,ok).compare("2-Test_001"), 0);
  }

  //////////////////////////////////////////////////////////////////////////
  // This test should fail because we have NOT matched the paddingChar and width
  // properly so we use boost to test for "NOT EQUAL (NE)"
  paddingChar = "0";
  width = -1;
  preText = "2-Test_";
  {
    IStringSectionPtr section (new DataSourcePathIndexSection(dimIndex, paddingChar, width, numericType) );
    section->setPreText(preText);
    int index = 1;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    BOOST_REQUIRE_NE(section->toString(index,ok).compare("2-Test_001"), 0);
  }

	return err;
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "DataSourcePathIndexSectionTest Test Running");
  test->add( BOOST_TEST_CASE( &DataSourcePathIndexSectionTest_EntryPoint), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}


