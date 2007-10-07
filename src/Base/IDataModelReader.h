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

#ifndef IDATAMODELREADER_H_
#define IDATAMODELREADER_H_

#include <Common/DLLExport.h>
#include <Common/MXATypes.h>

class MXADataModel;
class IMXAFileIODelegate;

/**
 * @brief Interface that defines what methods a class must implement to write
 * a data model to a file.
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.4 $
 *  
 */
class MXA_EXPORT IDataModelReader 
{

public:
  IDataModelReader(IMXAFileIODelegate* ioDelegate, MXADataModel* dataModel){};
  virtual ~IDataModelReader(){};

  /**
  * @brief Reads the data model from the file
  * @param locId The HDF5 ID or other unique ID of the file
  * @return Standard Error Condition
  */
  virtual int32 readDataModel(int32 locId) = 0;

  
protected:
  IDataModelReader(){};
    
private:
    IDataModelReader(const IDataModelReader&);   //Copy Constructor Not Implemented
    void operator=(const IDataModelReader&); //Copy Assignment Not Implemented
};

#endif /*IDATAMODELREADER_H_*/
