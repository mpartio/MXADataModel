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

#include <MXA/MXAConfiguration.h>

#include <MXA/Common/MXATypeDefs.h>
#include <MXA/MXATypes.h>
#include <MXA/Utilities/DataSourcePathBuilder.h>
#include <MXA/Utilities/IStringSection.h>
#include <MXA/Utilities/DataSourcePathTextSection.h>
#include <MXA/Utilities/DataSourcePathIndexSection.h>
#include "UnitTestSupport.hpp"


#include <iostream>



using boost::unit_test::test_suite;


void DataSourcePathTest()
{
#if 0
  // Scope for each test
  {
  DataSourcePathBuilder builder;

  IStringSectionPtr f0 ( new DataSourcePathTextSection("/Users/mjackson/Data/Timestep-") );
  builder.addSection(f0);

  IStringSectionPtr f1 ( new DataSourcePathIndexSection(0, '0', 4, DataSourcePathSection::IntegerType) );
  builder.addSection(f1);

  IStringSectionPtr f3 ( new DataSourcePathTextSection("/slice_"));
  builder.addSection(f3);

  IStringSectionPtr f4 ( new DataSourcePathIndexSection(1, '0', 3, DataSourcePathSection::IntegerType) );
  builder.addSection(f4);

  IStringSectionPtr f5 ( new DataSourcePathTextSection(".data") );
  builder.addSection(f5);
  int8_t ok = false;
  std::vector<uint32_t> dims;
  dims.push_back(23);
  dims.push_back(234);
  std::string path = builder.build(dims, ok);
   //std::cout << "Built Path: " << path << std::endl;
  MXA_REQUIRE( path.compare("/Users/mjackson/Data/Timestep-0023/slice_234.data") == 0);
  MXA_REQUIRE(ok == true);
  }

  // Scope for each test - This test will short the values of the dimensions so there
  // is not enough values in the dimension vector. This should produce an empty string
  {
    DataSourcePathBuilder builder;

    IStringSectionPtr f0 ( new DataSourcePathTextSection("/Users/mjackson/Data/Timestep-") );
    builder.addSection(f0);

    IStringSectionPtr f1 ( new DataSourcePathIndexSection(0, '0', 4, DataSourcePathSection::IntegerType) );
    builder.addSection(f1);

    IStringSectionPtr f3 ( new DataSourcePathTextSection("/slice_"));
    builder.addSection(f3);

    IStringSectionPtr f4 ( new DataSourcePathIndexSection(1, '0', 3, DataSourcePathSection::IntegerType) );
    builder.addSection(f4);

    IStringSectionPtr f5 ( new DataSourcePathTextSection(".data") );
    builder.addSection(f5);

    std::vector<uint32_t> dims;
    dims.push_back(23);
    //dims.push_back(234);
    int8_t ok = true;
    std::string path = builder.build(dims, ok);
   // std::cout << "Built Path: " << path << std::endl;
    MXA_REQUIRE( ok == false);
  }
#endif
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
  int err = EXIT_SUCCESS;

   MXA_REGISTER_TEST(DataSourcePathTest);

   PRINT_TEST_SUMMARY();
    return err;
}


