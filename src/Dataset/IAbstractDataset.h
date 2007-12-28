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
#ifndef _IAbstractDataset_h_
#define _IAbstractDataset_h_

#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>
#include <Base/IFileWriter.h>
#include <Base/IFileReader.h>

#include <string>
#include <vector>


/**'
 * @class IAbstractDataset IAbstractDataset.h Dataset/IAbstractDataset.h
 * @brief Abstract base class that represents a single data set with attached 
 * attributes. The dimensionality of the dataset is described by the 'datasetDimensions'
 * parameter. The number of elements in the vector is the number of dimensions and 
 * the value of each element is the size of each dimension. For example if you had
 * a 2D image of width 1440 and heigth 900, then the values in the vector would be 
 * 1440,900 and since there are 2 elements in the vector (vector.size() ) then we
 * know that the data is a 2D array. There are no guidelines about which dimension
 * (width or heigth) to put first.
 * @author Mike Jackson
 * @date Dec 28, 2007
 * @version $Revision: 1.1 $
 */
class MXA_EXPORT IAbstractDataset : public IFileWriter, public IFileReader
{

public:
  /**
   * @brief Constructor
   * @param datasetPath The path to the dataset in the HDF5 file
   * @param datasetDimensions The actual sizes of all the dimensions of the data
   */
  IAbstractDataset( const std::string &datasetPath, 
    std::vector<uint64> datasetDimensions);
  
  virtual ~IAbstractDataset();
  
  /**
   * @brief Returns the internal path representation to the dataset
   */
  std::string getDatasetPath();
  
  /**
   * @brief Returns the number of dimensions of the attribute data.
   */
  int32       getRank();
  
  /**
   * @brief Returns the actual sizes of the dimensions of the dataset
   */
  std::vector<uint64>  getDatasetDimensions();
  
  /**
   * @brief Returns the number of Attributes attached to this dataset
   */
  virtual int32 getNumberOfAttributes() = 0;
  
  /**
   * @brief Returns the vector of attributes for this dataset
   */
  virtual IAbstractAttributes  getAttributes() = 0;
  
  /**
   * @brief Adds an attribute to this dataset
   * @param attribute An instance of IAtbstractAttribute wrapped in a Boost shared pointer
   */
  virtual void addAttribute(IAbstractAttributePtr attribute) = 0;
  
  /**
   * @brief Removes an attribute by index.
   * @param index The index of the attribute to remove
   * @return Error Condition: Negative is error
   */
  virtual int32 removeAttribute(int32 index) = 0;
  
  /**
   * @brief Removes an attribute by matching pointer values
   * @param The Attribute to remove
   */
  virtual int32 removeAttribute(IAbstractAttributePtr attribute) = 0;
  
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
    virtual int32 writeToFile(IDataFilePtr dataFile) = 0;

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
    virtual int32 readFromFile(IDataFilePtr dataFile) = 0;
  
private:
  std::string        _datasetPath;
  std::vector<uint64> _datasetDimensions;
 

  IAbstractDataset(const IAbstractDataset&);   //Copy Constructor Not Implemented
  void operator=(const IAbstractDataset&); //Copy Assignment Not Implemented
};



#endif /* _IAbstractDataset_h_ */

