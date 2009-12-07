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

#include <iostream>

#include "UnitTestSupport.hpp"

int main(int argc, char **argv) {
  int err = EXIT_SUCCESS;
 std::cout << logTime() << " size of size_t" <<  sizeof(size_t) <<  std::endl;
 PRINT_TEST_SUMMARY();
  return err;
}

