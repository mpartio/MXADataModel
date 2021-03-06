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

#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <string>


/**
* @class MXAAsciiStringData MXAAsciiStringData.h HDF5/MXAAsciiStringData.h
* @brief
* @author mjackson
* @date Jan 4, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT MXAAsciiStringData : public MXAArrayTemplate<uint8_t>
{
  public:

    static IMXAArray::Pointer Create( const std::string &value);

    static MXAAsciiStringData* New( const std::string &value);

    static std::string toStdString(MXAAsciiStringData* strData);

    virtual ~MXAAsciiStringData();

    virtual int32_t getDataType();

    virtual std::string valueToString(char delimiter = ' ');

  protected:
    MXAAsciiStringData( size_t numElements);

  private:

    MXAAsciiStringData(const MXAAsciiStringData&);    //Not Implemented
    void operator=(const MXAAsciiStringData&); //Not Implemented

};

#endif //_H5AsciiAttribute_h_
