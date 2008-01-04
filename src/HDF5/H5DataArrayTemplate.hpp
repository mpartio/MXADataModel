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
#ifndef _H5DataArrayTemplate_h_
#define _H5DataArrayTemplate_h_

#include <Common/MXATypeDefs.h>
#include <Base/IDataFile.h>
#include <Core/MXAAbstractData.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Utilities.h>


/**
* @class H5DataArrayTemplate H5DataArrayTemplate.hpp HDF5/H5DataArrayTemplate.hpp
* @brief 
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.1 $
*/
template<typename T>
class H5DataArrayTemplate : public MXAAbstractData
{
  public:
   
/**
 * @brief Static constructor
 * @param datasetPath
 * @param numElements
 * @return Boost::Shared_Ptr wrapping an instance of H5DataArrayTemplateTemplate<T>
 */
   static MXAAbstractDataPtr CreateAbstractDataArray(const std::string &datasetPath, mxaIdType numElements)
   {
     H5DataArrayTemplate<T>* d = new H5DataArrayTemplate<T>(datasetPath, numElements, true);
     if ( d->_allocate() < 0)
     {  // Could not allocate enough memory, reset the pointer to null and return
       d = NULL;
     }
     MXAAbstractDataPtr ptr ( dynamic_cast<MXAAbstractData*>(d) );
     return ptr;
   }
    
/**
 * @brief
 * @param datasetPath
 * @param numElements
 * @return
 */
   static H5DataArrayTemplate<T>* New(const std::string &datasetPath, mxaIdType numElements)
   {
     H5DataArrayTemplate<T>* ptr = new H5DataArrayTemplate<T>(datasetPath, numElements, true);
     if (ptr->_allocate() < 0)
     { // Could not allocate enough memory, reset the pointer to null and return
       ptr = 0x0;
     }
     return ptr;
   }
    
/**   
 * @brief Destructor
 */
    virtual ~H5DataArrayTemplate()
    {
      //std::cout << "~H5DataArrayTemplateTemplate" << std::endl;
      if ((NULL != this->_data) && (true == this->_ownsData))
      {
        free(this->_data);
      }
    }
    
    
// -----------------------------------------------------------------------------
//  MXAAbstractDataArray Methods
// -----------------------------------------------------------------------------        
    void setDatasetPath(const std::string &datasetPath)
    {
      this->_datasetPath = datasetPath;
    }
    
