///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef H5POINTERATTRIBUTE_H_
#define H5POINTERATTRIBUTE_H_


#include <Dataset/IAbstractAttribute.h>
#include <HDF5/H5Lite.h>


/**
* @class H5PointerAttribute H5PointerAttribute.h Dataset/H5PointerAttribute.h
* @brief This class wraps a pointer to an array (single or multidimensional) of data.
* The ownership of the pointer remains with the original source and is the responsibility
* of the original source to make sure the memory is cleaned up. This class is 
* meant to be the quickest way to get an array into an attribute and therefor
* does not take responsibility for the memory clean up of the main array pointer.
* All other memory used/created by this class is properly cleaned up.
* @author Mike Jackson
* @date 12/27/2007
* @version $Revision: 1.1 $
*/
template<typename T>
class MXA_EXPORT H5PointerAttribute : public IAbstractAttribute
{
public:
  /**
   * @brief Static 'New' method that is used to create the attribute
   * @param datasetPath The internal HDF5 path to the dataset
   * @param key The name of the attribute
   * @param attributeDimensions The dimension sizes of the data set
   * @param data A pointer to the data. The data is NOT copied so be careful
   * of scope issues.
   * @return A boost shared_ptr wrapping an H5PointerAttribute instance.
   */
  static IAbstractAttributePtr New(const std::string &datasetPath, 
                                   const std::string &key, 
                                   std::vector<uint64> attributeDimensions, 
                                   T* data)
    {
      H5PointerAttribute<T>* ds = new H5PointerAttribute<T>(datasetPath, key, attributeDimensions, data);
      IAbstractAttributePtr ptr(ds);
      return ptr;
    }
  
  /**
   * @brief Destructor
   */
  virtual ~H5PointerAttribute() {}
  
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
  int32 writeToFile(IDataFilePtr dataFile)
  {
    int32 rank = this->getRank();
    std::vector<uint64> dimVec = this->getAttributeDimensions();
    uint64* dims = &(dimVec.front());
    return H5Lite::writePointerAttribute(dataFile->getFileId(), this->getDatasetPath(), this->getKey(), rank, dims, _data);
  }

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
  int32 readFromFile(IDataFilePtr dataFile)
  {
#warning Implement H5PointerAttribute::readFromFile
    return -1;
  }
  
private:
  T* _data;
  
  /**
   * @brief Protected contstructor.
   * @param datasetPath The internal HDF5 path to the dataset
   * @param key The name of the attribute
   * @param attributeDimensions The dimension sizes of the data set
   * @param data A pointer to the data. The data is NOT copied so be careful
   * of scope issues.
   */
  H5PointerAttribute(const std::string &datasetPath, 
                     const std::string &key,
                   std::vector<uint64> attributeDimensions,
                   T* data) :
    IAbstractAttribute(datasetPath, key, attributeDimensions ),
    _data(data)
    { }
  
  H5PointerAttribute(const H5PointerAttribute&);    // Copy Constructor Not Implemented
  void operator=(const H5PointerAttribute&);  // Operator '=' Not Implemented
};










#endif /*H5POINTERATTRIBUTE_H_*/
