///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _H5Data2DArray_h_
#define _H5Data2DArray_h_


#include <HDF5/H5DataArrayTemplate.hpp>
#include <Common/LogTime.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Image.h>
#include <HDF5/H5TiffIO.h>
#include <iostream>

#include <hdf5.h>

/**
* @class H5Data2DArray H5Data2DArray.h HDF5/H5Data2DArray.h
* @brief This class represents a generic 2D array of data.
* @author mjackson
* @date Jan 9, 2008
* @version $Revision: 1.2 $
*/
template<typename T>
class MXA_EXPORT H5Data2DArray : public H5DataArrayTemplate<T> 
{
  public:
    static MXAAbstractDataPtr CreateAbstractDataArray(const std::string &datasetPath, 
                                  int32 width, int32 height)
    {
      MXAAbstractDataPtr ptr;
      int32 err = 1;
      H5Data2DArray<T>* d = new H5Data2DArray<T>(datasetPath, width, height);
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
    
    static H5Data2DArray* New(const std::string &datasetPath, 
                          int32 width, int32 height)
    {
      int32 err = 1;
      H5Data2DArray* d = new H5Data2DArray(datasetPath, width, height);
      err = d->_allocate();
      if (err < 0)
      {
        delete d;
        d = NULL;
      }
      return d;
    }
    
    virtual ~H5Data2DArray() {}
     
  
    /**
     * Returns the number of dimensions the data has.
     */
    virtual int32 getNumberOfDimensions ()
    {
      return 2;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual void getDimensions(mxaIdType* dims)
    {
      dims[0] = _width;
      dims[1] = _height;
    }
        
    virtual int32 getWidth() { return _width; }
    virtual int32 getHeight() { return _height; }
    
    /**
     * @brief
     * @param x
     * @param y
     * @return
     */
    T* getPointer(int32 x, int32 y)
    { 
      if (x < 0 || y < 0 || y >= _height || x >= _width)
      {
        return NULL;
      }
      mxaIdType index = (_width * y ) + x ;
      return static_cast<T*>(this->getVoidPointer(index));
    }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual int32 resize(uint64 size)
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
        
    /**
     * @brief
     * @param width
     * @param height
     * @return
     */
    int32 resizeArray(mxaIdType width, mxaIdType height)
    {
      int32 err =  this->resize(width * height);
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
//  
// -----------------------------------------------------------------------------   
    virtual void initialize()
    {
      H5DataArrayTemplate<T>::initialize();
      this->_width = 0;
      this->_height = 0;
    }
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
    virtual int32 writeToFile (IDataFilePtr dataFile)
    {
      int32 err = -1;
      std::vector<uint64> dims (2, 0 );
      dims[0] = this->_width;
      dims[1] = this->_height;
      err = H5Utilities::createGroupsForDataset(this->getDatasetPath(), dataFile->getFileId() );
      if (err < 0)
      {
        return err;
      }
      err = H5Lite::writePointerDataset(dataFile->getFileId(), this->getDatasetPath(), 2, &(dims.front()), this->getPointer(0, 0) );

      return err;
    }
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------
    virtual int32 readFromFile(IDataFilePtr dataFile)
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
      if (dims.size() != 2)
      {
        return -1;
      }
      uint64 numElements = 1;
      for (std::vector<uint64>::size_type i = 0; i < dims.size(); ++i)
      {
        numElements = numElements * dims[i];
      }
      if (this->getNumberOfElements() != numElements)
      {
        err = this->resize(numElements); //Resize the array to hold the data from the file
        if (err < 0) { return err; }
      }
      
      this->_width = dims[0];
      this->_height = dims[1];
      err = H5Lite::readPointerDataset(fileId, this->getDatasetPath(), this->getPointer(0,0) );
      return err;
    }
    
    
  protected:  
      H5Data2DArray(const std::string &datasetPath, 
                 int32 width, int32 height):
        H5DataArrayTemplate<T>(datasetPath, width * height, true),
        _width(width),
        _height(height)
        { }
        
  private:
    int32 _width;
    int32 _height;
    
      H5Data2DArray(const H5Data2DArray&);    //Not Implemented
      void operator=(const H5Data2DArray&); //Not Implemented
  
};

#endif //_H5Data2DArray_h_




