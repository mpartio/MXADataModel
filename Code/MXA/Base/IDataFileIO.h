///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IFileWriter.h>
#include <MXA/Base/IFileReader.h>
#include <MXA/Base/IDataFile.h>


#ifndef __IDATAFILE_IO_H__
#define __IDATAFILE_IO_H__

//#include "MXA/MXADLLExport.h"
//#include "MXA/MXA.h"

/**
* @class IDataFileIO IDataFileIO.h PathToHeader/IDataFileIO.h
* @brief Pure Virtual class that combines IFileWriter and IFileReader classes
* @author mjackson
* @date Jan 3, 2008
* @version 1.3
*/
class MXA_EXPORT IDataFileIO : public IFileWriter,
                               public IFileReader
{
  public:
    MXA_SHARED_POINTERS(IDataFileIO);

    IDataFileIO() {}
    virtual ~IDataFileIO() {}


    /**
     * @brief Method to write the implementing class to an IDataFile object
     * @param dataFile The file to write the data into
     * @return Error condition. 0 or positive is success. Negative is failure
     */
    virtual int32_t writeToFile (IFileWriter::DataFileType dataFile) = 0;

    /**
     * @brief Method to read data into the implementing class from an IDataFile object
     * @param dataFile The file to read the data from
     * @return Error condition. 0 or positive is success. Negative is failure
     */
    virtual int32_t readFromFile (IFileReader::DataFileType dataFile) = 0;


  protected:

  private:
    IDataFileIO(const IDataFileIO&);    //Not Implemented
    void operator=(const IDataFileIO&); //Not Implemented

};

#endif //__IDATAFILE_IO_H__




