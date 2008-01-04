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

#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>
#include <Base/IFileWriter.h>
#include <Base/IFileReader.h>

/**
* @class IDataFileIO IDataFileIO.h PathToHeader/IDataFileIO.h
* @brief 
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT IDataFileIO  : public IFileWriter, public IFileReader
{
  public:
    IDataFileIO() {}
    virtual ~IDataFileIO() {}
    
    virtual int32 writeToFile (IDataFilePtr dataFile) = 0;
    virtual int32 readFromFile (IDataFilePtr dataFile) = 0;
    
    
  protected:  
  
  private:
    IDataFileIO(const IDataFileIO&);    //Not Implemented
    void operator=(const IDataFileIO&); //Not Implemented
  
};

#endif //_IDataFileIO_h_




