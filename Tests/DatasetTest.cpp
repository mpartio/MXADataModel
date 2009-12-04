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
#include <MXA/MXAConfiguration.h>
#include <Tests/MXAUnitTestDataFileLocations.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Base/IDataModel.h>
#include <MXA/Base/IFileWriter.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/HDF5/H5MXAUtilities.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>

#include <MXA/HDF5/H5Dataset.h>
#include <MXA/DataWrappers/MXAAsciiStringData.h>

#include <MXA/DataWrappers/MXA2DArray.hpp>
#include <MXA/DataWrappers/MXARGBImage.h>
#include <MXA/Utilities/MXAFileSystemPath.h>

// C++ Includes
#include <iostream>
#include <string>

//-- Boost Test Headers
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
#define IFILEWRITER_DYNAMIC_CAST(inVar, outvar)   \
  boost::shared_ptr<IFileWriter> outvar = boost::dynamic_pointer_cast<IFileWriter>(inVar);

#define SHARED_POINTER_DYNAMIC_CAST(fromVar, toType, toVar) \
  boost::shared_ptr<toType> toVar = boost::dynamic_pointer_cast<toType>(fromVar);

#define DECLARE_REFERENCE_VARIABLE(classType ,shared_ptr, ref) \
  classType &ref = *(shared_ptr.get());

#define WRAP_POINTER_IN_BOOST_SHARED_POINTER(Pointer, PointerType, BoostVariable)\
boost::shared_ptr<PointerType> BoostVariable (Pointer);

// Declare all the methods
IDataModel::Pointer createModel();
void DatasetTest();
void MakeDataRecord( const std::string &key, IDataModel::Pointer model);

