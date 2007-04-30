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

//-- MXA Includes
#include "Headers/DLLExport.h"
#include "Interfaces/IFileIODelegate.h"


//-- HDF Headers
#include "hdf5.h"

//-- STL Headers
#include <string>
#include <map>


class H5DataModelReader;
class H5DataModelWriter;

/**
 * @brief Concrete implementation of the IFileIODelegate interface. This class
 * supports reading/writing of the data model and data to and from HDF5 files.
 * @author Mike Jackson
 * @date March 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT H5IODelegate : public IFileIODelegate
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
  * @param closeWhenFinished
  * @return MXAError - Zero or Positive values are success. Negative values are errors
  */
  int32 writeModelToFile(std::string fileName, MXADataModel* model, bool closeWhenFinished=false);
  
  /** @brief Deserializes a Datamodel from a file on disk
  * @param fileName The name of the file (including path) to deserialize from
  * @return A pointer to a new'ed MXADataModel Object. It is up to the Programmer to
  * release the Object. If an Error occurs during the reading, then a NULL pointer
  * @param closeWhenFinished
  * is returned.
  */
  int32 readModelFromFile(std::string fileName, MXADataModel* model, bool closeWhenFinished=false);

  /**
   * @brief Checks if the file version of the data file is with in the bounds of the library to read/parse the data model
   * @param version The version to check.
   */
  bool supportedMXAFileVersion(float version);

  /**
   * @brief Is the file describe by an absolute path an MXA based data file
   * @param fileName The data file to check
   * @return True if the file is MXA based
   */
  bool isMXAFile(std::string fileName);
  
  /**
   * @brief Is the file identified by the given identifier an MXA Base data file
   */
  bool isMXAFile(int32 identifier);

  /**
   * @brief Opens an existing MXA Data File. IE an HDF5 file with the proper data model
   * encoded.
   * @param filename The absolute path of the file to open
   * @param readOnly True if you want the file opened as read only. False if you need to write something to the file
   * @return HDF5 file id
   */
  hid_t openMXAFile(std::string filename, bool readOnly=false);
  
  /**
   * @brief Creates a new blank HDF5 file
   * @param fileName The path + filename to create. At least the parent path must exist
   * @return HDF5 unique file id.
   */
  hid_t createMXAFile(std::string fileName);
  
  /**
   * @brief Closes the currently open file
   */
  void closeMXAFile();
  
  /**
   * @brief Returns the HDF file id of the currently open file
   */
  hid_t getOpenFileId() { return _fileId; }
  
  /**
   * @brief Returns the currently open filename as an absolute path
   */
  std::string getOpenFileName() { return this->_openFile; }

  
protected:
  


private:
    H5IODelegate(const H5IODelegate&);   //Copy Constructor Not Implemented
    void operator=(const H5IODelegate&); //Copy Assignment Not Implemented
    
    hid_t _fileId;
    std::string _openFile;
};



#endif /* _HDF5MODELCODEC_H_ */

