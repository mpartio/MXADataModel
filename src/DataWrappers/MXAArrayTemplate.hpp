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
#ifndef _MXAArrayTemplate_h_
#define _MXAArrayTemplate_h_

#include <Base/IMXAArray.h>
#include <Common/MXATypeDefs.h>

#ifdef MXA_USE_HDF5_PRIMITIVE_TYPES
#include <HDF5/H5Lite.h>
#endif
#include <Utilities/StringUtils.h>

// STL Includes
#include <sstream>

/**
* @class MXAArrayTemplate MXAArrayTemplate.hpp PathToHeader/MXAArrayTemplate.hpp
* @brief
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.2 $
*/
template<typename T>
class MXAArrayTemplate : public IMXAArray
{
  public:

/**
 * @brief Static constructor
 * @param numElements The number of elements in the internal array.
 * @return Boost::Shared_Ptr wrapping an instance of MXAArrayTemplateTemplate<T>
 */
    static IMXAArrayPtr CreateArray( uint64 numElements)
    {
      MXAArrayTemplate<T>* d = new MXAArrayTemplate<T>( numElements, true);
      if ( d->_allocate() < 0)
      {  // Could not allocate enough memory, reset the pointer to null and return
        d = NULL;
      }
      IMXAArrayPtr ptr ( static_cast<IMXAArray*>(d) );
      return ptr;
    }

/**
 * @brief Creates an IMXAArrayPtr object for the supplied arguments.
 * @param nDims Number of dimensions
 * @param dims Size of each dimension
 * @return Boost::Shared_Ptr wrapping an instance of MXAArrayTemplateTemplate<T>
 */
    static IMXAArrayPtr CreateMultiDimensionalArray(int32 nDims, const uint64* dims)
    {

      MXAArrayTemplate<T>* d = new MXAArrayTemplate<T>( nDims, dims, true);
      if ( d->_allocate() < 0)
      {  // Could not allocate enough memory, reset the pointer to null and return
        d = NULL;
      }
      IMXAArrayPtr ptr ( static_cast<IMXAArray*>(d) );
      return ptr;
    }


/**
 * @brief Creates an Attribute from a single value
 * @param value The value to store in the attribute
 * @return Boost::Shared_Ptr wrapping an instance of MXAArrayTemplateTemplate<T>
 */
    static IMXAArrayPtr CreateSingleValueArray( T value)
    {
      MXAArrayTemplate<T>* d = new MXAArrayTemplate<T>(1, true);
      if ( d->_allocate() < 0)
      {  // Could not allocate enough memory, reset the pointer to null and return
        d = NULL;
      }
      d->setValue(0, value);
      IMXAArrayPtr ptr ( static_cast<IMXAArray*>(d) );
      return ptr;
    }


/**
 * @brief Static construction of MXAArrayTemplate objects. YOU are
 * responsible for cleaning up the memory that this method creates.
 * @param numElements The number of elements in the internal array.
 * @return
 */
    static MXAArrayTemplate<T>* New( uint64 numElements)
    {
      MXAArrayTemplate<T>* ptr = new MXAArrayTemplate<T>( numElements, true);
      if (ptr->_allocate() < 0)
      { // Could not allocate enough memory, reset the pointer to null and return
        ptr = 0x0;
      }
      return ptr;
    }

    /**
     * @brief Creates a MultiDimensional Array allocating the memory immediately
     * @param nDims The number of dimensions
     * @param dims The size of each dimension
     * @return Pointer to Object or NULL if there was an error creating the object.
     */
    static MXAArrayTemplate<T>* New(int32 nDims, const uint64* dims)
    {
      MXAArrayTemplate<T>* d = new MXAArrayTemplate<T>( nDims, dims, true);
      if ( d->_allocate() < 0)
      {  // Could not allocate enough memory, reset the pointer to null and return
        d = NULL;
      }
      return d;
    }

/**
 * @brief Destructor
 */
    virtual ~MXAArrayTemplate()
    {
      //std::cout << "~MXAArrayTemplateTemplate" << std::endl;
      if ((NULL != this->_data) && (true == this->_ownsData))
      {
        free(this->_data);
      }
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
      this->_dims[0] = _nElements;
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
      if (i >= this->getNumberOfElements() )
      {
        return 0x0;
      }
      return (void*)(&(_data[i]) );
    }

/**
 * @brief Returns the value for a given index
 * @param i The index to return the value at
 * @return The value at index i
 */
    virtual T getValue(uint64 i)
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
#ifdef MXA_USE_HDF5_PRIMITIVE_TYPES
      return H5Lite::HDFTypeForPrimitive(t);
#else
#error This needs to be implemented for the NON-NDF5 case
#endif
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
    virtual T* getPointer(uint64 i)
    {
      return (T*)(&(_data[i]) );
    }
#if 0
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
#endif

    /**
     * @brief Prints information about the class to ostream
     * @param os
     * @param indent
     */
    virtual void printSelf(std::ostream &os, int32 indent)
    {
      std::string ind = StringUtils::indent(indent);
      os << ind << "MXAArrayTemplate<T>" << std::endl;
      ind = StringUtils::indent(indent + 1);
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

    /**
     * @brief Converts the data array into a string delimited by the supplied
     * delimiter.
     * @param delimiter The delimiter to use between each value. Default is a single space
     * @return The generated string
     */
    virtual std::string valueToString(char delimiter = ' ')
    {
      std::stringstream sstream;
      uint64 limit = _nElements - 1;
      for(uint64 i = 0; i < _nElements; ++i)
      {
        if (sizeof(T) != 1 )
         {
          sstream  << _data[i];
         }
         else
         {
           sstream  << static_cast<int32>(_data[i]);
         }
        if (i < limit)
        {
          sstream << delimiter;
        }
      }
      return sstream.str();
    }

  protected:

  /**
   * @brief Protected Constructor
   * @param numElements The number of elements in the internal array.
   * @param takeOwnership Will the class clean up the memory. Default=true
     */
      MXAArrayTemplate(int32 numElements,
                               bool takeOwnership = true) :
        _data(NULL),
        _nElements(numElements),
        _ownsData(takeOwnership)
      {
        _dims.resize(1);
        _dims[0] = numElements;
      }

  /**
   * @brief Constructor used to create an MXAArrayTemplate class that has multiple dimensions.
   * @param numDims The number of dimensions to the data set.
   * @param dims The actual values of the dimensions.
   * @param takeOwnership Will the class clean up the memory. Default=true
   */
      MXAArrayTemplate(int32 numDims,
                       const uint64* dims,
                       bool takeOwnership = true) :
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
     * @brief Allocates the memory needed for this class
     * @return 1 on success, -1 on failure
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
          this->_dims[0] = this->_nElements;
          this->_data = newArray;
          // This object has now allocated its memory and owns it.
          this->_ownsData = true;

          return this->_data;
        }


  private:

    T* _data;
    uint64 _nElements;
    bool _ownsData;

    std::vector<uint64> _dims;

    MXAArrayTemplate(const MXAArrayTemplate&);    //Not Implemented
    void operator=(const MXAArrayTemplate&); //Not Implemented

};

#endif //_MXAArrayTemplate_h_



