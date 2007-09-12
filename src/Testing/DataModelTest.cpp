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

//TODO: Write test to add/remove Dimensions
//TODO: Test to check validity of Model/Dimensions

#include <MXAConfiguration.h>
#include "Common/MXATypeDefs.h"
#include "Core/MXAConstants.h"
#include "Core/MXAAttribute.h"
#include "Core/MXADataModel.h"
#include "Core/MXADataDimension.h"
#include "Core/MXADataRecord.h"
#include "HDF5/H5IODelegate.h"


// C++ Includes
#include <iostream>
#include <vector>
#include <map>

//Boost Includes
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include <boost/test/unit_test.hpp>

using boost::unit_test::test_suite;

typedef boost::shared_ptr<MXAAttribute> MXAAttributePtr;


// -----------------------------------------------------------------------------
//  Define where to put our temporary files
// -----------------------------------------------------------------------------
#if defined (_WIN32)
  #define FILE_NAME_BEFORE "C:\\WINDOWS\\Temp\\DataModelTest-Before.h5"
  #define FILE_NAME_AFTER "C:\\WINDOWS\\Temp\\DataModelTest-After.h5"
#else 
  #define FILE_NAME_BEFORE "/tmp/DataModelTest-Before.h5"
  #define FILE_NAME_AFTER "/tmp/DataModelTest-After.h5"
