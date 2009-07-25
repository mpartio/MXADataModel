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
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXASupportFile.h>
#include <MXA/XML/XMLDataModelReader.h>
#include <MXA/XML/XMLDataModelWriter.h>
#include <Tests/MXAUnitTestDataFileLocations.h>
#include <MXA/DataWrappers/MXAAsciiStringData.h>
#include <MXA/Utilities/MXAFileSystemPath.h>

//-- C++ includes
#include <string>


//Boost Includes
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
//-- Boost Test Headers
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>



//typedef boost::shared_ptr<MXAAttribute> MXAAttributePtr;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
  MXAFileSystemPath::remove(MXAUnitTest::XMLTest::TestFile) ;
  MXAFileSystemPath::remove(MXAUnitTest::XMLTest::MasterFile);
  MXAFileSystemPath::remove(MXAUnitTest::XMLTest::TemplateTestFile) ;
  MXAFileSystemPath::remove(MXAUnitTest::XMLTest::TemplateCompleteFile);
#endif
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void MakeScalarAttribute(T value, std::string key, MXADataModel* model)
{
  IMXAArray::Pointer umd = MXAArrayTemplate<T>::CreateSingleValueArray(value);
  model->addUserMetaData(key, umd);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void MakeVectorAttribute(T value, std::string key, std::vector<uint64> &dims, MXADataModel* model)
{
  //std::vector<T> data;
  uint64 numelements = 1;
  for (std::vector<uint64>::iterator iter = dims.begin(); iter != dims.end(); ++iter)
  {
    numelements *= *(iter);
  }
  BOOST_REQUIRE(numelements == 10);
  BOOST_REQUIRE(dims.size() == 2);
  BOOST_REQUIRE(dims[0] == 5);
  BOOST_REQUIRE(dims[1] == 2);
  IMXAArray::Pointer vecPtr = MXAArrayTemplate<T>::CreateMultiDimensionalArray( dims.size(), &(dims.front()) );
  BOOST_REQUIRE ( vecPtr->getNumberOfElements() == numelements);
  BOOST_REQUIRE (vecPtr->getNumberOfDimensions() == 2);
   std::vector<uint64> mydims(dims.size(), 0);
  vecPtr->getDimensions( &(mydims.front() ) );
  BOOST_REQUIRE (mydims[0] == 5);
  BOOST_REQUIRE (mydims[1] == 2);

  // Copy data into the attribute container
  T* data = static_cast<T*>( vecPtr->getVoidPointer(0) );
  for (uint64 i = 0; i < numelements; ++i)
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
    dims.push_back(5);
    dims.push_back(2);

//    // integers
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
//
//   //Integer Numbers
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
    IMXAArray::Pointer s1 = MXAAsciiStringData::Create( "DaddyO");
    model->addUserMetaData("Password", s1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXADataModel::Pointer createModel()
{
    MXADataModel::Pointer modelPtr = MXADataModel::New();
    MXADataModel* model = modelPtr.get();
    model->setDataRoot(std::string("DataModelTest/Data/Root/Is/Here"));
    model->setModelType(MXA::MXACurrentFileType);
    model->setModelVersion(MXA::MXACurrentFileVersion);

    // ---------- Test creation/addition of Data Dimensions
    IDataDimension::Pointer dim0 = model->addDataDimension("Volume Fraction", "Vol Frac", 15, 20, 50, 2, 1);
    IDataDimension::Pointer dim1 = model->addDataDimension("Random Seed", "Rnd Seed", 10, 1000, 5000, 500, 1);
    IDataDimension::Pointer dim2 = model->addDataDimension("Timestep", "TS",100, 0, 99, 1, 1);
    IDataDimension::Pointer dim3 = model->addDataDimension("Slice", "slice", 256, 0, 255, 1, 1);

    //Create Data Records
    MXADataRecord::Pointer rec0 = MXADataRecord::New(0,std::string("Composition"), std::string("AltComp"));
    model->addDataRecord(rec0);

    MXADataRecord::Pointer rec1 = MXADataRecord::New(1, std::string("Order Parameters"), std::string("OP") );
    model->addDataRecord(rec1);
    //Create Data Records with Parents
    MXADataRecord::Pointer rec2 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
    model->addDataRecord(rec2, rec1);
    MXADataRecord::Pointer rec3 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
    model->addDataRecord(rec3, rec1);
    MXADataRecord::Pointer rec4 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
    model->addDataRecord(rec4, rec1);

    // Make this a child of the 'Order Parameters' Group
     MXADataRecord::Pointer rec5 = MXADataRecord::New(3, std::string("Order Parameters 2"), std::string("OP 2") );
     model->addDataRecord(rec5, rec1);
      //Create Data Records with Parents
      MXADataRecord::Pointer rec6 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
      model->addDataRecord(rec6, rec5);
      MXADataRecord::Pointer rec7 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
      model->addDataRecord(rec7, rec5);
      MXADataRecord::Pointer rec8 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
      model->addDataRecord(rec8, rec5);


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
    model->setRequiredMetaData(md);

    // Create User Defined MetaData
    CreateAttributes( model );

    ISupportFile::Pointer spFile = MXASupportFile::NewFromFileSystem(MXAUnitTest::SupportFileTest::TextInputFile, SupportFile::FileType::Text, false);
    model->addSupportFile(spFile);
    return modelPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXADataModel::Pointer createModelTemplate()
{
    MXADataModel::Pointer modelPtr = MXADataModel::New();
    MXADataModel* model = modelPtr.get();
    model->setDataRoot(std::string("DataModelTest/Data/Root/Is/Here"));
    model->setModelType(MXA::MXACurrentFileType);
    model->setModelVersion(MXA::MXACurrentFileVersion);

    // ---------- Test creation/addition of Data Dimensions
    MXADataDimension::Pointer dim0 = MXADataDimension::New("Volume Fraction", "Vol Frac");
    MXADataDimension::Pointer dim1 = MXADataDimension::New("Random Seed", "Rnd Seed");
    MXADataDimension::Pointer dim2 = MXADataDimension::New("Timestep", "TS");
    MXADataDimension::Pointer dim3 = MXADataDimension::New("Slice", "slice");

    model->addDataDimension(dim0);
    model->addDataDimension(dim1);
    model->addDataDimension(dim2);
    model->addDataDimension(dim3);

    //Create Data Records
    MXADataRecord::Pointer rec0 = MXADataRecord::New(0,std::string("Composition"), std::string("AltComp"));
    model->addDataRecord(rec0);

    MXADataRecord::Pointer rec1 = MXADataRecord::New(1, std::string("Order Parameters"), std::string("OP") );
    model->addDataRecord(rec1);
    //Create Data Records with Parents
    MXADataRecord::Pointer rec2 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
    model->addDataRecord(rec2, rec1);

    MXADataRecord::Pointer rec5 = MXADataRecord::New(1, std::string("Order Parameters 2"), std::string("OP 2") );
     model->addDataRecord(rec5, rec1);
     //Create Data Records with Parents
     MXADataRecord::Pointer rec6 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
     model->addDataRecord(rec6, rec5);
     MXADataRecord::Pointer rec7 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
     model->addDataRecord(rec7, rec5);
     MXADataRecord::Pointer rec8 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
     model->addDataRecord(rec8, rec5);

    MXADataRecord::Pointer rec3 = MXADataRecord::New(2, std::string("Eta2"), std::string("Alt Eta2") );
    model->addDataRecord(rec3, rec1);
    MXADataRecord::Pointer rec4 = MXADataRecord::New(3, std::string("Eta3"), std::string("Alt Eta3") );
    model->addDataRecord(rec4, rec1);

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
    model->setRequiredMetaData(md);

    // Create User Defined MetaData
   // CreateAttributes( model );
    return modelPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMasterXMLFile()
{
  std::cout << "Running GenerateMasterXMLFile........";
  std::string xmlFile(MXAUnitTest::XMLTest::MasterFile);
  {
    MXADataModel::Pointer model = createModel();
    XMLDataModelWriter writer(model, xmlFile);
    int32 err = writer.writeModelToFile(-1);
    BOOST_REQUIRE ( err >= 0);
    MXAAbstractAttributes attributes = model->getUserMetaData();
    BOOST_REQUIRE(attributes.size() == 21);
  }

  {
    IDataModel::Pointer model = MXADataModel::New();
    // Read the File back from xml
    XMLDataModelReader reader (model, xmlFile);
    int32 err = reader.readDataModel(-1);
    BOOST_REQUIRE ( err >= 0);
    BOOST_REQUIRE (model->getNumberOfDataDimensions() == 4);
    std::string errorMessage;
    BOOST_REQUIRE (model->isValid(errorMessage) == true);
    MXAAbstractAttributes attributes = model->getUserMetaData();
    BOOST_REQUIRE(attributes.size() == 21);
    BOOST_REQUIRE(model->getSupportFiles().size() == 1);
    std::cout << "....... Passed" << std::endl;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void XMLModelTest()
{
  std::cout << "XMLModelTest Running...";
  std::string masterXmlFile(MXAUnitTest::XMLTest::MasterFile);
  std::string outFile (MXAUnitTest::XMLTest::TestFile);
  std::string errorMessage;
  MXADataModel::Pointer model = MXADataModel::New();
  {
    XMLDataModelReader reader (model, masterXmlFile);
    int32 err = reader.readDataModel(-1);
    BOOST_REQUIRE ( err >= 0);
    BOOST_REQUIRE (model->getNumberOfDataDimensions() == 4);
    BOOST_REQUIRE (model->isValid(errorMessage) == true);
    MXAAbstractAttributes attributes = model->getUserMetaData();
    BOOST_REQUIRE(attributes.size() == 21);
  }

  {
    XMLDataModelWriter writer(model, outFile);
    int32 err = writer.writeModelToFile(-1);
    BOOST_REQUIRE ( err >= 0);
  }

  // Now compare the xml files
  std::vector<char> masterData;
  std::vector<char> testData;
  FILE* fp = fopen(masterXmlFile.c_str(), "r");
  if( NULL != fp)
  {
    char buf[1024];
    while(size_t len = fread(buf, 1, sizeof(buf), fp))
      masterData.insert(masterData.end(), buf, buf + len);
    fclose(fp);
    fp = NULL;
  }

  fp = fopen(outFile.c_str(), "r");
  if( NULL != fp)
  {
    char buf[1024];
    while(size_t len = fread(buf, 1, sizeof(buf), fp))
      testData.insert(testData.end(), buf, buf + len);
    fclose(fp);
    fp = NULL;
  }
  BOOST_REQUIRE (masterData == testData);
  std::cout << "....... Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void XMLTemplateTest()
{
  std::cout << "XMLTemplateTest Running..." ;
  std::string errorMessage;
  std::string templateFile (MXAUnitTest::XMLTest::TemplateTestFile);
  {
    MXADataModel::Pointer model = createModelTemplate();
    XMLDataModelWriter writer(model, templateFile);
    int32 err = writer.writeModelToFile(-1);
    BOOST_REQUIRE ( err >= 0);
  }
  MXADataModel::Pointer readModel = MXADataModel::New();
  XMLDataModelReader reader (readModel, templateFile);
  int32 err = reader.readDataModel(-1);
  BOOST_REQUIRE ( err < 0); // This SHOULD fail because we read in a partial model
  BOOST_REQUIRE ( readModel->isValid(errorMessage) == false);
  BOOST_REQUIRE ( readModel->getDataRecords().size() == 2);

  IDataDimension::Pointer dim0 = readModel->getDataDimension(0);
  dim0->setCount(15);
  dim0->setStartValue(20);
  dim0->setEndValue(50);
  dim0->setIncrement(2);
  dim0->setUniform(1);

  IDataDimension::Pointer dim1 = readModel->getDataDimension(1);
  dim1->setCount(10);
  dim1->setStartValue(1000);
  dim1->setEndValue(5000);
  dim1->setIncrement(500);
  dim1->setUniform(1);

  IDataDimension::Pointer dim2 = readModel->getDataDimension(2);
  dim2->setCount(100);
  dim2->setStartValue(0);
  dim2->setEndValue(99);
  dim2->setIncrement(1);
  dim2->setUniform(1);

  IDataDimension::Pointer dim3 = readModel->getDataDimension(3);
  dim3->setCount(256);
  dim3->setStartValue(0);
  dim3->setEndValue(255);
  dim3->setIncrement(1);
  dim3->setUniform(1);

  BOOST_REQUIRE ( readModel->isValid(errorMessage) == true); //Model should now validate since we have reasonable values for each dimension
  //We can write the model back out to XML without any errors
  {
    XMLDataModelWriter writer(readModel, MXAUnitTest::XMLTest::TemplateCompleteFile);
    err = writer.writeModelToFile(-1);
    BOOST_REQUIRE ( err >= 0);
  }
  std::cout << "....... Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE( "XML Tests" );
  test->add( BOOST_TEST_CASE( &GenerateMasterXMLFile), 0);
  test->add( BOOST_TEST_CASE( &XMLModelTest), 0);
  test->add( BOOST_TEST_CASE( &XMLTemplateTest), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}
