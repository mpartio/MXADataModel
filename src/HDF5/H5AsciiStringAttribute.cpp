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
  int32 err = 1;
  H5AsciiStringAttribute* d = new H5AsciiStringAttribute(datasetPath, attributeKey, 0);
  std::string::size_type size = value.size();
  std::string::size_type nullTermSize = size + 1;
  if (size > 0) // NOT an empty String
  {
    err = d->resize(nullTermSize);
    if (err > 0) //copy data into array
    {
      uint8* dest = d->getPointer(0);
      const char* source = value.c_str();
      ::memcpy(dest, source, value.size() + 1 );
    }
  }
  if (err >= 0)
  { // No errors, swap in the pointer
    ptr.reset(d); 
  }
  else
  {
    delete d; // Clean up the memory
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
  hid_t fileId = dataFile->getFileId();
  if (fileId < 0)
  {
    return -1;
  }
  herr_t err = -1;
  herr_t retErr = -1;
  H5T_class_t attr_type;
  size_t attr_size;
  hid_t typeId = -1;
  
  std::vector<uint64> dims;  //Reusable for the loop
   err = H5Lite::getAttributeInfo(fileId, this->getDatasetPath(), this->getAttributeKey(), dims, attr_type, attr_size, typeId);
   
   if (err < 0 )
   {
     CloseH5T(typeId, err, retErr); //Close the H5A type Id that was retrieved during the loop
     return err;
   }
   CloseH5T(typeId, err, retErr); //Close the H5A type Id that was retrieved during the loop
  std::string::size_type size = attr_size;
  if (size > 0) // NOT an empty String
  {
    err = this->resize(size);
    if (err > 0) //copy data into array
    {
      uint8* dest = static_cast<uint8*>(this->getVoidPointer(0) );
      if (NULL != dest) {
        err = H5Lite::readStringAttribute(fileId, this->getDatasetPath(), this->getAttributeKey(), dest);
      }
    }
  }
  
  return err;
}

