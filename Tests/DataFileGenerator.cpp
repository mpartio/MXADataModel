///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
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

    MXADataModel::Pointer modelPtr = MXADataModel::New();
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
    MXADataDimension::Pointer dim0 = MXADataDimension::New( DataGen::Dimension1, DataGen::Dimension1, 0, 2, 1, 2, 1, 1);
    model->addDataDimension(dim0);
    MXADataDimension::Pointer dim1 = MXADataDimension::New(DataGen::Dimension2, DataGen::Dimension2, 1, 3, 1, 3, 1, 1);
    model->addDataDimension(dim1);

    //Create the DataImport Class
    MXADataImport::Pointer dataImport( new MXADataImport() );

    // Create some Scalar Data
    MXADataRecord::Pointer scalarRec = MXADataRecord::New(1, DataGen::ScalarRec, DataGen::ScalarRec);
    modelPtr->addDataRecord(scalarRec);
    std::vector<hsize_t> scalarDims;
    scalarDims.push_back(1);
    err = this->makeRecords(modelPtr, dataImport, scalarRec, scalarDims);

    // ---------- Create top level Data Record to hold the vector data
    MXADataRecord::Pointer tableRec = MXADataRecord::New(0, DataGen::TableRec, DataGen::TableRec );
    modelPtr->addDataRecord(tableRec); // Add a top level Record
    std::vector<hsize_t> tableDims;
    tableDims.push_back(10); tableDims.push_back(10);
    err = this->makeRecords(modelPtr, dataImport, tableRec, tableDims);

    // Create some Scalar Data
    MXADataRecord::Pointer volumeRec = MXADataRecord::New(2, DataGen::VolumeRec, DataGen::VolumeRec);
    modelPtr->addDataRecord(volumeRec);
    std::vector<hsize_t> volumeDims;
    volumeDims.push_back(10); volumeDims.push_back(10); volumeDims.push_back(10);
    err = this->makeRecords(modelPtr, dataImport, volumeRec, volumeDims);

    // Write the model to the HDF5 File
    IDataFile::Pointer dataFile = H5MXADataFile::CreateFileWithModel(_filePath, modelPtr);
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
herr_t DataFileGenerator::makeRecords(MXADataModel::Pointer modelPtr, MXADataImport::Pointer dataImport, MXADataRecord::Pointer parentRec, std::vector<hsize_t> dims)
{
#if 0
  // Add all the Data for this record
  MXADataRecord::Pointer int8_tRec = MXADataRecord::New(0, DataGen::Int8Rec, DataGen::Int8Rec );
  modelPtr->addDataRecord(int8_tRec, parentRec);
  this->createAndStore(modelPtr, int8_tRec, dataImport, MXATypes::Int8Type, dims);

  MXADataRecord::Pointer int16_tRec = MXADataRecord::New(1, DataGen::Int16Rec, DataGen::Int16Rec );
  modelPtr->addDataRecord(int16_tRec, parentRec);
  this->createAndStore(modelPtr, int16_tRec, dataImport, MXATypes::Int16Type, dims);

  MXADataRecord::Pointer int32_tRec = MXADataRecord::New(2, DataGen::Int32Rec, DataGen::Int32Rec );
  modelPtr->addDataRecord(int32_tRec, parentRec);
  this->createAndStore(modelPtr, int32_tRec, dataImport, MXATypes::Int32Type, dims);

  MXADataRecord::Pointer int64_tRec = MXADataRecord::New(3, DataGen::Int64Rec, DataGen::Int64Rec );
  modelPtr->addDataRecord(int64_tRec, parentRec);
  this->createAndStore(modelPtr, int64_tRec, dataImport, MXATypes::Int64Type, dims);

  // Add all the Data for this record
  MXADataRecord::Pointer uint8_tRec = MXADataRecord::New(4, DataGen::Uint8_tRec, DataGen::Uint8_tRec );
  modelPtr->addDataRecord(uint8_tRec, parentRec);
  this->createAndStore(modelPtr, uint8_tRec, dataImport, MXATypes::Uint8_tType, dims);

  MXADataRecord::Pointer uint16_tRec = MXADataRecord::New(5, DataGen::Uint16_tRec, DataGen::Uint16_tRec );
  modelPtr->addDataRecord(uint16_tRec, parentRec);
  this->createAndStore(modelPtr, uint16_tRec, dataImport, MXATypes::Uint16_tType, dims);

  MXADataRecord::Pointer uint32_tRec = MXADataRecord::New(6, DataGen::Uint32_tRec, DataGen::Uint32_tRec );
  modelPtr->addDataRecord(uint32_tRec, parentRec);
  this->createAndStore(modelPtr, uint32_tRec, dataImport, MXATypes::Uint32_tType, dims);

  MXADataRecord::Pointer uint64_tRec = MXADataRecord::New(7, DataGen::Uint64_tRec, DataGen::Uint64_tRec );
  modelPtr->addDataRecord(uint64_tRec, parentRec);
  this->createAndStore(modelPtr, uint64_tRec, dataImport, MXATypes::Uint64_tType, dims);

  MXADataRecord::Pointer floatRec = MXADataRecord::New(8, DataGen::Float32Rec, DataGen::Float32Rec );
  modelPtr->addDataRecord(floatRec, parentRec);
  this->createAndStore(modelPtr, floatRec, dataImport, MXATypes::Float32Type, dims);

  MXADataRecord::Pointer doubleRec = MXADataRecord::New(9, DataGen::Float64Rec, DataGen::Float64Rec );
  modelPtr->addDataRecord(doubleRec, parentRec);
  this->createAndStore(modelPtr, doubleRec, dataImport, MXATypes::Float64Type, dims);
  #endif
  return 1;
}
