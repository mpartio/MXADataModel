//-- MXA Headers
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/HDF5/H5DataModelReader.h>
#include <MXA/DataWrappers/MXAAsciiStringData.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/Core/MXASupportFile.h>
#include <MXA/Utilities/StringUtils.h>

//-- STL Headers
#include <iostream>
#include <list>
#include <string>

#define CheckValidLocId(locId)\
  if (locId < 0 ) {std::cout << "Invalid HDF Location ID: " << locId << std::endl;return -1;}

//TODO: Read list of Indices for non uniform increment data sets


// -----------------------------------------------------------------------------
//  Constructor
// -----------------------------------------------------------------------------
H5DataModelReader::H5DataModelReader(IDataModelPtr dataModel)
{
  _dataModel = dataModel;
}

// -----------------------------------------------------------------------------
//  Destructor
// -----------------------------------------------------------------------------
H5DataModelReader::~H5DataModelReader()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readDataModel(hid_t locId)
{
  if ( (readModelType(locId)) < 0)
    return false;
  if ( (readModelVersion(locId)) < 0)
    return false;
  if ( (readDataRoot(locId)) < 0)
    return false;
  if ( (readDataDimensions(locId)) < 0)
    return false;
  if ( (readDataRecords(locId)) < 0)
    return false;
  if ( (readRequiredMetaData(locId) ) < 0)
    return false;
  if ( (readUserMetaData(locId) ) < 0)
    return false;
  if ( (readSupportFiles(locId) ) < 0)
    return false;

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readModelType(hid_t locId)
{
  herr_t err = -1;
  std::string fileType;
  err = H5Lite::readStringAttribute(locId, MXA::DataModelPath, MXA::ModelType, fileType);
  if(err < 0)
  {
    std::cout << "Error Reading FileType from HDF5 File" << std::endl;
  }
  else
  {
    _dataModel->setModelType(fileType);
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readModelVersion(hid_t locId)
{
  herr_t err = -1;
  float32 modelVersion;
  err = H5Lite::readScalarAttribute(locId, MXA::DataModel, MXA::ModelVersion, modelVersion);
  if(err < 0)
  {
    std::cout << "Error Reading modelVersion from HDF5 File" << std::endl;
  }
  else
  {
    _dataModel->setModelVersion(modelVersion);
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readDataRoot(hid_t locId)
{
  herr_t err = -1;
  std::string dataRoot;
  err = H5Lite::readStringDataset(locId, MXA::DataRootPath, dataRoot);
  if(err < 0)
  {
    std::cout << "Error Reading DataRoot from HDF5 File" << std::endl;
  }
  else
  {
    _dataModel->setDataRoot(dataRoot);
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readDataDimensions(hid_t locId)
{
  herr_t err = 0;
  std::list<std::string> dimNames;
  //Open the DataModel/DataDimensions Group
  hid_t dataDimId = H5Gopen(locId, MXA::DataDimensionsPath.c_str());
  if(dataDimId > 0) {
    err = H5Utilities::getGroupObjects(dataDimId, H5Utilities::MXA_DATASET,  dimNames);
    //std::cout << "Found " << dimNames.size() << " Dimensions:" << std::endl;
    for (std::list<std::string>::iterator iter = dimNames.begin(); iter != dimNames.end(); ++iter)
    {
      MXADataDimensionPtr dim = _loadDataDimension(dataDimId, *(iter) );
      if ( NULL != dim.get() ) {
        _dataModel->addDataDimension(dim);
      }
    }
  }
  else
  {
    std::cout << "Error trying to open HDF5 Group 'Data Model/Data Dimensions' " << std::endl;
    return err;
  }

  err = H5Gclose(dataDimId);
  if (err < 0)
  {
    std::cout << "Error trying to close HDF5 Group 'Data Model/Data Dimensions' " << std::endl;
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXADataDimensionPtr H5DataModelReader::_loadDataDimension(hid_t loc_id, std::string dimensionName)
{
 // MXADataDimension* node = NULL;
  std::string dimName, altName;
  int32 count=1, start_val=0, end_val=0, dim_order;
  int32 uniform=1, increment=1;
  herr_t err=0;

  // Read the Data Dimension values
  // Dimension Order

  err = H5Lite::readScalarDataset(loc_id, dimensionName, dim_order);
  if (err < 0) {
    std::cout << "Error reading dimension order value" << std::endl;
  }
  // Name
  err = H5Lite::readStringAttribute(loc_id, dimensionName, MXA::MXA_NAME_TAG, dimName);
  if (err < 0) {
    std::cout << "Error reading dimension name value" << std::endl;
  }
  // Alt Name
  err = H5Lite::readStringAttribute(loc_id, dimensionName, MXA::MXA_ALT_NAME_TAG, altName);
  if (err < 0) {
    std::cout << "Error reading dimension alt name value" << std::endl;
  }
  // Count
  err = H5Lite::readScalarAttribute(loc_id, dimensionName, MXA::MXA_COUNT_TAG, count);
  if (err < 0) {
    std::cout << "Error reading dimension count value" << std::endl;
  }
  // Start Value
  err = H5Lite::readScalarAttribute(loc_id, dimensionName, MXA::MXA_START_VALUE_TAG, start_val);
  if (err < 0) {
    std::cout << "Error reading dimension start index value" << std::endl;
  }
  // Uniform
  err = H5Lite::readScalarAttribute(loc_id, dimensionName, MXA::MXA_UNIFORM_TAG, uniform);
  if (err < 0) {
    std::cout << "No Uniform increment marker in dimension" << std::endl;
  }
  // Increment
  err = H5Lite::readScalarAttribute(loc_id, dimensionName, MXA::MXA_INCREMENT_TAG, increment);
  if (err < 0) {
    std::cout << "No increment value in dimension" << std::endl;
  }
  // End Value
  err = H5Lite::readScalarAttribute(loc_id, dimensionName, MXA::MXA_END_VALUE_TAG, end_val);
  if (err < 0) {
    std::cout << "Error reading dimension end index value" << std::endl;
  }
  // indices
  #if 0
  list<int> indices;
  err = H5Utilities::getAttributeValue(loc_id, dimensionName, MXA_INDICES_TAG, &indices);
  if (err < 0) {
    std::cout << "Error reading indicies values" << std::endl;
  }
  #endif

// Collect the dataset Attributes (all as strings)
//TODO: Read any extra attributes from the data dimension entry
 // std::map<std::string, std::string> attributes = H5Utilities::getAttributesMap(loc_id, name);

  MXADataDimensionPtr dimension;
  if (uniform == 1) {
   // std::cout << "Adding uniform dimension" << std::endl;
    dimension = MXADataDimension::New(dimName, altName, dim_order, count, start_val, end_val,increment, uniform);
   // std::cout << "add done" << std::endl;
  }
  else
  {
#if 0
    std::cout << "Adding nonuniform dimension" << std::endl;
    #warning I removed the 'indices' argument from the following method call. Not sure why we need it
    dimension = MXADataDimension::New(dimName, altName, dim_order, count, start_val, end_val,increment, uniform);
  //  std::cout << "Add done" << std::endl;
#endif
    std::cout << "ERROR adding nonUniform Data Dimension. This is not supported yet." << std::endl;
  }

  return dimension;
}


// -----------------------------------------------------------------------------
// Reads the Data Records from the HDF5 file into XMDataRecord objects
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readDataRecords(hid_t fileId)
{
  if (fileId < 0 )
  {
    std::cout << "Invalid HDF FileId: " << fileId << std::endl;
    return -1;
  }
  herr_t err = -1;
  hid_t recId = -1;
  // Open the Data Records Group
  recId = H5Gopen(fileId, MXA::DataRecordsPath.c_str());
  if (recId<0) {
    std::cout << "Error Opening '/Data Model/Data Records' HDF Group:" << recId << std::endl;
    return recId;
  }
  MXADataRecord::Pointer parent;
  err = _traverseDataRecords(recId, parent); //Kick off the recursive algorithm
  if (err < 0) {
    std::cout << "Error Traversing the Data Record tree" << std::endl;
  }
  recId = H5Gclose(recId);
  return err;
}

// -----------------------------------------------------------------------------
// Performs a Depth first traversal and parsing of thd Data Records Group
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::_traverseDataRecords( hid_t gid, MXADataRecord::Pointer parent) {
  if (gid < 0 )
  {
    std::cout << "Invalid HDF Object Id: " << gid << std::endl;
    return -1;
  }
  herr_t err = -1;
  hsize_t numObjects = 0;
  err = H5Gget_num_objs(gid, &numObjects);
  if (err < 0 ) {
    std::cout << "Error Retrieving the number of Data Records: " << err << std::endl;
    return err;
  }

  uint32 index = 0;

  for (index = 0; index < numObjects; ++index) {
    std::string objName;
    err = H5Utilities::objectNameAtIndex(gid, index, objName); //Get the HDF5 Object name
    if (err<0) {
      std::cout << "Error getting object Name for index." << std::endl;
      return -1;
    }
    MXADataRecord::Pointer rec = _loadDataRecord(gid, objName);
    //Check to make sure we got a good DataRecordNode, if not return an error
    if (NULL == rec.get() ) {
      return -1;
    }
    //Check whether we add this data record as a 'top level' or to another node as a child
    if (parent.get() != NULL)
    {
      _dataModel->addDataRecord(rec, parent);
    }
    else
    {
      _dataModel->addDataRecord(rec);
    }

    if ( H5Utilities::isGroup(gid, objName) ) {   //Data Record is a Group
      //std::cout << "Data Record is a HDF Group" << std::endl;
      hid_t currentGroup = H5Gopen(gid, objName.c_str() );
      if (currentGroup < 0) {
        std::cout << "Error Opening HDF5 Group" << std::endl;
        return -1;
      }
      err = _traverseDataRecords(currentGroup, rec);
      if (err<0) {
        std::cout << "Error Traversing the Data Records" << std::endl;
      }
      err = H5Gclose(currentGroup);
    }
  }
  return err;
}

// -----------------------------------------------------------------------------
//  Loads a DataRecord given its name and its parent hid_t value
// -----------------------------------------------------------------------------
MXADataRecord::Pointer H5DataModelReader::_loadDataRecord(hid_t loc_id, std::string name)
{
  herr_t err=0;
  //MXANode *node=NULL;
  std::string recName, altName;
  int32 guid=-1, luid =-1;
  MXADataRecord::Pointer record; // This will contain a NULL Pointer to the DataRecordNode
  // Read the Data Record Values
  // Name
  err = H5Lite::readStringAttribute(loc_id, name, MXA::MXA_NAME_TAG, recName);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return record;
  }
  // Alt Name
  err = H5Lite::readStringAttribute(loc_id, name, MXA::MXA_ALT_NAME_TAG, altName);
  if (err < 0) {
    std::cout << "Error reading data record alt_name value" << std::endl;
    return record;
  }
  // GUID
  err = H5Lite::readScalarAttribute(loc_id, name, MXA::MXA_GUID_TAG, guid);
  if (err < 0) {
    std::cout << "Error reading data record GUID value" << std::endl;
    return record;
  }
  // LUID
  err = H5Lite::readScalarAttribute(loc_id, name, MXA::MXA_LUID_TAG, luid);
  if (err < 0) {
    std::cout << "Error reading data record LUID value" << std::endl;
    return record;
  }

  record = MXADataRecord::New(luid, recName, altName);
  record->setGuid(guid); //Set the GUID from the file
  return record;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readRequiredMetaData(hid_t locId)
{
  std::string researcherName;
  std::string dateCreated;
  std::string derivedSrcFile;
  std::string description;
  std::string distributionRights;
  std::string datasetName;
  std::string pedigree;
  std::string releaseNumber;

  if (locId < 0 )
  {
    std::cout << "Invalid HDF FileId: " << locId << std::endl;
    return -1;
  }
  herr_t err = -1;

  err = H5Lite::readStringAttribute(locId, MXA::RequiredMetaDataPath, MXA::MXA_CREATOR_TAG, researcherName);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }

  err = H5Lite::readStringAttribute(locId, MXA::RequiredMetaDataPath, MXA::MXA_DATE_TAG, dateCreated);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(locId, MXA::RequiredMetaDataPath, MXA::MXA_DSET_NAME_TAG, datasetName);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(locId, MXA::RequiredMetaDataPath, MXA::MXA_DESCRIPTION_TAG, description);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(locId, MXA::RequiredMetaDataPath, MXA::MXA_PEDIGREE_TAG, pedigree);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(locId, MXA::RequiredMetaDataPath, MXA::MXA_DERIVED_SRC_TAG, derivedSrcFile);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(locId, MXA::RequiredMetaDataPath, MXA::MXA_RIGHTS_TAG, distributionRights);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(locId, MXA::RequiredMetaDataPath, MXA::MXA_RELEASE_NUMBER_TAG, releaseNumber);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  //If we are this far then the raw reads from the HDF file succeeded
  this->_dataModel->setRequiredMetaData(researcherName, dateCreated, datasetName, description,distributionRights, releaseNumber, pedigree, derivedSrcFile);

  return 1;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readUserMetaData(hid_t locId)
{
  MXAAbstractAttributes attributes;
  herr_t err = H5Utilities::readAllAttributes(locId, MXA::UserMetaDataPath, attributes);
  if (err >= 0)
  {
    this->_dataModel->setUserMetaData(attributes);
  }

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5DataModelReader::readSupportFiles(hid_t locId)
{
  herr_t err = 0;
  hid_t gid = H5Gopen(locId, MXA::SupportFilesPath.c_str() );
  if (gid < 0){
    return -1;
  }
  std::list<std::string> indices;
  err = H5Utilities::getGroupObjects(gid, H5Utilities::MXA_DATASET, indices);

  for (std::list<std::string>::iterator iter = indices.begin(); iter != indices.end(); ++iter)
  {
    ISupportFilePtr file = MXASupportFile::NewFromMXAFile(locId, *iter, false);
    this->_dataModel->addSupportFile(file, false);
  }
  err = H5Gclose(gid);
  return err;
}

