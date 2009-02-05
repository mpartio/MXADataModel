#include <Examples/DataImport/SimpleImportExample.h>

//-- MXA Headers
#include <MXA/Base/IImportDelegate.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXADataSource.h>
#include <MXA/Core/MXADataImport.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <MXA/DataImport/ImportDelegateManager.h>
#include <Examples/ExampleFileLocations.h>
#include <Examples/DataImport/ExampleImportDelegate.h>
#include <Examples/DataImport/ExampleImportDelegateFactory.h>

//-- STL Headers
#include <iostream>
#include <fstream>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SimpleImportExample::SimpleImportExample()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SimpleImportExample::~SimpleImportExample()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SimpleImportExample::runImport(const std::string &outputFilePath)
{
  // Create a DataModel
  MXADataModelPtr model = this->createSimpleModel();

  // Create some external files that our import delegate will read
  this->createTestFiles(model);

  // Create a DataFile object which will create the actual file on disk.
  IDataFilePtr dataFile = H5MXADataFile::CreateFileWithModel(outputFilePath, model);

  //Create the DataImport Class
  MXADataImportPtr dataImport(new MXADataImport());
  dataImport->setDataFile(dataFile);

  // Get an instance to the ImportDelegateManager
  ImportDelegateManagerPtr idManagerPtr = ImportDelegateManager::instance();
  // Get a reference to an Import Delegate given the name of the class
  IImportDelegatePtr delegatePtr = ImportDelegateManager::createNewImportDelegate(ExampleImport::Detail::ClassName);

  // We have two dimensions for this model, create a loop to create data sets for each possible dimension value
  IDataDimensionPtr dim0 = model->getDataDimension(0); // Get the first Dimension
  IDataDimensionPtr dim1 = model->getDataDimension(1); // Get the second Dimension

  // Create a DataRecord entry for the Data Model
  IDataRecordPtr record = model->getDataRecordByNamedPath("DataRecordContainer/Test Data/Deep Nested Data");

  // Set the start/end/increment values for each Data Dimension
  int32 dim0Start = dim0->getStartValue();
  int32 dim0End = dim0->getEndValue();
  int32 dim0Increment = dim0->getIncrement();

  int32 dim1Start = dim1->getStartValue();
  int32 dim1End = dim1->getEndValue();
  int32 dim1Increment = dim1->getIncrement();

  // Create a nested loop to create the necessary DataSource objects that will
  //  be used to import the data into the HDF5 file
  for (int32 i = dim0Start; i <= dim0End; i += dim0Increment)
  {
    for (int j = dim1Start; j <= dim1End; j = j + dim1Increment)
    {
      std::string filename(Examples::SimportImportDataInputFile);
      filename.append( StringUtils::numToString<int32>(i) );
      filename.append("_").append(StringUtils::numToString<int32>(j)).append(".data");

      //Create some Data Sources
      MXADataSourcePtr ds(new MXADataSource());
      std::vector<int32> dimValues;
      dimValues.push_back(i);
      dimValues.push_back(j);
      ds->setDimensionValues(dimValues);
      ds->setDataRecord(record);
      ds->setSourcePath(filename); //Since we are doing everything in software, this is unused
      ds->setImportDelegate(delegatePtr);
      ds->setDataModel(model);
      dataImport->addDataSource(ds);
    }
  }

  // Import the Data into the HDF5 File
  // std::cout << "IMPORTING DATA NOW" << std::endl;
  int32 err = dataImport->import();
  if (err < 0)
  {

  }
  dataFile->closeFile(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SimpleImportExample::createTestFiles(MXADataModelPtr model)
{
  // We have two dimensions for this model, create a loop to create data sets for each possible dimension value
    IDataDimensionPtr dim0 = model->getDataDimension(0); // Get the first Dimension
    IDataDimensionPtr dim1 = model->getDataDimension(1); // Get the second Dimension

    // Create a DataRecord entry for the Data Model
    IDataRecordPtr record = model->getDataRecordByNamedPath("DataRecordContainer/Test Data/Deep Nested Data");

    // Set the start/end/increment values for each Data Dimension
    int32 dim0Start = dim0->getStartValue();
    int32 dim0End = dim0->getEndValue();
    int32 dim0Increment = dim0->getIncrement();

    int32 dim1Start = dim1->getStartValue();
    int32 dim1End = dim1->getEndValue();
    int32 dim1Increment = dim1->getIncrement();

    // Create a nested loop to create the necessary DataSource objects that will
    //  be used to import the data into the HDF5 file
    for (int32 i = dim0Start; i <= dim0End; i += dim0Increment)
    {
      for (int32 j = dim1Start; j <= dim1End; j = j + dim1Increment)
      {
        //Create some Data files
        std::string filename(Examples::SimportImportDataInputFile);
        filename.append( StringUtils::numToString<int32>(i) );
        filename.append("_").append(StringUtils::numToString<int32>(j)).append(".data");
        std::ofstream file_stream;
        file_stream.open(filename.c_str(), std::ios::binary );
        std::cout << "Creating input file '" << filename << "'" << std::endl;
        int32 NX = 5; /* dataset dimensions */
        int32 NY = 3;
        int32 value = 0;
        for (int32 k = 0; k < NX; k++) {
          for (int32 l = 0; l < NY; l++) {
              value = l + k;
              file_stream.write((char *)(&value), sizeof(value));
          }
        }
        file_stream.flags();
      }
    }
}

// -----------------------------------------------------------------------------
//  Creates a Data Model programmitically to use with our data import
// -----------------------------------------------------------------------------
MXADataModelPtr SimpleImportExample::createSimpleModel()
{
  MXADataModelPtr modelPtr = MXADataModel::New();
  MXADataModel* model = modelPtr.get();
  model->setDataRoot(std::string("DataImport Example Data"));
  model->setModelType(MXA::MXACurrentFileType);
  model->setModelVersion(MXA::MXACurrentFileVersion);

  // ---------- Create 2 Data Dimensions
  MXADataDimensionPtr dim0 = MXADataDimension::New("Dimension 1", "Dim1", 0, 2, 1, 2, 1, 1);
  model->addDataDimension(dim0);
  MXADataDimensionPtr dim1 = MXADataDimension::New("Dimension 2", "Dim2", 1, 3, 1, 3, 1, 1);
  model->addDataDimension(dim1);

  // ---------- Create Data Records
  MXADataRecordPtr rec1 = MXADataRecord::New(0, std::string("DataRecordContainer"), std::string("DRC1"));
  model->addDataRecord(rec1);

  // ---------- Create Data Records with Parents
  MXADataRecordPtr rec2 = MXADataRecord::New(0, std::string("Test Data"), std::string("Test Data"));
  model->addDataRecord(rec2, rec1);
  MXADataRecordPtr rec3 = MXADataRecord::New(0, std::string("Deep Nested Data"), std::string("Nested Data"));
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

