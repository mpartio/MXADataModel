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

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void TestBMPIO()
{
  // Insert Test Code here.
  
  
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) {
    test_suite* test= BOOST_TEST_SUITE( "Bitmap IO Testing" );
    
    test->add( BOOST_TEST_CASE( &TestBMPIO), 0);

    return test; 
}
