///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#ifndef UNITTESTSUPPORT_HPP_
#define UNITTESTSUPPORT_HPP_


#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

namespace mxa
{
namespace unittest
{
  static std::string CurrentMethod("");
  static int numTestsPass = 0;
  static int numTestFailed = 0;
  static int numTests = 0;
}
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class TestException : public std::exception
{
  public:
     TestException(const std::string &what, const std::string &file, int lineNumber) :
      m_Message(what), m_File(file), m_LineNumber(lineNumber)
    {
    }

     virtual ~TestException() throw() {}

     std::string what() {
       std::stringstream ss;
       ss << "    Reason: " << m_Message << std::endl;
       ss << "    File:   " << m_File << std::endl;
       ss << "    Line:   " << m_LineNumber;
       return ss.str();
     }
protected:
     TestException() {};

  private:

     std::string m_Message;
     std::string m_File;
     int         m_LineNumber;

    // TestException(const TestException&);    // Copy Constructor Not Implemented
      void operator=(const TestException&);  // Operator '=' Not Implemented
};

#define NUM_COLS 70

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestPassed(const std::string &test)
{
  char msg[NUM_COLS];
  ::memset(msg, ' ', NUM_COLS);
  msg[NUM_COLS] = 0;
  std::string::size_type size = (NUM_COLS - sizeof("PASSED") - 1);
  ::strncpy( &(msg[size]) , "PASSED", sizeof("PASSED") );
  if (test.length() < size )
  {
    ::strncpy(msg, test.c_str(), test.length());
  }
  else
  {
    ::strncpy(msg, test.substr(0, size).c_str(), size);
  }
  std::cout << msg << std::endl;
  mxa::unittest::numTestsPass++;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestFailed(const std::string &test)
{
  char msg[NUM_COLS];
  ::memset(msg, ' ', NUM_COLS);
  msg[NUM_COLS] = 0;
  std::string::size_type size = (NUM_COLS - sizeof("FAILED") - 1);
  ::strncpy( &(msg[size]) , "FAILED", sizeof("FAILED") );
  if (test.length() < size )
  {
    ::strncpy(msg, test.c_str(), test.length());
  }
  else
  {
    ::strncpy(msg, test.substr(0, size).c_str(), size);
  }
  std::cout << msg << std::endl;
  mxa::unittest::numTestFailed++;
}

#define MXA_TEST_THROW_EXCEPTION( P)\
      throw TestException( P, __FILE__, __LINE__);\


#define MXA_ASSERT( P )\
  assert( (P) );


#define MXA_REQUIRE( P ) \
  if ( (P) == false) \
  {MXA_TEST_THROW_EXCEPTION( #P)}


#define MXA_REQUIRE_NE( L, R )\
  if ( (L) != (R) )\
  { std::string s ( #L);\
   s = s.append("==").append(#R);\
   MXA_TEST_THROW_EXCEPTION( s ) }


#define MXA_REQUIRE_EQUAL( L, R) \
    if ( (L) != (R) )\
    { std::string s ( #L);\
     s = s.append("==").append(#R);\
    MXA_TEST_THROW_EXCEPTION( s ) }


#define MXA_ENTER_TEST( test )\
  mxa::unittest::CurrentMethod = #test;\
  mxa::unittest::numTests++;


#define MXA_LEAVE_TEST( test )\
  TestPassed(#test);\
  mxa::unittest::CurrentMethod = "";


#define MXA_REGISTER_TEST( test )\
    try {\
      MXA_ENTER_TEST(test);\
      test;\
      MXA_LEAVE_TEST(test)\
    } catch (TestException& e)\
    {\
      TestFailed(mxa::unittest::CurrentMethod);\
      std::cout << e.what() << std::endl;\
      err = EXIT_FAILURE;\
    }

#define PRINT_TEST_SUMMARY()\
    std::cout << "Test Summary:" << std::endl;\
    std::cout << "  Tests Passed: " << mxa::unittest::numTestsPass << std::endl;\
    std::cout << "  Tests Failed: " << mxa::unittest::numTestFailed << std::endl;\
    std::cout << "  Total Tests:  " << mxa::unittest::numTests << std::endl;\
    if (mxa::unittest::numTestFailed > 0)\
    {\
      err = EXIT_FAILURE;\
    }\


#endif /* UNITTESTSUPPORT_HPP_ */