    std::string getDatasetPath()
    {
      return this->_datasetPath;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    
    virtual void takeOwnership()
    {
      this->_ownsData = true;
    }
    virtual void releaseOwnership()
    {
      this->_ownsData = false;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual void initialize()
    {
      if(NULL != this->_data && true == this->_ownsData)
        {
        free(this->_data);
        }
      this->_data = 0;
      this->_size = 0;
      this->_ownsData = true;
    }
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual int32 resize(mxaIdType size)
    {
      if(this->_resizeAndExtend(size) || size <= 0)
        {
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
    virtual void* getVoidPointer(mxaIdType i)
    {
      return (void*)(&(_data[i]) );
    }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual T getValue(mxaIdType i)
    {
      return this->_data[i];
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual mxaIdType getNumberOfElements()
    {
      return _size;
    }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual int32 getNumberOfDimensions()
    {
      return 1;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    void setValue(mxaIdType i, T value)
    {
      this->_data[i] = value;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual int32 getDataType()
    {
      T t;
      return H5Lite::HDFTypeForPrimitive(t);
    }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual T* getPointer(mxaIdType i)
    {
      return (T*)(&(_data[i]) );
    }
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
    virtual int32 writeToFile (IDataFilePtr dataFile)
    {
      int32 err = -1;
      int32 rank = this->getNumberOfDimensions();
      std::vector<hsize_t> dims (1, this->getNumberOfElements() );
      err = H5Utilities::createGroupsForDataset(_datasetPath, dataFile->getFileId() );
      if (err < 0)
      {
        return err;
      }
      err = H5Lite::writePointerDataset(dataFile->getFileId(), _datasetPath, rank, &(dims.front()), this->getPointer(0) );
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
     
      std::vector<uint64> dims;  //Reusable for the loop
      err = H5Lite::getDatasetInfo(fileId, _datasetPath, dims, attr_type, attr_size);
      if (err < 0 )
      {
        return err;
      }
      mxaIdType numElements = 1;
      for (std::vector<uint64>::size_type i = 0; i < dims.size(); ++i)
      {
        numElements = numElements * dims[i];
      }
      if (this->getNumberOfElements() != numElements)
      {
        err = this->resize(numElements); //Resize the array to hold the data from the file
        if (err < 0) { return err; }
      }
      
      err = H5Lite::readPointerDataset(fileId, this->_datasetPath, _data);
      return err;
    }
    
    
  protected:  
    /**    
     * @brief
     * @param datasetPath
     * @param numElements
     * @param takeOwnership
     */
      H5DataArrayTemplate(const std::string &datasetPath, mxaIdType numElements, bool takeOwnership = true) :
        _datasetPath(datasetPath),
        _data(NULL),
        _size(numElements),
        _ownsData(takeOwnership)
      {    }
        

    /**
     * @brief
     * @return
     */
        int32 _allocate()
        {
          if ( (NULL != this->_data) && (true == this->_ownsData) )
          {
            ::free(this->_data);
          }
          this->_data = NULL;
          this->_ownsData = true;
          int newSize = (this->_size > 0 ? this->_size : 1);
          this->_data = (T*)malloc(newSize * sizeof(T));
          if (!this->_data)
          {
            std::cout << DEBUG_OUT(logTime) << "Unable to allocate " << newSize << " elements of size " << sizeof(T) << " bytes. " << std::endl;
            return -1;
          }
          this->_size = newSize;
          return 1;
        }
        
    // -----------------------------------------------------------------------------
    //  
    // -----------------------------------------------------------------------------
      virtual T* _resizeAndExtend(mxaIdType size)
        {
          T* newArray;
          mxaIdType newSize;

          if (size > this->_size)
          {
            newSize = size;
          }
          else if (size == this->_size) // Requested size is equal to current size.  Do nothing.
          {
            return this->_data;
          }
          else // Requested size is smaller than current size.  Squeeze the memory.
          {
            newSize = size;
          }

          // Wipe out the array completely if new size is zero.
          if (newSize <= 0)
          {
            this->initialize();
            return 0;
          }

          // Allocate a new array if we DO NOT own the current array 
          if ( (NULL != this->_data) && (false == this->_ownsData))
          {
            // The old array is owned by the user so we cannot try to
            // reallocate it.  Just allocate new memory that we will own.
            newArray = (T*)malloc(newSize*sizeof(T));
            if (!newArray)
            {
              std::cout << DEBUG_OUT(logTime) << "Unable to allocate " << newSize << " elements of size " << sizeof(T) << " bytes. " << std::endl;
              return 0;
            }

            // Copy the data from the old array.
            memcpy(newArray, this->_data, (newSize < this->_size ? newSize : this->_size) * sizeof(T));
          }
          else
          {
            // Try to reallocate with minimal memory usage and possibly avoid copying.
            newArray = (T*)realloc(this->_data, newSize*sizeof(T));
            if (!newArray)
            {
              std::cout << DEBUG_OUT(logTime) << "Unable to allocate " << newSize << " elements of size " << sizeof(T) << " bytes. " << std::endl;
              return 0;
            }
          }

          // Allocation was successful.  Save it.
          this->_size = newSize;
          this->_data = newArray;
          // This object has now allocated its memory and owns it.
          this->_ownsData = true;

          return this->_data;
        } 
      
    
  private:
    std::string _datasetPath;
    T* _data;
    mxaIdType _size;
    bool _ownsData;
  
    H5DataArrayTemplate(const H5DataArrayTemplate&);    //Not Implemented
    void operator=(const H5DataArrayTemplate&); //Not Implemented

};

#endif //_H5DataArrayTemplate_h_



