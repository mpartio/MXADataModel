#include "MXARGBImage.h"
#include <Common/LogTime.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Image.h>
#include <iostream>

#include <hdf5.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArrayPtr MXARGBImage::CreateRGBImageArray( int32 width, int32 height)
{
  IMXAArrayPtr ptr;
  int32 err = 1;
  MXARGBImage* d = new MXARGBImage(width, height);
  err = d->_allocate();
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
MXARGBImage* MXARGBImage::New( int32 width, int32 height)
{
  int32 err = 1;
  MXARGBImage* d = new MXARGBImage( width, height);
  err = d->_allocate();
  if (err < 0)
  {
    delete d;
    d = NULL;
  }
  return d;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXARGBImage::MXARGBImage( int32 width, int32 height) :
MXAArrayTemplate<uint8>( (uint64)(width * height * 3), true),
_width(width * 3),
_height(height)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXARGBImage::~MXARGBImage()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 MXARGBImage::getNumberOfDimensions ()
{
  return 2;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint8* MXARGBImage::getPixelPointer(int32 x, int32 y)
{
  mxaIdType index = (_width * y ) + x * 3;
  return static_cast<uint8*>(this->getVoidPointer(index));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 MXARGBImage::resize(uint64 size)
{
  if(this->_resizeAndExtend(size) || size <= 0)
    {
    this->_width = size;
    this->_height = 1;
    return 1;
    }
  else
    {
    return 0;
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 MXARGBImage::resizeArray(mxaIdType width, mxaIdType height)
{
  int32 err =  this->resize(width * height * 3);
  this->_width = width * 3;
  this->_height = height;
  if (err == 0)
  {
    this->_width = 0;
    this->_height = 0;
  }
  return err;
}

#if 0
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
int32 MXARGBImage::writeToFile(IDataFilePtr dataFile)
{
  int32 err = -1;
  std::vector<uint64> dims (2, 0 );
  dims[0] = this->_width * 3;
  dims[1] = this->_height;
  err = H5Utilities::createGroupsForDataset(this->getDatasetPath(), dataFile->getFileId() );
  if (err < 0)
  {
    return err;
  }
  err = H5Lite::writePointerDataset(dataFile->getFileId(), this->getDatasetPath(), 2, &(dims.front()), this->getPixelPointer(0, 0) );

  return err;
}


// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------
int32 MXARGBImage::readFromFile(IDataFilePtr dataFile)
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

  std::vector<hsize_t> dims;  //Reusable for the loop
  err = H5Lite::getDatasetInfo(fileId, this->getDatasetPath(), dims, attr_type, attr_size);
  if (err < 0 )
  {
    return err;
  }
  int32 width = dims[0] / 3;
  int32 height = dims[1];
  err = this->resizeArray(width, height); //Resize the array to hold the data from the file
  if (err < 0) { return err; }
  err = H5Lite::readPointerDataset(fileId, this->getDatasetPath(), getPixelPointer(0,0));
  return err;
}
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXARGBImage::valueToString(char delimiter)
{
  std::stringstream sstream;

  uint64 nElements = this->getNumberOfElements();
  uint64 limit = nElements - 1;
  int32 wLimit = _width * 3;
  int32 width = 0;
  uint8* data = this->getPointer(0);
  for(uint64 i = 0; i < nElements; i=i+3)
  {

    sstream << "[" << data[i] << "," << data[i+1] << "," << data[i+2] << "]";
    if (i < limit && width != wLimit)
    {
      sstream << delimiter;
    }
    if (width == wLimit) // Format in multiple rows
    {
      sstream << "\n";
      width = 0;
    }
    width += 3;
  }
  return sstream.str();
}