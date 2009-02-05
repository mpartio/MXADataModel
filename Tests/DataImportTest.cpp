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
#include <MXA/Common/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Base/IImportDelegate.h>
#include <MXA/Base/IFileIODelegate.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/Core/MXADataImport.h>
#include <MXA/Core/MXADataSource.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/HDF5/H5TiffImportDelegateFactory.h>
#include <MXA/HDF5/H5BmpImportDelegateFactory.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <MXA/DataImport/DataImportXmlParser.h>
#include <MXA/DataImport/ImportDelegateManager.h>
#include "MXAUnitTestDataFileLocations.h"
#include "H5ImportTest.h"
#include "TiffMaker.h"

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

//-- Boost Filesystem Headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
  std::cout << "Removing Test files" << std::endl;
#if REMOVE_TEST_FILES
  boost::filesystem::remove(DATAIMPORT_TEST_SIMPLE_IMPORT);
  boost::filesystem::remove(DATAIMPORT_TEST_H5TIFF_OUTPUT_FILE);
#endif
}



// -----------------------------------------------------------------------------
//  Creates a DataImport class and some DataSources that then delegate out to
//  an instance of an IDataImportDelegate to do the actual import of data.
// -----------------------------------------------------------------------------
void ImportSimpleData(MXADataModelPtr model, std::string outputFilePath)
{
  IDataFilePtr dataFile = H5MXADataFile::CreateFileWithModel(outputFilePath, model);
  BOOST_REQUIRE (NULL != dataFile.get() );

  //Create the DataImport Class
  MXADataImportPtr dataImport( new MXADataImport() );
  dataImport->setDataFile(dataFile);

  // Register the known ImportDelegates, which will also instantiate the ImportDelegateManager instance.
  // The ImportDelegateManager instance is a singleton so only one can ever be active per program.
  ImportDelegateManager::registerKnownImportDeletegateFactories();

  // Register our Import Delegate by using the static methods from ImportDelegateManager
  AbstractImportDelegateFactoryPtr h5ImportTestDelegateFactory ( new H5ImportTestDelegateFactory() );
  ImportDelegateManager::registerImportDelegateFactory(h5ImportTestDelegateFactory);

  // Get an Instance to the ImportDelegateManager
  ImportDelegateManagerPtr importManager = ImportDelegateManager::instance();

  // Run a comparison of 2 AbstractImportDelegateFactoryPtrs to make sure they are the same.
  AbstractImportDelegateFactoryPtr factoryPtr2 = importManager->getImportDelegateFactory(H5ImportTest::Detail::ClassName);
  BOOST_REQUIRE (h5ImportTestDelegateFactory.get() == factoryPtr2.get() );

  // Create an Import Delegate to use for the DataSources
  IImportDelegatePtr delegatePtr = ImportDelegateManager::createNewImportDelegate(H5ImportTest::Detail::ClassName);
  // We are now going to get a reference to the ImportDelegate so we can set the default value that it writes.
  H5ImportTestDelegate* h5ImportDelegate = dynamic_cast<H5ImportTestDelegate*>(delegatePtr.get() );

  h5ImportDelegate->setValue(55);

  // We have two dimensions for this model, create a loop to create data sets for each possible dimension value
  IDataDimensionPtr dim0 = model->getDataDimension(0); // Get the first Dimension, since there is only one this works
  BOOST_REQUIRE(dim0 != NULL); // Used for Boost Unit Test Framework

  IDataDimensionPtr dim1 = model->getDataDimension(1);
  BOOST_REQUIRE(dim1 != NULL); // Used for Boost Unit Test Framework

  // Create a DataRecord entry for the Data Model
  IDataRecordPtr record = model->getDataRecordByNamedPath("DataRecordContainer/Test Data/Deep Nested Data");
  BOOST_REQUIRE(NULL != record.get()); // Used for Boost Unit Test Framework

  // Set the start/end/increment values for each Data Dimension
  int32 dim0Start = dim0->getStartValue();
  int32 dim0End = dim0->getEndValue();
  int32 dim0Increment = dim0->getIncrement();

  int32 dim1Start = dim1->getStartValue();
  int32 dim1End = dim1->getEndValue();
  int32 dim1Increment = dim1->getIncrement();

  // Create a nested loop to create the necessary DataSource objects that will
  //  be used to import the data into the HDF5 file
  //std::cout << "CREATING DATA SOURCES" << std::endl;
  for( int32 i = dim0Start; i <= dim0End; i += dim0Increment )
  {
    for (int j = dim1Start; j <= dim1End; j = j+ dim1Increment)
    {
      //Create some Data Sources
      MXADataSourcePtr ds( new MXADataSource() );
      std::vector<int32> dimValues;
      dimValues.push_back(i);
      dimValues.push_back(j);
      ds->setDimensionValues(dimValues);
      ds->setDataRecord(record);
      ds->setSourcePath(""); //Since we are doing everything in software, this is unused
      ds->setImportDelegate(delegatePtr);
      ds->setDataModel(model);
      dataImport->addDataSource(ds);
    }
  }

  // Import the Data into the HDF5 File
  //std::cout << "IMPORTING DATA NOW" << std::endl;
  int32 err = dataImport->import();
  BOOST_REQUIRE(err >= 0); // Used for Boost Unit Test Framework
  dataFile->closeFile(false);
}


