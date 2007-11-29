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
#ifndef _IFILEIO_DELEGATE_H
#define _IFILEIO_DELEGATE_H

//MXA Includes
#include <Common/DLLExport.h>
#include <Common/MXATypes.h>
#include <Core/MXADataModel.h>

//-- STL Includes
#include <string>

class MXADataModel;

/**
 * @brief This is the interface that delegates need to implement in order
 * to serialize/deserialize the model from a data file
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.8 $
 *   IMXADataModelCode.h
 */
class MXA_EXPORT IFileIODelegate
{

public:

  virtual ~IFileIODelegate(){};
  
  /** @brief Writes the Model to a data file. If the file does NOT exist, a new
   * file will be created. If the data file does exist and does NOT have a model
   * in the file, then the model will be written. If the data file DOES exist and
   * DOES have an existing model then the model information from the passed in model
   * will overwrite the model in the file.
  * @param fileName The name of the file (and path if needed) to write the model to
  * @param model The Model to serialize
  * @param closeWhenFinished Close the file when this operation is complete
  * @param deleteExisting Delete existing file of the same name and create a new file
  * @return MXAError - Zero or Positive values are success. Negative values are errors
  */
  virtual int32 writeModelToFile(const std::string &fileName, IDataModel* model, bool closeWhenFinished, bool deleteExisting) = 0;
  
  /** @brief Deserializes a Datamodel from a file on disk
  * @param fileName The name of the file (including path) to deserialize from
  * @param model The model to read the information into
  * @param closeWhenFinished Close the file when this operation is complete
  * @param openReadOnly Opens the file in a read only mode. Nothing can be written to the file
  * @return A pointer to a new'ed MXADataModel Object. It is up to the Programmer to
  * release the Object. If an Error occurs during the reading, then a NULL pointer
  * is returned.
  */
  virtual int32 readModelFromFile(const std::string &fileName, 
                                  IDataModel* model, 
                                  bool closeWhenFinished,
                                  bool openReadOnly) = 0;
  
  /**
   * @brief Checks if the file version of the data file is with in the bounds of the library to read/parse the data model
   * @param version The version to check.
   */
  virtual bool supportedMXAFileVersion(float version) = 0;

  /**
   * @brief Is the file describe by an absolute path an MXA based data file
   * @param fileName The data file to check
   * @return True if the file is MXA based
   */
  virtual bool isMXAFile(const std::string &fileName) = 0;

  /**
   * @brief Opens an existing MXA Data File. IE an HDF5 file with the proper data model
   * encoded.
   * @param filename The absolute path of the file to open
   * @param readOnly True if you want the file opened as read only. False if you need to write something to the file
   * @return HDF5 file id
   */
  virtual int32 openMXAFile(std::string filename, bool readOnly=false) = 0;
  
  /**
   * @brief Closes the currently open file
   */
  virtual void  closeMXAFile() = 0;
  
  /**
   * @brief Returns the HDF file id of the currently open file
   */
  virtual int32 getOpenFileId() = 0;
  
  /**
   * @brief Returns the currently open filename as an absolute path
   */
  virtual std::string getOpenFileName() = 0;
  
  
protected:
  IFileIODelegate(){}; // Need this protected
  
private:
  IFileIODelegate(const IFileIODelegate&);   //Copy Constructor Not Implemented
    void operator=(const IFileIODelegate&); //Copy Assignment Not Implemented
    
};



#endif /* _IFILEIO_DELEGATE_H */

