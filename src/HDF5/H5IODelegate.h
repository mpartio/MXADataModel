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
#ifndef _HDF5MODELCODEC_H_
#define _HDF5MODELCODEC_H_

#include "Interfaces/IFileIODelegate.h"


//-- HDF Headers
#include "hdf5.h"

//-- STL Headers
#include <string>
#include <map>


class H5DataModelReader;
class H5DataModelWriter;

/**
 * @brief
 * @author
 * @date
 * @version
 * @header
 */
class  H5IODelegate : public IFileIODelegate
{

public:

  H5IODelegate();
  virtual ~H5IODelegate();

// -----------------------------------------------------------------------------
//  IFileIODelegate Implementation
// -----------------------------------------------------------------------------
  
  /** @brief Writes the Model to a data file
  * @param filename The name of the file (and path if needed) to write the model to
  * @param model The Model to serialize
  * @return MXAError - Zero or Positive values are success. Negative values are errors
  */
  MXATypes::MXAError writeFromModel(std::string fileName,MXADataModel* model, bool closeWhenFinished=false);
  
  /** @brief Deserializes a Datamodel from a file on disk
  * @param fileName The name of the file (including path) to deserialize from
  * @return A pointer to a new'ed MXADataModel Object. It is up to the Programmer to
  * release the Object. If an Error occurs during the reading, then a NULL pointer
  * is returned.
  */
  MXATypes::MXAError readIntoModel(std::string fileName, MXADataModel* model, bool closeWhenFinished=false);

  bool supportedMXAFileVersion(float version);

  bool isMXAFile(std::string fileName);
  
  bool isMXAFile(int32 identifier);

  hid_t openMXAFile(std::string, bool readOnly=false);
  
  hid_t createMXAFile(std::string fileName);
  
  void closeMXAFile();
  
  
  hid_t getOpenFileId() { return _fileId; }
  
  herr_t  createGroupsFromPath(std::string path, int32 parent);
  
protected:
  hid_t _createGroup(hid_t loc_id, std::string group);

private:
    H5IODelegate(const H5IODelegate&);   //Copy Constructor Not Implemented
    void operator=(const H5IODelegate&); //Copy Assignment Not Implemented
    
    hid_t _fileId;
};



#endif /* _HDF5MODELCODEC_H_ */

