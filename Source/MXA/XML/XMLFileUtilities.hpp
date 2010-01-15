///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _XML_FILE_UTILITIES_H_
#define _XML_FILE_UTILITIES_H_

#include <MXA/Common/MXATypeDefs.h>
#include "MXA/Base/IDataModel.h"
#include "MXA/Core/MXADataModelWriter.hpp"
#include "MXA/Core/MXADataModelReader.hpp"
#include "MXA/XML/XMLStreamReaderDelegate.hpp"
#include "MXA/XML/XMLStreamWriterDelegate.hpp"

#include <iostream>
#include <string>

/**
* @class MXAXMLModelFileWriter MXAXMLModelFileWriter.h MXA/XML/XMLFileUtilities.hpp
* @brief This class is a simple wrapper class around the XML file writing functionality to reduce the
* amount of code required to write a Data Model to XML
* @author Michael A. Jackson for BlueQuartz Software
* @date Jan 15, 2010
* @version 1.0
*/
class MXAXMLModelFileWriter
{
public:

  typedef XMLStreamWriterDelegate<std::ofstream> FileStreamType;
  typedef MXADataModelWriter<FileStreamType>  FileWriterType;

  virtual ~MXAXMLModelFileWriter() {}

  /**
   * @brief Writes an IDataModel object to an XML file
   * @param model Valid file to write
   * @param xmlFile XML file to write to
   * @return positive on success, negative on error
   */
  static int32_t writeModel(IDataModel::Pointer model, const std::string &xmlFile)
  {
    if (NULL == model.get() ) { return -1; }
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
  MXAXMLModelFileWriter() {}

private:
  MXAXMLModelFileWriter(const MXAXMLModelFileWriter&);    //Not Implemented
  void operator=(const MXAXMLModelFileWriter&); //Not Implemented
};

/**
* @class MXAXMLModelFileReader MXAXMLModelFileReader.h MXA/XML/XMLFileUtilities.hpp
* @brief This class is a simple wrapper class around the XML file reading functionality to reduce the
* amount of code required to read a Data Model from XML
* @author Michael A. Jackson for BlueQuartz Software
* @date Jan 15, 2010
* @version 1.0
*/
class MXAXMLModelFileReader
{
public:
  typedef XMLStreamReaderDelegate<std::ifstream> FileStreamType;
  typedef  MXADataModelReader<FileStreamType>    FileReaderType;
  virtual ~MXAXMLModelFileReader() {}

  /**
   * @brief Reads a Data Model from the XML file. If there is an error during the read a NULL wrapped IDataModel will
   * be returned.
   * @param xmlFile The XML File containing the DataModel
   * @return A valid IDataModel* pointer wrapped in a boost::shared_ptr<>
   */
  static IDataModel::Pointer readModel( const std::string &xmlFile, bool returnValidModels = true)
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
    reader->setReturnValidModels(returnValidModels);
    model = reader->readModel();
    return model;
  }

protected:
  MXAXMLModelFileReader() {}

private:
  MXAXMLModelFileReader(const MXAXMLModelFileReader&);    //Not Implemented
  void operator=(const MXAXMLModelFileReader&); //Not Implemented

};

/**
* @class MXAXMLModelStringWriter MXAXMLModelStringWriter MXA/XML/XMLFileUtilities.hpp
* @brief This class is a wrapper class to write the data model xml to a std::string and return
* the string as a boost::shared_ptr
* @author Michael A. Jackson for BlueQuartz Software
* @date Jan 15, 2010
* @version 1.0
*/
class MXAXMLModelStringWriter
{
public:

  typedef XMLStreamWriterDelegate<std::ostringstream> StreamType;
  typedef MXADataModelWriter<StreamType>  WriterType;

  virtual ~MXAXMLModelStringWriter() {}

  /**
   * @brief Writes an IDataModel object to an XML file
   * @param model Valid file to write
   * @return std::string wrapped in a boost::shared_ptr (MXASharedString typedef @see MXATypeDefs.h). If there
   * was an error, then the wrapped pointer will be NULL.
   */
  static MXASharedString writeModel(IDataModel::Pointer model)
  {
    MXASharedString nullString = MXASharedString(static_cast<std::string*>(NULL));

    if (NULL == model.get() ) { return nullString; }
    StreamType::Pointer dlg = StreamType::New();
    std::ostringstream& out = *(dlg->getStreamPointer());
    if (NULL == dlg.get()  )
    {
      return nullString;
    }
    WriterType::Pointer writer =  WriterType::New(dlg);
    int32_t err = writer->writeModel(model);
    if (err < 0) { return nullString; }
    MXASharedString xmlString(new std::string);
    xmlString->append(out.str());
    return xmlString;
  }

protected:
  MXAXMLModelStringWriter() {}

private:
  MXAXMLModelStringWriter(const MXAXMLModelStringWriter&);    //Not Implemented
  void operator=(const MXAXMLModelStringWriter&); //Not Implemented
};

/**
* @class MXAXMLModelStringReader MXAXMLModelStringReader MXA/XML/XMLFileUtilities.hpp
* @brief This class is a simple wrapper class around the XML string stream reading functionality to reduce the
* amount of code required to read a Data Model from XML string
* @author Michael A. Jackson for BlueQuartz Software
* @date Jan 15, 2010
* @version 1.0
*/
class MXAXMLModelStringReader
{
public:
  typedef XMLStreamReaderDelegate<std::istringstream> StreamType;
  typedef  MXADataModelReader<StreamType>    ReaderType;
  virtual ~MXAXMLModelStringReader() {}

  /**
   * @brief Reads a Data Model from the XML file. If there is an error during the read a NULL wrapped IDataModel will
   * be returned.
   * @param xmlFile The XML File containing the DataModel
   * @return A valid IDataModel* pointer wrapped in a boost::shared_ptr<>
   */
  static IDataModel::Pointer readModel( const std::string &xmlString, bool returnValidModels = true)
  {
    IDataModel::Pointer model = IDataModel::NullPointer ();
    StreamType::Pointer dlg = StreamType::New();
    dlg->getStreamPointer()->str(xmlString);
    if (dlg.get() == NULL)
    {
      return model;
    }
    ReaderType::Pointer reader = ReaderType::New(dlg);
    reader->setReturnValidModels(returnValidModels);
    model = reader->readModel();
    return model;
  }

protected:
  MXAXMLModelStringReader() {}

private:
  MXAXMLModelStringReader(const MXAXMLModelStringReader&);    //Not Implemented
  void operator=(const MXAXMLModelStringReader&); //Not Implemented

};


#endif /* _XML_FILE_UTILITIES_H_ */
