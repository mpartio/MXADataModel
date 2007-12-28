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
#ifndef _H5STRINGATTRIBUTE_H_
#define _H5STRINGATTRIBUTE_H_

#include <Dataset/IAbstractAttribute.h>

/**
 * @class H5StringAttribute H5StringAttribute.h Dataset/H5StringAttribute.h
 * @brief Class to store an attribute that can be represented as a string in an 
 * HDF5 file.
 * @author Mike Jackson
 * @date Dec 28, 2007
 * @version $Revision: 1.1 $
 * @header
 */
class MXA_EXPORT H5StringAttribute : public IAbstractAttribute
{

public:
  /**
   * @brief Static 'New' method that is used to create a new instance of this class
   * @param datasetPath The path to the dataset
   * @param key The name of the attribute
   * @param value The actual value of the attribute. A copy WILL be made
   * @return Boost Shared Pointer wrapped instance of H5StringAttribute
   */
  static IAbstractAttributePtr New( const std::string &datasetPath,
                                    const std::string &key,
                                    const std::string &value);
  
  virtual ~H5StringAttribute();
  
  
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
  virtual int32 writeToFile(IDataFilePtr dataFile);

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
  virtual int32 readFromFile(IDataFilePtr dataFile);

private:
  std::string _value;
  
  /**
   * @brief Protect this constructor so that the user MUST use the Static New Method
   * @param datasetPath The path to the dataset
   * @param key The name of the attribute
   * @param value The actual value of the attribute. A copy WILL be made
   */
  H5StringAttribute(const std::string &path,
                    const std::string &key,
                    const std::string &value);
  
  
  H5StringAttribute(const H5StringAttribute&);   //Copy Constructor Not Implemented
  void operator=(const H5StringAttribute&); //Copy Assignment Not Implemented
};



#endif /* _H5STRINGATTRIBUTE_H_ */

