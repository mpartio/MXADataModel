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
#include "DataFileGenerator.h"

#include <MXA/HDF5/H5MXADataFile.h>

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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
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

    MXADataModelPtr modelPtr = MXADataModel::New();
    MXADataModel* model = modelPtr.get();
    model->setDataRoot( DataGen::DataRoot );
    model->setModelType(MXA::MXACurrentFileType);
    model->setModelVersion(MXA::MXACurrentFileVersion);

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

    // ---------- Create 2 Data Dimensions
    MXADataDimensionPtr dim0 = MXADataDimension::New( DataGen::Dimension1, DataGen::Dimension1, 0, 2, 1, 2, 1, 1);
    model->addDataDimension(dim0);
    MXADataDimensionPtr dim1 = MXADataDimension::New(DataGen::Dimension2, DataGen::Dimension2, 1, 3, 1, 3, 1, 1);
    model->addDataDimension(dim1);

    //Create the DataImport Class
    MXADataImportPtr dataImport( new MXADataImport() );

    // Create some Scalar Data
    MXADataRecordPtr scalarRec = MXADataRecord::New(1, DataGen::ScalarRec, DataGen::ScalarRec);
    modelPtr->addDataRecord(scalarRec);
    std::vector<hsize_t> scalarDims;
    scalarDims.push_back(1);
    err = this->makeRecords(modelPtr, dataImport, scalarRec, scalarDims);

    // ---------- Create top level Data Record to hold the vector data
    MXADataRecordPtr tableRec = MXADataRecord::New(0, DataGen::TableRec, DataGen::TableRec );
    modelPtr->addDataRecord(tableRec); // Add a top level Record
    std::vector<hsize_t> tableDims;
    tableDims.push_back(10); tableDims.push_back(10);
    err = this->makeRecords(modelPtr, dataImport, tableRec, tableDims);

    // Create some Scalar Data
    MXADataRecordPtr volumeRec = MXADataRecord::New(2, DataGen::VolumeRec, DataGen::VolumeRec);
    modelPtr->addDataRecord(volumeRec);
    std::vector<hsize_t> volumeDims;
    volumeDims.push_back(10); volumeDims.push_back(10); volumeDims.push_back(10);
    err = this->makeRecords(modelPtr, dataImport, volumeRec, volumeDims);

    // Write the model to the HDF5 File
    IDataFilePtr dataFile = H5MXADataFile::CreateFileWithModel(_filePath, modelPtr);
    if (NULL == dataFile.get() )
    {
      std::cout << logTime() << "Error writing Data Model" << std::endl;
      return err;
    }
    // Set the MXADataModel Object into the dataImport Object
    dataImport->setDataFile(dataFile);

    //std::cout << "IMPORTING DATA NOW" << std::endl;
    err = dataImport->import();
   // std::cout << logTime() << "err=" << err << std::endl;
    if (err < 0)
    {
      std::cout << logTime() << "Error Populating the H5 data file with test data" << std::endl;
    }


    return err;

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t DataFileGenerator::makeRecords(MXADataModelPtr modelPtr, MXADataImportPtr dataImport, MXADataRecordPtr parentRec, std::vector<hsize_t> dims)
{
#if 0
  // Add all the Data for this record
  MXADataRecordPtr int8Rec = MXADataRecord::New(0, DataGen::Int8Rec, DataGen::Int8Rec );
  modelPtr->addDataRecord(int8Rec, parentRec);
  this->createAndStore(modelPtr, int8Rec, dataImport, MXATypes::Int8Type, dims);

  MXADataRecordPtr int16Rec = MXADataRecord::New(1, DataGen::Int16Rec, DataGen::Int16Rec );
  modelPtr->addDataRecord(int16Rec, parentRec);
  this->createAndStore(modelPtr, int16Rec, dataImport, MXATypes::Int16Type, dims);

  MXADataRecordPtr int32Rec = MXADataRecord::New(2, DataGen::Int32Rec, DataGen::Int32Rec );
  modelPtr->addDataRecord(int32Rec, parentRec);
  this->createAndStore(modelPtr, int32Rec, dataImport, MXATypes::Int32Type, dims);

  MXADataRecordPtr int64Rec = MXADataRecord::New(3, DataGen::Int64Rec, DataGen::Int64Rec );
  modelPtr->addDataRecord(int64Rec, parentRec);
  this->createAndStore(modelPtr, int64Rec, dataImport, MXATypes::Int64Type, dims);

  // Add all the Data for this record
  MXADataRecordPtr uint8Rec = MXADataRecord::New(4, DataGen::Uint8Rec, DataGen::Uint8Rec );
  modelPtr->addDataRecord(uint8Rec, parentRec);
  this->createAndStore(modelPtr, uint8Rec, dataImport, MXATypes::Uint8Type, dims);

  MXADataRecordPtr uint16Rec = MXADataRecord::New(5, DataGen::Uint16Rec, DataGen::Uint16Rec );
  modelPtr->addDataRecord(uint16Rec, parentRec);
  this->createAndStore(modelPtr, uint16Rec, dataImport, MXATypes::Uint16Type, dims);

  MXADataRecordPtr uint32Rec = MXADataRecord::New(6, DataGen::Uint32Rec, DataGen::Uint32Rec );
  modelPtr->addDataRecord(uint32Rec, parentRec);
  this->createAndStore(modelPtr, uint32Rec, dataImport, MXATypes::Uint32Type, dims);

  MXADataRecordPtr uint64Rec = MXADataRecord::New(7, DataGen::Uint64Rec, DataGen::Uint64Rec );
  modelPtr->addDataRecord(uint64Rec, parentRec);
  this->createAndStore(modelPtr, uint64Rec, dataImport, MXATypes::Uint64Type, dims);

  MXADataRecordPtr float32Rec = MXADataRecord::New(8, DataGen::Float32Rec, DataGen::Float32Rec );
  modelPtr->addDataRecord(float32Rec, parentRec);
  this->createAndStore(modelPtr, float32Rec, dataImport, MXATypes::Float32Type, dims);

  MXADataRecordPtr float64Rec = MXADataRecord::New(9, DataGen::Float64Rec, DataGen::Float64Rec );
  modelPtr->addDataRecord(float64Rec, parentRec);
  this->createAndStore(modelPtr, float64Rec, dataImport, MXATypes::Float64Type, dims);
  #endif
  return 1;
}
