#include "RequiredMetaData.h"
#include <MXA/Common/LogTime.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/Utilities/StringUtils.h>

//-- C++ includes
#include <string>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IRequiredMetaData::Pointer RequiredMetaData::New( std::string researcherName,
                                                        std::string dateCreated,
                                                        std::string datasetName,
                                                        std::string description,
                                                        std::string distributionRights,
                                                        std::string releaseNumber,
                                                        std::string pedigree,
                                                        std::string derivedSrcFile)
{
  RequiredMetaData* md = new RequiredMetaData( researcherName,
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
IRequiredMetaData::Pointer RequiredMetaData::New(IRequiredMetaData::Pointer md)
{
 IRequiredMetaData::Pointer ptr = RequiredMetaData::New(md->getResearcherName(),
                                                         md->getDateTimeCreated(),
                                                         md->getDatasetName(),
                                                         md->getDescription(),
                                                         md->getDistributionRights(),
                                                         md->getReleaseNumber(), 
                                                         md->getPedigree(),
                                                         md->getDerivedSourceFile());
  
  return ptr;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IRequiredMetaData::Pointer RequiredMetaData::DefaultMetaData()
{
  RequiredMetaData* md = new RequiredMetaData( "researcherName",
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
RequiredMetaData::RequiredMetaData( std::string researcherName,
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
RequiredMetaData::~RequiredMetaData()
{

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RequiredMetaData::setAll( std::string researcherName,
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
void RequiredMetaData::setResearcherName(const std::string &researcherName)
{
  this->_researcherName = researcherName;
}
std::string RequiredMetaData::getResearcherName()
{
  return this->_researcherName;
  }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RequiredMetaData::setDateTimeCreated(const std::string &dateTime)
{
  this->_dateCreated = dateTime;
}
std::string RequiredMetaData::getDateTimeCreated()
{
  return this->_dateCreated;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RequiredMetaData::setDatasetName(const std::string &datasetName)
{
  this->_datasetName = datasetName;
}
std::string RequiredMetaData::getDatasetName()
{
  return this->_datasetName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RequiredMetaData::setDescription(const std::string &description)
{
  this->_description = description;
}
std::string RequiredMetaData::getDescription()
{
  return this->_description;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RequiredMetaData::setDistributionRights(const std::string &distributionRights)
{
  this->_distributionRights = distributionRights;
}
std::string RequiredMetaData::getDistributionRights()
{
  return this->_distributionRights;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RequiredMetaData::setReleaseNumber(const std::string &releaseNumber)
{
  this->_releaseNumber = releaseNumber;

}
std::string RequiredMetaData::getReleaseNumber()
{
  return this->_releaseNumber;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RequiredMetaData::setPedigree(const std::string &pedigree)
{
  this->_pedigree = pedigree;
}
std::string RequiredMetaData::getPedigree()
{
  return this->_pedigree;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RequiredMetaData::setDerivedSourceFile(const std::string &derivedSourceFile)
{
  this->_derivedSrcFile = derivedSourceFile;
}
std::string RequiredMetaData::getDerivedSourceFile()
{
  return this->_derivedSrcFile;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 RequiredMetaData::setValueForKey(const std::string &key, const std::string &value)
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
std::string RequiredMetaData::getValueForKey(const std::string &key)
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
int32 RequiredMetaData::writeToFile(IDataFilePtr dataFile)
{
  int32 err = -1;
  //TODO: This is NOT implemented yet.
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 RequiredMetaData::readFromFile(IDataFilePtr dataFile)
{
  //TODO: This is not implemented yet.
  return -1;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool RequiredMetaData::isValid(std::string message)
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
void RequiredMetaData::generateKeyValueMap(std::map<std::string, std::string> &requiredMetaData)
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
void RequiredMetaData::printSelf(std::ostream& os, int32 indent)
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



