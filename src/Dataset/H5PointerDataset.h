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
#ifndef H5POINTERDATASET_H_
#define H5POINTERDATASET_H_

#include <Dataset/IAbstractDataset.h>
#include <HDF5/H5Lite.h>


/**
* @class H5PointerDataset H5PointerDataset.h Dataset/H5PointerDataset.h
* @brief This class wraps a pointer to data that is to be written to an HDF5 datafile.
* This class does NOT make a copy of the data so be sure to scope your pointer
* appropriately. This is a templated class designed for primitive types.
* @author Mike Jackson
* @date 12/27/2007
* @version $Revision: 1.1 $
*/
template<typename T>
class MXA_EXPORT H5PointerDataset : public IAbstractDataset
{
public:
  /**
   * @brief Static constructor
   * @param datasetPath The path to the dataset in the HDF5 file
   * @param datasetDimensions The actual sizes of all the dimensions of the data
   * @param data The pointer to the data set.
   * @return Boost Shared pointer wrapped instance of H5PointerDataset
   */
	static IAbstractDatasetPtr New(const std::string &datasetPath, 
	                              std::vector<uint64> datasetDimensions,
	                              T* data)
	{
	  H5PointerDataset<T>* ds = new H5PointerDataset(datasetPath, datasetDimensions, data);
	  IAbstractDatasetPtr ptr (ds);
	  return ptr;
	}
	
	
	virtual ~H5PointerDataset() {}
	 
  /**
   * @brief Returns the number of Attributes attached to this dataset
   */
  int32 getNumberOfAttributes()
  {
    return _attributes.size();
  }
  
	/**
	 * @brief Returns the vector of attributes for this dataset
	 */
  IAbstractAttributes  getAttributes() { return this->_attributes; }
  
  /**
   * @brief Adds an attribute to this dataset
   * @param attribute An instance of IAtbstractAttribute wrapped in a Boost shared pointer
   */
  void addAttribute(IAbstractAttributePtr attribute)
  {
    this->_attributes.push_back(attribute);
  }
  
  /**
   * @brief Removes an attribute by index.
   * @param index The index of the attribute to remove
   * @return Error Condition: Negative is error
   */
  int32 removeAttribute(int32 index)
  {
    if (index < 0 || (uint32)index >= _attributes.size() )
    {
      return 0;
    }
    int32 err = -1;
    int32 i = 0;
    for (IAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter ) 
    {
      if (i == index)
      {
        _attributes.erase(iter);
        err = 1;
        break;
      }
      ++i;
    }
    return err;
  }
  
  /**
   * @brief Removes an attribute by matching pointer values
   * @param The Attribute to remove
   */
  int32 removeAttribute(IAbstractAttributePtr attribute)
  {
    int32 err = -1;
    IAbstractAttribute* attr = NULL;
    for (IAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter ) 
    {
      attr = (*iter).get();
      if (attribute.get() == attr)
      {
        _attributes.erase(iter);
        err = 1;
        break;
      }
    }
    return err;
  }
    
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
  int32 writeToFile(IDataFilePtr dataFile)
  {
    int32 err = H5Utilities::createGroupsForDataset(getDatasetPath(), dataFile->getFileId() );
    if (err < 0)
    {
      //    std::cout << "H5StringDataset::writeToFile - Error Creating dataset to file '" << dataFile->getFilename() << "'" << std::endl;
      return err;
    }
    int32 rank = this->getRank();
    std::vector<uint64> dimVec = this->getDatasetDimensions();
    uint64* dims = &(dimVec.front());
    err = H5Lite::writePointerDataset(dataFile->getFileId(), this->getDatasetPath(), rank, dims, _data);
    if(err < 0)
    {
      return err;
    }
    IAbstractAttribute* attr= NULL;
    for (IAbstractAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter)
    {
      attr = (*iter).get();
      err = attr->writeToFile(dataFile);
      if (err < 0)
      {
        return err;
      }
    }
    return 1;
  }

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
  int32 readFromFile(IDataFilePtr dataFile)
  {
#warning Implement H5PointerDataset::readFromFile
    return -1;
  }
  
private:
  T* _data;
  IAbstractAttributes _attributes;
  
  
  H5PointerDataset(const std::string &datasetPath, 
                   std::vector<uint64> datasetDimensions,
                   T* data) :
    IAbstractDataset(datasetPath, datasetDimensions ),
    _data(data)
    { }
  
  H5PointerDataset(const H5PointerDataset&);    // Copy Constructor Not Implemented
  void operator=(const H5PointerDataset&);  // Operator '=' Not Implemented
};

#endif /*H5POINTERDATASET_H_*/