#endif



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template<typename T>
void MakeScalarAttribute(T value, std::string key, MXADataModel* model)
{
  MXAAttributePtr umd = MXAAttribute::createAttribute<T>(key, value);
  model->addUserMetaData(umd);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template<typename T>
void MakeVectorAttribute(T value, std::string key, std::vector<uint64> &dims, MXADataModel* model)
{
    std::vector<T> data;
    uint64 numelements =1;
    for (std::vector<uint64>::iterator iter = dims.begin(); iter != dims.end(); ++iter)
    {
      numelements *= *(iter);
    }
    for (uint32 i = 0; i < numelements; ++i)
    {
      data.push_back(static_cast<T>(i * 1.5) );
    }
    MXAAttributePtr vecPtr = MXAAttribute::createAttribute(key, data, dims);
    model->addUserMetaData(vecPtr);
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
    MXAAttributePtr s1 =  MXAAttribute::createAttribute("Password", std::string("DaddyO") );
    model->addUserMetaData(s1);

}

// -----------------------------------------------------------------------------
//  Creates a Data Model to use
// -----------------------------------------------------------------------------
MXADataModelPtr createModel()
{
	  std::string errorMessage;
    MXADataModelPtr modelPtr = MXADataModel::New();
	  
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
	  IDataDimensionPtr dim0 = model->addDataDimension("Volume Fraction", "Vol Frac", 0, 15, 20, 50, 2, 1);
	  errorMessage.clear();
	  BOOST_REQUIRE ( (modelPtr->isValid(errorMessage) ) == false );
	  IDataDimensionPtr dim1 = model->addDataDimension("Random Seed", "Rnd Seed", 1, 10, 1000, 5000, 500, 1);
	  IDataDimensionPtr dim2 = model->addDataDimension("Timestep", "TS", 2, 100, 0, 99, 1, 1);
	  IDataDimensionPtr dim3 = model->addDataDimension("Slice", "slice", 3, 256, 0, 255, 1, 1);
	  	  

	  
	  MXADataRecordPtr rec1 = MXADataRecord::New(1, std::string("Order Parameters"), std::string("OP") );
	  model->addDataRecord(rec1);
	  //Create Data Records with Parents
	  MXADataRecordPtr rec2 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
	  model->addDataRecord(rec2, rec1);
	  MXADataRecordPtr rec3 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
	  model->addDataRecord(rec3, rec1);
	  MXADataRecordPtr rec4 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
	  model->addDataRecord(rec4, rec1);
	  
	   MXADataRecordPtr rec5 = MXADataRecord::New(3, std::string("Order Parameters 2"), std::string("OP 2") );
	    model->addDataRecord(rec5, rec1);
	    //Create Data Records with Parents
	    MXADataRecordPtr rec6 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
	    model->addDataRecord(rec6, rec5);
	    MXADataRecordPtr rec7 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
	    model->addDataRecord(rec7, rec5);
	    MXADataRecordPtr rec8 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
	    model->addDataRecord(rec8, rec5);
	  
    //Create Data Records
    MXADataRecordPtr rec0 = MXADataRecord::New(0,std::string("Composition"), std::string("AltComp"));
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
void TestRequiredMetaData()
{
  int32 err = -1;
  std::string errorMessage;
  std::cout << "TestRequiredMetaData Running...." << std::endl;
  MXADataModelPtr model = createModel();
  
  // Test setting the Required MetaData by individual strings
  std::string researcherName("");
  std::string dateCreated("");
  std::string datasetName("");
  std::string description("");
  std::string distributionRights("");
  std::string releaseNumber("");
  std::string pedigree("");
  std::string derivedSrcFile("");
  
  err = model->setRequiredMetaData(researcherName, dateCreated, 
                                   datasetName, description, distributionRights, 
                                   releaseNumber, pedigree, derivedSrcFile);
  errorMessage.clear();
  BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );
  
  
  //Create the Required MetaData 
  std::map<std::string, std::string> md;
  md[MXA::MXA_CREATOR_TAG] = "Mike Jackson";
  err = model->setRequiredMetaData(md);
  errorMessage.clear();
  BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );
  
  md[MXA::MXA_DATE_TAG] = "2006:12:24 15:34.51";
  md[MXA::MXA_DSET_NAME_TAG] = "TESTING Example Data Model";
  md[MXA::MXA_DESCRIPTION_TAG] = "Just a test case showing how to organize OIM FIB data";
  md[MXA::MXA_PEDIGREE_TAG] = "Original";
  md[MXA::MXA_DERIVED_SRC_TAG] = "Original Data Files";
  md[MXA::MXA_RIGHTS_TAG] = "Unlimited";
  md[MXA::MXA_RELEASE_NUMBER_TAG] = "AFRL/WS07-0476";
  
  err = model->setRequiredMetaData(md);
  errorMessage.clear();
  BOOST_REQUIRE ( (model->isValid(errorMessage) ) == true );

  researcherName = "Mike Jackson";
  dateCreated = "2006:12:24 15:34.51";
  datasetName = "TESTING Example Data Model";
  description = "Just a test case showing how to organize OIM FIB data";
  distributionRights = "Unlimited";
  releaseNumber = "AFRL/WS07-0476";
  pedigree = "Original";
  derivedSrcFile = "Original Data Files";
  
  err = model->setRequiredMetaData(researcherName, dateCreated, 
                                   datasetName, description, distributionRights, 
                                   releaseNumber, pedigree, derivedSrcFile);
  errorMessage.clear();
  BOOST_REQUIRE ( (model->isValid(errorMessage) ) == true );
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool recordExists(MXADataModelPtr model, std::string recName)
{
  IDataRecordPtr rec = model->getDataRecordByNamedPath(recName);
  return (rec.get() != NULL) ? true : false;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool recordInternalPathExists(MXADataModelPtr model, std::string recName)
{
  IDataRecordPtr rec = model->getDataRecordByInternalPath(recName);
  return (rec.get() != NULL) ? true : false;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void TestLookupTableGeneration()
{
  std::cout << "TestLookupTableGeneration Running...." << std::endl;
  MXADataModelPtr modelPtr = createModel();
  IDataRecords records = modelPtr->getDataRecords();
  IDataRecordLookupTable lut;
  MXADataRecord::generateLUT(lut, records);
  for (IDataRecordLookupTable::iterator iter = lut.begin(); iter != lut.end(); ++iter )
  {
    MXADataRecord* rec = dynamic_cast<MXADataRecord*>( (*(iter)).second.get() );
    std::cout << (*(iter)).first << "  " << rec->getRecordName() << std::endl;
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void TestRetrieveDataRecords()
{ 
  std::cout << "TestRetrieveDataRecords Running...." << std::endl;
  MXADataModelPtr model = createModel();
  BOOST_REQUIRE( recordExists(model, "/Order Parameters/Eta1/") == true);
  BOOST_REQUIRE( recordExists(model, "Order Parameters/Eta1") == true);
  BOOST_REQUIRE( recordExists(model, "/Order Parameters/Eta1") == true);
  BOOST_REQUIRE( recordExists(model, "Order Parameters/Eta1/") == true);
  BOOST_REQUIRE( recordExists(model, "Order Parameters/Eta1/Junk") == false);
  BOOST_REQUIRE( recordExists(model, "Order Parameters/Order Parameters 2/Junk") == false);
  BOOST_REQUIRE( recordExists(model, "Order Parameters/Order Parameters 2/Eta1") == true);
  BOOST_REQUIRE( recordExists(model, "Order Parameters/Order Parameters 2/") == true);
  BOOST_REQUIRE( recordExists(model, "/Composition") == true);
  BOOST_REQUIRE( recordExists(model, "/Composition") == true);
  BOOST_REQUIRE( recordExists(model, "/Composition/") == true);
  BOOST_REQUIRE( recordExists(model, "Composition/") == true);
  BOOST_REQUIRE( recordExists(model, "/Composition/Eta1") == false);

  BOOST_REQUIRE( recordExists(model, "") == false);
  BOOST_REQUIRE( recordExists(model, "/") == false);
  BOOST_REQUIRE( recordExists(model, "//") == false);
  BOOST_REQUIRE( recordExists(model, "///") == false);
  

  BOOST_REQUIRE( recordInternalPathExists(model, "") == false);
  BOOST_REQUIRE( recordInternalPathExists(model, "/") == false);
  BOOST_REQUIRE( recordInternalPathExists(model, "//") == false);
  BOOST_REQUIRE( recordInternalPathExists(model, "///") == false);
  BOOST_REQUIRE( recordInternalPathExists(model, "/1") == true);
  BOOST_REQUIRE( recordInternalPathExists(model, "/0/2") == false);
  BOOST_REQUIRE( recordInternalPathExists(model, "/0/0") == false);
  BOOST_REQUIRE( recordInternalPathExists(model, "/0/1/") == false);
  BOOST_REQUIRE( recordInternalPathExists(model, "/0/3/") == false);
  BOOST_REQUIRE( recordInternalPathExists(model, "/1/3/") == true);
  BOOST_REQUIRE( recordInternalPathExists(model, "/1/3/0") == true);
  BOOST_REQUIRE( recordInternalPathExists(model, "/1/3/3") == false);
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void TestDataDimensionMethods()
{
  std::cout << "TestDataDimensionMethods Running...." << std::endl;
  MXADataModelPtr model = createModel(); // Created on the stack
  int32 error = 0;
  
  MXADataDimension* dim0 = static_cast<MXADataDimension*>(model->getDataDimension(0) );
  MXADataDimension* dim1 = static_cast<MXADataDimension*>(model->getDataDimension(1) );
  MXADataDimension* dim2 = static_cast<MXADataDimension*>(model->getDataDimension(2) );
  MXADataDimension* dim3 = static_cast<MXADataDimension*>(model->getDataDimension(3) );
  MXADataDimension* dimNull = static_cast<MXADataDimension*>(model->getDataDimension(4) );
  BOOST_REQUIRE(dim0 != NULL);
  BOOST_REQUIRE(dim1 != NULL);
  BOOST_REQUIRE(dim2 != NULL);
  BOOST_REQUIRE(dim3 != NULL);
  BOOST_REQUIRE(dimNull == NULL); // The last one should be NULL because it does NOT exist

  //Test removing by index
  error = model->removeDataDimension(3); // Remove the last Dimension
  BOOST_REQUIRE( error > 0);
  MXADataDimension* test = static_cast<MXADataDimension*>(model->getDataDimension(2) );
  BOOST_REQUIRE(test != dim3);
  BOOST_REQUIRE(model->getDataDimensions().size() == 3);
  
  error = model->removeDataDimension(4); // Remove the last Dimension
  BOOST_REQUIRE( error < 0);
  BOOST_REQUIRE(model->getDataDimensions().size() == 3);
  
  //Test Removing by Name - This Dimension does exits and should decrement the count by 1
  error = model->removeDataDimension("Random Seed");
  BOOST_REQUIRE( error > 0);
  BOOST_REQUIRE(model->getDataDimensions().size() == 2);
  
  // Test removing a Dimension by name that does NOT exist.
  error = model->removeDataDimension("Junk");
  BOOST_REQUIRE( error < 0);
  BOOST_REQUIRE(model->getDataDimensions().size() == 2);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void WriteTestModel()
{
  std::cout << "WriteTestModel Running...." << std::endl;
  MXADataModelPtr modelPtr = createModel();
  MXADataModel* model = modelPtr.get();
  std::string fileName(FILE_NAME_BEFORE);
  BOOST_REQUIRE (model->writeModel(fileName) >= 0 );
  IODelegatePtr ioPtr;
  BOOST_REQUIRE ( model->writeModel(fileName, ioPtr) < 0);

}

// -----------------------------------------------------------------------------
//  Read a model from a data file on disk, then write that model back out to 
//  a new file. Use h5dump and diff to compare the files. They should be the same
//  assuming the original file only had a DataModel in it.
// -----------------------------------------------------------------------------
void ReReadTestModel()
{
  std::cout << "ReReadTestModel Running....." << std::endl;
  
  std::string inFile(FILE_NAME_BEFORE);
  std::string outFile(FILE_NAME_AFTER);
  
  {
    MXADataModelPtr rmodel  = MXADataModel::New();
    MXADataModel* model = rmodel.get();
    BOOST_REQUIRE ( model->readModel(inFile) >= 0);
   // model->printModel(std::cout, true);
    BOOST_REQUIRE ( model->writeModel(outFile) >= 0 );
    
    IODelegatePtr ioPtr;
    BOOST_REQUIRE ( model->writeModel(outFile, ioPtr) < 0);
  }
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) {
    test_suite* test= BOOST_TEST_SUITE( "Data Model Tests" );
    //test->add( new DataModelTest () );
    
    test->add( BOOST_TEST_CASE( &WriteTestModel), 0);
    test->add( BOOST_TEST_CASE( &ReReadTestModel), 0);
    test->add( BOOST_TEST_CASE( &TestRetrieveDataRecords), 0 );
    test->add( BOOST_TEST_CASE( &TestDataDimensionMethods), 0 );
    test->add( BOOST_TEST_CASE( &TestRequiredMetaData), 0);
    
    //test->add( BOOST_TEST_CASE( &TestLookupTableGeneration), 0);
    return test; 
}
