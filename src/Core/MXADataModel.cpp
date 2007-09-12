//-- MXA Headers
#include <MXAConfiguration.h>
#include "Common/LogTime.h"
#include "Base/IFileIODelegate.h"
#include "Core/MXADataModel.h"
#include "Utilities/StringUtils.h"
#include "XML/XMLDataModelWriter.h"

//-- Standard Library Headers
#include <iostream>

#if HDF5_SUPPORT
#include "HDF5/H5IODelegate.h"
#endif

using namespace MXA;

//-------------------------------------------------
// Constructor
//-------------------------------------------------
MXADataModel::MXADataModel() :
_fileVersion(-1.0),
_fileType(""),
_dataRoot("/"),
_researcherName(""),
_datasetDateCreated(""),
_derivedSourceFile(""),
_datasetDescription(""),
_distributionRights(""),
_datasetName(""),
_datasetPedigree(""),
_datasetPublicReleaseNumber("")
{
  #if HDF5_SUPPORT
  _ioDelegate.reset(new H5IODelegate());
  #else
  #warning HDF5_SUPPORT is OFF. There is NO default IODelegatePtr for MXADataModel
  #endif
}

// -----------------------------------------------------------------------------
//  Static New Method to construct a Model
// -----------------------------------------------------------------------------
MXADataModelPtr MXADataModel::New(float modelVersion, const std::string &type)
{
  MXADataModelPtr model(new MXADataModel());
  model->setModelVersion(modelVersion);
  model->setModelType(type);
  return model;
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataModel::~MXADataModel() 
{
	//std::cout << "$-$-$-$-$ MXADataModel Destructor $-$-$-$-$" << std::endl;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setModelVersion(float version) 
{
  this->_fileVersion= version;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
float MXADataModel::getModelVersion() 
{
  return this->_fileVersion;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setModelType(const std::string &modelType)
{
  this->_fileType = modelType;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataModel::getModelType()
{
  return MXA::MXACurrentFileType;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setDefaultTypeAndVersion()
{
  this->_fileType = MXA::MXACurrentFileType;
  this->_fileVersion = MXA::MXACurrentFileVersion;
}
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setDataRoot(const std::string &dataRoot) 
{
  StringUtils::ensureRightSlash(  const_cast<std::string&>(dataRoot) );
  this->_dataRoot = dataRoot;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataModel::getDataRoot()
{
  return _dataRoot;
}

// -----------------------------------------------------------------------------
//  Adds a data dimension object to our internal list
// -----------------------------------------------------------------------------
void MXADataModel::addDataDimension(IDataDimensionPtr dimension, bool setIndex)
{
  this->_dataDimensions.push_back(dimension);
  if (setIndex)
  {
    int32 index = this->_dataDimensions.size() - 1;
    if (index < 0) { index = 0;}
    dimension->setIndex(index);
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataDimensionPtr MXADataModel::addDataDimension(std::string name, std::string altName,
                                int32 index, int32 count, int32 startValue, 
                                int32 endValue, int32 increment, int32 uniform)
{
  MXADataDimensionPtr dim = 
    MXADataDimension::New(name, altName, index, count, startValue, endValue, increment, uniform);
  
  if (endValue == -1) {
    dim->setEndValue (startValue + (increment * (count - 1) ) );
  }

  this->addDataDimension(dim, false);
  return dim;
}

// -----------------------------------------------------------------------------
//  Removes a Data Dimension from this list using the index into the vector
// -----------------------------------------------------------------------------
int32 MXADataModel::removeDataDimension(int32 index)
{
  int32 success = -1;
  int32 i =0;
  for (IDataDimensions::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
  {
    if ( index == i ) 
    {
      _dataDimensions.erase( iter );
      success = 1;
      break;
    }
    ++i;
  }
  return success;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataModel::removeDataDimension(const std::string &dimensionName)
{
  int32 success = -1;
  int32 i =0;
  MXADataDimension* dim = NULL;
  for (IDataDimensions::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
  {
    dim = static_cast<MXADataDimension*>( (*(iter)).get() );
    if (dim->getDimensionName().compare(dimensionName) == 0)
    {
      _dataDimensions.erase( iter );
      success = 1;
      break;
    }
    ++i;
  }
  return success;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataDimensions& MXADataModel::getDataDimensions()
{
  return this->_dataDimensions;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataDimension* MXADataModel::getDataDimension(int32 index)
{
  if ( 0 <= index  &&  static_cast<uint32>(index) < this->_dataDimensions.size() )
  {
    IDataDimensionPtr ptr = this->_dataDimensions[index];
    return ptr.get();
  }
  return NULL;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataDimension* MXADataModel::getDataDimension(std::string dimName)
{
  MXADataDimension* currentDim = NULL;
  for (IDataDimensions::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    currentDim = static_cast<MXADataDimension*>((*(iter)).get()); // Cast down to the MXADataRecord Pointer
    if (   currentDim->getDimensionName() == dimName  ) {
      return currentDim;
    }
  }
  return NULL;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataModel::getNumberOfDataDimensions()
{
  return this->_dataDimensions.size();
}

// -----------------------------------------------------------------------------
//  Adds a Data Record to the top level of the DataRecords Group
// -----------------------------------------------------------------------------
void MXADataModel::addDataRecord(IDataRecordPtr record)
{
  this->_dataRecords.push_back(record);
}

// -----------------------------------------------------------------------------
//  Simply adds the record as a child of the parent. By adding it to an existing
//   parent in the hierarchy, the record will become part of the hierarchy
// -----------------------------------------------------------------------------
void MXADataModel::addDataRecord(IDataRecordPtr record, IDataRecordPtr parent)
{
  if (NULL != parent.get() )
  {
    parent->addChild(record);
  }
  else
  {
    this->_dataRecords.push_back(record);
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataRecords& MXADataModel::getDataRecords()
{
  return this->_dataRecords;
}

// -----------------------------------------------------------------------------
//  Gets an MXADataRecord by the path given
// -----------------------------------------------------------------------------
IDataRecordPtr MXADataModel::getDataRecordByNamedPath(std::string path, IDataRecord* parentPtr)
{
 // std::cout << "Looking for Path: " << path << std::endl;
  // Remove any trailing slash in the path
  MXADataRecord* currentRec = NULL;
  IDataRecordPtr rec;
  IDataRecord* parent = static_cast<IDataRecord*>(parentPtr);
  
  // remove any front slash
  std::string::size_type pos = path.find_first_of("/", 0);
  if ( 0 == pos ) 
  {
    path = path.substr(1, path.size());
  }
  
  //Remove any trailing slash
  pos = path.find_last_of("/");
  if ( pos == (path.size() - 1) ) // slash was in the last position
  {
    path = path.substr(0, pos);
  }
  
  if (path.size() == 0) 
  {
    return rec; // The path that was passed in was only a slash.. 
  }
  

  std::string first;
  std::string second;
  IDataRecords records;
  if ( NULL == parent) // No parent, so start at the top
  {
    records = this->_dataRecords;
  }
  else 
  {
     records = parent->getChildren();
  }
  
  for (IDataRecords::iterator iter = records.begin(); iter != records.end(); ++iter)
  {
    currentRec = static_cast<MXADataRecord*>((*(iter)).get()); // Cast down to the MXADataRecord Pointer
    std::string recName = currentRec->getRecordName();
    pos = path.find_first_of("/", 0);
    if (pos == std::string::npos) // No slash found
    {
      first = path;
      second = "";
    } 
    else 
    {
      first = path.substr(0, pos);
      second = path.substr(pos, path.size());
    }
    if ( first.compare(recName) == 0)
    {
      if (second.empty()) // Only return if we are at the end of the path
      { 
        return boost::dynamic_pointer_cast<MXADataRecord>(*iter);
      } 
      else
      {
        rec = getDataRecordByNamedPath(second, currentRec);
        if (rec.get() != NULL)
        {
          break;
        }
      }
    }
  }
  return rec;
}

// -----------------------------------------------------------------------------
//  Gets an MXADataRecord by the path given
// -----------------------------------------------------------------------------
IDataRecordPtr MXADataModel::getDataRecordByInternalPath(std::string path, IDataRecord* parent)
{
 // std::cout << "Looking for Path: " << path << std::endl;
  // Remove any trailing slash in the path
  IDataRecord* currentRec = NULL;
  IDataRecordPtr rec;

  // remove any front slash
  std::string::size_type pos = path.find_first_of("/", 0);
  if ( 0 == pos ) 
  {
    path = path.substr(1, path.size());
  }
  
  //Remove any trailing slash
  pos = path.find_last_of("/");
  if ( pos == (path.size() - 1) ) // slash was in the last position
  {
    path = path.substr(0, pos);
  }
  
  if (path.size() == 0) 
  {
    return rec; // The path that was passed in was only a slash.. 
  }
  

  std::string first;
  std::string second;
  IDataRecords records;
  if ( NULL == parent) // No parent, so start at the top
  {
    records = this->_dataRecords;
  }
  else 
  {
     records = parent->getChildren();
  }
  
  for (IDataRecords::iterator iter = records.begin(); iter != records.end(); ++iter)
  {
    currentRec = static_cast<MXADataRecord*>((*(iter)).get()); // Cast down to the MXADataRecord Pointer
    std::string recName = StringUtils::numToString( currentRec->getLuid() );
    pos = path.find_first_of("/", 0);
    if (pos == std::string::npos) // No slash found
    {
      first = path;
      second = "";
    } 
    else 
    {
      first = path.substr(0, pos);
      second = path.substr(pos, path.size());
    }
    if ( first.compare(recName) == 0)
    {
      if (second.empty()) // Only return if we are ath the end of the path
      { 
        return boost::dynamic_pointer_cast<IDataRecord>(*iter);
      } 
      else
      {
        rec = getDataRecordByInternalPath(second, static_cast<IDataRecord*>(currentRec) );
        if (rec.get() != NULL)
        {
          break;
        }
      }
    }
  }
  return rec;
}

// -----------------------------------------------------------------------------
//  Sets all the Required MetaData in one shot
// -----------------------------------------------------------------------------
MXATypes::MXAError MXADataModel::setRequiredMetaData(std::string researcherName, 
              std::string dateCreated, 
				      std::string datasetName, 
              std::string description,
				      std::string distributionRights,
				      std::string releaseNumber,
				      std::string pedigree, 
              std::string derivedSrcFile)
{
  
  _researcherName = researcherName;
  _datasetDateCreated = dateCreated;
  _derivedSourceFile = derivedSrcFile;
  _datasetDescription = description;
  _distributionRights = distributionRights;
  _datasetName = datasetName;
  _datasetPedigree = pedigree;
  _datasetPublicReleaseNumber = releaseNumber;
  return 1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXATypes::MXAError MXADataModel::setRequiredMetaData(std::map<std::string, std::string> &requiredMetaData)
{
  std::string message;
  if ( MXADataModel::validateRequiredMetaData(requiredMetaData, message) < 0)
  {
    std::cout << "Meta Data did not Validate." << std::endl;
  }
  
  this->_researcherName = requiredMetaData[MXA::MXA_CREATOR_TAG];
  this->_datasetDateCreated = requiredMetaData[MXA::MXA_DATE_TAG];
  this->_derivedSourceFile = requiredMetaData[MXA::MXA_DERIVED_SRC_TAG];
  this->_datasetDescription = requiredMetaData[MXA::MXA_DESCRIPTION_TAG];
  this->_distributionRights = requiredMetaData[MXA::MXA_RIGHTS_TAG];
  this->_datasetName = requiredMetaData[MXA::MXA_DSET_NAME_TAG];
  this->_datasetPedigree = requiredMetaData[MXA::MXA_PEDIGREE_TAG];
  this->_datasetPublicReleaseNumber = requiredMetaData[MXA::MXA_RELEASE_NUMBER_TAG];
  
  return 1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::getRequiredMetaData(std::map<std::string, std::string> &requiredMetaData)
{
  requiredMetaData.clear(); //Clear the map
  requiredMetaData[MXA::MXA_CREATOR_TAG] = this->_researcherName;
  requiredMetaData[MXA::MXA_DATE_TAG] = this->_datasetDateCreated;
  requiredMetaData[MXA::MXA_DERIVED_SRC_TAG] = this->_derivedSourceFile;
  requiredMetaData[MXA::MXA_DESCRIPTION_TAG] = this->_datasetDescription;
  requiredMetaData[MXA::MXA_RIGHTS_TAG] = this->_distributionRights;
  requiredMetaData[MXA::MXA_DSET_NAME_TAG] = this->_datasetName;
  requiredMetaData[MXA::MXA_PEDIGREE_TAG] = this->_datasetPedigree;
  requiredMetaData[MXA::MXA_RELEASE_NUMBER_TAG] = this->_datasetPublicReleaseNumber;
}

// -----------------------------------------------------------------------------
//  Adds a key/value pair to the User Meta Data section
// -----------------------------------------------------------------------------
void MXADataModel::addUserMetaData( MXAAttributePtr userMetaData)
{
  this->_userMetaData.push_back(userMetaData);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::addUserMetaData(const std::string &key, const std::string &value)
{
  MXAAttributePtr umd = MXAAttribute::createAttribute(key, value);
  addUserMetaData(umd);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAUserMetaData&  MXADataModel::getUserMetaData()
{
  return this->_userMetaData;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::printModel(std::ostream& os, int32 indent)
{
  os << StringUtils::indent(indent) << "--------- MXA DataModel---------" << std::endl;
  os << StringUtils::indent(indent) << "FileVersion: " << getModelVersion() << std::endl;
  os << StringUtils::indent(indent) << "FileType: " << getModelType() << std::endl;
  os << StringUtils::indent(indent) << "DataModel" << std::endl;
  indent++;
  os << StringUtils::indent(indent) << "DataRoot: " << getDataRoot() << std::endl;
  os << StringUtils::indent(indent) << "Data Dimensions [" << this->_dataDimensions.size() << "]" << std::endl;
  printDataDimensions(os, ++indent);
  indent--;
  os << StringUtils::indent(indent) << "Data Records" << std::endl;
  printDataRecords(os, indent + 1);
  printRequiredMetaData(os, indent);
  printUserMetaData(os, indent);
}

// -----------------------------------------------------------------------------
//  Prints the hierarchy of data records to the supplied ostream
// -----------------------------------------------------------------------------
void MXADataModel::printDataRecords(std::ostream& os, int32 indent)
{
  IDataRecord* rec = NULL;
  IDataRecordPtr ptr;
  for (IDataRecords::iterator iter = _dataRecords.begin(); iter != _dataRecords.end(); ++iter)
  {
    ptr = (*iter);
    rec = ptr.get();
    if ( rec == NULL)
    {
      std::cout << "MXADataRecord Pointer was NULL" << std::endl;
      continue;
    }
    rec->printDataRecord(os, indent);
  }
}

// -----------------------------------------------------------------------------
//  Prints the Data Dimensions
// -----------------------------------------------------------------------------
void MXADataModel::printDataDimensions(std::ostream& os, int32 indent)
{
  MXADataDimension* dim;
  for (IDataDimensions::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
  {
    dim = dynamic_cast<MXADataDimension*>( (*iter).get() );
    dim->printNode(os, indent);
    //(*(iter))->printNode(os, indent);
  }  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::printRequiredMetaData(std::ostream& os, int32 indent)
{
  std::string ind = StringUtils::indent(indent);
  os << ind << "Required Meta Data" << std::endl;
  ind = StringUtils::indent(indent + 1);
  os << ind << "Researcher Name: " << _researcherName << std::endl;
  os << ind << "Date Created: " << _datasetDateCreated << std::endl;
  os << ind << "Derived Source File: " << _derivedSourceFile << std::endl;
  os << ind << "Data Description: " << _datasetDescription << std::endl;
  os << ind << "Distribution Rights: " << _distributionRights << std::endl;
  os << ind << "Dataset Name: " << _datasetName << std::endl;
  os << ind << "Dataset Pedigree: " << _datasetPedigree << std::endl;
  os << ind << "Public Release Number: " << _datasetPublicReleaseNumber << std::endl;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::printUserMetaData(std::ostream& os, int32 indent)
{
  os << StringUtils::indent(indent) << "User Meta Data" << std::endl;
  MXAAttribute* attr;
  for (MXAUserMetaData::iterator iter = _userMetaData.begin(); iter != _userMetaData.end(); ++iter)
  {
    attr = (*iter).get();
    os << StringUtils::indent(indent + 1) << attr->getKey() << " ---> " << attr->valueToString() << std::endl;
  }
}


// -----------------------------------------------------------------------------
//  Reads the datamodel from the filename given using the defualt IODelegatePtr
// -----------------------------------------------------------------------------
MXATypes::MXAError MXADataModel::readModel(const std::string &fileName, bool closeWhenFinished)
{
  if (this->_ioDelegate.get() != NULL)
  {
    return this->_ioDelegate->readModelFromFile(fileName, this, closeWhenFinished);
  }
  return -1;
}

// -----------------------------------------------------------------------------
//  Reads the datamodel using a different IODelegatePtr than provided as a default
// -----------------------------------------------------------------------------
MXATypes::MXAError MXADataModel::readModel(const std::string &fileName, IODelegatePtr ioDelegate, bool closeWhenFinished)
{
  if (ioDelegate.get() != NULL)
  {
    return ioDelegate->readModelFromFile(fileName, this, closeWhenFinished);
  } 
  return -1;
}

// -----------------------------------------------------------------------------
//  Reads the datamodel from the filename given using the defualt IODelegatePtr
// -----------------------------------------------------------------------------
MXATypes::MXAError MXADataModel::writeModel(const std::string &fileName, bool closeWhenFinished)
{
  if (this->_ioDelegate.get() != NULL)
  {
    return this->_ioDelegate->writeModelToFile(fileName, this, closeWhenFinished);
  }
  return -1;
}

// -----------------------------------------------------------------------------
//  Reads the datamodel using a different IODelegatePtr than provided as a default
// -----------------------------------------------------------------------------
MXATypes::MXAError MXADataModel::writeModel(const std::string &fileName, IODelegatePtr ioDelegate, bool closeWhenFinished)
{
  if (ioDelegate.get() != NULL)
  {
    return ioDelegate->writeModelToFile(fileName, this, closeWhenFinished);
  } 
  return -1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setIODelegate(IODelegatePtr ioDelegate)
{
  this->_ioDelegate = ioDelegate;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IODelegatePtr MXADataModel::getIODelegate()
{
  return this->_ioDelegate;
}


// -----------------------------------------------------------------------------
//  Use a new set of indices to generate a path
// -----------------------------------------------------------------------------
std::string MXADataModel::generatePathToDataset ( std::vector<int32> &indices,  IDataRecord* record) 
{
  std::string path;
  //Put the data root on first
  path += this->getDataRoot();
  //Put the Data Dimensions on Next
  for (std::vector<int32>::iterator iter = indices.begin(); iter != indices.end(); ++iter ) {
    path += StringUtils::numToString(*iter);
    path += "/";
  }
   // Now build up the DataRecord path
  path = path + record->generatePath();
  return path;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXATypes::MXAError MXADataModel::validateRequiredMetaData(MXARequiredMetaData &requiredMetaData, std::string &message)
{
  MXATypes::MXAError err = 0;

  MXARequiredMetaData::iterator iter = requiredMetaData.find(MXA::MXA_CREATOR_TAG);
  if (requiredMetaData.end() == iter )
  {
    message.append("Required MetaData Value for Key: ").append(MXA::MXA_CREATOR_TAG).append(" Not Found.\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_DATE_TAG);
  if (requiredMetaData.end() == iter )
  {
    message.append("Required MetaData Value for Key: ").append(MXA::MXA_DATE_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_DERIVED_SRC_TAG);
  if (requiredMetaData.end() == iter )
  {
    message.append("Required MetaData Value for Key: ").append(MXA::MXA_DERIVED_SRC_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_DESCRIPTION_TAG);
  if (requiredMetaData.end() == iter )
  {
    message.append("Required MetaData Value for Key: ").append(MXA::MXA_DESCRIPTION_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_RIGHTS_TAG);
  if (requiredMetaData.end() == iter )
  {
    message.append("Required MetaData Value for Key: ").append(MXA::MXA_RIGHTS_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_DSET_NAME_TAG);
  if (requiredMetaData.end() == iter )
  {
    message.append("Required MetaData Value for Key: ").append(MXA::MXA_DSET_NAME_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_PEDIGREE_TAG);
  if (requiredMetaData.end() == iter )
  {
    message.append("Required MetaData Value for Key: ").append(MXA::MXA_PEDIGREE_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_RELEASE_NUMBER_TAG);
  if (requiredMetaData.end() == iter )
  {
    message.append("Required MetaData Value for Key: ").append(MXA::MXA_RELEASE_NUMBER_TAG).append(" Not Found.").append("\n");
    err = -1;
  }
  
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXADataModel::isValid(std::string &message)
{
  bool valid = true;
  if (this->_fileVersion < 0.0)
  {
    valid = false;
    message.append("File Version is Unset.\n");
  }
  
  if (this->_fileType.empty())
  {
    valid = false;
    message.append("File Type is not set.\n");
  }
  
  if ( this->_dataDimensions.size() == 0) 
  {
    valid = false;
    message.append("Model does not have any Data Dimensions. There should be at least one.\n");
  }
  
  if (this->_dataRecords.size() == 0)
  {
    valid = false;
    message.append("Model does not have any Data Records. There should be at least one.\n");
  }
  
  if (this->_dataRoot.empty())
  {
    message.append("The 'Data Root' property is empty..\n");
    valid = false;
  }
  
  for (IDataDimensions::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    MXADataDimension* dim = NULL;
    dim = static_cast<MXADataDimension*>((*(iter)).get());
    if ( dim->isValid(message) == false )
    {
      valid = false;
    }
  }
  
  if (_researcherName.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_researcherName} missing value.\n");
  }
  
  if (_datasetDateCreated.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_datasetDateCreated} missing value.\n");
  }
  
  if (_derivedSourceFile.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_derivedSourceFile} missing value.\n");
  }
  
  if (_datasetDescription.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_datasetDescription} missing value.\n");
  }
  
  if (_distributionRights.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_distributionRights} missing value.\n");
  }
  
  if (_datasetPedigree.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_datasetPedigree} missing value.\n");
  }
  
  if (_datasetPublicReleaseNumber.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_datasetPublicReleaseNumber} missing value.\n");
  }

  return valid;
}


