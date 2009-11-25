#include "MXAAsciiStringData.h"
#include <MXA/Common/LogTime.h>

#if MXA_HDF5_SUPPORT
#include <hdf5.h>
#include <MXA/HDF5/H5Lite.h>

#endif

#include <iostream>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArray::Pointer MXAAsciiStringData::Create(const std::string &value)
{
  IMXAArray::Pointer ptr;
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
  MXAAsciiStringData* d = new MXAAsciiStringData(0);
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
MXAAsciiStringData::MXAAsciiStringData( size_t numElements) :
 MXAArrayTemplate<uint8>( static_cast<int32>(numElements), true)
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