// -----------------------------------------------------------------------------
//  Creates a Data Model programmitically to use with our data import
// -----------------------------------------------------------------------------
MXADataModelPtr createSimpleModel()
{
	  MXADataModelPtr modelPtr = MXADataModel::New();
	  MXADataModel* model = modelPtr.get();
	  model->setDataRoot(std::string("Our Experiment/Laboratory Data"));
	  model->setModelType(MXA::MXACurrentFileType);
	  model->setModelVersion(MXA::MXACurrentFileVersion);

	  // ---------- Create 2 Data Dimensions
	  MXADataDimensionPtr dim0 = MXADataDimension::New("Dimension 1", "Dim1", 0, 2, 1, 2, 1, 1);
	  model->addDataDimension(dim0);
    MXADataDimensionPtr dim1 = MXADataDimension::New("Dimension 2", "Dim2", 1, 3, 1, 3, 1, 1);
    model->addDataDimension(dim1);

	  // ---------- Create Data Records
	  MXADataRecordPtr rec1 = MXADataRecord::New(0, std::string("DataRecordContainer"), std::string("DRC1") );
	  model->addDataRecord(rec1);

    // ---------- Create Data Records with Parents
	  MXADataRecordPtr rec2 = MXADataRecord::New(0, std::string("Test Data"), std::string("Test Data") );
	  model->addDataRecord(rec2, rec1);
    MXADataRecordPtr rec3 = MXADataRecord::New(0, std::string("Deep Nested Data"), std::string("Nested Data") );
    model->addDataRecord(rec3, rec2);

	  // ---------- Create the Required MetaData
	  std::map<std::string, std::string> md;
	  md[MXA::MXA_CREATOR_TAG] = "Mike Jackson";
	  md[MXA::MXA_DATE_TAG] = "2006:12:24 15:34.51";
	  md[MXA::MXA_DSET_NAME_TAG] = "Testing Data Import";
	  md[MXA::MXA_DESCRIPTION_TAG] = "Application to test importing data to the data file";
	  md[MXA::MXA_PEDIGREE_TAG] = "Original";
	  md[MXA::MXA_DERIVED_SRC_TAG] = "Original Data Files";
	  md[MXA::MXA_RIGHTS_TAG] = "Unlimited";
	  md[MXA::MXA_RELEASE_NUMBER_TAG] = "Not Applicable";
	  model->setRequiredMetaData(md);

	  return modelPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int SimpleTest()
{
  std::cout << "-------- Running SimpleImportTest ------------------------" << std::endl;
int32 err = 0;
MXADataModelPtr model = createSimpleModel();
ImportSimpleData(model, DATAIMPORT_TEST_SIMPLE_IMPORT);

return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CreateTiffImages()
{
  // Create the XMLParser/Importer Object
  DataImportXmlParser importer;
  importer.setXMLInputFile(DATAIMPORT_TEST_IMPORT_XML_FILE);
  // The xmlfile will define the output file so we do NOT need to set it here.
  int32 err = importer.parseXMLFile();
  BOOST_REQUIRE(err >= 0);

  IDataSources dataSources = importer.getDataSources();

  TiffMaker tiffMaker;
  for (IDataSources::iterator iter = dataSources.begin(); iter != dataSources.end(); ++iter ) {
    std::string tiffPath = (*iter).get()->getSourcePath();
   // std::cout << "Making Tiff at " << tiffPath << std::endl;
    tiffMaker.createTiffFile(tiffPath);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int XMLImportTest()
{
  std::cout << "-------- Starting With XML Based H5 Tiff import Test -----------------" << std::endl;
  // Instantiate the Instance Manager for import delegates
  ImportDelegateManagerPtr idManager = ImportDelegateManager::instance();

  //Register to be able to import Tiff images
  H5TiffImportDelegateFactory* ptr = new H5TiffImportDelegateFactory();
  AbstractImportDelegateFactoryPtr h5TiffImportDelegateFactory(ptr);
  ptr->setImportAsGrayScale(true);
  ptr->setFileNotFoundIsError(false);
  ImportDelegateManager::registerImportDelegateFactory(h5TiffImportDelegateFactory);
  // Create the tiff images to use for the import test
  CreateTiffImages();

  DataImportXmlParser importer;
  importer.setXMLInputFile(DATAIMPORT_TEST_IMPORT_XML_FILE);
  // The xmlfile will define the output file so we do NOT need to set it here.
  int32 err = importer.import(); // Run the Import
  if (err < 0)
  {
    std::cout << "Error Importing Data Files. Check any output for possible error logs." << std::endl;
  }
  BOOST_REQUIRE (err >= 0);
  std::cout << "-------- Done With XML Based H5 Tiff import Test -----------------" << std::endl;
  return err;
}


// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "Data Import Test");
  test->add( BOOST_TEST_CASE( &XMLImportTest), 0);
  test->add( BOOST_TEST_CASE( &SimpleTest), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}



