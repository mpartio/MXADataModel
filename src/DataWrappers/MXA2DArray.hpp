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
#ifndef _MXA2DArray_h_
#define _MXA2DArray_h_


#include <DataWrappers/MXAArrayTemplate.hpp>
#include <Common/LogTime.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Image.h>
#include <HDF5/H5TiffIO.h>
#include <iostream>

#include <hdf5.h>

/**
* @class MXA2DArray MXA2DArray.hpp HDF5/MXA2DArray.hpp
* @brief This class represents a generic 2D array of data.
* @author mjackson
* @date Jan 9, 2008
* @version $Revision: 1.2 $
*/
template<typename T>
class MXA2DArray : public MXAArrayTemplate<T>
{
  public:
    /**
     * @brief Creates a MXA2DArray wrapped in a Boost::SharedPointer that represents a 2D array
     * typically used for images. The memory is allocated immediately. If the memory
     * can not be allocated then a NULL wrapped pointer is returned.
     * @param width The width of the array
     * @param height The height of the array.
     * @return Boost::shared_pointer wrapped MXA2DArray pointer
     */
    static IMXAArrayPtr CreateAbstractDataArray( int32 width, int32 height)
    {
      IMXAArrayPtr ptr;
      int32 err = 1;
      MXA2DArray<T>* d = new MXA2DArray<T>( width, height);
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

    /**
     * @brief Creates and allocates an MXA2DArray object. You are repsonsible for cleaning
     * up the memory if you select to allocate the array in this way.
     * @param width The width of the array
     * @param height The height of the array.
     * @return Pointer to an MXA2DArray Object
     */
    static MXA2DArray* New( int32 width, int32 height)
    {
      int32 err = 1;
      MXA2DArray* d = new MXA2DArray( width, height);
      err = d->_allocate();
      if (err < 0)
      {
        delete d;
        d = NULL;
      }
      return d;
    }

    virtual ~MXA2DArray() {}


    /**
     * @brief Returns the number of dimensions the data has.
     */
    virtual int32 getNumberOfDimensions ()
    {
      return 2;
    }


    /**
     * @brief Returns the sizes of the dimensions
     * @param dims A pointer to an array of at least size=2.
     */
    virtual void getDimensions(uint64* dims)
    {
      dims[0] = _width;
      dims[1] = _height;
    }

    /**
     * @brief Returns the width of the array
     * @return
     */
    virtual int32 getWidth() { return _width; }

    /**
     * @brief Returns the height of the array.
     * @return
     */
    virtual int32 getHeight() { return _height; }

    /**
     * @brief Returns the a pointer to the data value located at pixel (x,y)
     * @param x The x location of the pixel
     * @param y The y location of the pixel
     * @return A pointer to the value.
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


    /**
     * @brief Resizes the array
     * @param size The new size of the array
     * @return 1 if the resize succeeded, 0 on error
     */
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
     * @brief Resizes the data array to the specified width and height
     * @param width The new width of the array
     * @param height The new height of the array
     * @return 1 on success and Zero (0) on failure
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


    /**
     * @brief Initializes the array to widht and height  = 0
     */
    virtual void initialize()
    {
      MXAArrayTemplate<T>::initialize();
      this->_width = 0;
      this->_height = 0;
    }

#if 0
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
#endif

    /**
     * @brief Converts the data array into a string delimited by the supplied
     * delimiter.
     * @param delimiter The delimiter to use between each value. Default is a single space
     * @return The generated string
     */
    virtual std::string valueToString(char delimiter = ' ')
    {
      std::stringstream sstream;
      uint64 nElements = this->getNumberOfElements();
      uint64 limit = nElements - 1;
      int32 width = 0;
      T* data = this->getPointer(0, 0);
      for(uint64 i = 0; i < nElements; ++i)
      {
        if (sizeof(T) != 1 )
         {
          sstream  << data[i];
         }
         else
         {
           sstream  << static_cast<int32>(data[i]);
         }
        if (i < limit && width != _width)
        {
          sstream << delimiter;
        }
        if (width == _width) // Format in multiple rows
        {
          sstream << "\n";
          width = 0;
        }
        ++width;
      }
      return sstream.str();
    }


  protected:
      MXA2DArray( int32 width, int32 height):
        MXAArrayTemplate<T>( width * height, true),
        _width(width),
        _height(height)
        { }

  private:
    int32 _width;
    int32 _height;

      MXA2DArray(const MXA2DArray&);    //Not Implemented
      void operator=(const MXA2DArray&); //Not Implemented

};

#endif //_MXA2DArray_h_
