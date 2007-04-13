
#include "Headers/MXATypes.h"
#include "Headers/MXATypeDefs.h"
#include "Interfaces/IDataImportDelegate.h"
#include "Interfaces/IFileIODelegate.h"
#include "MXADataModel/MXADataModel.h"
#include "MXADataModel/MXADataDimension.h"
#include "MXADataModel/MXADataRecord.h"
#include "MXADataModel/MXADataImport.h"
#include "MXADataModel/MXADataSource.h"

#include "HDF5/H5Lite.h"

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>

using boost::unit_test::test_suite;

// -----------------------------------------------------------------------------
//  This class is used to show simply how to write some data into an HDF5 file.
//  Some of the features of the IDataImportDelegate are not shown. This class
//  will simply write a single value to the HDF5 file.
// -----------------------------------------------------------------------------
class H5ImportTestDelegate: public IDataImportDelegate
{
public:
  H5ImportTestDelegate(){};
  virtual ~H5ImportTestDelegate(){};
  
  int32 importDataSource(MXADataSourcePtr dataSource, MXADataModelPtr model)
  {
    //std::cout << dataSource->generateInternalPath() << std::endl;
    int32 value = 55;
    std::string path ( dataSource->generateInternalPath() );
    hid_t fileId = model->getIODelegate()->getOpenFileId();
    uint32 pos = path.find_last_of("/");
    std::string parentPath ( path.substr(0, pos)  );
    model->getIODelegate()->createGroupsFromPath(parentPath,  fileId);
    //Write the Data to the HDF5 File
    return H5Lite::writeDataset(fileId, path , value, H5Lite::HDFTypeForPrimitive(value));
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
  dataImport->setOutputFilePath(outputFilePath);
  dataImport->setDataModel(model);
  
  // Create an Import Delegate to use for the DataSources
  IDataImportDelegatePtr delegatePtr( new H5ImportTestDelegate());
  
  
  // We have two dimensions for this model, create a loop to create data sets for each possible dimension value
  MXADataDimension* dim0 = model->getDataDimension(0); // Get the first Dimension, since there is only one this works
  BOOST_REQUIRE(dim0 != NULL);
  
  MXADataDimension* dim1 = model->getDataDimension(1);
  BOOST_REQUIRE(dim1 != NULL);
  
  
  MXADataRecordPtr record = model->getDataRecordByPath("DataRecordContainer/Test Data/Deep Nested Data");
  BOOST_REQUIRE(NULL != record.get());

  int dim0Start = dim0->getStartValue();
  int dim0End = dim0->getEndValue();
  int dim0Increment = dim0->getIncrement();
  
  int dim1Start = dim1->getStartValue();
  int dim1End = dim1->getEndValue();
  int dim1Increment = dim1->getIncrement();
  
  for( int i = dim0Start; i <= dim0End; i += dim0Increment )
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
  
  int32 err = dataImport->import();
  BOOST_REQUIRE(err >= 0);
  
}


// -----------------------------------------------------------------------------
//  Creates a Data Model to use
// -----------------------------------------------------------------------------
MXADataModelPtr createSimpleModel()
{
	  MXADataModelPtr modelPtr = MXADataModel::New();
	  MXADataModel* model = modelPtr.get();
	  model->setDataRoot(std::string("Our Experiment/Laboratory Data"));
	  model->setFileType(MXA::MXACurrentFileType);
	  model->setFileVersion(MXA::MXACurrentFileVersion);

	  // ---------- Test creation/addition of Data Dimensions
	  MXADataDimensionPtr dim0 = MXADataDimension::New("Dimension 1", "Dim1", 0, 3, 1, 3, 1, 1);
	  model->addDataDimension(dim0);
    MXADataDimensionPtr dim1 = MXADataDimension::New("Dimension 2", "Dim2", 1, 4, 2, 8, 2, 1);
    model->addDataDimension(dim1);
	  	  
	  //Create Data Records
	  MXADataRecordPtr rec1 = MXADataRecord::New(0, std::string("DataRecordContainer"), std::string("DRC1") );
	  model->addDataRecord(rec1);
	  //Create Data Records with Parents
	  MXADataRecordPtr rec2 = MXADataRecord::New(0, std::string("Test Data"), std::string("Test Data") );
	  model->addDataRecord(rec2, rec1);
    MXADataRecordPtr rec3 = MXADataRecord::New(0, std::string("Deep Nested Data"), std::string("Nested Data") );
    model->addDataRecord(rec3, rec2);   

	  //Create the Required MetaData 
	  std::map<std::string, std::string> md;
	  md[MXA::MXA_CREATOR_TAG] = "Mike Jackson";
	  md[MXA::MXA_DATE_TAG] = "2006:12:24 15:34.51";
	  md[MXA::MXA_DSET_NAME_TAG] = "Testing Data Import";
	  md[MXA::MXA_DESCRIPTION_TAG] = "Application to test importing data to the data file";
	  md[MXA::MXA_PEDIGREE_TAG] = "Original";
	  md[MXA::MXA_DERIVED_SRC_TAG] = "Original Data Files";
	  md[MXA::MXA_RIGHTS_TAG] = "Unlimited";
	  md[MXA::MXA_RELEASE_TAG] = "90312901291239012390";
	  model->setRequiredMetaData(md);
	  
	  
	  return modelPtr;
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int DataImportTest ()
{
  std::string outputFile("/tmp/DataImportTest.h5");
  MXADataModelPtr model = createSimpleModel();
  BOOST_REQUIRE(model->writeModel(outputFile, false) >= 0); //Leave the file open for the import
  ImportSimpleData(model, outputFile);
  return 0;
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
    test_suite* test= BOOST_TEST_SUITE( "Data Import Tests" );
    test->add( BOOST_TEST_CASE( &DataImportTest), 0);
    return test; 
}



