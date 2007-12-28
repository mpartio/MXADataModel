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
#ifndef _IABSTRACTATTRIBUTE_H_
#define _IABSTRACTATTRIBUTE_H_

#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>
#include <Base/IFileWriter.h>
#include <Base/IFileReader.h>
#include <string>

/**
 * @brief Abstract base class for attributes of datasets. This is designed to be
 * subclassed with concrete implementation.
 * @author Mike Jackson
 * @date 12/27/2007
 * @version $Revision: 1.1 $
 * @header
 */
class MXA_EXPORT IAbstractAttribute : public IFileWriter, public IFileReader
{

public:
  /**
   * @brief Constructor
  * @param datasetPath The internal HDF5 path to the dataset
  * @param key The name of the attribute
  * @param attributeDimensions The dimension sizes of the data set
  * of scope issues.
  */
  IAbstractAttribute(const std::string &datasetPath,
                     const std::string &key,
                    std::vector<uint64> attributeDimensions);
  
 virtual ~IAbstractAttribute();
  /**
   * @brief Returns the name of the attribute
   */
 std::string getKey();
 /**
  * @brief Sets the name of the attribute
  * @param key THe new name of the attribute
  */
 void setKey (const std::string &key);

 /**
  * @brief Returns the internal path representation to the dataset
  */
 std::string getDatasetPath();
 
 /**
  * @brief Returns the number of dimensions of the attribute data.
  */
 int64       getRank();
 
 /**
  * @brief Returns the actual sizes of the dimensions of the attribute data
  */
 std::vector<uint64>  getAttributeDimensions();
 
 
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
   virtual int32 writeToFile(IDataFilePtr dataFile) = 0;
// 
// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
   virtual int32 readFromFile(IDataFilePtr dataFile) = 0;
  
private:
  std::string        _key;
  std::string        _datasetPath;
  std::vector<uint64> _datasetDimensions;
  
  IAbstractAttribute(const IAbstractAttribute&);   //Copy Constructor Not Implemented
  void operator=(const IAbstractAttribute&); //Copy Assignment Not Implemented
  
};



#endif /* _IABSTRACTATTRIBUTE_H_ */

