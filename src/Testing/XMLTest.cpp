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

#include <MXAConfiguration.h>
#include "Common/MXATypeDefs.h"
#include "Core/MXADataModel.h"
#include "XML/XMLIODelegate.h"

#include <string>


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
  #define XML_TEST_FILE "C:\\WINDOWS\\Temp\\XML_Test.xml"
#define MASTER_XML_FILE "C:\\WINDOWS\\Temp\\XMLMaster.xml"
#else 
  #define XML_TEST_FILE "/tmp/XML_Test.xml"
  #define MASTER_XML_FILE "/tmp/XMLMaster.xml"
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
    dims.push_back(5);
    dims.push_back(2);
    
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
//  
// -----------------------------------------------------------------------------
MXADataModelPtr createModel()
{
    MXADataModelPtr modelPtr = MXADataModel::New();
    MXADataModel* model = modelPtr.get();
    model->setDataRoot(std::string("DataModelTest/Data/Root/Is/Here"));
    model->setFileType(MXA::MXACurrentFileType);
    model->setFileVersion(MXA::MXACurrentFileVersion);

    // ---------- Test creation/addition of Data Dimensions
    MXADataDimensionPtr dim0 = model->addDataDimension("Volume Fraction", "Vol Frac", 0, 15, 20, 50, 2, 1);
    MXADataDimensionPtr dim1 = model->addDataDimension("Random Seed", "Rnd Seed", 1, 10, 1000, 5000, 500, 1);
    MXADataDimensionPtr dim2 = model->addDataDimension("Timestep", "TS", 2, 100, 0, 99, 1, 1);
    MXADataDimensionPtr dim3 = model->addDataDimension("Slice", "slice", 3, 256, 0, 255, 1, 1);
        
    //Create Data Records
    MXADataRecordPtr rec0 = MXADataRecord::New(0,std::string("Composition"), std::string("AltComp"));
    model->addDataRecord(rec0);
  //  rec0->addUserDefinedAttribute("Rendering Hint", RenderHint::ImageGrayScale);
    
    MXADataRecordPtr rec1 = MXADataRecord::New(1, std::string("Order Parameters"), std::string("OP") );
    model->addDataRecord(rec1);
    //Create Data Records with Parents
    MXADataRecordPtr rec2 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
    model->addDataRecord(rec2, rec1);
    MXADataRecordPtr rec3 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
    model->addDataRecord(rec3, rec1);
    MXADataRecordPtr rec4 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
    model->addDataRecord(rec4, rec1);
    
     MXADataRecordPtr rec5 = MXADataRecord::New(2, std::string("Order Parameters 2"), std::string("OP 2") );
      model->addDataRecord(rec5);
      //Create Data Records with Parents
      MXADataRecordPtr rec6 = MXADataRecord::New(0, std::string("Eta1"), std::string("Alt Eta1") );
      model->addDataRecord(rec6, rec5);
      MXADataRecordPtr rec7 = MXADataRecord::New(1, std::string("Eta2"), std::string("Alt Eta2") );
      model->addDataRecord(rec7, rec5);
      MXADataRecordPtr rec8 = MXADataRecord::New(2, std::string("Eta3"), std::string("Alt Eta3") );
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
    md[MXA::MXA_RELEASE_TAG] = "AFRL/WS07-0476";
    model->setRequiredMetaData(md);
    
    // Create User Defined MetaData
    CreateAttributes( model );
    return modelPtr;
}

void GenerateMasterXMLFile()
{
  std::cout << "Writing Model as XML...." << std::endl;
  MXADataModelPtr model = createModel();
  std::string xmlFile(MASTER_XML_FILE);

  XMLIODelegate iodelegate; // Create on the stack
  BOOST_REQUIRE ( iodelegate.writeModelToFile(xmlFile, model.get(), true) >= 0);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLModelTest()
{
  //std::cout << "Reading MXA Model from XML File... " << std::endl;
  std::string xmlFile(MASTER_XML_FILE);
  MXADataModelPtr model = MXADataModel::New();
  XMLIODelegate iodelegate; // Create on the stack
  BOOST_REQUIRE ( iodelegate.readModelFromFile(xmlFile, model.get(), true) >= 0);
  BOOST_REQUIRE ( iodelegate.writeModelToFile(XML_TEST_FILE, model.get(), true) >= 0);
  
  // Now compare the xml files
  std::vector<char> inData;
  std::vector<char> outData;
  FILE* fp = fopen(MASTER_XML_FILE, "r");
  if( NULL != fp)
  {
    char buf[1024];
    while(size_t len = fread(buf, 1, sizeof(buf), fp))
      inData.insert(inData.end(), buf, buf + len);
    fclose(fp);
    fp = NULL;
  }
  
  fp = fopen(XML_TEST_FILE, "r");
  if( NULL != fp)
  {
    char buf[1024];
    while(size_t len = fread(buf, 1, sizeof(buf), fp))
      outData.insert(outData.end(), buf, buf + len);
    fclose(fp);
    fp = NULL;
  }
  
  BOOST_REQUIRE (inData == outData);
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) {
    test_suite* test= BOOST_TEST_SUITE( "Data Model Tests" );
    test->add( BOOST_TEST_CASE( &GenerateMasterXMLFile), 0);
    test->add( BOOST_TEST_CASE( &XMLModelTest), 0);
    return test; 
}