int recLuid = 0;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
  MXAFileSystemPath::remove(MXAUnitTest::DatasetTest::TestFile) ;
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void MakeDataRecord( const std::string &key, IDataModel::Pointer model)
{
  MXADataRecord::Pointer rec = MXADataRecord::New(recLuid++, key, key );
  model->addDataRecord(rec);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void MakeAttribute(const std::string &dsPath, IDataset::Pointer dataset)
{
  int32 numElements = 1;
  T value = 0xFF;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  attributeKey = "H5Attribute<" + attributeKey + ">";

  MXAArrayTemplate<T>* attribute = MXAArrayTemplate<T>::New( numElements);
  IMXAArray::Pointer attr (static_cast<IMXAArray*>(attribute));
  for (int32 i = 0; i < numElements; ++i)
  {
    attribute->setValue (i, i*4) ;
  }
  dataset->addAttribute(attributeKey, attr);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MakeStringAttribute(const std::string &dsPath, IDataset::Pointer dataset)
{
  std::string value ("attribute value");
  std::string attributeKey ("H5AsciiStringAttribute");

  IMXAArray::Pointer strAttr = MXAAsciiStringData::Create( value);
  dataset->addAttribute(attributeKey, strAttr);
}

// -----------------------------------------------------------------------------
//  Creates a Data Model to use
// -----------------------------------------------------------------------------
IDataModel::Pointer createModel()
{
    std::string errorMessage;
    IDataModel::Pointer model = MXADataModel::New();

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
    IDataDimension::Pointer dim0 = model->addDataDimension("Data Container", "Data Container",  10, 0, 9, 1, 1);
    errorMessage.clear();
    BOOST_REQUIRE ( (model->isValid(errorMessage) ) == false );

    //Create Data Records for the Pointer Tests
    MakeDataRecord( "Dataset Int 8", model);
    MakeDataRecord( "Dataset UInt 8", model);
    MakeDataRecord( "Dataset Int 16", model);
    MakeDataRecord( "Dataset UInt 16", model);
    MakeDataRecord( "Dataset Int 32", model);
    MakeDataRecord( "Dataset UInt 32", model);
    MakeDataRecord( "Dataset Int 64", model);
    MakeDataRecord( "Dataset UInt 64", model);
    // Floating point Numbers
    MakeDataRecord( "Dataset Float 32", model);
    MakeDataRecord( "Dataset Float 64", model);

    // Create Data record for the String test
    MakeDataRecord( "String", model);

    // Create Data Records for the 2DArray Tests
    MakeDataRecord( "2DArray Int 8", model);
    MakeDataRecord( "2DArray UInt 8", model);
    MakeDataRecord( "2DArray Int 16", model);
    MakeDataRecord( "2DArray UInt 16", model);
    MakeDataRecord( "2DArray Int 32", model);
    MakeDataRecord( "2DArray UInt 32", model);
    MakeDataRecord( "2DArray Int 64", model);
    MakeDataRecord( "2DArray UInt 64", model);
    // Floating point Numbers
    MakeDataRecord( "2DArray Float 32", model);
    MakeDataRecord( "2DArray Float 64", model);

    MakeDataRecord( "RGB Image", model);

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
int32 _WriteDatasetTest( const std::string &recName, IDataFile::Pointer dataFile)
{
//  T t;
//   std::cout << "Running _WriteDatasetTest<" << H5Lite::HDFTypeForPrimitiveAsStr(t) << ">" << std::endl;
  IDataModel::Pointer model = dataFile->getDataModel();
  IDataRecord::Pointer rec = model->getDataRecordByNamedPath(recName, NULL);
  BOOST_REQUIRE(rec.get() != NULL);

  std::vector<int32> mxaDims;
  mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
  std::string dsPath = H5MXAUtilities::generateH5PathToDataset(model, mxaDims, rec);
  int32 err = 1;

  // Create the data
  int rank = 1;
  std::vector<uint64> dims(rank, 0);

  uint64 numElements = 5;
  MXAArrayTemplate<T>* data = MXAArrayTemplate<T>::New( numElements);
  IMXAArray::Pointer dataPtr (static_cast<IMXAArray*>(data));
  BOOST_REQUIRE(data != 0x0);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  data->getDimensions( &(dims.front() )  );
  BOOST_REQUIRE(dims[0] == numElements);

  numElements = 0; // Resize the array to zero
  err = data->resize(numElements);
  BOOST_REQUIRE(err == 1);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  data->getDimensions( &(dims.front() )  );
  BOOST_REQUIRE(dims[0] == numElements);

  numElements = 10; // Resize the array to 10
  err = data->resize(numElements);
  BOOST_REQUIRE(err == 1);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  data->getDimensions( &(dims.front() )  );
  BOOST_REQUIRE(dims[0] == numElements);
  for (uint64 i = 0; i < numElements; ++i) {
    data->setValue(i, static_cast<T>(i) );
  }
  // Actually set some meaningful data to the array
  T* value = static_cast<T*>(data->getVoidPointer(0) );
  for (uint64 i = 0; i < numElements; ++i) {
    BOOST_REQUIRE(value[i] == static_cast<T>(i) );
  }

  // Create the dataset that will hold the data and associated attributes
  IDataset::Pointer ds = H5Dataset::CreateDatasetPtr(dsPath, dataPtr);

  //Create Attributes for each primitive type
  MakeAttribute<int8>(dsPath, ds );
  MakeAttribute<uint8>( dsPath, ds );
  MakeAttribute<int16>( dsPath, ds );
  MakeAttribute<uint16>( dsPath, ds );
  MakeAttribute<int32>( dsPath, ds );
  MakeAttribute<uint32>(dsPath, ds );
  MakeAttribute<int64>( dsPath, ds );
  MakeAttribute<uint64>(dsPath, ds );
  MakeAttribute<float32>(dsPath, ds );
  MakeAttribute<float64>( dsPath, ds );
  MakeStringAttribute(dsPath, ds);

  // Write the data to the file
  err = ds->writeToFile(dataFile);
  BOOST_REQUIRE(err >= 0);

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void _WriteStringDataTest ( const std::string &recName, IDataFile::Pointer dataFile)
{
  IDataModel::Pointer model = dataFile->getDataModel();
  IDataRecord::Pointer rec = model->getDataRecordByNamedPath(recName, NULL);
  BOOST_REQUIRE(rec.get() != NULL);

  std::vector<int32> mxaDims;
  mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
  std::string dsPath = H5MXAUtilities::generateH5PathToDataset(model, mxaDims, rec);
  int32 err = 1;

  std::string stringData ("This is some string data");
  IMXAArray::Pointer strData = MXAAsciiStringData::Create(stringData);

  // Create the dataset that will hold the data and associated attributes
  IDataset::Pointer ds = H5Dataset::CreateDatasetPtr(dsPath, strData);


  //Create Attributes for each primitive type
  MakeAttribute<int8>(dsPath, ds );
  MakeAttribute<uint8>( dsPath, ds );
  MakeAttribute<int16>( dsPath, ds );
  MakeAttribute<uint16>( dsPath, ds );
  MakeAttribute<int32>( dsPath, ds );
  MakeAttribute<uint32>(dsPath, ds );
  MakeAttribute<int64>( dsPath, ds );
  MakeAttribute<uint64>(dsPath, ds );
  MakeAttribute<float32>(dsPath, ds );
  MakeAttribute<float64>( dsPath, ds );
  MakeStringAttribute(dsPath, ds);

  // Write the data to the file
  err = ds->writeToFile(dataFile);
  BOOST_REQUIRE(err >= 0);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
int32 _Write2DArrayTest( const std::string &recName, IDataFile::Pointer dataFile)
{
//  T t;
//  std::cout << "Running _Write2DArrayTest<" << H5Lite::HDFTypeForPrimitiveAsStr(t) << ">" << std::endl;
  IDataModel::Pointer model = dataFile->getDataModel();
  IDataRecord::Pointer rec = model->getDataRecordByNamedPath(recName, NULL);
  BOOST_REQUIRE(rec.get() != NULL);

  std::vector<int32> mxaDims;
  mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
  std::string dsPath = H5MXAUtilities::generateH5PathToDataset(model, mxaDims, rec);
  int32 err = 1;

  // Create the data
  uint64 numElements = 256 * 100;
  int rank = 2;
  std::vector<uint64> dims(rank, 0);
  MXA2DArray<T>* data = MXA2DArray<T>::New( 256, 100);
  IMXAArray::Pointer dataPtr (static_cast<IMXAArray*>(data) ); //Let boost manage the pointer

  BOOST_REQUIRE(data != 0x0);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  data->getDimensions( &(dims.front() ) );
  BOOST_REQUIRE(dims[0] = 256);
  BOOST_REQUIRE(dims[1] = 100);

  numElements = 0; // Resize the array to zero
  err = data->resizeArray(0, 0);
  BOOST_REQUIRE(err == 1);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  BOOST_REQUIRE(data->getVoidPointer(0) == NULL); // Check for NULL pointer
  data->getDimensions( &(dims.front() ) );
  BOOST_REQUIRE(dims[0] == 0);
  BOOST_REQUIRE(dims[1] == 0);

  numElements = 10; // Resize the array
  err = data->resize(10);
  BOOST_REQUIRE(err == 1);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  BOOST_REQUIRE(data->getVoidPointer(10) == NULL); // Check for NULL pointer
  BOOST_REQUIRE(data->getVoidPointer(11) == NULL); // Check for NULL pointer

  BOOST_REQUIRE(data->getNumberOfDimensions() == 2);
  BOOST_REQUIRE(data->getPointer(10, 0) == NULL);
  BOOST_REQUIRE(data->getPointer(0, 1) == NULL);
  BOOST_REQUIRE(data->getPointer(-1, 0) == NULL);
  BOOST_REQUIRE(data->getPointer(0, -1) == NULL);
  BOOST_REQUIRE(data->getPointer(0, 0) != NULL);
  BOOST_REQUIRE(data->getWidth() == 10);
  BOOST_REQUIRE(data->getHeight() == 1);
  data->getDimensions( &(dims.front() ) );
  BOOST_REQUIRE(dims[0] == 10);
  BOOST_REQUIRE(dims[1] == 1);

  numElements = 212 * 120; // Resize the array
  err = data->resizeArray(212, 120);
  BOOST_REQUIRE(err == 1);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  BOOST_REQUIRE(data->getWidth() == 212);
  BOOST_REQUIRE(data->getHeight() == 120);
  data->getDimensions( &(dims.front() ) );
  BOOST_REQUIRE(dims[0] == 212);
  BOOST_REQUIRE(dims[1] == 120);
  for (uint64 i = 0; i < numElements; ++i) {
    data->setValue(i, static_cast<T>(i) );
  }

  T* value = static_cast<T*>(data->getVoidPointer(0) );
  for (uint64 i = 0; i < numElements; ++i) {
    BOOST_REQUIRE(value[i] == static_cast<T>(i) );
  }

  // Create the dataset that will hold the data and associated attributes
  IDataset::Pointer ds = H5Dataset::CreateDatasetPtr(dsPath, dataPtr);
  BOOST_REQUIRE (ds.get() != NULL);

  // Write the data to the file
  err = ds->writeToFile(dataFile);
  BOOST_REQUIRE(err >= 0);

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void _WriteRGBImageTest( const std::string &recName, IDataFile::Pointer dataFile)
{
  IDataModel::Pointer model = dataFile->getDataModel();
  IDataRecord::Pointer rec = model->getDataRecordByNamedPath(recName, NULL);
  BOOST_REQUIRE(rec.get() != NULL);

  std::vector<int32> mxaDims;
  mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
  std::string dsPath = H5MXAUtilities::generateH5PathToDataset(model, mxaDims, rec);
  int32 err = 1;

  // Create the data
  uint64 numElements = 256 * 100 * 3;
  int rank = 2;
  std::vector<uint64> dims(rank, 0);
  MXARGBImage* data = MXARGBImage::New( 256, 100);
  IMXAArray::Pointer dataPtr (static_cast<IMXAArray*>(data) ); //Let boost manage the pointer
  BOOST_REQUIRE(data != 0x0);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  BOOST_REQUIRE(data->getWidth() == 256);
  BOOST_REQUIRE(data->getHeight() == 100);
  data->getDimensions( &(dims.front() ) );
  BOOST_REQUIRE(dims[1] == 256 * 3);
  BOOST_REQUIRE(dims[0] == 100);

  numElements = 150 * 101 * 3;
  data->resizeArray(150, 101);
  BOOST_REQUIRE(data->getNumberOfElements() == numElements);
  BOOST_REQUIRE(data->getWidth() == 150);
  BOOST_REQUIRE(data->getHeight() == 101);
  data->getDimensions( &(dims.front() ) );
  BOOST_REQUIRE(dims[1] == 150 * 3);
  BOOST_REQUIRE(dims[0] == 101);

  //put some actual data into the image
  uint8* pixel = data->getPixelPointer(0,0);
  int32 xDim = 150;
  int32 yDim = 101;
  for (int i = 0; i < yDim; ++i) {
    for (int j = 0; j < xDim; ++j) {
      *pixel = i + j;
      pixel++; // move the pointer by 1 byte
      *pixel = i;
      pixel++; // move the pointer by 1 byte
      *pixel = j;
      pixel++; // move the pointer by 1 byte
    }
  }

  // Create the dataset that will hold the data and associated attributes
  IDataset::Pointer ds = H5Dataset::CreateDatasetPtr(dsPath, dataPtr);
  BOOST_REQUIRE (ds.get() != NULL);

  // Write the data to the file
  err = ds->writeToFile(dataFile);
  BOOST_REQUIRE(err >= 0);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void WriteDatasetTest()
{
  std::cout << "Running _WriteDatasetTest<T> Test ....";
  // Create our Test File to output our test data into
  std::string testFile(MXAUnitTest::DatasetTest::TestFile);
  IDataModel::Pointer model = createModel();
  IDataFile::Pointer dataFile = H5MXADataFile::CreateFileWithModel(testFile, model);
 BOOST_REQUIRE( NULL != dataFile.get() );

  // These will test WRITING Data to the data file
  _WriteDatasetTest<int8>( "Dataset Int 8", dataFile );
  _WriteDatasetTest<uint8>( "Dataset UInt 8", dataFile );
  _WriteDatasetTest<int16>( "Dataset Int 16", dataFile );
  _WriteDatasetTest<uint16>( "Dataset UInt 16", dataFile );
  _WriteDatasetTest<int32>( "Dataset Int 32", dataFile );
  _WriteDatasetTest<uint32>( "Dataset UInt 32", dataFile );
  _WriteDatasetTest<int64>( "Dataset Int 64", dataFile );
  _WriteDatasetTest<uint64>( "Dataset UInt 64", dataFile );
  _WriteDatasetTest<float32>( "Dataset Float 32", dataFile );
  _WriteDatasetTest<float64>( "Dataset Float 64", dataFile );
  std::cout << "... Passed." << std::endl;

  std::cout << "Running _WriteStringDataTest Test ....";
  _WriteStringDataTest("String", dataFile);
  std::cout << "... Passed." << std::endl;

  std::cout << "Running _Write2DArrayTest<T> Test ....";
  _Write2DArrayTest<int8>( "2DArray Int 8", dataFile );
  _Write2DArrayTest<uint8>( "2DArray UInt 8", dataFile );
  _Write2DArrayTest<int16>( "2DArray Int 16", dataFile );
  _Write2DArrayTest<uint16>( "2DArray UInt 16", dataFile );
  _Write2DArrayTest<int32>( "2DArray Int 32", dataFile );
  _Write2DArrayTest<uint32>( "2DArray UInt 32", dataFile );
  _Write2DArrayTest<int64>( "2DArray Int 64", dataFile );
  _Write2DArrayTest<uint64>( "2DArray UInt 64", dataFile );
  _Write2DArrayTest<float32>( "2DArray Float 32", dataFile );
  _Write2DArrayTest<float64>( "2DArray Float 64", dataFile );
  std::cout << "... Passed." << std::endl;

  std::cout << "Running _WriteRGBImageTest Test ....";
  _WriteRGBImageTest( "RGB Image", dataFile );

  std::cout << "... Passed." << std::endl;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 _readDatasetTest(const std::string &recName, IDataFile::Pointer dataFile)
{
  IDataModel::Pointer model = dataFile->getDataModel();
  IDataRecord::Pointer rec = model->getDataRecordByNamedPath(recName, NULL);
  BOOST_REQUIRE(rec.get() != NULL);

  std::vector<int32> mxaDims;
  mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
  std::string dsPath = H5MXAUtilities::generateH5PathToDataset(model, mxaDims, rec);

  // This will also indirectly test H5Utilities.readDataArray and H5Utilities::readAttributeArray
  IDataset::Pointer ds = H5Dataset::LoadFromFile(dataFile, dsPath);
  BOOST_REQUIRE(ds.get() != NULL);
  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 _readRGBImageTest(const std::string &recName, IDataFile::Pointer dataFile)
{
  IDataModel::Pointer model = dataFile->getDataModel();
  IDataRecord::Pointer rec = model->getDataRecordByNamedPath(recName, NULL);
  BOOST_REQUIRE(rec.get() != NULL);

  std::vector<int32> mxaDims;
  mxaDims.push_back(2); // This data set is for index '2' of the 'Data Container' MXA Data Dimension
  std::string dsPath = H5MXAUtilities::generateH5PathToDataset(model, mxaDims, rec);

  //MXARGBImage* data = MXARGBImage::New( 150, 101);
 // IMXAArray::Pointer dataPtr (static_cast<IMXAArray*>(data) ); //Let boost manage the pointer
  IMXAArray::Pointer dataPtr = H5MXAUtilities::readData(dataFile, dsPath);
  uint64 numElements = 150 * 101 * 3;
  int rank = 2;
  std::vector<uint64> dims(rank, 0);
  //BOOST_REQUIRE(dataPtr->getWidth() == 150);
  //BOOST_REQUIRE(dataPtr->getHeight() == 101);
  BOOST_REQUIRE(dataPtr->getNumberOfElements() == numElements);
  dataPtr->getDimensions( &(dims.front() ) );
  BOOST_REQUIRE(dims[1] == 150 * 3);
  BOOST_REQUIRE(dims[0] == 101);
#if 0
#warning RGBImage class needs to have special file writing code

  uint8* pixel = dataPtr->getPixelPointer(0,0);
  int32 xDim = 150;
  int32 yDim = 101;
  for (int i = 0; i < yDim; ++i) {
    for (int j = 0; j < xDim; ++j) {
      pixel = data->getPixelPointer(j, i);
      std::cout << "x:y " << j << ":" << i << " " << (int)pixel[0] << " " << (int)pixel[1] << " " << (int)pixel[2] << std::endl;
      BOOST_REQUIRE( pixel[0] == (i + j) ) ;
      BOOST_REQUIRE( pixel[1] == i ) ;
      BOOST_REQUIRE( pixel[2] == j ) ;
    }
  }
#endif
  return 1;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ReadDatasetTest( )
{
  std::cout << "Running ReadDatasetTest Test ....";
  // Create our Test File to output our test data into
  std::string testFile(MXAUnitTest::DatasetTest::TestFile);
  IDataFile::Pointer dataFile = H5MXADataFile::OpenFile(testFile, true);
  BOOST_REQUIRE(dataFile.get() != NULL);
  _readDatasetTest( "Dataset Int 8", dataFile );
  _readDatasetTest( "Dataset UInt 8", dataFile );
  _readDatasetTest( "Dataset Int 16", dataFile );
  _readDatasetTest( "Dataset UInt 16", dataFile );
  _readDatasetTest( "Dataset Int 32", dataFile );
  _readDatasetTest( "Dataset UInt 32", dataFile );
  _readDatasetTest( "Dataset Int 64", dataFile );
  _readDatasetTest( "Dataset UInt 64", dataFile );
  _readDatasetTest( "Dataset Float 32", dataFile );
  _readDatasetTest( "Dataset Float 64", dataFile );
  _readDatasetTest( "String", dataFile );

  _readRGBImageTest( "RGB Image", dataFile);

  std::cout << "... Passed." << std::endl;
}

// -----------------------------------------------------------------------------
//  This will test the byte swapping routine of the MXAArrayTemplate.hpp class
// -----------------------------------------------------------------------------
void TestDataWrapper()
{
  uint32 i32Array[10] = { 0x00000001,0x00000002,0x00000003,0x00000004,0x00000005,
                          0x00000006,0x00000007,0x00000008,0x00000009,0x0000000A};
  uint32 i32ArraySwap[10] = { 0x01000000,0x02000000,0x03000000,0x04000000,0x05000000,
                              0x06000000,0x07000000,0x08000000,0x09000000,0x0A000000};

  MXAArrayTemplate<uint32>* ui32;
  IMXAArray::Pointer ptr32 = MXAArrayTemplate<uint32>::CreateArray(10);
  ui32 = dynamic_cast<MXAArrayTemplate<uint32>*>(ptr32.get());
  for (uint32 i = 0; i < 10; ++i) {
    ui32->setValue(i, i32Array[i]);
  }
  ui32->byteSwapElements();
  for (int i = 0; i < 10; ++i) {
   //  std::cout << ui32->getValue(i) << std::endl;
     BOOST_REQUIRE(ui32->getValue(i) == i32ArraySwap[i]);
  }


  uint64 i64Array[10] = { 0x0000000000000001,0x0000000000000002,0x0000000000000003,0x0000000000000004,0x0000000000000005,
                          0x0000000000000006,0x0000000000000007,0x0000000000000008,0x0000000000000009,0x000000000000000A};
  uint64 i64ArraySwap[10] = { 0x0100000000000000ull,0x0200000000000000ull,0x0300000000000000ull,0x0400000000000000ull,0x0500000000000000ull,
                              0x0600000000000000ull,0x0700000000000000ull,0x0800000000000000ull,0x0900000000000000ull,0x0A00000000000000ull};
  MXAArrayTemplate<uint64>* ui64;
  IMXAArray::Pointer ptr64 = MXAArrayTemplate<uint64>::CreateArray(10);
  ui64 = dynamic_cast<MXAArrayTemplate<uint64>*>(ptr64.get());
  for (uint64 i = 0; i < 10; ++i) {
    ui64->setValue(i, i64Array[i]);
  }
  ui64->byteSwapElements();
  for (int i = 0; i < 10; ++i) {
   //  std::cout << ui32->getValue(i) << std::endl;
     BOOST_REQUIRE(ui64->getValue(i) == i64ArraySwap[i]);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ATest()
{
  std::string testFile(MXAUnitTest::DatasetTest::TestFile);
  IDataFile::Pointer dataFile = H5MXADataFile::OpenFile(testFile, true);
  BOOST_REQUIRE(dataFile.get() != NULL);
  hid_t fileId = dataFile->getFileId();
  double d = 1111111.111111;
  int err = H5Lite::readScalarAttribute(fileId, "/DatasetTest/2/0", "2H5Attribute<H5T_NATIVE_DOUBLE>", d);
  BOOST_REQUIRE(err >= 0);
}


// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] )
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE( "Dataset Tests" );

  test->add( BOOST_TEST_CASE( &WriteDatasetTest), 0);
  test->add( BOOST_TEST_CASE( &ReadDatasetTest), 0);
  test->add( BOOST_TEST_CASE( &TestDataWrapper), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}



