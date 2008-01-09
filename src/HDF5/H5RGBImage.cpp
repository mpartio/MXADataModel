#include "H5RGBImage.h"
#include <Common/LogTime.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Image.h>
#include <HDF5/H5TiffIO.h>
#include <iostream>

#include <hdf5.h>


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAbstractDataPtr H5RGBImage::CreateAbstractDataArray(const std::string &datasetPath, 
                                          int32 width, int32 height)
{
  MXAAbstractDataPtr ptr;
  int32 err = 1;
  H5RGBImage* d = new H5RGBImage(datasetPath, width, height);
  err = d->_allocate();
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
H5RGBImage* H5RGBImage::New(const std::string &datasetPath, 
                            int32 width, int32 height)
{
  int32 err = 1;
  H5RGBImage* d = new H5RGBImage(datasetPath, width, height);
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
H5RGBImage::H5RGBImage(const std::string &datasetPath, 
                       int32 width, int32 height) :
H5DataArrayTemplate<uint8>(datasetPath, width * height * 3, true),
_width(width),
_height(height)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5RGBImage::~H5RGBImage()
{
  
}


/**
 * Returns the number of dimensions the data has.
 */
int32 H5RGBImage::getNumberOfDimensions ()
{
  return 2;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
uint8* H5RGBImage::getPixelPointer(int32 x, int32 y)
{
  mxaIdType index = (_width * y * 3) + x * 3;
  return static_cast<uint8*>(this->getVoidPointer(index));
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5RGBImage::resize(mxaIdType size)
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
int32 H5RGBImage::resizeArray(mxaIdType width, mxaIdType height)
{
  int32 err =  this->resize(width * height * 3);
  this->_width = width;
  this->_height = height;
  if (err == 0)
  {
    this->_width = 0;
    this->_height = 0;
  }
  return err;
}
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
int32 H5RGBImage::writeToFile(IDataFilePtr dataFile)
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
int32 H5RGBImage::readFromFile(IDataFilePtr dataFile)
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

