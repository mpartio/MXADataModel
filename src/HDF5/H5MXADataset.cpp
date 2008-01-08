#include "H5MXADataset.h"
#include <Core/MXAAbstractData.h>
#include <Core/MXAAbstractAttribute.h>
#include <HDF5/H5DataArrayTemplate.hpp>
#include <HDF5/H5Utilities.h>

#include <list>


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDatasetPtr H5MXADataset::LoadFromFile(IDataFilePtr dataFile, const std::string &datasetPath)
{
  MXAAbstractDataPtr data = H5Utilities::readDataArray(dataFile, datasetPath);
  if (NULL == data.get() )
  {
    IDatasetPtr ptr;
    return ptr;
  }
  IDatasetPtr ptr (new H5MXADataset(data));
  //Load all the Attributes
  std::list<std::string> attributeNames;
  int32 err = H5Utilities::getAllAttributeNames(dataFile->getFileId(), datasetPath, attributeNames );
  if (err < 0)
  {
    return ptr;
  }
  for (std::list<std::string>::iterator iter = attributeNames.begin(); iter != attributeNames.end(); ++iter ) 
  {
    MXAAbstractAttributePtr attr = H5Utilities::readAttributeArray(dataFile, datasetPath, *iter);
    if (attr.get() != NULL)
    {
      ptr->addAttribute(attr);
    }
    else
    {  // Problems loading the attribute
      H5MXADataset* nullPtr = 0x0;
      ptr.reset(nullPtr);
      break;
    }
  }
  return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDatasetPtr H5MXADataset::CreateDatasetPtr(MXAAbstractDataPtr data)
{
  IDatasetPtr ptr (new H5MXADataset(data));
  return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5MXADataset::H5MXADataset(MXAAbstractDataPtr data) :
_data(data)
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
 H5MXADataset::~H5MXADataset()
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string H5MXADataset::getDatasetPath()
{
  std::string path;
  if (this->_data.get() != NULL)
  {
    path = this->_data->getDatasetPath();
  }
  return path;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5MXADataset::setData(MXAAbstractDataPtr data)
{
  this->_data = data;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAbstractDataPtr H5MXADataset::getData()
{
  return this->_data;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5MXADataset::addAttribute (MXAAbstractAttributePtr attribute)
{
  for (MXAAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter)
  {
    if ( (*iter).get() == attribute.get() )
    {
      return;
    }
  }
  this->_attributes.push_back(attribute);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5MXADataset::removeAttribute (const std::string &attributeKey)
{
  for (MXAAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter)
  {
    if ( (*iter)->getAttributeKey().compare(attributeKey) == 0 )
    {
      this->_attributes.erase(iter);
      return;
    }
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAbstractAttributePtr H5MXADataset::getAttribute(const std::string &attributeKey)
{
  for (MXAAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter)
  {
     if ( (*iter)->getAttributeKey().compare(attributeKey) == 0 )
    {
      return *iter;
    }
  }
  MXAAbstractAttributePtr ptr;
  return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5MXADataset::writeToFile (IDataFilePtr dataFile)
{
  herr_t err = -1;
  err = this->_data->writeToFile(dataFile);
  if (err < 0)
  {
    return err;
  }
  
  for (MXAAbstractAttributes::iterator iter = this->_attributes.begin(); iter != this->_attributes.end(); ++iter)
  {
    err = (*iter)->writeToFile(dataFile);
    if (err < 0)
    {
      return err;
    }
  }
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5MXADataset::readFromFile (IDataFilePtr dataFile)
{
  std::string datasetPath = this->getDatasetPath();
  if (datasetPath.empty() == true)
  {
    return -1;
  }
  MXAAbstractDataPtr data = H5Utilities::readDataArray(dataFile, datasetPath );
  if (data.get() == NULL)
  {
    return -1;
  }
  this->_data = data;
  //Load all the Attributes
  std::list<std::string> attributeNames;
  int32 err = H5Utilities::getAllAttributeNames(dataFile->getFileId(), datasetPath, attributeNames );
  if (err < 0)
  {
    return err;
  }
  this->_attributes.clear(); // Clear any attributes first
  for (std::list<std::string>::iterator iter = attributeNames.begin(); iter != attributeNames.end(); ++iter ) {
    MXAAbstractAttributePtr attr = H5Utilities::readAttributeArray(dataFile, datasetPath, *iter);
    if (attr.get() != NULL)
    {
      this->addAttribute(attr);
    }
  }
  return err;
}




