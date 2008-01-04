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
#ifndef _H5AsciiAttribute_h_
#define _H5AsciiAttribute_h_

#include <HDF5/H5DataArrayTemplate.hpp>
#include <string>


/**
* @class H5AsciiAttribute H5AsciiAttribute.h HDF5/H5AsciiAttribute.h
* @brief 
* @author mjackson
* @date Jan 4, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT H5AsciiStringData : public H5DataArrayTemplate<uint8>
{
  public:
    
    static MXAAbstractDataPtr New(const std::string &datasetPath, const std::string &value);
   
    static std::string toStdString(H5AsciiStringData* strData);
    
    virtual ~H5AsciiStringData();

    virtual int32 getDataType();
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
    virtual int32 writeToFile (IDataFilePtr dataFile);
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------
    virtual int32 readFromFile(IDataFilePtr dataFile);
        
  protected:  
    H5AsciiStringData(const std::string &datasetPath, mxaIdType numElements);
    
  private:
   
    H5AsciiStringData(const H5AsciiStringData&);    //Not Implemented
    void operator=(const H5AsciiStringData&); //Not Implemented
         
};

#endif //_H5AsciiAttribute_h_
