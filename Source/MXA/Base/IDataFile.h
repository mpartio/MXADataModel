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
#ifndef IMXAFILE_H_
#define IMXAFILE_H_

#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>

#include <string>

/**
* @class IDataFile IDataFile.h Base/IDataFile.h
* @brief Represents an MXAData File object and the various operations that can
* be performed on that file
* @author mjackson
* @date Dec 14, 2007
* @version $Revision: 1.1 $
*/
class MXA_EXPORT IDataFile
{
  public:
    explicit IDataFile(const std::string &filename) {};
    virtual ~IDataFile() {};


    /**
     * @brief returns the name of the file that is actively being read from or
     * written to.
     */
    virtual std::string getFilename() = 0;

    /**
     * @brief returns the DataModel as read from the data file
     * @return A DataModelPtr oject. A NULL wrapped pointer is perfectly valid
     */
    virtual IDataModelPtr getDataModel() = 0;

// -----------------------------------------------------------------------------
//  Basic File Operations

    /**
     * @brief Creates a new data file, first deleting and existing file with the same name
     * @return Error code < 0 is error. 0 or positive is Success
     */
    virtual int32 createFile() = 0;

    /**
     * @brief Opens an existing data file.
     * @param readOnly Open the file in Read-Only Mode
     * @return Error code < 0 is error. 0 or positive is Success
     */
    virtual int32 openFile(bool readOnly) = 0;

    /**
     * @brief Closes the data file. Any further access to the file will fail.
     * @return Error code < 0 is error. 0 or positive is Success
     */
    virtual int32 closeFile(bool saveModel) = 0;

    /**
     * @brief Checks if the file version of the data file is with in the bounds of the library to read/parse the data model
     */
    virtual float supportedMXAFileVersion() = 0;

    /**
     * @brief Returns if the file is currently in the "open" state. If this file
     * is OPEN then file operations (Read/Write data or model) can be performed
     * on the file
     */
    virtual bool isFileOpen() = 0;

    /**
     * @brief Was the file opened in ReadOnly mode
     */
    virtual bool isReadOnly() = 0;

    /**
     * @brief Returns the unique identifier for the open file
     * @return
     */
    virtual int32 getFileId() = 0;

// -----------------------------------------------------------------------------
//  Data Model Related Methods
    /**
     * @brief Writes the Data Model into the underlying data file overwriting any
     * existing model. This may have adverse effects on the ability to retrieve
     * data from the data file if the data model differs in structure from the original
     * data model
     * @return Error code < 0 is error. 0 or positive is Success
     */
    virtual int32 saveDataModel() = 0;


// -----------------------------------------------------------------------------
//  Writing data related methods

    virtual int32 writeData ( const IDatasetPtr dataset) = 0;

    virtual int32 readData (const IDatasetPtr dataset) = 0;


  protected:

  private:
      IDataFile(const IDataFile&);    //Not Implemented
      void operator=(const IDataFile&); //Not Implemented

};


#endif /*IMXAFILE_H_*/
