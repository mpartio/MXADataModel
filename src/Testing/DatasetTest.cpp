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

//-- MXA Includes
#include <MXAConfiguration.h>
#include <TestDataFileLocations.h>
#include <Common/MXATypeDefs.h>
#include <Base/IDataFile.h>
#include <Base/IDataModel.h>
#include <Base/IFileWriter.h>
#include <Core/MXADataModel.h>
#include <HDF5/H5MXADataFile.h>
#include <HDF5/H5Utilities.h>

#include <Dataset/IAbstractDataset.h>
#include <Dataset/IAbstractAttribute.h>
#include <Dataset/H5StringDataset.h>
#include <Dataset/H5StringAttribute.h>
#include <Dataset/H5PointerDataset.h>
#include <Dataset/H5PointerAttribute.h>
#include <Dataset/H5VectorRefDataset.h>

// C++ Includes
#include <iostream>
#include <string>

//-- Boost Test Headers
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
//-- Boost Filesystem Headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
#define IFILEWRITER_DYNAMIC_CAST(inVar, outvar)   \
  boost::shared_ptr<IFileWriter> outvar = boost::dynamic_pointer_cast<IFileWriter>(inVar);

#define SHARED_POINTER_DYNAMIC_CAST(fromVar, toType, toVar) \
  boost::shared_ptr<toType> toVar = boost::dynamic_pointer_cast<toType>(fromVar);

#define DECLARE_REFERENCE_VARIABLE(classType ,shared_ptr, ref) \
  classType &ref = *(shared_ptr.get());


#define CreatePointerAttribute(U, dsPath, key) \
  name.clear(); \
  name.append(key).append(" attribute");\
  U U##vec[4];\
  for (int i = 0; i < 4; ++i) { U##vec[i] = (U)(127); } \
  attr = H5PointerAttribute<U>::New(dsPath, name, attrDimensions, U##vec );\
  testDataset->addAttribute(attr);

#define CreateVectorAttribute(U, dsPath, key) \
name.clear(); \
name.append(key).append(" attribute");\
U U##vec[4];\
for (int i = 0; i < 4; ++i) { U##vec[i] = (U)(127); } \
attr = H5PointerAttribute<U>::New(dsPath, name, attrDimensions, U##vec );\
testDataset->addAttribute(attr);

// Declare all the methods
IDataModelPtr createModel();
void DatasetTest();
void MakeDataRecord( const std::string &key, IDataModelPtr model);


int recLuid = 0;


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
  BOOST_REQUIRE ( boost::filesystem::remove(DATASET_TEST_FILE) == true );
#endif
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------

void MakeDataRecord( const std::string &key, IDataModelPtr model)
{
  MXADataRecordPtr rec = MXADataRecord::New(recLuid++, key, key );
  model->addDataRecord(rec);
}

// -----------------------------------------------------------------------------
//  Creates a Data Model to use
// -----------------------------------------------------------------------------
IDataModelPtr createModel()
{
    std::string errorMessage;
    IDataModelPtr model = MXADataModel::New();
    
    BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );
    
    model->setDataRoot(std::string("DatasetTest/"));
    errorMessage.clear();
    BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );
    model->setModelType(MXA::MXACurrentFileType);
    errorMessage.clear();
    BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );
    model->setModelVersion(MXA::MXACurrentFileVersion);
    errorMessage.clear();
    BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );

    // ---------- Test creation/addition of Data Dimensions
    IDataDimensionPtr dim0 = model->addDataDimension("Data Container", "Data Container",  10, 0, 9, 1, 1);
    errorMessage.clear();
    BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );
       
    //Create Data Records for the Pointer Tests
    MakeDataRecord( "Pointer Int 8", model);
    MakeDataRecord( "Pointer UInt8", model);
    MakeDataRecord( "Pointer Int 16", model);
    MakeDataRecord( "Pointer UInt16", model);
    MakeDataRecord( "Pointer Int 32", model);
    MakeDataRecord( "Pointer UInt32", model);
    MakeDataRecord( "Pointer Int 64", model);
    MakeDataRecord( "Pointer UInt64", model);
    // Floating point Numbers
    MakeDataRecord( "Pointer Float 32", model);
    MakeDataRecord( "Pointer Float 64", model);
    
    // Create Data record for the String test
    MakeDataRecord( "String", model);
    
    // Create Data Records for the Vector Tests
    MakeDataRecord( "Vector Int 8", model);
    MakeDataRecord( "Vector UInt8", model);
    MakeDataRecord( "Vector Int 16", model);
    MakeDataRecord( "Vector UInt16", model);
    MakeDataRecord( "Vector Int 32", model);
    MakeDataRecord( "Vector UInt32", model);
    MakeDataRecord( "Vector Int 64", model);
    MakeDataRecord( "Vector UInt64", model);
    // Floating point Numbers
    MakeDataRecord( "Vector Float 32", model);
    MakeDataRecord( "Vector Float 64", model);

    errorMessage.clear();
    BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );

    //Create the Required MetaData 
    std::map<std::string, std::string> md;
    md[MXA::MXA_CREATOR_TAG] = "Mike Jackson";
    md[MXA::MXA_DATE_TAG] = "2007:12:24 15:34.51";
    md[MXA::MXA_DSET_NAME_TAG] = "TESTING Example Data Model";
    md[MXA::MXA_DESCRIPTION_TAG] = "Testing the Data Containers API";
    md[MXA::MXA_PEDIGREE_TAG] = "Original";
    md[MXA::MXA_DERIVED_SRC_TAG] = "Original Data Files";
    md[MXA::MXA_RIGHTS_TAG] = "Unlimited";
    md[MXA::MXA_RELEASE_NUMBER_TAG] = "Not Applicable";
    
    int32 err = -1;
    err = model->setRequiredMetaData(md);
    errorMessage.clear();
    BOOST_REQUIRE ( (model->isValid(errorMessage) ) == true );
    return model;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template<typename T> 
