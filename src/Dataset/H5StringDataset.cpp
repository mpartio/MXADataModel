#include "H5StringDataset.h"
#include <Base/IDataFile.h>
#include <Dataset/IAbstractAttribute.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Utilities.h>

//-- C++ Includes
#include <iostream>

// -----------------------------------------------------------------------------
// Protect this constructor so that the user MUST use the Static New Method 
// -----------------------------------------------------------------------------
H5StringDataset::H5StringDataset(const std::string &datasetPath, const std::string &value) :
  IAbstractDataset(datasetPath, std::vector<uint64>(1, value.size() ) ),
  _value(value)
  {}

  // -----------------------------------------------------------------------------
  //  Static New method
  // -----------------------------------------------------------------------------
IAbstractDatasetPtr H5StringDataset::New(const std::string &path, const std::string &value)
{
  H5StringDataset* ds = new H5StringDataset(path, value);
  IAbstractDatasetPtr ptr(ds); // Wrap pointer in a Boost Shared Ptr
  return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5StringDataset::~H5StringDataset()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5StringDataset::getNumberOfAttributes()
{
  return _attributes.size();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IAbstractAttributes H5StringDataset::getAttributes()
{
  return this->_attributes;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5StringDataset::addAttribute(IAbstractAttributePtr attribute)
{
  this->_attributes.push_back(attribute);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5StringDataset::removeAttribute(int32 index)
{
  if (index < 0 || (uint32)index >= _attributes.size() )
  {
    return 0;
  }
  int32 err = -1;
  int32 i = 0;
  for (IAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter ) 
  {
    if (i == index)
    {
      _attributes.erase(iter);
      err = 1;
      break;
    }
    ++i;
  }
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5StringDataset::removeAttribute(IAbstractAttributePtr attribute)
{
  int32 err = -1;
  IAbstractAttribute* attr = NULL;
  for (IAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter ) 
  {
    attr = (*iter).get();
    if (attribute.get() == attr)
    {
      _attributes.erase(iter);
      err = 1;
      break;
    }
  }
  return err;
}

// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
int32 H5StringDataset::writeToFile(IDataFilePtr dataFile)
{
  // std::cout << "H5StringDataset::writeToFile" << std::endl;
  int32 err = H5Utilities::createGroupsForDataset(getDatasetPath(), dataFile->getFileId() );
  if (err < 0)
  {
    //    std::cout << "H5StringDataset::writeToFile - Error Creating dataset to file '" << dataFile->getFilename() << "'" << std::endl;
    return err;
  }
  err = H5Lite::writeStringDataset(dataFile->getFileId(), getDatasetPath().c_str(), _value.c_str() );
  if (err < 0)
  {
    std::cout << "H5StringDataset::writeToFile - Error Writing dataset to file '" << dataFile->getFilename() << "'" << std::endl;
    return err;
  }
  IAbstractAttribute* attr= NULL;
  for (IAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter)
  {
    attr = (*iter).get();
    err = attr->writeToFile(dataFile);
    if (err < 0)
    {
      return err;
    }
  }
  return 1;
}

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
int32 H5StringDataset::readFromFile(IDataFilePtr dataFile)
{
  std::cout << "H5StringDataset::readFromFile" << std::endl;
#warning IMPLEMENT H5StringDataset::readFromFile
  return 1;
}
