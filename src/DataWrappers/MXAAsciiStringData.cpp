#include "MXAAsciiStringData.h"
#include <Common/LogTime.h>
#include <HDF5/H5Lite.h>


#include <iostream>

#include <hdf5.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArrayPtr MXAAsciiStringData::Create(const std::string &value)
{
  IMXAArrayPtr ptr;
  int32 err = 1;
  MXAAsciiStringData* d = new MXAAsciiStringData( 0);
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
    ptr.reset( static_cast<IMXAArray*>(d) );
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
MXAAsciiStringData* MXAAsciiStringData::New(const std::string &value)
{
  int32 err = 1;
  MXAAsciiStringData* d = new MXAAsciiStringData( 0);
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
  if (err < 0)
  {
    delete d; // Clean up the memory
    d = NULL;
  }
  return d;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAAsciiStringData::toStdString(MXAAsciiStringData* strData)
{
  const char* p = static_cast<const char*>(strData->getVoidPointer(0) );
  std::string str (p, strData->getNumberOfElements());
  return str;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAAsciiStringData::valueToString(char delimiter)
{
  const char* p = static_cast<const char*>(this->getVoidPointer(0) );
  std::string str (p, this->getNumberOfElements());
  return str;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXAAsciiStringData::MXAAsciiStringData( mxaIdType numElements) :
 MXAArrayTemplate<uint8>( numElements, true)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXAAsciiStringData::~MXAAsciiStringData()
{

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 MXAAsciiStringData::getDataType()
{
  return H5T_STRING;
}

#if 0
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
int32 MXAAsciiStringData::writeToFile(IDataFilePtr dataFile)
{
  int32 err = -1;
  std::vector<hsize_t> dims(1, this->getNumberOfElements() );
  err = H5Utilities::createGroupsForDataset(this->getDatasetPath(), dataFile->getFileId() );
  if (err < 0)
  {
    return err;
  }
  err = H5Lite::writeStringDataset(dataFile->getFileId(), this->getDatasetPath(), this->getNumberOfElements(), static_cast<const char*>(this->getVoidPointer(0) ) );
  return err;
}

// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------
int32 MXAAsciiStringData::readFromFile(IDataFilePtr dataFile)
{
  hid_t fileId = dataFile->getFileId();
  if (fileId < 0)
  {
    return fileId;
  }
  herr_t err = -1;
  H5T_class_t attr_type;
  size_t attr_size;
  std::string res;

  std::vector<hsize_t> dims;
  err = H5Lite::getDatasetInfo(fileId, this->getDatasetPath(), dims, attr_type, attr_size);
  if (err < 0)
  {
    return err;
  }
  std::string::size_type size = dims[0];
  if (size > 0) // NOT an empty String
  {
    err = this->resize(size);
    if (err > 0) //copy data into array
    {
      uint8* dest = static_cast<uint8*>(this->getVoidPointer(0) );
      if (NULL != dest) {
        err = H5Lite::readStringDataset(fileId, this->getDatasetPath(), dest);
      }
    }
  }

  return err;
}

#endif

