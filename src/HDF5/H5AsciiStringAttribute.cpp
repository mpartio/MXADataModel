#include "H5AsciiStringAttribute.h"
#include <Common/LogTime.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Utilities.h>

#include <iostream>

#include <hdf5.h>


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAbstractAttributePtr H5AsciiStringAttribute::New(const std::string &datasetPath, 
                                                    const std::string &attributeKey,
                                                    const std::string &value)
{
  MXAAbstractAttributePtr ptr;
  
  H5AsciiStringAttribute* d = new H5AsciiStringAttribute(datasetPath, attributeKey, 0);
  if (d->resize( value.size() ) > 0)
  {
    ptr.reset(d);
    if (value.size() > 0)
    {  //copy data into array only if there is something to copy
      uint8* dest = d->getPointer(0);
      const char* source = value.c_str();
      ::memcpy(dest, source, value.size() + 1 );
    }
  }
  return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string H5AsciiStringAttribute::toStdString(H5AsciiStringAttribute* strData)
{
  const char* p = static_cast<const char*>(strData->getVoidPointer(0) );
  std::string str (p, strData->getNumberOfElements());
  return str;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5AsciiStringAttribute::H5AsciiStringAttribute(const std::string &datasetPath, 
                                               const std::string &attributeKey,
                                               mxaIdType numElements) :
 H5AttributeArrayTemplate<uint8>(datasetPath, attributeKey, numElements, true)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5AsciiStringAttribute::~H5AsciiStringAttribute()
{

}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5AsciiStringAttribute::getDataType()
{
  return H5T_STRING;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
const char* H5AsciiStringAttribute::getCharPointer(mxaIdType index)
{
  return static_cast<const char*>(this->getVoidPointer(index) );
}

// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
int32 H5AsciiStringAttribute::writeToFile(IDataFilePtr dataFile)
{
  if (dataFile->getFileId() < 0)
  {
    return -1;
  }
  if (this->getVoidPointer(0) == NULL)
  {
    return -1;
  }
  return H5Lite::writeStringAttribute(dataFile->getFileId(), this->getDatasetPath(), this->getAttributeKey(), this->getNumberOfElements(), this->getCharPointer(0) );
}

// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------
int32 H5AsciiStringAttribute::readFromFile(IDataFilePtr dataFile)
{
  if (dataFile->getFileId() < 0)
  {
    return -1;
  }
  herr_t err = -1;

  std::string resultStr;
  err = H5Lite::readStringAttribute(dataFile->getFileId(), this->getDatasetPath(), this->getAttributeKey(), resultStr);
  if (err < 0)
  {
    this->initialize();
    return err;
  }
  err = this->resize(resultStr.size() );
  if ( err < 0)
  {
    return err;
  }
  uint8* dest = static_cast<uint8*>(this->getVoidPointer(0) );
  const char* source = resultStr.c_str();
  ::memcpy(dest, source, resultStr.size() );
  return err;
}

