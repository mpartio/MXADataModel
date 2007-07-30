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
_fileVersion(0.0),
_fileType(""),
_dataRoot(""),
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
MXADataModelPtr MXADataModel::New()
{
  MXADataModelPtr model(new MXADataModel());
  return model;
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataModel::~MXADataModel() 
{
//	std::cout << "$-$-$-$-$ MXADataModel Destructor $-$-$-$-$" << std::endl;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setFileVersion(float version) 
{
  this->_fileVersion= version;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
float MXADataModel::getFileVersion() 
{
  return this->_fileVersion;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::setFileType(const std::string &fileType)
{
  this->_fileType = fileType;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataModel::getFileType()
{
  return MXA::MXACurrentFileType;
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
void MXADataModel::addDataDimension(MXADataDimensionPtr dimension, bool setIndex)
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
MXADataDimensionPtr MXADataModel::addDataDimension(std::string name, std::string altName,
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
//  Removes a Data Dimension from this list
// -----------------------------------------------------------------------------
int32 MXADataModel::removeDataDimension(MXADataDimension* &dimension)
{
  int32 success = -1;
  for (MXADataDimensions::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
  {
    if ( (*(iter)).get() == dimension ) 
    {
      _dataDimensions.erase( iter );
      dimension = NULL; // Set the incoming pointer to NULL because it will be invalid after this operation
      success = 1;
      break;
    }
  }
  return success;
}

// -----------------------------------------------------------------------------
//  Removes a Data Dimension from this list using the index into the vector
// -----------------------------------------------------------------------------
int32 MXADataModel::removeDataDimension(int32 index)
{
  int32 success = -1;
  int32 i =0;
  for (MXADataDimensions::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
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
  for (MXADataDimensions::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
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
MXADataDimensions& MXADataModel::getDataDimensions()
{
  return this->_dataDimensions;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataDimension* MXADataModel::getDataDimension(int32 index)
{
  if ( 0 <= index  &&  static_cast<uint32>(index) < this->_dataDimensions.size() )
  {
    MXANodePtr ptr = this->_dataDimensions[index];
    return static_cast<MXADataDimension*>(ptr.get());
  }
  return NULL;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataDimension* MXADataModel::getDataDimension(std::string dimName)
{
  MXADataDimension* currentDim = NULL;
  for (MXADataDimensions::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
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
void MXADataModel::addDataRecord(MXADataRecordPtr record)
{
  this->_dataRecords.push_back(record);
}

// -----------------------------------------------------------------------------
//  Simply adds the record as a child of the parent. By adding it to an existing
//   parent in the hierarchy, the record will become part of the hierarchy
// -----------------------------------------------------------------------------
void MXADataModel::addDataRecord(MXADataRecordPtr record, MXADataRecordPtr parent)
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
MXADataRecords& MXADataModel::getDataRecords()
{
  return this->_dataRecords;
}

// -----------------------------------------------------------------------------
//  Gets an MXADataRecord by the path given
// -----------------------------------------------------------------------------
MXADataRecordPtr MXADataModel::getDataRecordByPath(std::string path, MXADataRecord* parent)
{
 // std::cout << "Looking for Path: " << path << std::endl;
  // Remove any trailing slash in the path
  MXADataRecord* currentRec = NULL;
  boost::shared_ptr<MXADataRecord> rec;

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
  //  std::cout << "  Removal of front and trailing slashes caused string length to be 0" << std::endl;
    return rec; // The path that was passed in was only a slash.. 
  }
  

  std::string first;
  std::string second;
  MXADataRecords records;
  if ( NULL == parent) // No parent, so start at the top
  {
    records = this->_dataRecords;
  }
  else 
  {
     records = parent->getChildren();
  }
  for (MXADataRecords::iterator iter = records.begin(); iter != records.end(); ++iter)
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
      if (second.empty()) // Only return if we are ath the end of the path
      { 
        return boost::dynamic_pointer_cast<MXADataRecord>(*iter);
      } 
      else
      {
        rec = getDataRecordByPath(second, currentRec);
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
//  
// -----------------------------------------------------------------------------
void MXADataModel::createDataRecordLUT(std::map<int64, MXADataRecordPtr> &lut)
{
 
  std::cout << "WARNING: createDataRecordLUT() is NOT implemented yet" << std::endl;
  return;
     
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
  
  if (researcherName.empty()) return -1;
  _researcherName = researcherName;
  
  if (dateCreated.empty()) return -1;
  _datasetDateCreated = dateCreated;
  
  if (derivedSrcFile.empty()) return -1;
  _derivedSourceFile = derivedSrcFile;
  
  if (description.empty()) return -1;
  _datasetDescription = description;
  
  if (distributionRights.empty()) return -1;
  _distributionRights = distributionRights;
  
  if (datasetName.empty()) return -1;
  _datasetName = datasetName;
  
  if (pedigree.empty()) return -1;
  _datasetPedigree = pedigree;
  
  if (releaseNumber.empty()) return -1;
  _datasetPublicReleaseNumber = releaseNumber;
  return 1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXATypes::MXAError MXADataModel::setRequiredMetaData(std::map<std::string, std::string> &requiredMetaData)
{
  
  if ( MXADataModel::validateRequiredMetaData(requiredMetaData) )
  {
    return -1;
  }
  
  this->_researcherName = requiredMetaData[MXA::MXA_CREATOR_TAG];
  this->_datasetDateCreated = requiredMetaData[MXA::MXA_DATE_TAG];
  this->_derivedSourceFile = requiredMetaData[MXA::MXA_DERIVED_SRC_TAG];
  this->_datasetDescription = requiredMetaData[MXA::MXA_DESCRIPTION_TAG];
  this->_distributionRights = requiredMetaData[MXA::MXA_RIGHTS_TAG];
  this->_datasetName = requiredMetaData[MXA::MXA_DSET_NAME_TAG];
  this->_datasetPedigree = requiredMetaData[MXA::MXA_PEDIGREE_TAG];
  this->_datasetPublicReleaseNumber = requiredMetaData[MXA::MXA_RELEASE_TAG];
  
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
  requiredMetaData[MXA::MXA_RELEASE_TAG] = this->_datasetPublicReleaseNumber;
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
  os << _indent(indent) << "---------XMA DataModel---------" << std::endl;
  os << _indent(indent) << "FileVersion: " << getFileVersion() << std::endl;
  os << _indent(indent) << "FileType: " << getFileType() << std::endl;
  os << _indent(indent) << "DataModel" << std::endl;
  indent++;
  os << _indent(indent) << "DataRoot: " << getDataRoot() << std::endl;
  os << _indent(indent) << "Data Dimensions" << std::endl;
  printDataDimensions(os, ++indent);
  indent--;
  os << _indent(indent) << "Data Records" << std::endl;
  printDataRecords(os, indent + 1);
  printRequiredMetaData(os, indent);
  printUserMetaData(os, indent);
}

// -----------------------------------------------------------------------------
//  Prints the hierarchy of data records to the supplied ostream
// -----------------------------------------------------------------------------
void MXADataModel::printDataRecords(std::ostream& os, int32 indent)
{
  for (MXADataRecords::iterator iter = _dataRecords.begin(); iter != _dataRecords.end(); ++iter)
  {
    (*(iter))->printNode(os, indent);
  }
}

// -----------------------------------------------------------------------------
//  Prints the Data Dimensions
// -----------------------------------------------------------------------------
void MXADataModel::printDataDimensions(std::ostream& os, int32 indent)
{
  for (MXADataDimensions::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
  {
    (*(iter))->printNode(os, indent);
  }  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataModel::printRequiredMetaData(std::ostream& os, int32 indent)
{
  std::string ind = _indent(indent);
  os << ind << "Required Meta Data" << std::endl;
  ind = _indent(indent + 1);
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
  os << _indent(indent) << "User Meta Data" << std::endl;
  MXAAttribute* attr;
  for (MXAUserMetaData::iterator iter = _userMetaData.begin(); iter != _userMetaData.end(); ++iter)
  {
    attr = (*iter).get();
    os << _indent(indent + 1) << attr->getKey() << " ---> " << attr->valueToString() << std::endl;
  }
}


// -----------------------------------------------------------------------------
//  Creates an indentation string
// -----------------------------------------------------------------------------
std::string MXADataModel::_indent(int depth)
{
  const int32 indentSize = 2;
  return std::string(indentSize * depth, ' ');
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
//TODO: Better way to check for invalid ioDelegate
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
std::string MXADataModel::generatePathToDataset ( std::vector<int32> &indices,  MXADataRecord* record) 
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
MXATypes::MXAError MXADataModel::validateRequiredMetaData(MXARequiredMetaData &requiredMetaData)
{
  MXATypes::MXAError err = 0;
  std::string errorString("Required Meta Data Missing:\n");
  MXARequiredMetaData::iterator iter = requiredMetaData.find(MXA::MXA_CREATOR_TAG);
  if (requiredMetaData.end() == iter )
  {
    errorString.append("Required MetaData Value for Key: ").append(MXA::MXA_CREATOR_TAG).append(" Not Found.\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_DATE_TAG);
  if (requiredMetaData.end() == iter )
  {
    errorString.append("Required MetaData Value for Key: ").append(MXA::MXA_DATE_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_DERIVED_SRC_TAG);
  if (requiredMetaData.end() == iter )
  {
    errorString.append("Required MetaData Value for Key: ").append(MXA::MXA_DERIVED_SRC_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_DESCRIPTION_TAG);
  if (requiredMetaData.end() == iter )
  {
    errorString.append("Required MetaData Value for Key: ").append(MXA::MXA_DESCRIPTION_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_RIGHTS_TAG);
  if (requiredMetaData.end() == iter )
  {
    errorString.append("Required MetaData Value for Key: ").append(MXA::MXA_RIGHTS_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_DSET_NAME_TAG);
  if (requiredMetaData.end() == iter )
  {
    errorString.append("Required MetaData Value for Key: ").append(MXA::MXA_DSET_NAME_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_PEDIGREE_TAG);
  if (requiredMetaData.end() == iter )
  {
    errorString.append("Required MetaData Value for Key: ").append(MXA::MXA_PEDIGREE_TAG).append(" Not Found.").append("\n");
    err = -1;
  }

  iter = requiredMetaData.find(MXA::MXA_RELEASE_TAG);
  if (requiredMetaData.end() == iter )
  {
    errorString.append("Required MetaData Value for Key: ").append(MXA::MXA_RELEASE_TAG).append(" Not Found.").append("\n");
    err = -1;
  }
  if (err < 0)
  {
    std::cout << logTime() << errorString << std::endl;
  }
  
  return err;
}

