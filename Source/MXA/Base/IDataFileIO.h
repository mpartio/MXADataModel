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
#ifndef _IDataFileIO_h_
#define _IDataFileIO_h_

#include <MXA/Common/DLLExport.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Base/IFileWriter.h>
#include <MXA/Base/IFileReader.h>

/**
* @class IDataFileIO IDataFileIO.h PathToHeader/IDataFileIO.h
* @brief Pure Virtual class that combines IFileWriter and IFileReader classes
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT IDataFileIO  : public IFileWriter, public IFileReader
{
  public:
    IDataFileIO() {}
    virtual ~IDataFileIO() {}


    /**
     * @brief Method to write the implementing class to an IDataFile object
     * @param dataFile The file to write the data into
     * @return Error condition. 0 or positive is success. Negative is failure
     */
    virtual int32 writeToFile (IDataFilePtr dataFile) = 0;

    /**
     * @brief Method to read data into the implementing class from an IDataFile object
     * @param dataFile The file to read the data from
     * @return Error condition. 0 or positive is success. Negative is failure
     */
    virtual int32 readFromFile (IDataFilePtr dataFile) = 0;


  protected:

  private:
    IDataFileIO(const IDataFileIO&);    //Not Implemented
    void operator=(const IDataFileIO&); //Not Implemented

};

#endif //_IDataFileIO_h_




