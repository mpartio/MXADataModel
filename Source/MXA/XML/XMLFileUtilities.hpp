///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _XML_FILE_UTILITIES_H_
#define _XML_FILE_UTILITIES_H_

#include "MXA/Base/IDataModel.h"
#include "MXA/Core/MXADataModelWriter.hpp"
#include "MXA/Core/MXADataModelReader.hpp"
#include "MXA/XML/XMLStreamReaderDelegate.hpp"
#include "MXA/XML/XMLStreamWriterDelegate.hpp"

#include <iostream>
#include <string>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class MXAXMLModelFileWriter
{
public:

  typedef XMLStreamWriterDelegate<std::ofstream> FileStreamType;
  typedef MXADataModelWriter<FileStreamType>  FileWriterType;

  static int32_t writeModel(IDataModel::Pointer model, const std::string &xmlFile)
  {
    FileStreamType::Pointer dlg = FileStreamType::New();
    std::ofstream& out = *(dlg->getStreamPointer());
    out.open(xmlFile.c_str());
    if (NULL == dlg.get() || false == out.is_open() )
    {
      return -1;
    }

    FileWriterType::Pointer writer =  FileWriterType::New(dlg);

    return  writer->writeModel(model);
  }

protected:
  MXAXMLModelFileWriter() {};
  ~MXAXMLModelFileWriter() {};

private:
  MXAXMLModelFileWriter(const MXAXMLModelFileWriter&);    //Not Implemented
  void operator=(const MXAXMLModelFileWriter&); //Not Implemented
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class MXAXMLModelFileReader
{
public:
  typedef XMLStreamReaderDelegate<std::ifstream> FileStreamType;
  typedef  MXADataModelReader<FileStreamType>    FileReaderType;

  static IDataModel::Pointer readModel( const std::string &xmlFile)
  {
    IDataModel::Pointer model = IDataModel::NullPointer ();
    FileStreamType::Pointer dlg = FileStreamType::New();
    std::ifstream& out = *(dlg->getStreamPointer());
    out.open(xmlFile.c_str());
    if (dlg.get() == NULL || out.is_open() == false)
    {
      return model;
    }
    FileReaderType::Pointer reader = FileReaderType::New(dlg);
    model = reader->readModel();
    return model;
  }
protected:
  MXAXMLModelFileReader() {}
  ~MXAXMLModelFileReader() {}

private:
  MXAXMLModelFileReader(const MXAXMLModelFileReader&);    //Not Implemented
  void operator=(const MXAXMLModelFileReader&); //Not Implemented

};


#endif /* _XML_FILE_UTILITIES_H_ */