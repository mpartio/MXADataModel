#ifndef _H5DataArrayTemplate_h_
#define _H5DataArrayTemplate_h_

#include <Dataset/IDataArray.h>


/**
* @class H5DataArrayTemplate H5DataArrayTemplate.h Dataset/H5DataArrayTemplate.h
* @brief 
* @author mjackson
* @date Dec 31, 2007
* @version $Revision: 1.1 $
*/
template<typename T>
class H5DataArrayTemplate : public IDataArray
{
  public:

/**
 * @brief Static constructor
 * @param numElements
 * @return Boost::Shared_Ptr wrapping an instance of H5DataArrayTemplate<T>
 */
    static IDataArrayPtr New(mxaIdType numElements)
    {
      H5DataArrayTemplate<T>* d = new H5DataArrayTemplate<T>(numElements, true);
      if ( d->_allocate() < 0)
      {  // Could not allocate enough memory, reset the pointer to null and return
        d = NULL;
      }
      IDataArrayPtr ptr ( dynamic_cast<IDataArray*>(d) );
      return ptr;
    }
    
/**
 * @brief
 * @param numElements
 * @return
 */
    static H5DataArrayTemplate<T>* NewArray(mxaIdType numElements)
    {
      H5DataArrayTemplate<T>* ptr = new H5DataArrayTemplate<T>(numElements, true);
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
      if ((NULL != this->_data) && (true == this->_ownsData))
      {
        free(this->_data);
      }
    }
     

    
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
    virtual int32 writeDataToFile(IDataFilePtr dataFile, const std::string &datasetPath)
    {
      int32 err = -1;
      int32 rank = this->getNumberOfDimensions();
      std::vector<hsize_t> dims (1, this->_size);
      err = H5Utilities::createGroupsForDataset(datasetPath, dataFile->getFileId() );
      if (err < 0)
      {
        return err;
      }
      err = H5Lite::writePointerDataset(dataFile->getFileId(), datasetPath, rank, &(dims.front()), this->_data);
      return err;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual int32 writeAttributeToFile(IDataFilePtr dataFile, const std::string &datasetPath, const std::string &key)
    {
      int32 rank = this->getNumberOfDimensions();
      std::vector<hsize_t> dims (1, this->_size);
      return H5Lite::writePointerAttribute(dataFile->getFileId(), datasetPath, key, rank, &(dims.front()), this->_data);
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
    virtual int32 readDataFromFile(IDataFilePtr dataFile, const std::string &datasetPath)
    {
      return -1;
    }

    virtual int32 readAttributeFromFile(IDataFilePtr dataFile, const std::string &datasetPath, const std::string &key)
    {
      return -1;
    }
    
    
    virtual void debugPrint(std::ostream &out)
    {
      for (mxaIdType i = 0; i < this->_size; ++i) {
        out << this->_data[i] << " ";
      }
      out << std::endl;
    }
        
  protected:
    
/**    
 * @brief
 * @param numElements
 * @param takeOwnership
 */
    H5DataArrayTemplate(mxaIdType numElements, bool takeOwnership = true) :
      _data(NULL),
      _size(numElements),
      _ownsData(takeOwnership)
    {    }
    

/**
 * @brief
 * @param numElements
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
    
    T* _data;
    mxaIdType _size;
    bool _ownsData;
    
    H5DataArrayTemplate(const H5DataArrayTemplate&);    //Not Implemented
    void operator=(const H5DataArrayTemplate&); //Not Implemented
  
};

#endif //_H5DataArrayTemplate_h_




