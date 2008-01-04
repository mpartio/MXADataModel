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
#ifndef _H5AsciiStringAttribute_h_
#define _H5AsciiStringAttribute_h_

#include <Common/MXATypeDefs.h>
#include <Base/IDataFile.h>
#include <HDF5/H5AttributeArrayTemplate.hpp>
#include <string>

/**
* @class H5AsciiStringAttribute H5AsciiStringAttribute.h HDF5/H5AsciiStringAttribute.h
* @brief 
* @author mjackson
* @date Jan 4, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT H5AsciiStringAttribute : public H5AttributeArrayTemplate<uint8>
{
  public:
    static MXAAbstractAttributePtr New(const std::string &datasetPath, 
                                       const std::string &attributeKey,
                                       const std::string &value);
   
    static std::string toStdString(H5AsciiStringAttribute* strData);

    virtual ~H5AsciiStringAttribute();

    virtual int32 getDataType();
    
    virtual const char* getCharPointer(mxaIdType index);
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
    virtual int32 writeToFile (IDataFilePtr dataFile);
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------
    virtual int32 readFromFile(IDataFilePtr dataFile);
    
  protected:  
    H5AsciiStringAttribute(const std::string &datasetPath, 
                           const std::string &attributeKey,
                           mxaIdType numElements);
  private:
      H5AsciiStringAttribute(const H5AsciiStringAttribute&);    //Not Implemented
      void operator=(const H5AsciiStringAttribute&); //Not Implemented
  
};

#endif //_H5AsciiStringAttribute_h_



