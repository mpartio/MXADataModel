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
#ifndef _H5STRINGDATASET_CPP_
#define _H5STRINGDATASET_CPP_

#include "IAbstractDataset.h"

/**
 * @brief This class represents an HDF5 data set that is stored as a string
 * @author Mike Jackson
 * @date Dec 28, 2007
 * @version $Revision: 1.1 $
 * @header Dataset/H5StringDataset.h
 */
class MXA_EXPORT H5StringDataset : public IAbstractDataset
{

public:
  /**
   * Static 'New' method used to create this class.
   * @param datasetPath The complete path to the dataset in the HDF5 file.
   * @param value The actual string to be stored in the HDF5 file. A copy of the
   * string is made and stored in this class.
   * @return A Boost Shared Pointer wrapped instance of H5StringDataset
   */
  static IAbstractDatasetPtr New( const std::string &datasetPath, 
                                  const std::string &value);

  virtual ~H5StringDataset();

  /**
   * @brief Returns the vector of attributes for this dataset
   */
  IAbstractAttributes  getAttributes();
  
  /**
   * @brief Returns the number of Attributes attached to this dataset
   */
  int32 getNumberOfAttributes();
  
  /**
   * @brief Adds an attribute to this dataset
   * @param attribute An instance of IAtbstractAttribute wrapped in a Boost shared pointer
   */
  void addAttribute(IAbstractAttributePtr attribute);
  
  /**
   * @brief Removes an attribute by index.
   * @param index The index of the attribute to remove
   * @return Error Condition: Negative is error
   */
  int32 removeAttribute(int32 index);
  
  /**
   * @brief Removes an attribute by matching pointer values
   * @param The Attribute to remove
   */
  int32 removeAttribute(IAbstractAttributePtr attribute);

    
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
  int32 writeToFile(IDataFilePtr dataFile);

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
  int32 readFromFile(IDataFilePtr dataFile);
    
    
private:
  std::string _value;
  IAbstractAttributes _attributes;
   
   
  // Protect this constructor so that the user MUST use the Static New Method
  H5StringDataset(const std::string &datasetPath, const std::string &value);
  
  H5StringDataset(const H5StringDataset&);   //Copy Constructor Not Implemented
  void operator=(const H5StringDataset&); //Copy Assignment Not Implemented
  
};



#endif /* _H5STRINGDATASET_CPP_ */

