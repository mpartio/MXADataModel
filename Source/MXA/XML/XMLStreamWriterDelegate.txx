/*
 * XMLStreamWriterDelegate<T>.cpp
 *
 *  Created on: Dec 2, 2009
 *      Author: mjackson
 */

#include "XMLStreamWriterDelegate.hpp"
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include "MXA/XML/XMLConstants.h"
#include "MXA/XML/XMLMXAAttributeWriter.hpp"

#if 0
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
IMXADataModelWriterDelegate::Pointer XMLStreamWriterDelegate<T>::New(OutputStreamPointer stream)
{
  IMXADataModelWriterDelegate::Pointer sharedPtr(new XMLStreamWriterDelegate<T>(stream));
  return sharedPtr;
}

#endif
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
XMLStreamWriterDelegate<T>::XMLStreamWriterDelegate(StreamPointer stream) :
_dataRecordIndentation(2)
{
  m_StreamPointer = stream;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
XMLStreamWriterDelegate<T>::~XMLStreamWriterDelegate()
{
  // TODO Auto-generated destructor stub
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
int32_t XMLStreamWriterDelegate<T>::writeModel(IDataModel::Pointer model)
{
  int32_t err = 1;
  std::ostream& stream = *(m_StreamPointer.get());
  m_DataModel = model;
  stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       << "<!DOCTYPE File_Root SYSTEM \"" << MXA::XML_DTD_URL << "\">\n";

  std::map<std::string, std::string> attrs;
  attrs[MXA_XML::Model_Type] = m_DataModel->getModelType();
  std::stringstream sstream;
  sstream << m_DataModel->getModelVersion();
  attrs[MXA_XML::Model_Version] = sstream.str();

  // Write the File_Root opening tag
  _openTag(MXA_XML::File_Root, 0);

  // Write the Data_Model tag and contents
  _openTag(MXA_XML::Data_Model, 1, true, attrs);
  _writeDataRoot(2);
  writeDataDimensions(2);
  writeDataRecords(2);
  _closeGroupTag(MXA_XML::Data_Model, 1);

  // Write the Meta_Data tag and contents
  _openTag(MXA_XML::Meta_Data, 1);
  writeRequiredMetaData(2);
  writeUserMetaData(2);
  _closeGroupTag(MXA_XML::Meta_Data, 1);


  // Write the Support_Files tag and contents
  _openTag(MXA::SupportFiles, 1, true);
  writeSupportFiles(2);
  _closeGroupTag(MXA::SupportFiles, 1);

  _closeGroupTag(MXA_XML::File_Root, 0);

  m_DataModel = IDataModel::NullPointer(); // Remove any reference this class has to the data model
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void XMLStreamWriterDelegate<T>::_replaceAll(std::string &str, std::string search_str,
         std::string replace_str)
{
  std::string::size_type search_len = search_str.length();
  std::string::const_iterator iter;

  std::string::size_type start=0;
  bool found=true;

  for (;found;) {
    start = str.find(search_str, start);
    if (start != std::string::npos) {
      str.erase(start, search_len);
      str.insert(start, replace_str);
    } else {
      found = false;
    }
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
std::string XMLStreamWriterDelegate<T>::escapedText(const std::string &str)
{
  std::string result = str;
  _replaceAll(result, "&", "&amp;");
  _replaceAll(result, "<", "&lt;");
  _replaceAll(result, ">", "&gt;");
  return result;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
std::string XMLStreamWriterDelegate<T>::escapedAttribute(const std::string &str)
{
  std::string result = escapedText(str);
  _replaceAll(result, "\"", "&quot;");
  result.insert(0, "\"");
  result.append("\"");
  return result;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
std::string XMLStreamWriterDelegate<T>::_toUpper(const std::string str)
{
  std::string res = str;
  int32_t (*pf)(int)= toupper;
  transform(res.begin(), res.end(), res.begin(), pf);
  return res;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void XMLStreamWriterDelegate<T>::_openTag(const std::string &tagName,
                                  int32_t depth,
                                  bool group,
                                  std::map<std::string, std::string> &attributes )
{
  std::map<std::string, std::string>::size_type numAttributes = attributes.size();
  std::ostream& stream = *(m_StreamPointer.get());
  stream << StringUtils::indent(depth) << "<" << tagName;

  if (numAttributes > 0)
  {
    std::string label, value;
    std::map<std::string, std::string>::iterator iter;
    for (iter=attributes.begin(); iter!=attributes.end(); ++iter) {
      label = iter->first;
      value = escapedAttribute(iter->second);
      stream << " " << label << "=" << value;
    }
  }
  stream  << (group ? ">" : " />") << "\n";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void XMLStreamWriterDelegate<T>::_openTag(const std::string &tagName, int32_t depth, bool group)
{
  std::map<std::string, std::string> attrs;
  _openTag(tagName, depth, group, attrs);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void XMLStreamWriterDelegate<T>::_closeGroupTag(const std::string &tagName, int32_t depth)
{
  std::ostream& stream = *(m_StreamPointer.get());
  stream << StringUtils::indent(depth) << "</" << tagName << ">" << "\n";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void XMLStreamWriterDelegate<T>::_writeDataRoot(int32_t depth)
{
  std::map<std::string, std::string> attrs;
  attrs[MXA_XML::Attribute::Name] = m_DataModel->getDataRoot();
  _openTag(MXA_XML::Data_Root, depth, false, attrs);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
int32_t XMLStreamWriterDelegate<T>::writeDataDimensions(int32_t depth)
{
  _openTag(MXA_XML::Data_Dimensions, depth);
  IDataDimension::Container dimensions = m_DataModel->getDataDimensions();
  MXADataDimension* dim;
  for (IDataDimension::Container::iterator iter = dimensions.begin(); iter < dimensions.end(); ++iter )
  {
    dim = static_cast<MXADataDimension*> ( (*(iter)).get() );
    writeDataDimension(dim);
  }
  _closeGroupTag(MXA_XML::Data_Dimensions, depth);
  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
int32_t XMLStreamWriterDelegate<T>::writeDataRecords(int32_t depth)
{

  _openTag(MXA_XML::Data_Records, this->_dataRecordIndentation);
  IDataRecord::Container records =  m_DataModel->getDataRecords();
  MXADataRecord* rec;
  int32_t err = 0;
  for ( IDataRecord::Container::iterator iter = records.begin(); iter < records.end(); ++iter )
  {
    rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
    writeDataRecord(rec);
  }
  _closeGroupTag(MXA_XML::Data_Records, depth);
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
int32_t XMLStreamWriterDelegate<T>::writeRequiredMetaData(int32_t depth)
{
  std::map<std::string, std::string> meta;
  m_DataModel->getRequiredMetaData();
  IRequiredMetaData::Pointer metaData = m_DataModel->getRequiredMetaData();
  metaData->generateKeyValueMap(meta);
  _openTag(MXA_XML::Required_MD, depth, false, meta);
  return 1;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
int32_t XMLStreamWriterDelegate<T>::writeUserMetaData(int32_t depth)
{
  std::map<std::string, std::string> meta;
  _openTag(MXA_XML::UserDefined_MD, depth, true, meta);

  int32_t err = 0;
  //int32_t fileId = 0;

  MXAAbstractAttributes metadata = m_DataModel->getUserMetaData();
  for (MXAAbstractAttributes::iterator iter = metadata.begin(); iter!=metadata.end(); iter++) {
    IMXAArray::Pointer attr = (*iter).second;
    std::string attributeKey = (*iter).first;
    XMLMXAAttributeWriter writer(m_StreamPointer);
    if (NULL != attr.get() )
    {
      err = writer.writeAttribute(attributeKey, attr);
      if(err<0) {std::cout << "Error Writing User MetaData Attribute " << MXA::UserMetaDataPath  << " Key:" << attributeKey << std::endl; break;}
    }
  }
  _closeGroupTag(MXA_XML::UserDefined_MD, depth - 1);
  return err;
}

// -----------------------------------------------------------------------------
//  IDataDimensionWriter Interface
// -----------------------------------------------------------------------------
template<typename T>
int32_t XMLStreamWriterDelegate<T>::writeDataDimension(IDataDimension* dim)
{
  std::map<std::string, std::string> attrs;
  attrs[MXA::MXA_NAME_TAG] = dim->getDimensionName();
  attrs[MXA::MXA_ALT_NAME_TAG] = dim->getAltName();
  this->_dataRecordIndentation += 2;
  //Check for uninitialized values since these are all optional. If any of them
  // are NOT initialized then do not write them to the file.
  if (dim->isPropertyInitialized( dim->getStartValue() ) )
  {
    attrs[MXA::MXA_START_VALUE_TAG] =  StringUtils::numToString(dim->getStartValue() );
  }
  if ( dim->isPropertyInitialized(dim->getEndValue() ) )
  {
    attrs[MXA::MXA_END_VALUE_TAG] = StringUtils::numToString(dim->getEndValue() );
  }
  if (dim->isPropertyInitialized(dim->getIncrement()))
  {
    attrs[MXA::MXA_INCREMENT_TAG] = StringUtils::numToString(dim->getIncrement() );
  }
  if (dim->isPropertyInitialized(dim->getCount()) )
  {
    attrs[MXA::MXA_COUNT_TAG] = StringUtils::numToString(dim->getCount() );
  }
  if (dim->isPropertyInitialized(dim->getIndex() ) )
  {
    attrs[MXA::MXA_INDEX_TAG] = StringUtils::numToString(dim->getIndex() );
  }
  if (dim->isPropertyInitialized(dim->getUniform() ) )
  {
    attrs[MXA::MXA_UNIFORM_TAG] = StringUtils::numToString(dim->getUniform() );
  }
  _openTag(MXA_XML::Dimension, this->_dataRecordIndentation, false, attrs);
  this->_dataRecordIndentation -= 2;
  return 1;
}

// -----------------------------------------------------------------------------
//  IDataRecordWriter Interface
// -----------------------------------------------------------------------------
template<typename T>
int32_t XMLStreamWriterDelegate<T>::writeDataRecord(IDataRecord* record)
{
  //int32_t depth = 3;
  int32_t err = -1;
  std::map<std::string, std::string> attrs;
  attrs[MXA_XML::Attribute::Name] = record->getRecordName();
  attrs[MXA_XML::Attribute::AltName] = record->getAltName();
  this->_dataRecordIndentation += 2;
  if ( dynamic_cast<IDataRecord*>(record)->hasChildren() ) {
    _openTag(MXA_XML::Signal_Group, this->_dataRecordIndentation, true, attrs);
    MXADataRecord* rec;
    IDataRecord::Container records = dynamic_cast<IDataRecord*>(record)->getChildren();
    for ( IDataRecord::Container::iterator iter = records.begin(); iter < records.end(); ++iter )
    {
      rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
//      err = rec->writeRecord(this);
      err = writeDataRecord(rec);  // <== THIS IS RECURSIVE CODE
    }
    _closeGroupTag(MXA_XML::Signal_Group, this->_dataRecordIndentation);
  } else {
    _openTag(MXA_XML::Signal, this->_dataRecordIndentation, false, attrs);
  }
  this->_dataRecordIndentation -= 2;
  return err;
}

// -----------------------------------------------------------------------------
//  IDataModelWriter Interface
// -----------------------------------------------------------------------------
template<typename T>
int32_t XMLStreamWriterDelegate<T>::writeSupportFiles(int32_t indentDepth)
{
  int32_t err = 0;
  std::map<std::string, std::string> attrs;
  ISupportFile::Container files = this->m_DataModel->getSupportFiles();
  ISupportFile* file;
  for (ISupportFile::Container::iterator iter = files.begin(); iter != files.end(); ++iter ) {
    file = (*iter).get();
    if (NULL != file)
    {
      attrs.clear(); // Clear any attributes
      attrs[MXA::MXA_FILESYSTEM_PATH_TAG] = file->getFileSystemPath();
      attrs[MXA::MXA_FILETYPE_TAG] = file->getFileType();
      _openTag(MXA_XML::Support_File, indentDepth, false, attrs);
    }
  }

  return err;
}

