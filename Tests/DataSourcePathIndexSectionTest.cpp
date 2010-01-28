///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include <MXA/MXATypes.h>
#include <MXA/DataImport/DataImportXmlParser.h>
#include <MXA/Utilities/IStringSection.h>
#include <MXA/Utilities/DataSourcePathIndexSection.h>
#include "MXAUnitTestDataFileLocations.h"
#include <MXA/Utilities/MXADir.h>
#include "UnitTestSupport.hpp"


//-- STL includes
#include <iostream>
#include <string>
#include <map>



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
  MXADir::remove(Testing::DataSourcePathIndexSectionTest::DataSourcePathIndexSectionTest_OuptutFile);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int DataSourcePathIndexSectionTest_EntryPoint()
{
	int err = 0;
	int8_t ok = 1;
	std::string preText("0-Test_");

	int32_t dimIndex = 0;
	std::string paddingChar("");
	int32_t width = -1;
	std::string numericType ("Integer");
	{
    IStringSection::Pointer section (new DataSourcePathIndexSection(dimIndex, paddingChar, width, numericType) );
    section->setPreText(preText);
    int index = 1;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;

    DataSourcePathIndexSection* ptr = dynamic_cast<DataSourcePathIndexSection*>(section.get());
    std::string s = ptr->toString(index, ok);

    MXA_REQUIRE_EQUAL(section->toString(index,ok).compare("0-Test_1"), 0);
    index = 10;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    MXA_REQUIRE_EQUAL(section->toString(index,ok).compare("0-Test_10"), 0);
    index = 100;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    MXA_REQUIRE_EQUAL(section->toString(index,ok).compare("0-Test_100"), 0);
	}
  //////////////////////////////////////////////////////////////////////////
  paddingChar = "0";
  width = 3;
  preText = "1-Test_";
  {
    IStringSection::Pointer section (new DataSourcePathIndexSection(dimIndex, paddingChar, width, numericType) );
    section->setPreText(preText);
    int index = 1;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    MXA_REQUIRE_EQUAL(section->toString(index,ok).compare("1-Test_001"), 0);
    index = 10;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    MXA_REQUIRE_EQUAL(section->toString(index,ok).compare("1-Test_010"), 0);
    index = 100;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    MXA_REQUIRE_EQUAL(section->toString(index,ok).compare("1-Test_100"), 0);
    index = 1000;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    MXA_REQUIRE_EQUAL(section->toString(index,ok).compare("1-Test_1000"), 0);
  }

  //////////////////////////////////////////////////////////////////////////
  // This test should fail because we have NOT matched the paddingChar and width
  // properly so we use boost to test for "NOT EQUAL (NE)"
  paddingChar = "";
  width = 3;
  preText = "2-Test_";
  {
    IStringSection::Pointer section (new DataSourcePathIndexSection(dimIndex, paddingChar, width, numericType) );
    section->setPreText(preText);
    int index = 1;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    MXA_REQUIRE(section->toString(index,ok).compare("2-Test_001") != 0);
  }

  //////////////////////////////////////////////////////////////////////////
  // This test should fail because we have NOT matched the paddingChar and width
  // properly so we use boost to test for "NOT EQUAL (NE)"
  paddingChar = "0";
  width = -1;
  preText = "2-Test_";
  {
    IStringSection::Pointer section (new DataSourcePathIndexSection(dimIndex, paddingChar, width, numericType) );
    section->setPreText(preText);
    int index = 1;
    std::cout <<  "'" << section->toString(index, ok) << "'" << std::endl;
    MXA_REQUIRE(section->toString(index,ok).compare("2-Test_001") !=  0);
  }

	return err;
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
  int err = EXIT_SUCCESS;
  MXA_REGISTER_TEST( DataSourcePathIndexSectionTest_EntryPoint() );
  MXA_REGISTER_TEST(RemoveTestFiles() );
  return err;
}


