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
#include "MXADataModel/MXADataModel.h"
#include "Interfaces/IDataModelWriter.h"
#include "HDF5/H5IODelegate.h"

//-- STL Headers
#include <iostream>
#include <string>

//-- HDF Headers
#include "hdf5.h"

class H5IODelegate;
class MXANode;


/**
 * @brief
 * @auther
 * @date
 * @version
 * @header
 */
class H5DataModelWriter : public IDataModelWriter
{

public:
  H5DataModelWriter(IFileIODelegate* ioDelegate, MXADataModel* dataModel);
  virtual ~H5DataModelWriter();

  bool writeModelToFile(hid_t fileId);

 // int32 writeAttribute( int32 locationId, MXAAttribute* attribute);

  int32 writeDataModelTemplate(hid_t uniqueId);
  int32 writeDataDimensions(hid_t);
  int32 writeDataRecords(hid_t);
  int32 writeRequiredMetaData(hid_t);
  int32 writeUserMetaData(hid_t);
  //int32 writeSources(hid_t);
    
protected:

  int32 _traverseDataRecords(hid_t gid,  MXADataRecords &records);
  
private:
  H5IODelegate* _ioDelegate;
  MXADataModel* _dataModel;
  
  H5DataModelWriter(const H5DataModelWriter&);   //Copy Constructor Not Implemented
  void operator=(const H5DataModelWriter&); //Copy Assignment Not Implemented
};



#endif /* _HDFWRITER_H_ */

