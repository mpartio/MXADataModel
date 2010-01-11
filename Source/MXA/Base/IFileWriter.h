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
#ifndef _IFileWriter_h_
#define _IFileWriter_h_

#include <MXA/Common/MXATypeDefs.h>

class IDataFile;

/**
* @class IFileWriter IFileWriter.h Testing/IFileWriter.h
* @brief Defines a basic file writing interface
* @author mjackson
* @date Dec 19, 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT IFileWriter
{
  public:
    MXA_SHARED_POINTERS(IFileWriter)
    typedef boost::shared_ptr<IDataFile>    DataFileType;
    
    IFileWriter(){};
    virtual ~IFileWriter(){};

    /**
     * @brief Method to write the implementing class to an IDataFile object
     * @param dataFile The file to write the data into
     * @return Error condition. 0 or positive is success. Negative is failure
     */
    virtual int32_t writeToFile(DataFileType dataFile) = 0;

  protected:

  private:
      IFileWriter(const IFileWriter&);    //Not Implemented
      void operator=(const IFileWriter&); //Not Implemented

};

#endif //_IFileWriter_h_




