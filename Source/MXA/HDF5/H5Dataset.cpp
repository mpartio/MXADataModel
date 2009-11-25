#include "H5Dataset.h"
#include <MXA/Base/IDataFile.h>
#include <MXA/Base/IMXAArray.h>
//#include <MXA/Core/MXAAbstractAttribute.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/HDF5/H5MXAUtilities.h>

#include <list>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataset::Pointer H5Dataset::LoadFromFile(IDataFile::Pointer dataFile, const std::string &datasetPath)
{
  IMXAArray::Pointer data = H5MXAUtilities::readData(dataFile, datasetPath);

  if (NULL == data.get() )
  {
    IDataset::Pointer ptr;
    return ptr;
  }
  IDataset::Pointer ptr (new H5Dataset(datasetPath, data));
  //Load all the Attributes
  std::list<std::string> attributeNames;
  int32 err = H5Utilities::getAllAttributeNames(dataFile->getFileId(), datasetPath, attributeNames );
  if (err < 0)
  {
    return ptr;
  }
  for (std::list<std::string>::iterator iter = attributeNames.begin(); iter != attributeNames.end(); ++iter )
  {
    IMXAArray::Pointer attr = H5MXAUtilities::readAttribute(dataFile, datasetPath, *iter);
    if (attr.get() != NULL)
    {
      ptr->addAttribute(*iter, attr);
    }
    else
    {  // Problems loading the attribute
      H5Dataset* nullPtr = 0x0;
      ptr.reset(nullPtr);
      break;
    }
  }
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataset::Pointer H5Dataset::CreateDatasetPtr(const std::string &datasetPath, IMXAArray::Pointer data)
{
  IDataset::Pointer ptr (new H5Dataset(datasetPath, data));
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5Dataset::H5Dataset(const std::string &datasetPath, IMXAArray::Pointer data) :
_data(data),
_datasetPath(datasetPath)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
 H5Dataset::~H5Dataset()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5Dataset::setDatasetPath(const std::string &path)
{
  this->_datasetPath = path;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string H5Dataset::getDatasetPath()
{
  return this->_datasetPath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5Dataset::setData(IMXAArray::Pointer data)
{
  this->_data = data;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArray::Pointer H5Dataset::getData()
{
  return this->_data;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5Dataset::addAttribute (const std::string &attributeKey, IMXAArray::Pointer attribute)
{

  MXAAbstractAttributes::iterator iter = _attributes.find(attributeKey);
  if ( iter != _attributes.end() )
  {
    //Key was already in the map of attributes
    IMXAArray::Pointer ptr = (*iter).second;
    if (ptr.get() != attribute.get()  && (NULL != attribute.get() ))
    {
      //Attribute was NOT the same one
      (*iter).second = attribute;
    }
  }
  else // Key was not found in the attributes map so add it
  {
    this->_attributes[attributeKey] = attribute;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5Dataset::removeAttribute (const std::string &attributeKey)
{
  MXAAbstractAttributes::iterator iter = _attributes.find(attributeKey);
  if ( iter != _attributes.end() )
  {
    _attributes.erase(attributeKey);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArray::Pointer H5Dataset::getAttribute(const std::string &attributeKey)
{
  MXAAbstractAttributes::iterator iter = _attributes.find(attributeKey);
  if ( iter != _attributes.end() )
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
int32 H5Dataset::writeToFile (IDataFile::Pointer dataFile)
{
  herr_t err = -1;
  err = H5Utilities::createGroupsForDataset( this->_datasetPath, dataFile->getFileId());
  err = H5Lite::writeMXAArray(dataFile->getFileId(), this->_datasetPath, this->_data.get() );
  if (err < 0)
  {
    std::cout << "Error Writing Array. " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
    return err;
  }

  IMXAArray* attr = NULL;
  for (MXAAbstractAttributes::iterator iter = this->_attributes.begin(); iter != this->_attributes.end(); ++iter)
  {
    std::string attributeName = (*iter).first;
    attr = (*iter).second.get();
    if (NULL != attr)
    {
      err = H5Lite::writeMXAAttribute(dataFile->getFileId(), this->_datasetPath, attributeName, attr);
      if (err < 0)
      {
        std::cout << "Error Writing Attribute. " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
        return err;
      }
    }
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5Dataset::readFromFile (IDataFile::Pointer dataFile)
{
  std::string datasetPath = this->getDatasetPath();
  if (datasetPath.empty() == true)
  {
    return -1;
  }
  IMXAArray::Pointer data = H5MXAUtilities::readData(dataFile, datasetPath );
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
    IMXAArray::Pointer attr = H5MXAUtilities::readAttribute(dataFile, datasetPath, *iter);
    if (attr.get() != NULL)
    {
      this->addAttribute(*iter, attr);
    }
  }
  return err;
}




