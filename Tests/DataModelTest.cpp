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

//TODO: Write a Unit Test for Data Dimension Insertion

#include <MXA/MXAConfiguration.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXAEndian.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Base/IRequiredMetaData.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <MXA/HDF5/H5MXARequiredMetaData.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/DataWrappers/MXAAsciiStringData.h>
#include <Tests/MXAUnitTestDataFileLocations.h>
#include <MXA/Utilities/MXAFileSystemPath.h>

// C++ Includes
#include <iostream>
#include <vector>
#include <map>

//Boost Includes
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

//-- Boost Test Headers
#include <boost/test/unit_test.hpp>

//typedef boost::shared_ptr<MXAAttribute> MXAAttributePtr;


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
  MXAFileSystemPath::remove(MXAUnitTest::DataModelTest::BeforeH5File);
  MXAFileSystemPath::remove(MXAUnitTest::DataModelTest::AfterH5File);
  MXAFileSystemPath::remove(MXAUnitTest::DataModelTest::OverwriteH5File);
#endif
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
void TestRequiredMetaData()
{
  int32 err = -1;
  std::string errorMessage;
  std::cout << "TestRequiredMetaData Running...." ;
  MXADataModel::Pointer model = createModel();

  // Test setting the Required MetaData by individual strings
  std::string researcherName("");
  std::string dateCreated("");
  std::string datasetName("");
  std::string description("");
  std::string distributionRights("");
  std::string releaseNumber("");
  std::string pedigree("");
  std::string derivedSrcFile("");

  IRequiredMetaData::Pointer metaData = H5MXARequiredMetaData::New(researcherName, dateCreated,
                                   datasetName, description, distributionRights,
                                   releaseNumber, pedigree, derivedSrcFile);
  BOOST_REQUIRE(metaData->isValid(errorMessage) == false);



  err = model->setRequiredMetaData(researcherName, dateCreated,
                                   datasetName, description, distributionRights,
                                   releaseNumber, pedigree, derivedSrcFile);
  errorMessage.clear();
  BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );

  err = model->setRequiredMetaData(metaData);
  errorMessage.clear();
  BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );


  //Create the Required MetaData
  std::map<std::string, std::string> md;
  md[MXA::MXA_CREATOR_TAG] = "Mike Jackson";
  err = model->setRequiredMetaData(md);
  BOOST_REQUIRE(err == -1); // The data should NOT validate
  errorMessage.clear();

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
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE ( (model->isValid(errorMessage) ) == true );

  metaData = H5MXARequiredMetaData::New(researcherName, dateCreated,
                                     datasetName, description, distributionRights,
                                     releaseNumber, pedigree, derivedSrcFile);
  BOOST_REQUIRE(metaData->isValid(errorMessage) == true);
  err = model->setRequiredMetaData(metaData);
  errorMessage.clear();
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE ( (model->isValid(errorMessage) ) == true );

  std::cout << "......Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool recordExists(MXADataModel::Pointer model, std::string recName)
{
  IDataRecord::Pointer rec = model->getDataRecordByNamedPath(recName);
  return (rec.get() != NULL) ? true : false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool recordInternalPathExists(MXADataModel::Pointer model, std::string recName)
{
  IDataRecord::Pointer rec = model->getDataRecordByInternalPath(recName);
  return (rec.get() != NULL) ? true : false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestLookupTableGeneration()
{
  std::cout << "TestLookupTableGeneration Running...." << std::endl;
  MXADataModel::Pointer modelPtr = createModel();
  IDataRecord::Container records = modelPtr->getDataRecords();
  IDataRecord::LookupTable lut;
  MXADataRecord::generateLUT(lut, records);
  for (IDataRecord::LookupTable::iterator iter = lut.begin(); iter != lut.end(); ++iter )
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
  std::cout << "TestRetrieveDataRecords Running....";
  MXADataModel::Pointer model = createModel();
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
  std::cout << "......Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestDataDimensionMethods()
{
  std::cout << "Test DataDimensionMethods Running....";
  MXADataModel::Pointer model = createModel(); // Created on the stack
  int32 error = 0;

  IDataDimension::Pointer dim0 = model->getDataDimension(0);
  IDataDimension::Pointer dim1 = model->getDataDimension(1);
  IDataDimension::Pointer dim2 = model->getDataDimension(2);
  IDataDimension::Pointer dim3 = model->getDataDimension(3);
  IDataDimension::Pointer dimNull = model->getDataDimension(4);
  BOOST_REQUIRE(dim0.get() != NULL);
  BOOST_REQUIRE(dim1.get() != NULL);
  BOOST_REQUIRE(dim2.get() != NULL);
  BOOST_REQUIRE(dim3.get() != NULL);
  BOOST_REQUIRE(dimNull.get() == NULL); // The last one should be NULL because it does NOT exist

  //Test removing by index
  error = model->removeDataDimension(3); // Remove the last Dimension
  BOOST_REQUIRE( error > 0);
  IDataDimension::Pointer test = model->getDataDimension(2);
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
  std::cout << "......Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void WriteTestModel()
{
  {
    std::cout << "WriteTestModel Running....";
    MXADataModel::Pointer modelPtr = createModel();
    IDataFile::Pointer dataFile = H5MXADataFile::CreateFileWithModel(MXAUnitTest::DataModelTest::BeforeH5File, modelPtr);
    BOOST_REQUIRE(dataFile.get() != 0x0);
    std::cout << "......Passed" << std::endl;
  }

  {
    std::cout << "Read DataModel Running...." ;
    IDataFile::Pointer dataFile = H5MXADataFile::OpenFile(MXAUnitTest::DataModelTest::BeforeH5File, false);
    BOOST_REQUIRE(dataFile.get() != 0x0);
    IDataModel::Pointer modelPtr = dataFile->getDataModel();
    BOOST_REQUIRE(modelPtr.get() != 0x0);
    // Resave the model. This should work with out any problems.
    BOOST_REQUIRE (dataFile->saveDataModel() >= 0);
    std::cout << "......Passed" << std::endl;
  }
}

// -----------------------------------------------------------------------------
//  Read a model from a data file on disk, then write that model back out to
//  a new file. Use h5dump and diff to compare the files. They should be the same
//  assuming the original file only had a DataModel in it.
// -----------------------------------------------------------------------------
void ReWriteModelTest()
{
  std::cout << "ReWriteModelTest Running.....";

  std::string inFilename(MXAUnitTest::DataModelTest::BeforeH5File);
  std::string outFilename(MXAUnitTest::DataModelTest::AfterH5File);

  {
    IDataFile::Pointer dataFile = H5MXADataFile::OpenFile(inFilename, false);
    BOOST_REQUIRE(dataFile.get() != 0x0);

    IDataModel::Pointer modelPtr  = dataFile->getDataModel();
    BOOST_REQUIRE(modelPtr.get() != 0x0);

    IDataFile::Pointer outFile = H5MXADataFile::CreateFileWithModel(outFilename, modelPtr);
    BOOST_REQUIRE(outFile.get() != 0x0);
  }

  std::cout << "......Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestDimensionCount()
{
  std::cout << "Testing Dimension Count Running...." ;
  IDataDimension::Pointer dim = MXADataDimension::New("Test", "Test", 0, 10, 0, 9, 1, 1);
  int32 count = dim->calculateCount();
  BOOST_REQUIRE(count == 10);

  dim->setStartValue(1);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 9);

  dim->setStartValue(2);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 8);

  dim->setStartValue(-1);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 11);


  dim->setStartValue(-10);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 20);

  dim->setIncrement(3);
  dim->setStartValue(0);
  dim->setEndValue(8);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 3);

  dim->setEndValue(9);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 4);

  dim->setIncrement(2);
  dim->setStartValue(0);
  dim->setEndValue(8);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 5);

  dim->setIncrement(3);
  dim->setStartValue(1);
  dim->setEndValue(30);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 10);

  dim->setEndValue(31);
  count = dim->calculateCount();
  BOOST_REQUIRE(count == 11);
  std::cout << "......Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestEndianSwap()
{
  std::cout << "Testing Endian Swapping Running....";
  uint32 value = 0xAABBCCDD;
  MXA::Endian::reverseBytes ( value );
  BOOST_REQUIRE(0xDDCCBBAA == value);

  uint16 value16 = 0xAABB;
  MXA::Endian::reverseBytes( value16);
  BOOST_REQUIRE(0xBBAA == value16);
  std::cout << "......Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestDataRecordRemoval()
{
  std::cout << "TestDataRecordRemoval Running....";
  MXADataModel::Pointer modelPtr = createModel();
  MXADataModel* model = modelPtr.get();
  IDataRecord::Pointer nullRec;
  IDataRecord::Pointer r0;
  {
    IDataRecord::Container records = model->getDataRecords();
    r0 = records[0];
    BOOST_REQUIRE( r0.use_count() == 3);
    model->removeDataRecord(r0);
    BOOST_REQUIRE( r0.use_count() == 2);
  }
  BOOST_REQUIRE( model->getDataRecords().size() == 1);
  BOOST_REQUIRE( r0.use_count() == 1);
  r0.reset(nullRec.get());
  std::cout << "......Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestDataModelOverWrite()
{
  std::cout << "TestDataModelOverWrite Running...." ;
  std::string outFilename (MXAUnitTest::DataModelTest::OverwriteH5File);
  {


    MXADataModel::Pointer modelPtr = createModel();
    IDataFile::Pointer dataFile = H5MXADataFile::CreateFileWithModel(outFilename, modelPtr);
    // Make sure nothing went wrong in the file creation
    BOOST_REQUIRE(dataFile.get() != 0x0);
    //Make sure the model pointers are the same. They SHOULD be.
    BOOST_REQUIRE(dataFile->getDataModel().get() == modelPtr.get() );

    MXADataModel* model = modelPtr.get();
    // Update one of the dimensions
    IDataDimension::Pointer dim0 = model->getDataDimension(0);
    //IDataDimension::Pointer dim0 = model->addDataDimension("Volume Fraction", "Vol Frac",  15, 20, 50, 2, 1);
    dim0->setStartValue(0);
    dim0->setEndValue(9);
    dim0->setIncrement(1);
    int32 count = dim0->calculateCount();
    model->setDataRoot("New/Data/Root/");
    BOOST_REQUIRE(count == 10);
    int8  i8  = -8 * 2;
    uint8 ui8 = 8 * 2;
    int16 i16 = -16 * 2;
    uint16 ui16 = 16 * 2;
    int32 i32 = -32 * 2;
    uint32 ui32 = 32 * 2;
    int64 i64 = -64 * 2;
    uint64 ui64 = 64 * 2;
    float32 f32 = 32.32f * 2;
    float64 f64 = 64.64 * 2;
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
    int32 err = dataFile->saveDataModel();
    BOOST_REQUIRE(err >= 0); // This must pass
  }
  //Reread the model from the file and compare values
  {
    IDataFile::Pointer dataFile = H5MXADataFile::OpenFile(outFilename, true);
    BOOST_REQUIRE(dataFile.get() != 0x0);
    IDataModel::Pointer modelPtr = dataFile->getDataModel();
    IDataDimension::Pointer dim0 = modelPtr->getDataDimension(0);
    BOOST_REQUIRE ( dim0->getCount() == 10);
    BOOST_REQUIRE ( dim0->getStartValue() == 0);
    BOOST_REQUIRE ( dim0->getEndValue() == 9);
    BOOST_REQUIRE ( dim0->getIncrement() == 1);
    BOOST_REQUIRE ( modelPtr->getDataRoot().compare("New/Data/Root/") == 0);
  }

  std::cout << "......Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//  Shared Pointer Test
// -----------------------------------------------------------------------------
void SharedPointerTest()
{
  std::cout << "SharedPointerTest Running...." ;
  
  MXADataModel::Pointer model = createModel();
  IDataDimension::Pointer dim = model->getDataDimension(0);

  IDataDimension::Pointer nDim = MXADataDimension::New("Name", "Name", 0, 10, 0, 9, 1, 1);
  IDataModel::Pointer nModel = MXADataModel::New(MXA::MXACurrentFileVersion, MXA::ModelType, "Data");
  nModel->addDataDimension(nDim);
  //nModel->printModel(std::cout, 2);
  std::cout << "......Passed" << std::endl;
  
}

// -----------------------------------------------------------------------------
//  Test the copy constructors
// -----------------------------------------------------------------------------
void CopyConstructorTest()
{
  std::cout << "CopyConstructorTest Running...." ;
  
  IDataDimension::Pointer nDim = MXADataDimension::New("Name", "Name", 0, 10, 0, 9, 1, 1);
  IDataDimension::Pointer copy = MXADataDimension::New(nDim);
  BOOST_REQUIRE(nDim.get() != copy.get());
  
  IDataRecord::Pointer rec = MXADataRecord::New(0, "Name", "Name");
  IDataRecord::Pointer rCopy = MXADataRecord::New(rec);
  BOOST_REQUIRE(rec.get() != rCopy.get());
  
  std::cout << "......Passed" << std::endl;
  
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] )
{

  boost::unit_test::test_suite* test= BOOST_TEST_SUITE( "Data Model Tests" );
  //test->add( new DataModelTest () );
  test->add( BOOST_TEST_CASE( &SharedPointerTest), 0);
  test->add( BOOST_TEST_CASE( &WriteTestModel), 0);
  test->add( BOOST_TEST_CASE( &ReWriteModelTest), 0);
  test->add( BOOST_TEST_CASE( &TestRetrieveDataRecords), 0 );
  test->add( BOOST_TEST_CASE( &TestDataDimensionMethods), 0 );
  test->add( BOOST_TEST_CASE( &TestRequiredMetaData), 0);
  test->add (BOOST_TEST_CASE( &TestDimensionCount), 0);
  test->add (BOOST_TEST_CASE( &TestEndianSwap), 0);
  test->add (BOOST_TEST_CASE( &TestDataRecordRemoval), 0);
  test->add (BOOST_TEST_CASE( &TestDataModelOverWrite), 0);
  test->add (BOOST_TEST_CASE( &SharedPointerTest), 0);
  test->add (BOOST_TEST_CASE( &CopyConstructorTest), 0);
  test->add (BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}

