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
#include "Common/MXATypes.h"
#include "Common/MXATypeDefs.h"
#include "Common/LogTime.h"
#include "Base/IDataImportDelegate.h"
#include "Base/IFileIODelegate.h"
#include "Core/MXADataModel.h"
#include "Core/MXADataDimension.h"
#include "Core/MXADataRecord.h"
#include "Core/MXADataImport.h"
#include "Core/MXADataSource.h"

#include "HDF5/H5Lite.h"
#include "HDF5/H5Utilities.h"

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;

// -----------------------------------------------------------------------------
//  Define where to put our temporary files
// -----------------------------------------------------------------------------
#if defined (_WIN32)
  #define FILE_NAME_BEFORE "C:\\WINDOWS\\Temp\\DataImportTest-Before.h5"
  #define FILE_NAME_AFTER "C:\\WINDOWS\\Temp\\DataImportTest-After.h5"
#else 
  #define FILE_NAME_BEFORE "/tmp/DataImportTest-Before.h5"
  #define FILE_NAME_AFTER "/tmp/DataImportTest-After.h5"
#endif


/**
*  @brief This class is used to show simply how to write some data into an HDF5 file.
*  Some of the features of the IDataImportDelegate are not shown. This class
*  will simply write a single value to the HDF5 file.
* @author Mike Jackson
* @date April 2007
*/
class H5ImportTestDelegate: public IDataImportDelegate
{
public:
  H5ImportTestDelegate(){};
  virtual ~H5ImportTestDelegate(){};

// -----------------------------------------------------------------------------
//  Implemented Method from the IDataImportDelegate interface 
// -----------------------------------------------------------------------------
  int32 importDataSource(IDataSourcePtr dataSource, IDataModelPtr model)
  {
    std::string path ( dataSource->generateInternalPath() );
    uint32 pos = path.find_last_of("/");
    std::string parentPath ( path.substr(0, pos)  );
    int32 value = 55;
    hid_t fileId = model->getIODelegate()->getOpenFileId();
    H5Utilities::createGroupsFromPath(parentPath,  fileId);
    //Write the Data to the HDF5 File
    return H5Lite::writeScalarDataset(fileId, path, value);
  }
  
private:
  H5ImportTestDelegate(const H5ImportTestDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5ImportTestDelegate&); //Copy Assignment Not Implemented
  
};



// -----------------------------------------------------------------------------
//  Creates a DataImport class and some DataSources that then delegate out to 
//  an instance of an IDataImportDelegate to do the actual import of data.
// -----------------------------------------------------------------------------
void ImportSimpleData(MXADataModelPtr model, std::string outputFilePath)
{
  //Create the DataImport Class
  MXADataImportPtr dataImport( new MXADataImport() );
  // Set the HDF5 Output file to write all the data into
  dataImport->setOutputFilePath(outputFilePath);
  // Set the MXADataModel Object into the dataImport Object
  dataImport->setDataModel(model);
  
  // Create an Import Delegate to use for the DataSources
  IDataImportDelegatePtr delegatePtr( new H5ImportTestDelegate());
  
  // We have two dimensions for this model, create a loop to create data sets for each possible dimension value
  MXADataDimension* dim0 = model->getDataDimension(0); // Get the first Dimension, since there is only one this works
  BOOST_REQUIRE(dim0 != NULL); // Used for Boost Unit Test Framework
  
  MXADataDimension* dim1 = model->getDataDimension(1);
  BOOST_REQUIRE(dim1 != NULL); // Used for Boost Unit Test Framework
  
  // Create a DataRecord entry for the Data Model
  MXADataRecordPtr record = model->getDataRecordByNamedPath("DataRecordContainer/Test Data/Deep Nested Data");
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
	  md[MXA::MXA_RELEASE_NUMBER_TAG] = "90312901291239012390";
	  model->setRequiredMetaData(md);
	  
	  return modelPtr;
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int DataImportTest ()
{
  std::cout << logTime() << "----- Running DataImport Test ------------- " << std::endl;
  std::string outputFile(FILE_NAME_BEFORE);
  MXADataModelPtr model = createSimpleModel();
  BOOST_REQUIRE(model->writeModel(outputFile, false) >= 0); //Leave the file open for the import
  ImportSimpleData(model, outputFile);

  return 0;
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) {
  framework::master_test_suite().p_name.value = "Unit test example 03";

   // with explicit registration we could specify a test case timeout
   framework::master_test_suite().add( BOOST_TEST_CASE( &DataImportTest ), 0, /* timeout */ 2 );

   return 0;
}



