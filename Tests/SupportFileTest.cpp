///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////

#include <MXA/Common/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Base/ISupportFile.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXASupportFile.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <MXA/Utilities/MXAFileSystemPath.h>

//-- MXA Unit Test Headers
#include "MXAUnitTestDataFileLocations.h"
#include "TiffMaker.h"

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
  std::cout << "   Removing Test files" << std::endl;
#if REMOVE_TEST_FILES
  MXAFileSystemPath::remove(MXAUnitTest::SupportFileTest::OutputFile);
  MXAFileSystemPath::remove(MXAUnitTest::SupportFileTest::BinaryInputFile);
#endif
}


// -----------------------------------------------------------------------------
//  Creates a Data Model programmitically to use with our data import
// -----------------------------------------------------------------------------
MXADataModel::Pointer createSimpleModel()
{
    MXADataModel::Pointer modelPtr = MXADataModel::New();
    MXADataModel* model = modelPtr.get();
    model->setDataRoot(std::string("Our Experiment/Laboratory Data"));
    model->setModelType(MXA::MXACurrentFileType);
    model->setModelVersion(MXA::MXACurrentFileVersion);

    // ---------- Create 2 Data Dimensions
    MXADataDimension::Pointer dim0 = MXADataDimension::New("Dimension 1", "Dim1", 0, 2, 1, 2, 1, 1);
    model->addDataDimension(dim0);
    MXADataDimension::Pointer dim1 = MXADataDimension::New("Dimension 2", "Dim2", 1, 3, 1, 3, 1, 1);
    model->addDataDimension(dim1);

    // ---------- Create Data Records
    MXADataRecord::Pointer rec1 = MXADataRecord::New(0, std::string("DataRecordContainer"), std::string("DRC1") );
    model->addDataRecord(rec1);

    // ---------- Create Data Records with Parents
    MXADataRecord::Pointer rec2 = MXADataRecord::New(0, std::string("Test Data"), std::string("Test Data") );
    model->addDataRecord(rec2, rec1);
    MXADataRecord::Pointer rec3 = MXADataRecord::New(0, std::string("Deep Nested Data"), std::string("Nested Data") );
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
void CreateInputFiles()
{
  // Create a Simple Tiff file to use as a binary file.
  TiffMaker tiffMaker;
  tiffMaker.createTiffFile(MXAUnitTest::SupportFileTest::BinaryInputFile);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void TestMXASupportFile()
{
  std::cout << "|--TestMXASupportFile" << std::endl;
  ISupportFile::Pointer file =
      MXASupportFile::NewFromFileSystem(MXAUnitTest::SupportFileTest::BinaryInputFile, SupportFile::FileType::Binary, false);
  uint8* contents = NULL;
  // Get the contents which should be NULL because the file has NOT been read yet
  contents = file->getFilePointer(0);
  BOOST_REQUIRE(contents == NULL);
  // Make sure the file is NOT cached in the object
  BOOST_REQUIRE (file->isFileCached() == false);
  BOOST_REQUIRE (file->getFileSize() > 0); // The file size should be Zero

  BOOST_REQUIRE (file->readFromFileSystem() >= 0); // Read the file into the object
  BOOST_REQUIRE (file->getFilePointer(0) != NULL);
  BOOST_REQUIRE (file->getFileSize() > 0);
  BOOST_REQUIRE (file->isFileCached() == true);

  file->flushCachedFile();
  BOOST_REQUIRE (file->getFilePointer(0) == NULL);
  BOOST_REQUIRE (file->getFileSize() > 0);
  BOOST_REQUIRE (file->isFileCached() == false);

  // Read the file again
  BOOST_REQUIRE (file->readFromFileSystem() >= 0); // Read the file into the object
  file->setFileSystemPath(MXAUnitTest::SupportFileTest::TextInputFile);
  BOOST_REQUIRE (file->getFilePointer(0) == NULL);
  BOOST_REQUIRE (file->getFileSize() > 0);
  BOOST_REQUIRE (file->isFileCached() == false);



}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestMXARead()
{
  std::cout << "|--MXARead Test" << std::endl;
  IDataFile::Pointer datafile = H5MXADataFile::OpenFile(MXAUnitTest::SupportFileTest::OutputFile, true);
  BOOST_REQUIRE( datafile.get() != NULL);

  ISupportFile::Container files = datafile->getDataModel()->getSupportFiles();
  ISupportFile::Pointer file;
  for (ISupportFile::Container::iterator iter = files.begin(); iter != files.end(); ++iter)
  {
    file = *iter;
    BOOST_REQUIRE (file->getFilePointer(0) == NULL);
    BOOST_REQUIRE (file->getFileSize() > 0);
    BOOST_REQUIRE (file->isFileCached() == false);

    BOOST_REQUIRE (file->readFromMXAFile() >= 0); // Read the file into the object
    BOOST_REQUIRE (file->getFilePointer(0) != NULL);
    BOOST_REQUIRE (file->getFileSize() > 0);
    BOOST_REQUIRE (file->isFileCached() == true);
     file->flushCachedFile();
    BOOST_REQUIRE (file->getFilePointer(0) == NULL);
    BOOST_REQUIRE (file->getFileSize() > 0);
    BOOST_REQUIRE (file->isFileCached() == false);
  }

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestMXAWrite()
{
  std::cout << "|--MXAWrite Test" << std::endl;
  int32 err = 0;
  MXADataModel::Pointer model = createSimpleModel();
  ISupportFile::Pointer binaryFile = MXASupportFile::NewFromFileSystem(MXAUnitTest::SupportFileTest::BinaryInputFile, SupportFile::FileType::Binary);
  ISupportFile::Pointer textFile = MXASupportFile::NewFromFileSystem(MXAUnitTest::SupportFileTest::TextInputFile, SupportFile::FileType::Text);
  model->addSupportFile(binaryFile, true);
  model->addSupportFile(textFile, true);

  IDataFile::Pointer mxaDataFile = H5MXADataFile::CreateFileWithModel(MXAUnitTest::SupportFileTest::OutputFile, model);
  if ( NULL == mxaDataFile.get()) { err = -1; } else { err = 0; };
  BOOST_REQUIRE(err >= 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int SupportFileTest_EntryPoint()
{
  std::cout << "Running SupportFileTest" << std::endl;
  int32 err = 0;
  // Create the input files first
  CreateInputFiles();

  // Run Some basic tests on MXASupportFile class
  TestMXASupportFile();

  // Test Reading a few files from the file system and writing those to and HDF5 file
  TestMXAWrite();

  // Test Reading the file contents back from the MXA file
  TestMXARead();



  return err;
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "Data Import Test");
  test->add( BOOST_TEST_CASE( &SupportFileTest_EntryPoint), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}

