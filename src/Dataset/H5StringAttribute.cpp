#include "H5StringAttribute.h"
#include <Base/IDataFile.h>
#include <HDF5/H5Lite.h>

//-- C++ Includes
#include <iostream>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5StringAttribute::H5StringAttribute(const std::string &path,
                                      const std::string &key,
                                      const std::string &value)  :
IAbstractAttribute(path, key, std::vector<uint64>(1, value.size() ) ),
_value(value)
{}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IAbstractAttributePtr H5StringAttribute::New( const std::string &path,
                                  const std::string &key,
                                  const std::string &value)
{ 
  H5StringAttribute* ds = new H5StringAttribute(path, key, value);
  IAbstractAttributePtr ptr (ds); // Wrap pointer in a Boost Shared Ptr
  return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5StringAttribute::~H5StringAttribute()
{
}

// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
int32 H5StringAttribute::writeToFile(IDataFilePtr dataFile)
{
  return H5Lite::writeStringAttribute(dataFile->getFileId(), this->getDatasetPath(), this->getKey(), _value);
}

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
int32 H5StringAttribute::readFromFile(IDataFilePtr dataFile)
{
#warning Implement H5StringAttribute::readFromFile
  std::cout << "H5StringAttribute::readFromFile" << std::endl;
  return 1;
}  
