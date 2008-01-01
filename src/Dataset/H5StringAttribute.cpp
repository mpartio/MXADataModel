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
_value(value)
{}


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
  return H5Lite::writeStringAttribute(dataFile->getFileId(), _datasetPath, _key, _value);
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
