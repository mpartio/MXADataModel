///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#include "MXA/Base/IMXADataModelWriterDelegate.h"
#include "MXA/Core/MXADataModelWriter.hpp"
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/HDF5/H5WriterDelegate.h>
#include "MXA/XML/XMLStreamWriterDelegate.hpp"
#include "MXA/Utilities/MXAFileSystemPath.h"
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/DataWrappers/MXAAsciiStringData.h>
#include "MXAUnitTestDataFileLocations.h"

#include <iostream>

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


namespace Testing
{
namespace WriterDelegateTest
{
	const std::string TestFile ("WriterDelegateTest_OutputFile.dat");
}

}

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
void MakeVectorAttribute(T value, std::string key, std::vector<uint64> &dims, MXADataModel* model)
{
    //std::vector<T> data;
    uint64 numelements =1;
    for (std::vector<uint64>::iterator iter = dims.begin(); iter != dims.end(); ++iter)
    {
      numelements *= *(iter);
    }

    IMXAArray::Pointer vecPtr = MXAArrayTemplate<T>::CreateMultiDimensionalArray( static_cast<int>(dims.size()), &(dims.front()) );
    // Copy data into the attribute container
    T* data = static_cast<T*>( vecPtr->getVoidPointer(0) );
    for (uint32 i = 0; i < numelements; ++i)
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

    int8  i8  = -8;
    uint8 ui8 = 8;
    int16 i16 = -16;
    uint16 ui16 = 16;
    int32 i32 = -32;
    uint32 ui32 = 32;
    int64 i64 = -64;
    uint64 ui64 = 64;
    float32 f32 = 32.32f;
    float64 f64 = 64.64;


    //Create vector attributes
    std::vector<uint64> dims;
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

    BOOST_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
    MXADataModel* model = modelPtr.get();
    model->setDataRoot(std::string("DataModelTest/Data/Root/Is/Here"));
    errorMessage.clear();
    BOOST_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
    model->setModelType(MXA::MXACurrentFileType);
    errorMessage.clear();
    BOOST_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
    model->setModelVersion(MXA::MXACurrentFileVersion);
    errorMessage.clear();
    BOOST_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );

    // ---------- Test creation/addition of Data Dimensions
    IDataDimension::Pointer dim0 = model->addDataDimension("Volume Fraction", "Vol Frac",  15, 20, 50, 2, 1);
    errorMessage.clear();
    BOOST_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
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
    BOOST_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );

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

    int32 err = -1;
    err = model->setRequiredMetaData(md);
    errorMessage.clear();
    BOOST_REQUIRE ( (modelPtr->isValid(errorMessage) ) == true );
    // Create User Defined MetaData;
    CreateAttributes( model );
    return modelPtr;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
  std::cout << "   Removing Test files" << std::endl;
#if REMOVE_TEST_FILES
  MXAFileSystemPath::remove(Testing::WriterDelegateTest::TestFile);
#endif
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int WriterDelegateTest_EntryPoint()
{
	int err = 0;

//	 {
//	  typedef H5WriterDelegate H5DelegateType;
//	  H5DelegateType::Pointer delegate = H5DelegateType::New();
//
//    MXADataModelWriter<H5DelegateType>::Pointer writer =
//                             MXADataModelWriter<H5DelegateType>::New(delegate);
//    MXADataModel::Pointer model = createModel();
//      //  err = writer->writeModel(model);
//	  }


	{
	  typedef XMLStreamWriterDelegate<std::ostringstream> XMLStreamDelegateType;
	  XMLStreamDelegateType::Pointer delegate = XMLStreamDelegateType::New();
    MXADataModelWriter<XMLStreamDelegateType>::Pointer writer =
                             MXADataModelWriter<XMLStreamDelegateType>::New(delegate);

    MXADataModel::Pointer model = createModel();
    err = writer->writeModel(model);

    std::ostringstream& ref = *(delegate->getStreamPointer());
    std::cout << "ref: <============>\n" << ref.str() << std::endl;
	}

	{
	  MXADataModel::Pointer model = createModel();

	  typedef XMLStreamWriterDelegate<std::ofstream> FileStreamType;
	  FileStreamType::Pointer delegate = FileStreamType::New();
	  std::ofstream& out = *(delegate->getStreamPointer());
	  out.open(Testing::WriterDelegateTest::TestFile.c_str());
	  if (NULL == delegate.get() || false == out.is_open() )
	   {
	    BOOST_ASSERT(1 == 0);
	   }

	  MXADataModelWriter<FileStreamType>::Pointer writer =
	                            MXADataModelWriter<FileStreamType>::New(delegate);
	   err = writer->writeModel(model);
	   delegate->getStreamPointer()->flush();
	   delegate->getStreamPointer()->close();
	}




	return err;
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "WriterDelegateTest Test Running");
  test->add( BOOST_TEST_CASE( &WriterDelegateTest_EntryPoint), 0);
 // test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}


