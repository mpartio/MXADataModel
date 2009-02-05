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
#ifndef _IFileReader_h_
#define _IFileReader_h_

#include <MXA/Common/MXATypeDefs.h>

/**
* @class IFileReader IFileReader.h Testing/IFileReader.h
* @brief Defines a basic file reading interface
* @author mjackson
* @date Dec 19, 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT IFileReader
{
  public:
    IFileReader(){};
    virtual ~IFileReader(){};

  /**
   * @brief Method to read data into the implementing class from an IDataFile object
   * @param dataFile The file to read the data from
   * @return Error condition. 0 or positive is success. Negative is failure
   */
    virtual int32 readFromFile(IDataFilePtr dataFile) = 0;

  protected:

  private:
      IFileReader(const IFileReader&);    //Not Implemented
      void operator=(const IFileReader&); //Not Implemented

};

#endif //_IFileReader_h_