int32 PointerTest(T theType, const std::string &recName, IDataFilePtr dataFile)
{
  IDataModelPtr model = dataFile->getDataModel();
  IDataRecordPtr rec = model->getDataRecordByNamedPath(recName, NULL);
  BOOST_REQUIRE(rec.get() != NULL);

  std::vector<int32> mxaDims;
  mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
  std::string dsPath = H5Utilities::generateH5PathToDataset(model, mxaDims, rec);

  std::vector<uint64> datasetDims(1, 10); // 1 element with a value of 10
  T dvec[10];
  for (int i = 0; i < 10; ++i)
  {
    dvec[i] = (T)(i * 5643.234523);
  } // Fill with some data
  IAbstractDatasetPtr testDataset = H5PointerDataset<T>::New(dsPath, datasetDims, dvec);
  BOOST_REQUIRE(1 == testDataset->getDatasetDimensions().size() );
  BOOST_REQUIRE(10 == testDataset->getDatasetDimensions().at(0) );

  //Create a string Attribute 
  IAbstractAttributePtr strAttribute = H5StringAttribute::New(dsPath, "String Attribute", "Test String");
  BOOST_REQUIRE (strAttribute.get() != NULL);
  testDataset->addAttribute(strAttribute);

  // Create a Pointer based Attribute
  IAbstractAttributePtr attr;
  std::string name;
  std::vector<uint64> attrDimensions(1, 4); // 1 element with a value of 10
  
  CreatePointerAttribute(int8, dsPath, "Int 8")
  CreatePointerAttribute(uint8, dsPath, "UInt8")
  CreatePointerAttribute(int16, dsPath, "Int 16")
  CreatePointerAttribute(uint16, dsPath, "UInt16")
  CreatePointerAttribute(int32, dsPath, "Int 32")
  CreatePointerAttribute(uint32, dsPath, "UInt32")
  CreatePointerAttribute(int64, dsPath, "Int 64")
  CreatePointerAttribute(uint64, dsPath, "UInt64")
  //  //Floating Point Numbers
  CreatePointerAttribute(float32, dsPath, "Float 32")
  CreatePointerAttribute(float64, dsPath, "Float 64")

  int32 err = dataFile->writeData(testDataset);
  
  int numAttrs = testDataset->getNumberOfAttributes();
  BOOST_REQUIRE(numAttrs == 11);
  err = testDataset->removeAttribute(attr);
  BOOST_REQUIRE (err >= 0);
  numAttrs = testDataset->getNumberOfAttributes();
  BOOST_REQUIRE(numAttrs == 10);
  err = testDataset->removeAttribute(9); // Remove the last attribute
  BOOST_REQUIRE (err >= 0);
  numAttrs = testDataset->getNumberOfAttributes();
  BOOST_REQUIRE(numAttrs == 9);
  
  // try removing a value outside the range of the attributes
  err = testDataset->removeAttribute(9); 
  BOOST_REQUIRE (err == 0);
  numAttrs = testDataset->getNumberOfAttributes();
  BOOST_REQUIRE(numAttrs == 9);
  
  err = testDataset->removeAttribute(-1); 
  BOOST_REQUIRE (err == 0);
  numAttrs = testDataset->getNumberOfAttributes();
  BOOST_REQUIRE(numAttrs == 9);
  
  // Try removing the first attribute at index 0
  err = testDataset->removeAttribute(0); 
  BOOST_REQUIRE (err >= 0);
  numAttrs = testDataset->getNumberOfAttributes();
  BOOST_REQUIRE(numAttrs == 8);
  
  
  // Try removing an attribute from the middle
  err = testDataset->removeAttribute(5); 
  BOOST_REQUIRE (err >= 0);
  numAttrs = testDataset->getNumberOfAttributes();
  BOOST_REQUIRE(numAttrs == 7);
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5StringDatasetTest(IDataFilePtr dataFile)
{
   IDataModelPtr model = dataFile->getDataModel();
   IDataRecordPtr rec = model->getDataRecordByNamedPath("String", NULL);
   BOOST_REQUIRE(rec.get() != NULL);

   std::vector<int32> mxaDims;
   mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
   std::string data ("This is the string Dataset");
   std::string dsPath = H5Utilities::generateH5PathToDataset(model, mxaDims, rec);

   // Create the dataset
   IAbstractDatasetPtr testDataset = H5StringDataset::New(dsPath, data);
   BOOST_REQUIRE (testDataset.get() != NULL);
     
  //Create a string Attribute 
   IAbstractAttributePtr strAttribute = H5StringAttribute::New(dsPath, "String Attribute", "Test String");
   BOOST_REQUIRE (strAttribute.get() != NULL);
   testDataset->addAttribute(strAttribute);

   // Create a Pointer based Attribute
   IAbstractAttributePtr attr;
   std::string name;
   std::vector<uint64> attrDimensions(1, 4); // 1 element with a value of 10
   
   CreatePointerAttribute(int8, dsPath, "Int 8")
   CreatePointerAttribute(uint8, dsPath, "UInt8")
   CreatePointerAttribute(int16, dsPath, "Int 16")
   CreatePointerAttribute(uint16, dsPath, "UInt16")
   CreatePointerAttribute(int32, dsPath, "Int 32")
   CreatePointerAttribute(uint32, dsPath, "UInt32")
   CreatePointerAttribute(int64, dsPath, "Int 64")
   CreatePointerAttribute(uint64, dsPath, "UInt64")
   //  //Floating Point Numbers
   CreatePointerAttribute(float32, dsPath, "Float 32")
   CreatePointerAttribute(float64, dsPath, "Float 64")

   int32 err = dataFile->writeData(testDataset);
   
   int numAttrs = testDataset->getNumberOfAttributes();
   BOOST_REQUIRE(numAttrs == 11);
   err = testDataset->removeAttribute(attr);
   BOOST_REQUIRE (err >= 0);
   numAttrs = testDataset->getNumberOfAttributes();
   BOOST_REQUIRE(numAttrs == 10);
   err = testDataset->removeAttribute(9); // Remove the last attribute
   BOOST_REQUIRE (err >= 0);
   numAttrs = testDataset->getNumberOfAttributes();
   BOOST_REQUIRE(numAttrs == 9);
   
   // try removing a value outside the range of the attributes
   err = testDataset->removeAttribute(9); 
   BOOST_REQUIRE (err == 0);
   numAttrs = testDataset->getNumberOfAttributes();
   BOOST_REQUIRE(numAttrs == 9);
   
   err = testDataset->removeAttribute(-1); 
   BOOST_REQUIRE (err == 0);
   numAttrs = testDataset->getNumberOfAttributes();
   BOOST_REQUIRE(numAttrs == 9);
   
   // Try removing the first attribute at index 0
   err = testDataset->removeAttribute(0); 
   BOOST_REQUIRE (err >= 0);
   numAttrs = testDataset->getNumberOfAttributes();
   BOOST_REQUIRE(numAttrs == 8);
   
   
   // Try removing an attribute from the middle
   err = testDataset->removeAttribute(5); 
   BOOST_REQUIRE (err >= 0);
   numAttrs = testDataset->getNumberOfAttributes();
   BOOST_REQUIRE(numAttrs == 7);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template<typename T> 
int32 VectorTest(T theType, const std::string &recName, IDataFilePtr dataFile)
{
  IDataModelPtr model = dataFile->getDataModel();
  IDataRecordPtr rec = model->getDataRecordByNamedPath(recName, NULL);
  BOOST_REQUIRE(rec.get() != NULL);
  
  std::vector<int32> mxaDims;
  mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
  std::string dsPath = H5Utilities::generateH5PathToDataset(model, mxaDims, rec);
  // Watch out for Scope issues with feeding H5VectorDataset the data variable. If 
  //  'data' were to go out of scope BEFORE the write operation then junk
  //  is going to be written to the file
  std::vector<uint64> datasetDims(1, 10); // 1 element with a value of 10
  std::vector<T> data(10, 0);
  for (int i = 0; i < 10; ++i) { data[i] = (T)(100); }
  IAbstractDatasetPtr testDataset = H5VectorRefDataset<T>::New(dsPath, datasetDims, data);
  
  
  int32 err = dataFile->writeData(testDataset);
  
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DatasetTest()
{
  std::cout << "Running Dataset Test ....";
  // Create our Test File to output our test data into
  std::string testFile (DATASET_TEST_FILE);
  IDataModelPtr model = createModel();
  IDataFilePtr dataFile = H5MXADataFile::CreateFileWithModel(testFile, model);
  
  // Test String datasets
  H5StringDatasetTest(dataFile);
  
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
  
  // Test the Pointer Datasets
  BOOST_REQUIRE ( PointerTest( i8, "Pointer Int 8", dataFile) >= 0 );
  BOOST_REQUIRE ( PointerTest( ui8, "Pointer UInt8", dataFile) >= 0);
  BOOST_REQUIRE ( PointerTest( i16, "Pointer Int 16", dataFile) >= 0);
  BOOST_REQUIRE ( PointerTest( ui16, "Pointer UInt16", dataFile) >= 0);
  BOOST_REQUIRE ( PointerTest( i32, "Pointer Int 32", dataFile) >= 0);
  BOOST_REQUIRE ( PointerTest( ui32, "Pointer UInt32", dataFile) >= 0);
  BOOST_REQUIRE ( PointerTest( i64, "Pointer Int 64", dataFile) >= 0);
  BOOST_REQUIRE ( PointerTest( ui64, "Pointer UInt64", dataFile) >= 0);
  // Floating point Numbers
  BOOST_REQUIRE ( PointerTest( f32, "Pointer Float 32", dataFile) >= 0);
  BOOST_REQUIRE ( PointerTest( f64, "Pointer Float 64", dataFile) >= 0);
  
  // Test the Vector based dataset classes
  BOOST_REQUIRE ( VectorTest( i8, "Vector Int 8", dataFile) >= 0 );
  BOOST_REQUIRE ( VectorTest( ui8, "Vector UInt8", dataFile) >= 0);
  BOOST_REQUIRE ( VectorTest( i16, "Vector Int 16", dataFile) >= 0);
  BOOST_REQUIRE ( VectorTest( ui16, "Vector UInt16", dataFile) >= 0);
  BOOST_REQUIRE ( VectorTest( i32, "Vector Int 32", dataFile) >= 0);
  BOOST_REQUIRE ( VectorTest( ui32, "Vector UInt32", dataFile) >= 0);
  BOOST_REQUIRE ( VectorTest( i64, "Vector Int 64", dataFile) >= 0);
  BOOST_REQUIRE ( VectorTest( ui64, "Vector UInt64", dataFile) >= 0);
  // Floating point Numbers
  BOOST_REQUIRE ( VectorTest( f32, "Vector Float 32", dataFile) >= 0);
  BOOST_REQUIRE ( VectorTest( f64, "Vector Float 64", dataFile) >= 0);
  
  std::cout << "... Passed." << std::endl;
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) 
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE( "Dataset Tests" );
  test->add( BOOST_TEST_CASE( &DatasetTest), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test; 
}



