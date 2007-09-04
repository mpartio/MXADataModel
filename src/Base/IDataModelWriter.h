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
#ifndef _IDATAMODELWRITER_H_
#define _IDATAMODELWRITER_H_

#include "Common/DLLExport.h"
#include "Common/MXATypes.h"

#include <boost/shared_ptr.hpp>

class MXADataModel;
class MXAAttribute;
class IFileIODelegate;

/**
 * @brief Abstract class that formally defines the interface that subclasses must
 * implement to be able to write data models to files or other media
 * @author Mike Jackson
 * @date March 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT IDataModelWriter 
{

public:
  IDataModelWriter(IFileIODelegate* ioDelegate, MXADataModel* dataModel){};
  virtual ~IDataModelWriter(){};
  
  /**
  * @brief writes the model to the given file
  * @param fileId The unique ID of a file on the underlying filesystem
  * @return If the write was successful
  */
  virtual int32 writeModelToFile(int32 fileId) = 0;
  
  /**
  * @brief 
  * @param fileId The unique ID of a file on the underlying filesystem
  * @return If the write was successful
  */
  virtual int32 writeDataModelTemplate(int32 uniqueId) = 0;
  
  /**
  * @brief 
  * @param fileId The unique ID of a file on the underlying filesystem
  * @return If the write was successful
  */
  virtual int32 writeDataDimensions(int32 uniqueId) = 0;
  
  /**
  * @brief 
  * @param fileId The unique ID of a file on the underlying filesystem
  * @return If the write was successful
  */
  virtual int32 writeDataRecords(int32 uniqueId) = 0;
  
  /**
  * @brief 
  * @param fileId The unique ID of a file on the underlying filesystem
  * @return If the write was successful
  */
  virtual int32 writeRequiredMetaData(int32 uniqueId) = 0;
  
  /**
  * @brief 
  * @param fileId The unique ID of a file on the underlying filesystem
  * @return If the write was successful
  */
  virtual int32 writeUserMetaData(int32 uniqueId) = 0;
  
protected:
    IDataModelWriter(){};
    
private:
    IDataModelWriter(const IDataModelWriter&);   //Copy Constructor Not Implemented
    void operator=(const IDataModelWriter&); //Copy Assignment Not Implemented
};



#endif /* _IDATAMODELWRITER_H_ */

