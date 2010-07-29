#include "H5MXARequiredMetaData.h"
#include <MXA/Common/LogTime.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/Utilities/StringUtils.h>
#if MXA_HDF5_SUPPORT
#include <MXA/HDF5/H5Lite.h>
#endif
//-- C++ includes
#include <string>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IRequiredMetaData::Pointer H5MXARequiredMetaData::New( std::string researcherName,
                                                        std::string dateCreated,
                                                        std::string datasetName,
                                                        std::string description,
                                                        std::string distributionRights,
                                                        std::string releaseNumber,
                                                        std::string pedigree,
                                                        std::string derivedSrcFile)
{
  H5MXARequiredMetaData* md = new H5MXARequiredMetaData( researcherName,
                                                         dateCreated,
                                                         datasetName,
                                                         description,
                                                         distributionRights,
                                                         releaseNumber,
                                                         pedigree,
                                                         derivedSrcFile);
  IRequiredMetaData::Pointer ptr(md);
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IRequiredMetaData::Pointer H5MXARequiredMetaData::DefaultMetaData()
{
  H5MXARequiredMetaData* md = new H5MXARequiredMetaData( "researcherName",
                                                         "2007/12/23 16:00:00",
                                                         "datasetName",
                                                         "description",
                                                         "Unlimited",
                                                         "releaseNumber",
                                                         "Derived",
                                                         "derivedSrcFile");
  IRequiredMetaData::Pointer ptr(md);
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5MXARequiredMetaData::H5MXARequiredMetaData( std::string researcherName,
                                              std::string dateCreated,
                                              std::string datasetName,
                                              std::string description,
                                              std::string distributionRights,
                                              std::string releaseNumber,
                                              std::string pedigree,
                                              std::string derivedSrcFile) :
_researcherName( researcherName),
_dateCreated(dateCreated ),
_datasetName(datasetName ),
_description( description),
_distributionRights( distributionRights),
_releaseNumber( releaseNumber),
_pedigree( pedigree),
_derivedSrcFile( derivedSrcFile)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5MXARequiredMetaData::~H5MXARequiredMetaData()
{

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setAll( std::string researcherName,
                                              std::string dateCreated,
                                              std::string datasetName,
                                              std::string description,
                                              std::string distributionRights,
                                              std::string releaseNumber,
                                              std::string pedigree,
                                              std::string derivedSrcFile)
{
  this->_researcherName = researcherName;
  this->_dateCreated = dateCreated ;
  this->_datasetName = datasetName ;
  this->_description = description;
  this->_distributionRights = distributionRights;
  this->_releaseNumber = releaseNumber;
  this->_pedigree = pedigree;
  this->_derivedSrcFile = derivedSrcFile;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setResearcherName(const std::string &researcherName)
{
  this->_researcherName = researcherName;
}
std::string H5MXARequiredMetaData::getResearcherName()
{
  return this->_researcherName;
  }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setDateTimeCreated(const std::string &dateTime)
{
  this->_dateCreated = dateTime;
}
std::string H5MXARequiredMetaData::getDateTimeCreated()
{
  return this->_dateCreated;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setDatasetName(const std::string &datasetName)
{
  this->_datasetName = datasetName;
}
std::string H5MXARequiredMetaData::getDatasetName()
{
  return this->_datasetName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setDescription(const std::string &description)
{
  this->_description = description;
}
std::string H5MXARequiredMetaData::getDescription()
{
  return this->_description;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setDistributionRights(const std::string &distributionRights)
{
  this->_distributionRights = distributionRights;
}
std::string H5MXARequiredMetaData::getDistributionRights()
{
  return this->_distributionRights;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setReleaseNumber(const std::string &releaseNumber)
{
  this->_releaseNumber = releaseNumber;

}
std::string H5MXARequiredMetaData::getReleaseNumber()
{
  return this->_releaseNumber;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setPedigree(const std::string &pedigree)
{
  this->_pedigree = pedigree;
}
std::string H5MXARequiredMetaData::getPedigree()
{
  return this->_pedigree;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::setDerivedSourceFile(const std::string &derivedSourceFile)
{
  this->_derivedSrcFile = derivedSourceFile;
}
std::string H5MXARequiredMetaData::getDerivedSourceFile()
{
  return this->_derivedSrcFile;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t H5MXARequiredMetaData::setValueForKey(const std::string &key, const std::string &value)
{
  if (key.compare(MXA::MXA_CREATOR_TAG) == 0 )
    {
      this->_researcherName = value;
      return 1;
    }
  if (key.compare(MXA::MXA_DATE_TAG) == 0 )
    {
      this->_dateCreated = value;
      return 1;
    }
  if (key.compare(MXA::MXA_DSET_NAME_TAG) == 0 )
    {
      this->_datasetName = value;
      return 1;
    }
  if (key.compare(MXA::MXA_DESCRIPTION_TAG) == 0 )
    {
      this->_description = value;
      return 1;
    }
  if (key.compare(MXA::MXA_RELEASE_NUMBER_TAG) == 0 )
    {
      this->_releaseNumber = value;
      return 1;
    }
  if (key.compare(MXA::MXA_PEDIGREE_TAG) == 0 )
    {
      this->_pedigree = value;
      return 1;
    }
  if (key.compare(MXA::MXA_DERIVED_SRC_TAG) == 0 )
    {
      this->_derivedSrcFile = value;
      return 1;
    }
  std::cout << logTime() << "A key value of '" << key << "' does not match any of the known keys. The Meta Data value was not set." << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string H5MXARequiredMetaData::getValueForKey(const std::string &key)
{
  if (key.compare(MXA::MXA_CREATOR_TAG) == 0)
  {
    return this->_researcherName;
  }
  if (key.compare(MXA::MXA_DATE_TAG) == 0)
  {
    return this->_dateCreated;
  }
  if (key.compare(MXA::MXA_DSET_NAME_TAG) == 0)
  {
    return this->_datasetName;
  }
  if (key.compare(MXA::MXA_DESCRIPTION_TAG) == 0)
  {
    return this->_description;
  }
  if (key.compare(MXA::MXA_RELEASE_NUMBER_TAG) == 0)
  {
    return this->_releaseNumber;
  }
  if (key.compare(MXA::MXA_PEDIGREE_TAG) == 0)
  {
    return this->_pedigree;
  }
  if (key.compare(MXA::MXA_DERIVED_SRC_TAG) == 0)
  {
    return this->_derivedSrcFile;
  }
#if DEBUG
  std::cout << logTime() << "A key value of '" << key << "' does not match any of the known keys. An empty Meta Data string is being returned." << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
#endif
  return std::string();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t H5MXARequiredMetaData::writeToFile(IDataFile::Pointer dataFile)
{
  herr_t err = -1;
  int32_t data = 0;
  hid_t fileId = dataFile->getFileId();
  //err = this->_writeScalarDataset(fileId, const_cast<std::string&>(MXA::RequiredMetaDataPath), data);
  err = H5Lite::writeScalarDataset(fileId,const_cast<std::string&>(MXA::RequiredMetaDataPath), data );

  if (err < 0)
  {
    std::cout << logTime() << "Error Creating Dataset for RequiredMetaData." << err << std::endl;
    return err;
  }
  std::map<std::string, std::string> meta;
  this->generateKeyValueMap(meta);

  for (std::map<std::string, std::string>::iterator iter=meta.begin(); iter!=meta.end(); iter++) {
    err = H5Lite::writeStringAttribute(fileId, const_cast<std::string&>(MXA::RequiredMetaDataPath), const_cast<std::string&>( iter->first), iter->second);
    if(err<0) {std::cout << "Error Writing Required MetaData: " << iter->first << "=" << iter->second << " at path " << MXA::RequiredMetaDataPath << std::endl; break;}
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t H5MXARequiredMetaData::readFromFile(IDataFile::Pointer dataFile)
{

  hid_t fileId = dataFile->getFileId();
  if (fileId < 0 )
  {
    std::cout << logTime() << "Invalid HDF FileId: " << fileId << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
    return -1;
  }
  herr_t err = -1;

  err = H5Lite::readStringAttribute(fileId, MXA::RequiredMetaDataPath, MXA::MXA_CREATOR_TAG, _researcherName);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }

  err = H5Lite::readStringAttribute(fileId, MXA::RequiredMetaDataPath, MXA::MXA_DATE_TAG, _dateCreated);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(fileId, MXA::RequiredMetaDataPath, MXA::MXA_DSET_NAME_TAG, _datasetName);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(fileId, MXA::RequiredMetaDataPath, MXA::MXA_DESCRIPTION_TAG, _description);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(fileId, MXA::RequiredMetaDataPath, MXA::MXA_PEDIGREE_TAG, _pedigree);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(fileId, MXA::RequiredMetaDataPath, MXA::MXA_DERIVED_SRC_TAG, _derivedSrcFile);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(fileId, MXA::RequiredMetaDataPath, MXA::MXA_RIGHTS_TAG, _distributionRights);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }
  err = H5Lite::readStringAttribute(fileId, MXA::RequiredMetaDataPath, MXA::MXA_RELEASE_NUMBER_TAG, _releaseNumber);
  if (err < 0) {
    std::cout << "Error reading data record name value" << std::endl;
    return -1;
  }

  return 1;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool H5MXARequiredMetaData::isValid(std::string &message)
{
  bool valid = true;
  if (_researcherName.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_researcherName} missing value.\n");
  }

  if (_dateCreated.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_datasetDateCreated} missing value.\n");
  }

  if (_derivedSrcFile.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_derivedSourceFile} missing value.\n");
  }

  if (_description.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_datasetDescription} missing value.\n");
  }

  if (_distributionRights.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_distributionRights} missing value.\n");
  }

  if (_pedigree.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_datasetPedigree} missing value.\n");
  }

  if (_releaseNumber.empty() )
  {
    valid = false;
    message.append("Required Meta Data {_datasetPublicReleaseNumber} missing value.\n");
  }

  return valid;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::generateKeyValueMap(std::map<std::string, std::string> &requiredMetaData)
{
  requiredMetaData[MXA::MXA_CREATOR_TAG] = _researcherName;
  requiredMetaData[MXA::MXA_DATE_TAG] = _dateCreated;
  requiredMetaData[MXA::MXA_DERIVED_SRC_TAG] = _derivedSrcFile;
  requiredMetaData[MXA::MXA_DESCRIPTION_TAG] = _description;
  requiredMetaData[MXA::MXA_RIGHTS_TAG] = _distributionRights;
  requiredMetaData[MXA::MXA_DSET_NAME_TAG] = _datasetName;
  requiredMetaData[MXA::MXA_PEDIGREE_TAG] = _pedigree;
  requiredMetaData[MXA::MXA_RELEASE_NUMBER_TAG] = _releaseNumber;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXARequiredMetaData::printSelf(std::ostream& os, int32_t indent)
{
  std::string ind = StringUtils::indent(indent);
  os << ind << "Required Meta Data" << std::endl;
  ind = StringUtils::indent(indent + 1);
  os << ind << MXA::MXA_CREATOR_TAG <<":" << _researcherName << std::endl;
  os << ind << MXA::MXA_DATE_TAG  <<":" << _dateCreated << std::endl;
  os << ind << MXA::MXA_DERIVED_SRC_TAG  <<":" << _derivedSrcFile << std::endl;
  os << ind << MXA::MXA_DESCRIPTION_TAG  <<":" << _description << std::endl;
  os << ind << MXA::MXA_RIGHTS_TAG  <<":" << _distributionRights << std::endl;
  os << ind << MXA::MXA_DSET_NAME_TAG  <<":" << _datasetName << std::endl;
  os << ind << MXA::MXA_PEDIGREE_TAG  <<":" << _pedigree << std::endl;
  os << ind << MXA::MXA_RELEASE_NUMBER_TAG  <<":" << _releaseNumber << std::endl;
}



