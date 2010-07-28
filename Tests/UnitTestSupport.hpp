///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#ifndef UNITTESTSUPPORT_HPP_
#define UNITTESTSUPPORT_HPP_

#include <MXA/MXATypes.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/Core/MXASupportFile.h>
#include <MXA/DataWrappers/MXAAsciiStringData.h>


//-- C++ Includes
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

#define NUM_COLS 70


namespace mxa
{
namespace unittest
{
  static std::string CurrentMethod("");
  static int numTestsPass = 0;
  static int numTestFailed = 0;
  static int numTests = 0;

  static char TestMessage[NUM_COLS + 1];
  static const char Passed[6] = { 'P', 'A', 'S', 'S', 'E', 'D'};
  static const char Failed[6] = { 'F', 'A', 'I', 'L', 'E', 'D'};
  static int SizeOfPassed = 6;
  static int SizeOfFailed = 6;
}
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class TestException : public std::exception
{
  public:
    /**
    * @brief
    * @param what
    * @param file
    * @param lineNumber
    */
     TestException(const std::string &what, const std::string &filename, int lineNumber) :
      m_Message(what), m_Filename(filename), m_LineNumber(lineNumber)
    {
    }

    /**
    * @brief Copy Constructor
    */
     TestException(const TestException &te)
     {
       m_Message = (&te)->getMessage();
       m_Filename = te.getFilename();
       m_LineNumber = te.getLineNumber();
     }

     virtual ~TestException() throw() {}

     /**
     * @brief Over ride from base class
     */
     std::string what() {
       std::stringstream ss;
       ss << "    Reason: " << m_Message << std::endl;
       ss << "    File:   " << m_Filename << std::endl;
       ss << "    Line:   " << m_LineNumber;
       return ss.str();
     }

     MXA_INSTANCE_STRING_PROPERTY(Message)
       std::string getMessage() const { return m_Message; }
     MXA_INSTANCE_STRING_PROPERTY(Filename)
       std::string getFilename() const { return m_Filename; }

     MXA_INSTANCE_PROPERTY(int, LineNumber)
       int getLineNumber() const { return m_LineNumber; }

  protected:
     TestException() {};

  private:


      void operator=(const TestException&);  // Operator '=' Not Implemented
};


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestPassed(const std::string &test)
{
  ::memset(mxa::unittest::TestMessage, ' ', NUM_COLS); // Splat Spaces across the entire message
  mxa::unittest::TestMessage[NUM_COLS] = 0;  // Make sure it is null terminated

  std::string::size_type size = NUM_COLS - mxa::unittest::SizeOfPassed;
  ::strncpy( &(mxa::unittest::TestMessage[size]) , mxa::unittest::Passed, mxa::unittest::SizeOfPassed );
  if (test.length() < size )
  {
    ::strncpy(mxa::unittest::TestMessage, test.c_str(), test.length());
  }
  else
  {
    ::strncpy(mxa::unittest::TestMessage, test.substr(0, size).c_str(), size);
  }
  mxa::unittest::TestMessage[NUM_COLS] = 0;  // Make sure it is null terminated
  std::cout << mxa::unittest::TestMessage << std::endl;
  mxa::unittest::numTestsPass++;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestFailed(const std::string &test)
{
  ::memset(mxa::unittest::TestMessage, ' ', NUM_COLS); // Splat Spaces across the entire message
  mxa::unittest::TestMessage[NUM_COLS] = 0;  // Make sure it is null terminated

  std::string::size_type size = NUM_COLS - mxa::unittest::SizeOfFailed;
  ::strncpy( &(mxa::unittest::TestMessage[size]) , mxa::unittest::Failed, mxa::unittest::SizeOfFailed );
  if (test.length() < size )
  {
    ::strncpy(mxa::unittest::TestMessage, test.c_str(), test.length());
  }
  else
  {
    ::strncpy(mxa::unittest::TestMessage, test.substr(0, size).c_str(), size);
  }
  mxa::unittest::TestMessage[NUM_COLS] = 0;  // Make sure it is null terminated
  std::cout << mxa::unittest::TestMessage << std::endl;
  mxa::unittest::numTestFailed++;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
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



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void MakeScalarAttribute(T value, std::string key, MXADataModel* model)
{
  IMXAArray::Pointer umd = MXAArrayTemplate<T>::CreateSingleValueArray( value);
  model->addUserMetaData(key, umd);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void MakeVectorAttribute(T value, std::string key, std::vector<uint64_t> &dims, MXADataModel* model)
{
    //std::vector<T> data;
    uint64_t numelements =1;
    for (std::vector<uint64_t>::iterator iter = dims.begin(); iter != dims.end(); ++iter)
    {
      numelements *= *(iter);
    }

    IMXAArray::Pointer vecPtr = MXAArrayTemplate<T>::CreateMultiDimensionalArray( static_cast<int>(dims.size()), &(dims.front()) );
    // Copy data into the attribute container
    T* data = static_cast<T*>( vecPtr->getVoidPointer(0) );
    for (uint32_t i = 0; i < numelements; ++i)
    {
      data[i] = static_cast<T>(i * 1.5);
    }
    model->addUserMetaData(key, vecPtr);
}

// -----------------------------------------------------------------------------
//  Tests the creation of every type of Attribute that is supported
// -----------------------------------------------------------------------------
void CreateAttributes(MXADataModel* model)
{

    int8_t  i8  = -8;
    uint8_t ui8 = 8;
    int16_t i16 = -16;
    uint16_t ui16 = 16;
    int32_t i32 = -32;
    uint32_t ui32 = 32;
    int64_t i64 = -64;
    uint64_t ui64 = 64;
    float32 f32 = 32.32f;
    float64 f64 = 64.64;


    //Create vector attributes
    std::vector<uint64_t> dims;
    dims.push_back(3);
    dims.push_back(3);

    // integers
    MakeVectorAttribute( i8, "Vector Int8", dims, model);
    MakeVectorAttribute( ui8, "Vector UInt8", dims, model);
    MakeVectorAttribute( i16, "Vector Int16", dims, model);
    MakeVectorAttribute( ui16, "Vector UInt16", dims, model);
    MakeVectorAttribute( i32, "Vector Int32", dims, model);
    MakeVectorAttribute( ui32, "Vector UInt32", dims, model);
    MakeVectorAttribute( i64, "Vector Int64", dims, model);
    MakeVectorAttribute( ui64, "Vector UInt64", dims, model);

    // Floating point
    MakeVectorAttribute( f32, "Vector Float 32", dims, model);
    MakeVectorAttribute( f64, "Vector Float 64", dims, model);


   //Integer Numbers
    MakeScalarAttribute( i8, "Scalar Int 8", model);
    MakeScalarAttribute( ui8, "Scalar UInt8", model);
    MakeScalarAttribute( i16, "Scalar Int 16", model);
    MakeScalarAttribute( ui16, "Scalar UInt16", model);
    MakeScalarAttribute( i32, "Scalar Int 32", model);
    MakeScalarAttribute( ui32, "Scalar UInt32", model);
    MakeScalarAttribute( i64, "Scalar Int 64", model);
    MakeScalarAttribute( ui64, "Scalar UInt64", model);
    // Floating point Numbers
    MakeScalarAttribute( f32, "Scalar Float 32", model);
    MakeScalarAttribute( f64, "Scalar Float 64", model);

    // String attributes
    IMXAArray::Pointer s1 = MXAAsciiStringData::Create("DaddyO");
    model->addUserMetaData("Password", s1);

}

// -----------------------------------------------------------------------------
//  Creates a Data Model to use
// -----------------------------------------------------------------------------
MXADataModel::Pointer createModel()
{
    std::string errorMessage;
    MXADataModel::Pointer modelPtr = MXADataModel::New();

    MXA_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
    MXADataModel* model = modelPtr.get();
    model->setDataRoot(std::string("DataModelTest/Data/Root/Is/Here"));
    errorMessage.clear();
    MXA_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
    model->setModelType(MXA::MXACurrentFileType);
    errorMessage.clear();
    MXA_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
    model->setModelVersion(MXA::MXACurrentFileVersion);
    errorMessage.clear();
    MXA_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );

    // ---------- Test creation/addition of Data Dimensions
    IDataDimension::Pointer dim0 = model->addDataDimension("Volume Fraction", "Vol Frac",  15, 20, 50, 2, 1);
    errorMessage.clear();
    MXA_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
    IDataDimension::Pointer dim1 = model->addDataDimension("Random Seed", "Rnd Seed",  10, 1000, 5000, 500, 1);
    IDataDimension::Pointer dim2 = model->addDataDimension("Timestep", "TS",  100, 0, 99, 1, 1);
    IDataDimension::Pointer dim3 = model->addDataDimension("Slice", "slice",  256, 0, 255, 1, 1);



    MXADataRecord::Pointer rec1 = MXADataRecord::New(1, std::string("Order Parameters"), std::string("OP") );
    model->addDataRecord(rec1);
    //Create Data Records with Parents
    MXADataRecord::Pointer rec2 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
    model->addDataRecord(rec2, rec1);
    MXADataRecord::Pointer rec3 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
    model->addDataRecord(rec3, rec1);
    MXADataRecord::Pointer rec4 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
    model->addDataRecord(rec4, rec1);

     MXADataRecord::Pointer rec5 = MXADataRecord::New(3, std::string("Order Parameters 2"), std::string("OP 2") );
      model->addDataRecord(rec5, rec1);
      //Create Data Records with Parents
      MXADataRecord::Pointer rec6 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
      model->addDataRecord(rec6, rec5);
      MXADataRecord::Pointer rec7 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
      model->addDataRecord(rec7, rec5);
      MXADataRecord::Pointer rec8 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
      model->addDataRecord(rec8, rec5);

    //Create Data Records
    MXADataRecord::Pointer rec0 = MXADataRecord::New(0,std::string("Composition"), std::string("AltComp"));
    model->addDataRecord(rec0);
    errorMessage.clear();
    MXA_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );

    //Create the Required MetaData
    std::map<std::string, std::string> md;
    md[MXA::MXA_CREATOR_TAG] = "Mike Jackson";
    md[MXA::MXA_DATE_TAG] = "2006:12:24 15:34.51";
    md[MXA::MXA_DSET_NAME_TAG] = "TESTING Example Data Model";
    md[MXA::MXA_DESCRIPTION_TAG] = "Just a test case showing how to organize OIM FIB data";
    md[MXA::MXA_PEDIGREE_TAG] = "Original";
    md[MXA::MXA_DERIVED_SRC_TAG] = "Original Data Files";
    md[MXA::MXA_RIGHTS_TAG] = "Unlimited";
    md[MXA::MXA_RELEASE_NUMBER_TAG] = "AFRL/WS07-0476";

    int32_t err = -1;
    err = model->setRequiredMetaData(md);
    errorMessage.clear();
    MXA_REQUIRE ( (modelPtr->isValid(errorMessage) ) == true );
    // Create User Defined MetaData;
    CreateAttributes( model );
    return modelPtr;
}




#endif /* UNITTESTSUPPORT_HPP_ */
