///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
//-- MXA Headers
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/Core/RequiredMetaData.h>
#include <MXA/HDF5/H5MXARequiredMetaData.h>


//-------------------------------------------------
// Constructor
//-------------------------------------------------
MXADataModel::MXADataModel() :
_fileVersion(-1.0),
_fileType(""),
_dataRoot("/")
{
  #if MXA_HDF5_SUPPORT
 // _ioDelegate.reset(new H5IODelegate());
  #else
  #warning MXA_HDF5_SUPPORT is OFF. There is NO default IODelegatePtr for MXADataModel
  #endif
}

// -----------------------------------------------------------------------------
//  Static New Method to construct a Model
// -----------------------------------------------------------------------------
MXADataModel::Pointer MXADataModel::New(float modelVersion, const std::string &type, const std::string &dataRoot)
{
  MXADataModel::Pointer model(new MXADataModel());
  model->setModelVersion(modelVersion);
  model->setModelType(type);
  model->setDataRoot(dataRoot);
  return model;
}

// -----------------------------------------------------------------------------
//  Static New Method to construct a deep copy from another model
// -----------------------------------------------------------------------------
MXADataModel::Pointer New(IDataModel::Pointer model)
{
  MXADataModel::Pointer cpModel = MXADataModel::New(model->getModelVersion(),
                                              model->getModelType(),
                                              model->getDataRoot() );

  //Copy the Data Dimensions
  int nDims = model->getNumberOfDataDimensions();
  IDataDimension::Pointer dim;
  for(int i = 0; i < nDims; ++i)
  {
    dim = model->getDataDimension(i);
    IDataDimension::Pointer copy = MXADataDimension::New(dim);
    cpModel->addDataDimension(copy);
  }
  // Copy the Required Meta Data
  IRequiredMetaData::Pointer rm = model->getRequiredMetaData();
  IRequiredMetaData::Pointer cpReqMD = RequiredMetaData::New(rm);
  cpModel->setRequiredMetaData(cpReqMD);

  //Copy the User Defined Meta Data

  //Copy any support files

  //Recursively copy the Data Records


  return cpModel;
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
  this->_dataRoot = dataRoot;
  if (false == StringUtils::hasRightSlash(this->_dataRoot))
  {
    this->_dataRoot += "/";
  }
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
void MXADataModel::addDataDimension(IDataDimension::Pointer dimension)
{
  this->_dataDimensions.push_back(dimension);
  int32_t index = static_cast<int32_t>(this->_dataDimensions.size()) - 1;
  if (index < 0) { index = 0;}
  dimension->setIndex(index);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataDimension::Pointer MXADataModel::addDataDimension(std::string name, std::string altName,
                                int32_t count, int32_t startValue,
                                int32_t endValue, int32_t increment, int32_t uniform)
{
  int32_t index = -1;
  MXADataDimension::Pointer dim =
    MXADataDimension::New(name, altName, index, count, startValue, endValue, increment, uniform);

  if (endValue == -1) {
    dim->setEndValue (startValue + (increment * (count - 1) ) );
  }
  dim->calculateCount();
  this->addDataDimension(dim); // The index will be set correctly in this method
  return dim;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXADataModel::insertDataDimension(IDataDimension::Pointer dimension, int32_t index)
{
  if(index < 0 ) { return -1; } // Trying to add a negative index.
  if (static_cast<IDataDimension::Container::size_type>(index) >= this->_dataDimensions.size() )
  {
    this->_dataDimensions.resize( index ); // This may increase the size of the vector with Null wrapped shared pointers
  }
  IDataDimension::Container::iterator dIter = this->_dataDimensions.begin();
  this->_dataDimensions.insert(dIter + index, dimension);
  //Update the index value for each dimension
  index = 0;
  IDataDimension* dim;
  for (IDataDimension::Container::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    dim = (*iter).get();
    if (NULL == dim) { continue; }
    dim->setIndex(index);
    ++index;
  }
  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataModel::squeezeDataDimensions()
{
  for (IDataDimension::Container::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    if ( (*iter).get() == NULL)
    {
      this->_dataDimensions.erase(iter);
      iter = this->_dataDimensions.begin();
    }
  }
  int32_t index = 0;
  IDataDimension* dim;
  for (IDataDimension::Container::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    dim = (*iter).get();
    if (NULL == dim) { continue; }
    dim->setIndex(index);
    ++index;
  }
}


// -----------------------------------------------------------------------------
//  Removes a Data Dimension from this list using the index into the vector
// -----------------------------------------------------------------------------
int32_t MXADataModel::removeDataDimension(int32_t index)
{
  int32_t success = -1;
  int32_t i = 0;
  for (IDataDimension::Container::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
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
int32_t MXADataModel::removeDataDimension(const std::string &dimensionName)
{
  int32_t success = -1;
  int32_t i =0;
  MXADataDimension* dim = NULL;
  for (IDataDimension::Container::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
  {
    dim = static_cast<MXADataDimension*>( (*(iter)).get() );
    if (NULL == dim) { continue; }
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
int32_t MXADataModel::moveDataDimension(int32_t fromIndex, int32_t toIndex)
{
  // Validate both indices
  if (fromIndex < 0 || static_cast<IDataDimension::Container::size_type>(fromIndex) >= this->_dataDimensions.size() ||
      toIndex < 0 || static_cast<IDataDimension::Container::size_type>(toIndex) >= this->_dataDimensions.size() )
   {
     std::cout << "Tried to move data dimension from index " << fromIndex <<
     " to index " << toIndex << ". The valid range is 0 to " << this->_dataDimensions.size() - 1 << std::endl;
     return -1;
   }
  if (fromIndex == toIndex) { return 1; } // Moving index to itself.

  IDataDimension::Pointer src = this->_dataDimensions[fromIndex];
  this->_dataDimensions.erase(this->_dataDimensions.begin() + fromIndex);
  this->_dataDimensions.insert(this->_dataDimensions.begin() + toIndex, src);

  //Update the index value for each dimension
  int32_t index = 0;
  IDataDimension* dim;
  for (IDataDimension::Container::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    dim = (*iter).get();
    if (NULL == dim) { continue; }
    dim->setIndex(index);
    ++index;
  }

  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXADataModel::swapDataDimensions(int32_t index1, int32_t index2)
{
  if (index1 < 0 || static_cast<IDataDimension::Container::size_type>(index1) >= this->_dataDimensions.size() ||
      index2 < 0 || static_cast<IDataDimension::Container::size_type>(index2) >= this->_dataDimensions.size() )
   {
     std::cout << "Tried to swap data dimensions " << index1 <<
     " and " << index2 << ". The valid range is 0 to " << this->_dataDimensions.size() - 1 << std::endl;
     return -1;
   }

  if (index1 == index2)
  {
    return 1;
  }

  IDataDimension::Pointer src = this->_dataDimensions[index1];
  IDataDimension::Pointer dest = this->_dataDimensions[index2];

  this->_dataDimensions[index1] = dest;
  this->_dataDimensions[index2] = src;

  //Update the index value for each dimension
  int32_t index = 0;
  IDataDimension* dim;
  for (IDataDimension::Container::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    dim = (*iter).get();
    if (NULL == dim) { continue; }
    dim->setIndex(index);
    ++index;
  }

  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataDimension::Container& MXADataModel::getDataDimensions()
{
  return this->_dataDimensions;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataDimension::Pointer MXADataModel::getDataDimension(int32_t index)
{
  if ( 0 <= index  &&  static_cast<uint32_t>(index) < this->_dataDimensions.size() )
  {
    IDataDimension::Pointer ptr = this->_dataDimensions[index];
    return ptr;
  }
  IDataDimension::Pointer nullDim;
  return nullDim;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataDimension::Pointer MXADataModel::getDataDimension(std::string dimName)
{
  MXADataDimension* currentDim = NULL;
  for (IDataDimension::Container::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    currentDim = static_cast<MXADataDimension*>((*(iter)).get()); // Cast down to the MXADataRecord Pointer
    if (NULL == currentDim) { continue; }
    if (   currentDim->getDimensionName() == dimName  ) {
     // return currentDim;
     return *(iter);
    }
  }
  IDataDimension::Pointer nullDim;
  return nullDim;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXADataModel::getNumberOfDataDimensions()
{
  return static_cast<int32_t>(this->_dataDimensions.size());
}

// -----------------------------------------------------------------------------
//  Adds a Data Record to the top level of the DataRecords Group
// -----------------------------------------------------------------------------
void MXADataModel::addDataRecord(IDataRecord::Pointer record)
{
  this->_dataRecords.push_back(record);
}

// -----------------------------------------------------------------------------
//  Simply adds the record as a child of the parent. By adding it to an existing
//   parent in the hierarchy, the record will become part of the hierarchy
// -----------------------------------------------------------------------------
void MXADataModel::addDataRecord(IDataRecord::Pointer record, IDataRecord::Pointer parent)
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
int32_t MXADataModel::removeDataRecord(IDataRecord::Pointer record)
{
  int32_t err = -1;
  IDataRecord::Pointer parentPtr = record.get()->getParent().lock();
  if (NULL != parentPtr.get() ) // The record has a valid parent
  {
    parentPtr->removeChild( record.get() );
    err = 1;
  }

  if (NULL == parentPtr.get() || -1 == parentPtr->getGuid() )
// The record does NOT have a valid parent and is a top level Data Record
  {
    for (IDataRecord::Container::iterator iter = this->_dataRecords.begin(); iter != this->_dataRecords.end(); ++iter)
    {
      if ( (*(iter)).get() == record.get() )
      {
        this->_dataRecords.erase(iter);
        err = 1;
        break;
      }
    }
  }
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataRecord::Container& MXADataModel::getDataRecords()
{
  return this->_dataRecords;
}

// -----------------------------------------------------------------------------
//  Gets an MXADataRecord by the path given
// -----------------------------------------------------------------------------
IDataRecord::Pointer MXADataModel::getDataRecordByNamedPath(const std::string &path, IDataRecord* parentPtr)
{
 // std::cout << "Looking for Path: " << path << std::endl;
  // Remove any trailing slash in the path
  MXADataRecord* currentRec = NULL;
  IDataRecord::Pointer rec;
  IDataRecord* parent = static_cast<IDataRecord*>(parentPtr);
  std::string mPath (path);
  // remove any front slash
  std::string::size_type pos = mPath.find_first_of("/", 0);
  if ( 0 == pos )
  {
    mPath = mPath.substr(1, mPath.size());
  }

  //Remove any trailing slash
  pos = mPath.find_last_of("/");
  if ( pos == (mPath.size() - 1) ) // slash was in the last position
  {
    mPath = mPath.substr(0, pos);
  }

  if (mPath.size() == 0)
  {
    return rec; // The path that was passed in was only a slash..
  }


  std::string first;
  std::string second;
  IDataRecord::Container records;
  if ( NULL == parent) // No parent, so start at the top
  {
    records = this->_dataRecords;
  }
  else
  {
     records = parent->getChildren();
  }

  for (IDataRecord::Container::iterator iter = records.begin(); iter != records.end(); ++iter)
  {
    currentRec = static_cast<MXADataRecord*>((*(iter)).get()); // Cast down to the MXADataRecord Pointer
    std::string recName = currentRec->getRecordName();
    pos = mPath.find_first_of("/", 0);
    if (pos == std::string::npos) // No slash found
    {
      first = mPath;
      second = "";
    }
    else
    {
      first = mPath.substr(0, pos);
      second = mPath.substr(pos, mPath.size());
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
IDataRecord::Pointer MXADataModel::getDataRecordByInternalPath(const std::string &path, IDataRecord* parent)
{
 // std::cout << "Looking for Path: " << path << std::endl;
  // Remove any trailing slash in the path
  IDataRecord* currentRec = NULL;
  IDataRecord::Pointer rec;
  std::string mPath(path);
  // remove any front slash
  std::string::size_type pos = mPath.find_first_of("/", 0);
  if ( 0 == pos )
  {
    mPath = mPath.substr(1, mPath.size());
  }

  //Remove any trailing slash
  pos = mPath.find_last_of("/");
  if ( pos == (mPath.size() - 1) ) // slash was in the last position
  {
    mPath = mPath.substr(0, pos);
  }

  if (mPath.size() == 0)
  {
    return rec; // The path that was passed in was only a slash..
  }


  std::string first;
  std::string second;
  IDataRecord::Container records;
  if ( NULL == parent) // No parent, so start at the top
  {
    records = this->_dataRecords;
  }
  else
  {
     records = parent->getChildren();
  }

  for (IDataRecord::Container::iterator iter = records.begin(); iter != records.end(); ++iter)
  {
    currentRec = static_cast<MXADataRecord*>((*(iter)).get()); // Cast down to the MXADataRecord Pointer
    std::string recName = StringUtils::numToString( currentRec->getLuid() );
    pos = mPath.find_first_of("/", 0);
    if (pos == std::string::npos) // No slash found
    {
      first = mPath;
      second = "";
    }
    else
    {
      first = mPath.substr(0, pos);
      second = mPath.substr(pos, mPath.size());
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
int32_t MXADataModel::setRequiredMetaData(std::string researcherName,
              std::string dateCreated,
				      std::string datasetName,
              std::string description,
				      std::string distributionRights,
				      std::string releaseNumber,
				      std::string pedigree,
              std::string derivedSrcFile)
{
#if MXA_HDF5_SUPPORT
  _requiredMetaData = H5MXARequiredMetaData::New( researcherName,
                                                  dateCreated,
                                                  datasetName,
                                                  description,
                                                  distributionRights,
                                                  releaseNumber,
                                                  pedigree,
                                                  derivedSrcFile);
#endif
  std::string message;
  if ( _requiredMetaData->isValid(message) == true) { return 1; }
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXADataModel::setRequiredMetaData(std::map<std::string, std::string> &requiredMetaData)
{
#if MXA_HDF5_SUPPORT
  _requiredMetaData = H5MXARequiredMetaData::New( requiredMetaData[MXA::MXA_CREATOR_TAG],
                                                  requiredMetaData[MXA::MXA_DATE_TAG],
                                                  requiredMetaData[MXA::MXA_DSET_NAME_TAG],
                                                  requiredMetaData[MXA::MXA_DESCRIPTION_TAG],
                                                  requiredMetaData[MXA::MXA_RIGHTS_TAG],
                                                  requiredMetaData[MXA::MXA_RELEASE_NUMBER_TAG],
                                                  requiredMetaData[MXA::MXA_PEDIGREE_TAG],
                                                  requiredMetaData[MXA::MXA_DERIVED_SRC_TAG]);
#else
 _requiredMetaData = RequiredMetaData::New( requiredMetaData[MXA::MXA_CREATOR_TAG],
                                            requiredMetaData[MXA::MXA_DATE_TAG],
                                            requiredMetaData[MXA::MXA_DSET_NAME_TAG],
                                            requiredMetaData[MXA::MXA_DESCRIPTION_TAG],
                                            requiredMetaData[MXA::MXA_RIGHTS_TAG],
                                            requiredMetaData[MXA::MXA_RELEASE_NUMBER_TAG],
                                            requiredMetaData[MXA::MXA_PEDIGREE_TAG],
                                            requiredMetaData[MXA::MXA_DERIVED_SRC_TAG]);
  #endif
  std::string message;
  if ( _requiredMetaData->isValid(message) == true) { return 1; }
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXADataModel::setRequiredMetaData(IRequiredMetaData::Pointer metaData)
{
  this->_requiredMetaData = metaData;
  std::string message;
  if ( _requiredMetaData->isValid(message) == true) { return 1; }
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IRequiredMetaData::Pointer MXADataModel::getRequiredMetaData()
{
  return this->_requiredMetaData;
}

// -----------------------------------------------------------------------------
//  Adds a key/value pair to the User Meta Data section
// -----------------------------------------------------------------------------
void MXADataModel::addUserMetaData(const std::string &attributeKey, IMXAArray::Pointer userMetaData)
{

  MXAAbstractAttributes::iterator iter = this->_userMetaData.find(attributeKey);
  if ( iter != this->_userMetaData.end() )
  {
    //Key was already in the map of attributes
    IMXAArray::Pointer ptr = (*iter).second;
    if (ptr.get() != userMetaData.get()  && (NULL != userMetaData.get() ))
    {
      //Attribute was NOT the same one
      (*iter).second = userMetaData;
    }
  }
  else // Key was not found in the attributes map so add it
  {
    this->_userMetaData[attributeKey] = userMetaData;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataModel::removeUserMetaData(const std::string &attributeKey)
{
  MXAAbstractAttributes::iterator iter = this->_userMetaData.find(attributeKey);
  if ( iter != this->_userMetaData.end() )
  {
    this->_userMetaData.erase(attributeKey);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArray::Pointer MXADataModel::getUserMetaData(const std::string &attributeKey)
{
  MXAAbstractAttributes::iterator iter = this->_userMetaData.find(attributeKey);
  if ( iter != this->_userMetaData.end() )
  {
    //Key was already in the map of attributes
    IMXAArray::Pointer ptr = (*iter).second;
    return ptr;
  }
  IMXAArray::Pointer ptr;
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataModel::setUserMetaData( MXAAbstractAttributes &attributes)
{
  this->_userMetaData = attributes;
}


// -----------------------------------------------------------------------------
//
MXAAbstractAttributes  MXADataModel::getUserMetaData()
// -----------------------------------------------------------------------------
{
  return this->_userMetaData;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataModel::printModel(std::ostream& os, int32_t indent)
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
void MXADataModel::printDataRecords(std::ostream& os, int32_t indent)
{
  IDataRecord* rec = NULL;
  IDataRecord::Pointer ptr;
  for (IDataRecord::Container::iterator iter = _dataRecords.begin(); iter != _dataRecords.end(); ++iter)
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
void MXADataModel::printDataDimensions(std::ostream& os, int32_t indent)
{
  MXADataDimension* dim;
  for (IDataDimension::Container::iterator iter = _dataDimensions.begin(); iter != _dataDimensions.end(); ++iter)
  {
    dim = dynamic_cast<MXADataDimension*>( (*iter).get() );
    if (NULL == dim) { continue; }
    dim->printNode(os, indent);
    //(*(iter))->printNode(os, indent);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataModel::printRequiredMetaData(std::ostream& os, int32_t indent)
{
  if (_requiredMetaData.get() != NULL) {
    _requiredMetaData->printSelf(os, indent);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataModel::printUserMetaData(std::ostream& os, int32_t indent)
{
  os << StringUtils::indent(indent) << "User Meta Data" << std::endl;
  IMXAArray* attr;
  for (MXAAbstractAttributes::iterator iter = _userMetaData.begin(); iter != _userMetaData.end(); ++iter)
  {
    attr = (*iter).second.get();
    attr->printSelf(os, indent);
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXADataModel::validateRequiredMetaData(MXARequiredMetaData &requiredMetaData, std::string &message)
{
  int32_t err = 0;

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

  MXADataDimension* dim = NULL;
  for (IDataDimension::Container::iterator iter = this->_dataDimensions.begin(); iter != this->_dataDimensions.end(); ++iter ) {
    dim = static_cast<MXADataDimension*>((*(iter)).get());
    if (NULL == dim)
    {
      valid = false;
      message.append("MXA Data Dimension is NULL. All MXA Data Dimension objects must be valid.\n");
      break;
    }
    if ( dim->isValid(message) == false )
    {
      valid = false;
    }
  }
  if (this->_requiredMetaData.get() == NULL)
  {
    valid = false;
    message.append("Required Meta Data Object Container is NULL.\n");
  }
  else
  {
    if (_requiredMetaData->isValid(message) == false)
    {
      valid = false;
    }
  }
  return valid;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataModel::addSupportFile(ISupportFile::Pointer supportFile, bool updateIndex)
{
  this->_supportFiles.push_back(supportFile);
  if (true == updateIndex) {
    supportFile->setIndex( static_cast<int32_t>(this->_supportFiles.size()) - 1);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFile::Container MXADataModel::getSupportFiles()
{
  return this->_supportFiles;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFile::Pointer MXADataModel::getSupportFile(int index)
{
  if (index >=0 && (unsigned int)(index) < this->_supportFiles.size() )
  return this->_supportFiles[index];

  // Return a NULL pointer if we are out of bounds
  ISupportFile::Pointer sfile;
  return sfile;
}
