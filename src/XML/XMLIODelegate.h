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
#ifndef XMLIODELEGATE_H_
#define XMLIODELEGATE_H_

#include <Common/MXATypeDefs.h>
#include <Common/DLLExport.h>
#include <Base/IFileIODelegate.h>

#include <string>

/**
 * @brief Concrete implementation of the IFileIODelegate interface. This class
 * supports reading/writing of the data model and data to and from XML files.
 * @author Mike Jackson
 * @date June 2007
 * @version $Revision: 1.9 $
 *  
 */
class MXA_EXPORT XMLIODelegate : public IFileIODelegate
{
public:
	XMLIODelegate();
	virtual ~XMLIODelegate();
	
	// -----------------------------------------------------------------------------
	//  IFileIODelegate Implementation
	// -----------------------------------------------------------------------------
	
	/** @brief Writes the Model to a data file
	  * @param fileName The name of the file (and path if needed) to write the model to
	  * @param model The Model to serialize
	  * @param closeWhenFinished
	  * @param deleteExisting
	  * @return MXAError - Zero or Positive values are success. Negative values are errors
	  */
	  int32 writeModelToFile(const std::string &fileName, IDataModelPtr model, bool closeWhenFinished, bool deleteExisting) ;
	  
	  /** @brief Deserializes a Datamodel from a file on disk
	  * @param fileName The name of the file (including path) to deserialize from
	  * @param model The model to read the information into
	  * @param closeWhenFinished
	  * @param openReadOnly
	  * @return A pointer to a new'ed MXADataModel Object. It is up to the Programmer to
	  * release the Object. If an Error occurs during the reading, then a NULL pointer
	  * is returned.
	  */
	  int32 readModelFromFile(const std::string &fileName, IDataModelPtr model, bool closeWhenFinished, bool openReadOnly) ;
	  
	  /**
	   * @brief Checks if the file version of the data file is with in the bounds of the library to read/parse the data model
	   * @param version The version to check.
	   */
	  bool supportedMXAFileVersion(float version) ;

	  /**
	   * @brief Is the file describe by an absolute path an MXA based data file
	   * @param fileName The data file to check
	   * @return True if the file is MXA based
	   */
	  bool isMXAFile(const std::string &fileName) ;
	  
	  /**
	   * @brief Is the file identified by the given identifier an MXA Base data file
	   */
	  bool isMXAFile(int32 identifier) ;

	  /**
	   * @brief Opens an existing MXA Data File. IE an HDF5 file with the proper data model
	   * encoded.
	   * @param filename The absolute path of the file to open
	   * @param readOnly True if you want the file opened as read only. False if you need to write something to the file
	   * @return HDF5 file id
	   */
	  int32 openMXAFile(std::string filename, bool readOnly=false) ;
	 
	  /**
	   * @brief Closes the currently open file
	   */
	  void  closeMXAFile() ;
	  
	  /**
	   * @brief Returns the HDF file id of the currently open file
	   */
	  int32 getOpenFileId() ;
	  
	  /**
	   * @brief Returns the currently open filename as an absolute path
	   */
	  std::string getOpenFileName() ;
	
private:
    XMLIODelegate(const XMLIODelegate&);   //Copy Constructor Not Implemented
    void operator=(const XMLIODelegate&); //Copy Assignment Not Implemented
    
    std::string _openFile; // The value of the currently opened file
    int32 _fileId;  // The value of the currently opened file id - Not used for XML
};

#endif /*XMLIODELEGATE_H_*/
