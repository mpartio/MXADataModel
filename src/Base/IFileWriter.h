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

#include <Common/MXATypeDefs.h>

/**
* @class IFileWriter IFileWriter.h Testing/IFileWriter.h
* @brief 
* @author mjackson
* @date Dec 19, 2007
* @version $Revision: 1.1 $
*/
class IFileWriter 
{
  public:
    IFileWriter(){};
    virtual ~IFileWriter(){};
      
    virtual int32 writeToFile(IDataFilePtr dataFile) = 0;
    
  protected:  
  
  private:
      IFileWriter(const IFileWriter&);    //Not Implemented
      void operator=(const IFileWriter&); //Not Implemented
  
};

#endif //_IFileWriter_h_




