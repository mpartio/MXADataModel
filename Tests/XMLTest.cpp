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

#include "UnitTestSupport.hpp"
#include <MXA/XML/XMLFileUtilities.hpp>
#include <Tests/MXAUnitTestDataFileLocations.h>
#include <MXA/Utilities/MXAFileSystemPath.h>


//-- C++ includes
#include <string>


//Boost Includes
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>


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
  int32_t err = 0;
  std::string xmlFile(MXAUnitTest::XMLTest::MasterFile);
  {
    MXADataModel::Pointer model = createModel();
    ISupportFile::Pointer spFile = MXASupportFile::NewFromFileSystem(MXAUnitTest::SupportFileTest::TextInputFile, SupportFile::FileType::Text, false);
    model->addSupportFile(spFile);

    err = MXAXMLModelFileWriter::writeModel(model, xmlFile);
    MXA_REQUIRE ( err >= 0);
    MXAAbstractAttributes attributes = model->getUserMetaData();
    MXA_REQUIRE(attributes.size() == 21);
  }

  {
    // Read the File back from xml
    IDataModel::Pointer model = MXAXMLModelFileReader::readModel(xmlFile);
    MXA_REQUIRE ( NULL != model.get() );
    MXA_REQUIRE (model->getNumberOfDataDimensions() == 4);
    std::string errorMessage;
    MXA_REQUIRE (model->isValid(errorMessage) == true);
    MXAAbstractAttributes attributes = model->getUserMetaData();
    MXA_REQUIRE(attributes.size() == 21);
    MXA_REQUIRE(model->getSupportFiles().size() == 1);
  //  std::cout << "....... Passed" << std::endl;
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void XMLModelTest()
{
 // std::cout << "XMLModelTest Running...";
  std::string masterXmlFile(MXAUnitTest::XMLTest::MasterFile);
  std::string outFile (MXAUnitTest::XMLTest::TestFile);
  std::string errorMessage;
  {
    IDataModel::Pointer model = MXAXMLModelFileReader::readModel(masterXmlFile);
    MXA_REQUIRE (model.get() != NULL)
    MXA_REQUIRE (model->getNumberOfDataDimensions() == 4);
    MXA_REQUIRE (model->isValid(errorMessage) == true);
    MXAAbstractAttributes attributes = model->getUserMetaData();
    MXA_REQUIRE(attributes.size() == 21);
  }

  {
    MXADataModel::Pointer model = createModel();
    ISupportFile::Pointer spFile = MXASupportFile::NewFromFileSystem(MXAUnitTest::SupportFileTest::TextInputFile, SupportFile::FileType::Text, false);
    model->addSupportFile(spFile);
    int32_t err = MXAXMLModelFileWriter::writeModel(model, outFile);
    MXA_REQUIRE ( err >= 0);
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
  MXA_REQUIRE (masterData == testData);
 // std::cout << "....... Passed" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void XMLTemplateTest()
{
 // std::cout << "XMLTemplateTest Running..." ;
  std::string errorMessage;
  std::string templateFile (MXAUnitTest::XMLTest::TemplateTestFile);
  {
    MXADataModel::Pointer model = createModelTemplate();
    int32_t err = MXAXMLModelFileWriter::writeModel(model, templateFile);
    MXA_REQUIRE ( err >= 0);
  }
  // Read the template back into an IDataModel Object
  {
    IDataModel::Pointer readModel = MXAXMLModelFileReader::readModel(templateFile, false);

    MXA_REQUIRE ( NULL != readModel.get() ); // This SHOULD fail because we read in a partial model
    MXA_REQUIRE ( readModel->isValid(errorMessage) == false);
    MXA_REQUIRE ( readModel->getDataRecords().size() == 2);

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

    MXA_REQUIRE ( readModel->isValid(errorMessage) == true); //Model should now validate since we have reasonable values for each dimension
    //We can write the model back out to XML without any errors
    {
//      XMLDataModelWriter writer(readModel, MXAUnitTest::XMLTest::TemplateCompleteFile);
//      err = writer.writeModel(-1);
//      MXA_REQUIRE ( err >= 0);

      typedef XMLStreamWriterDelegate<std::ofstream> FileStreamType;
      FileStreamType::Pointer delegate = FileStreamType::New();
      std::ofstream& out = *(delegate->getStreamPointer());
      out.open(MXAUnitTest::XMLTest::TemplateCompleteFile.c_str());
      if (NULL == delegate.get() || false == out.is_open() )
       {
        MXA_ASSERT(1 == 0);
       }
      MXADataModelWriter<FileStreamType>::Pointer writer = MXADataModelWriter<FileStreamType>::New(delegate);
      int32_t err = writer->writeModel(readModel);
      MXA_REQUIRE ( err >= 0);
    }
  }
 // std::cout << "....... Passed" << std::endl;
}


int main(int argc, char **argv) {


  int err = EXIT_SUCCESS;

  MXA_REGISTER_TEST( GenerateMasterXMLFile() )
  MXA_REGISTER_TEST( XMLModelTest() )
  MXA_REGISTER_TEST( XMLTemplateTest() )
  MXA_REGISTER_TEST( RemoveTestFiles() )

  PRINT_TEST_SUMMARY();
  return err;
}

