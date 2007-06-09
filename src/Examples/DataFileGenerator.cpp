#include "DataFileGenerator.h"



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataFileGenerator::DataFileGenerator()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataFileGenerator::~DataFileGenerator()
{
}

void DataFileGenerator::setFilePath(const std::string &filePath)
{
  this->_filePath = filePath;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t DataFileGenerator::generate()
{
  herr_t err = 1;
  MXADataModelPtr model = createSimpleModel();
  err = model->writeModel(this->_filePath, false); //Leave the file open for the import
  if (err < 0)
  {
    std::cout << logTime() << "Error writing Data Model" << std::endl;
    return err;
  }
  ImportSimpleData(model, this->_filePath);
  return err;
}


// -----------------------------------------------------------------------------
//  Creates a DataImport class and some DataSources that then delegate out to 
//  an instance of an IDataImportDelegate to do the actual import of data.
// -----------------------------------------------------------------------------
herr_t DataFileGenerator::ImportSimpleData(MXADataModelPtr model, std::string outputFilePath)
{
  //Create the DataImport Class
  MXADataImportPtr dataImport( new MXADataImport() );
  // Set the HDF5 Output file to write all the data into
  dataImport->setOutputFilePath(outputFilePath);
  // Set the MXADataModel Object into the dataImport Object
  dataImport->setDataModel(model);
  
  // Create an Import Delegate to use for the DataSources
  IDataImportDelegatePtr delegatePtr( new ScalarDataDelegate());
  IDataImportDelegatePtr vectorDelegatePtr( new VectorDataDelegate());
  // We have two dimensions for this model, create a loop to create data sets for each possible dimension value
  MXADataDimension* dim0 = model->getDataDimension(0); // Get the first Dimension, since there is only one this works
 if (dim0 == NULL) 
 {
   std::cout << "Error getting the data dimension from the model: dim0" << std::endl;
   return -1;
 }
  
  MXADataDimension* dim1 = model->getDataDimension(1);
  if (dim1 == NULL) 
  {
    std::cout << "Error getting the data dimension from the model: dim1" << std::endl;
    return -1;
  }
  
  // Create a DataRecord entry for the Data Model
  MXADataRecordPtr scalarRec = model->getDataRecordByPath("DataRecordContainer/Test Data/Scalar");
  if (NULL == scalarRec.get())
  {
    std::cout << "Error getting the data record: Scalar" << std::endl;
    return -1;
  }

  MXADataRecordPtr vectorRec = model->getDataRecordByPath("DataRecordContainer/Test Data/2D Array");
  if (NULL == vectorRec.get())
  {
    std::cout << "Error getting the data record: 2D Array" << std::endl;
    return -1;
  }
  
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
      //Create a Scalar Data Source
      MXADataSourcePtr ds( new MXADataSource() );
      std::vector<int32> dimValues;
      dimValues.push_back(i);
      dimValues.push_back(j);
      ds->setDimensionValues(dimValues);
      ds->setDataRecord(scalarRec);
      ds->setSourcePath(""); //Since we are doing everything in software, this is unused
      ds->setImportDelegate(delegatePtr);
      ds->setDataModel(model);
      dataImport->addDataSource(ds);
      
      MXADataSourcePtr ds2( new MXADataSource() );
      ds2->setDimensionValues(dimValues);
      ds2->setDataRecord(vectorRec);
      ds2->setSourcePath(""); //Since we are doing everything in software, this is unused
      ds2->setImportDelegate(vectorDelegatePtr);
      ds2->setDataModel(model);
      dataImport->addDataSource(ds2);
    }
  }
  
  // Import the Data into the HDF5 File
  //std::cout << "IMPORTING DATA NOW" << std::endl;
  herr_t err = dataImport->import();
 // std::cout << logTime() << "err=" << err << std::endl;
  if (err < 0)
  {
    std::cout << logTime() << "Error Populating the H5 data file with test data" << std::endl;
  }
  return err;
}


// -----------------------------------------------------------------------------
//  Creates a Data Model programmitically to use with our data import
// -----------------------------------------------------------------------------
MXADataModelPtr DataFileGenerator::createSimpleModel()
{
    MXADataModelPtr modelPtr = MXADataModel::New();
    MXADataModel* model = modelPtr.get();
    model->setDataRoot(std::string("Our Experiment/Laboratory Data"));
    model->setFileType(MXA::MXACurrentFileType);
    model->setFileVersion(MXA::MXACurrentFileVersion);

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
    MXADataRecordPtr rec3 = MXADataRecord::New(0, std::string("Scalar"), std::string("Scalar Data") );
    model->addDataRecord(rec3, rec2);   
    MXADataRecordPtr rec4 = MXADataRecord::New(1, std::string("2D Array"), std::string("Vector Data") );
    model->addDataRecord(rec4, rec2);

    // ---------- Create the Required MetaData 
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


