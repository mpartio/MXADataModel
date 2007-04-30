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
#ifndef _HDFWRITER_H_
#define _HDFWRITER_H_

// MXA Includes
#include "Headers/DLLExport.h"
#include "MXADataModel/MXADataModel.h"
#include "Interfaces/IDataModelWriter.h"
#include "HDF5/H5IODelegate.h"

//-- STL Headers
#include <iostream>
#include <string>

//-- HDF Headers
#include "hdf5.h"

//Forward Declare classes
class H5IODelegate;
class MXANode;


/**
 * @brief Writes the DataModel to an HDF5 file
 * @author Mike Jackson
 * @date March 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT H5DataModelWriter : public IDataModelWriter
{

public:
  H5DataModelWriter(IFileIODelegate* ioDelegate, MXADataModel* dataModel);
  virtual ~H5DataModelWriter();

  /**
   * @brief Writes the Data Model to the HDF5 file
  * @param fileId HDF5 FileId to write to
   * @return true on success
   */
  int32 writeModelToFile(hid_t fileId);

  /**
   * @brief Writes the Data Model skeleton groups
  * @param uniqueId HDF5 FileId to write to
   * @return
   */
  int32 writeDataModelTemplate(hid_t uniqueId);
  
  /**
   * @brief Writes the Data Dimensions from the Model
  * @param fileId HDF5 FileId to write to
  * @return
  */
  int32 writeDataDimensions(hid_t fileId);
  
  /**
  * @brief Writes the Data Records
  * @param fileId HDF5 FileId to write to
  * @return
  */
  int32 writeDataRecords(hid_t fileId);
  
  /**
   * @brief Writes the Required MetaData
  * @param fileId HDF5 FileId to write to
  * @return
  */
  int32 writeRequiredMetaData(hid_t fileId);
  
  /**
   * @brief Writes the User Meta Data
  * @param fileId HDF5 FileId to write to
  * @return
  */
  int32 writeUserMetaData(hid_t fileId);

    
protected:
  
  /**
   * @brief Walks the Data records hierarchy
* @param gid
* @param records
* @return
*/
  int32 _traverseDataRecords(hid_t gid,  MXADataRecords &records);
  
private:
  H5IODelegate* _ioDelegate;
  MXADataModel* _dataModel;
  
  H5DataModelWriter(const H5DataModelWriter&);   //Copy Constructor Not Implemented
  void operator=(const H5DataModelWriter&); //Copy Assignment Not Implemented
};



#endif /* _HDFWRITER_H_ */

