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
#ifndef _H5AttributeArrayTemplate_h_
#define _H5AttributeArrayTemplate_h_

#include <Core/MXAAbstractAttribute.h>
#include <Base/IDataFile.h>
#include <HDF5/H5Lite.h>
#include <Utilities/StringUtils.h>

/**
* @class H5AttributeArrayTemplate H5AttributeArrayTemplate.hpp PathToHeader/H5AttributeArrayTemplate.hpp
* @brief 
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.6 $
*/
template<typename T>
class H5AttributeArrayTemplate : public MXAAbstractAttribute
{
  public:
   
/**
 * @brief Static constructor
 * @param datasetPath The path to the dataset in the HDF5 file
 * @param attributeKey The string name of the attribute
 * @param numElements The number of elements in the internal array.
 * @return Boost::Shared_Ptr wrapping an instance of H5AttributeArrayTemplateTemplate<T>
 */
    static MXAAbstractAttributePtr CreateAbstractAttributeArray(const std::string &datasetPath, const std::string &attributeKey, uint64 numElements)
    {
      H5AttributeArrayTemplate<T>* d = new H5AttributeArrayTemplate<T>(datasetPath, attributeKey, numElements, true);
      if ( d->_allocate() < 0)
      {  // Could not allocate enough memory, reset the pointer to null and return
        d = NULL;
      }
      MXAAbstractAttributePtr ptr ( dynamic_cast<MXAAbstractAttribute*>(d) );
      return ptr;
    }
    
/**    
 * @brief
 * @param datasetPath The path to the dataset in the HDF5 file
 * @param attributeKey The string name of the attribute
 * @param nDims
 * @param dims
 * @return Boost::Shared_Ptr wrapping an instance of H5AttributeArrayTemplateTemplate<T>
 */
    static MXAAbstractAttributePtr CreateAbstractAttributeMultiDimensionalArray(const std::string &datasetPath, 
                                                                                const std::string &attributeKey, 
                                                                                int32 nDims,
                                                                                const uint64* dims)
    {

      H5AttributeArrayTemplate<T>* d = new H5AttributeArrayTemplate<T>(datasetPath, attributeKey, nDims, dims, true);
      if ( d->_allocate() < 0)
      {  // Could not allocate enough memory, reset the pointer to null and return
        d = NULL;
      }
      
#if 0
      std::cout << logTime() << "CreateAbstractAttributeMultiDimensionalArray" << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
      std::cout << "dims.size(): " << nDims << std::endl;
      std::cout << "dims[0]: " << dims[0] << std::endl;
      std::cout << "dims[1]: " << dims[1] << std::endl;
#endif
      MXAAbstractAttributePtr ptr ( dynamic_cast<MXAAbstractAttribute*>(d) );
      return ptr;
    }
    
    
/**
 * @brief Creates an Attribute from a single value
 * @param datasetPath The path to the dataset in the HDF5 file
 * @param attributeKey The string name of the attribute
 * @param value The value to store in the attribute
 * @return Boost::Shared_Ptr wrapping an instance of H5AttributeArrayTemplateTemplate<T>
 */
    static MXAAbstractAttributePtr CreateScalarAttribute(const std::string &datasetPath, const std::string &attributeKey, T value)
    {
      H5AttributeArrayTemplate<T>* d = new H5AttributeArrayTemplate<T>(datasetPath, attributeKey, 1, true);
      if ( d->_allocate() < 0)
      {  // Could not allocate enough memory, reset the pointer to null and return
        d = NULL;
      }
      d->setValue(0, value);
      MXAAbstractAttributePtr ptr ( dynamic_cast<MXAAbstractAttribute*>(d) );
      return ptr;
    }
    
    
/**
 * @brief Static construction of H5AttributeArrayTemplate objects. YOU are 
 * responsible for cleaning up the memory that this method creates.
 * @param datasetPath The path to the dataset in the HDF5 file
 * @param attributeKey The string name of the attribute
 * @param numElements The number of elements in the internal array.
 * @return
 */
    static H5AttributeArrayTemplate<T>* New(const std::string &datasetPath, const std::string &attributeKey, uint64 numElements)
    {
      H5AttributeArrayTemplate<T>* ptr = new H5AttributeArrayTemplate<T>(datasetPath, attributeKey, numElements, true);
      if (ptr->_allocate() < 0)
      { // Could not allocate enough memory, reset the pointer to null and return
        ptr = 0x0;
      }
      return ptr;
    }
    
/**   
 * @brief Destructor
 */
    virtual ~H5AttributeArrayTemplate()
    {
      //std::cout << "~H5AttributeArrayTemplateTemplate" << std::endl;
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
//  MXAAbstractAttribute Methods
// -----------------------------------------------------------------------------
    void setAttributeKey(const std::string &attributeKey)
    {
      this->_attributeKey = attributeKey;
    }
    
    std::string getAttributeKey()
    {
      return this->_attributeKey;
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
      this->_nElements = 0;
      this->_ownsData = true;
    }
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual int32 resize(uint64 size)
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
    virtual void* getVoidPointer(uint64 i)
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
    virtual uint64 getNumberOfElements()
    {
      return _nElements;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual void getDimensions(uint64* dims)
    {
      uint64 nBytes = _dims.size() * sizeof(uint64);
      ::memcpy(dims, &(_dims.front()), nBytes );
    }
        
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual int32 getNumberOfDimensions()
    {
      return this->_dims.size();
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    void setValue(uint64 i, T value)
    {
      this->_data[i] = value;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual int32 getDataType()
    {
      T t = 0x0;
      return H5Lite::HDFTypeForPrimitive(t);
    }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    virtual T* getPointer(uint64 i)
    {
      return (T*)(&(_data[i]) );
    }
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation
// -----------------------------------------------------------------------------
    virtual int32 writeToFile(IDataFilePtr dataFile)
    {
      if (dataFile->getFileId() < 0)
      {
        return -1;
      }
      int32 rank = this->getNumberOfDimensions();
      return H5Lite::writePointerAttribute(dataFile->getFileId(), _datasetPath, _attributeKey, rank, &(_dims.front()), this->getPointer(0) );
    }
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------    
    virtual int32 readFromFile(IDataFilePtr dataFile)
    {
      if (dataFile->getFileId() < 0)
      {
        return -1;
      }
      herr_t err = -1;
      hid_t typeId = -1;
      H5T_class_t attr_type;
      size_t attr_nElements;
      std::string res;
     
      std::vector<uint64> dims;  //Reusable for the loop
      err = H5Lite::getAttributeInfo(dataFile->getFileId(), _datasetPath, _attributeKey, dims, attr_type, attr_nElements, typeId);
      if (err < 0)
      {
        return err;
      }
      err = H5Tclose(typeId);
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
      if (err < 0 ) {std::cout << "Error closing DataType" << std::endl; return err;}
      err = H5Lite::readPointerAttribute(dataFile->getFileId(), _datasetPath, _attributeKey, _data);
      return err;
    } 
    
    /**
     * @brief Prints information about the class to ostream
     * @param os
     * @param indent
     */
    virtual void printSelf(std::ostream &os, int32 indent)
    {
      std::string ind = StringUtils::indent(indent);
      os << ind << "H5AttributeArrayTemplate<T>" << std::endl;
      ind = StringUtils::indent(indent + 1);
      os << ind << "Dataset Path: " << this->getDatasetPath() << std::endl;
      os << ind << "Attribute Key: " << this->getAttributeKey() << std::endl;
      os << ind << "Number of Elements: " << this->getNumberOfElements() << std::endl;
      os << ind << "Number of Dimensions: " << this->getNumberOfDimensions() << std::endl;
      os << ind << "DataType: " << this->getDataType() << std::endl;
      os << ind << "Begin Data" << std::endl;
      os << ind << "{";
      T* data = this->getPointer(0);
      for (uint64 i = 0; i < this->getNumberOfElements(); ++i) 
      {
        os << ind << *data << " ";
        if (i%10 ==0)
        {
          os << std::endl;
        }
      }
      os << ind << "}" << std::endl;
    }
    
    
  protected:  
/**    
 * @brief Protected Constructor
 * @param datasetPath The path to the dataset in the HDF5 file
 * @param attributeKey The string name of the attribute
 * @param numElements The number of elements in the internal array.
 * @param takeOwnership Will the class clean up the memory. Default=true
     */
      H5AttributeArrayTemplate(const std::string &datasetPath, const std::string &attributeKey, int32 numElements, bool takeOwnership = true) :
        _datasetPath(datasetPath),
        _attributeKey(attributeKey),
        _data(NULL),
        _nElements(numElements),
        _ownsData(takeOwnership)
      {    
        _dims.resize(1);
        _dims[0] = numElements;
      }
 
      /**
       * @brief
 * @param datasetPath The path to the dataset in the HDF5 file
 * @param attributeKey The string name of the attribute
 * @param numDims 
 * @param dims
 * @param takeOwnership Will the class clean up the memory. Default=true
 */
      H5AttributeArrayTemplate(const std::string &datasetPath, 
                               const std::string &attributeKey, 
                               int32 numDims, 
                               const uint64* dims, 
                               bool takeOwnership = true) :
        _datasetPath(datasetPath),
        _attributeKey(attributeKey),
        _data(NULL),
        _ownsData(takeOwnership)
      {    
        _dims.resize(numDims);
        ::memcpy( &(_dims.front()), dims, numDims * sizeof(uint64));
        _nElements = 1;
        for(int32 i = 0; i < numDims; ++i)
        {
          _nElements = _nElements * dims[i];
        }
      }
      

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
          int newSize = (this->_nElements > 0 ? this->_nElements : 1);
          this->_data = (T*)malloc(newSize * sizeof(T));
          if (!this->_data)
          {
            std::cout << DEBUG_OUT(logTime) << "Unable to allocate " << newSize << " elements of size " << sizeof(T) << " bytes. " << std::endl;
            return -1;
          }
          this->_nElements = newSize;
          return 1;
        }
        
      /**
       * @brief resizes the internal array to be 'size' elements in length
       * @param size
       * @return Pointer to the internal array
       */
      virtual T* _resizeAndExtend(uint64 size)
        {
          T* newArray;
          uint64 newSize;

          if (size > this->_nElements)
          {
            newSize = size;
          }
          else if (size == this->_nElements) // Requested size is equal to current size.  Do nothing.
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
            memcpy(newArray, this->_data, (newSize < this->_nElements ? newSize : this->_nElements) * sizeof(T));
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
          this->_nElements = newSize;
          this->_data = newArray;
          // This object has now allocated its memory and owns it.
          this->_ownsData = true;

          return this->_data;
        } 
      
    
  private:
    std::string _datasetPath;
    std::string _attributeKey;
    T* _data;
    uint64 _nElements;
    bool _ownsData;
    
    std::vector<uint64> _dims;
  
    H5AttributeArrayTemplate(const H5AttributeArrayTemplate&);    //Not Implemented
    void operator=(const H5AttributeArrayTemplate&); //Not Implemented

};

#endif //_H5AttributeArrayTemplate_h_



