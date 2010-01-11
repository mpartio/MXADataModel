///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include "MXA/XML/DataModelXMLEvtHandler.h"
#include "MXA/Core/MXADataDimension.h"
#include "MXA/Core/MXADataRecord.h"
#include "MXA/Core/MXASupportFile.h"
#include "MXA/DataWrappers/MXAAsciiStringData.h"

#include <string>
#include <map>

typedef  std::map<std::string, std::string>        XMLAttributeMap;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataModelXMLEvtHandler::DataModelXMLEvtHandler(IDataModel::Pointer model) :
m_ParseError(0),
_indent(0)
{
  m_DataModel = model;
  m_Parser = NULL;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataModelXMLEvtHandler::~DataModelXMLEvtHandler()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::OnStartElement(const XML_Char* name, const XML_Char** attrs)
{
    std::string currentTag(name);
    if ( currentTag.compare(MXA_XML::Data_Dimensions) == 0 )
    {
          onData_DimensionsStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Data_Model) == 0 )
    {
          onData_ModelStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Data_Records) == 0 )
    {
          onData_RecordsStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Data_Root) == 0 )
    {
          onData_RootStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Dimension) == 0 )
    {
          onDimensionStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::File_Root) == 0 )
    {
          onFile_RootStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Meta_Data) == 0 )
    {
          onMeta_DataStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Required_MD) == 0 )
    {
          onRequired_MDStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Signal) == 0 )
    {
          onSignalStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Signal_Group) == 0 )
    {
          onSignal_GroupStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::UserDefined_MD) == 0 )
    {
          onUserDefined_MDStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::UserMetaData::Tag) == 0 )
    {
          onUserMetaDataStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA::SupportFiles) == 0 )
    {
          onSupportFilesStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Support_File) == 0 )
    {
          onSupportFileStartTag(name, attrs);
    }
} // End OnStartElement(...)


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::OnEndElement(const XML_Char* name)
{
    std::string currentTag(name);
    if ( currentTag.compare(MXA_XML::Data_Dimensions) == 0 )
    {
          onData_DimensionsEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Data_Model) == 0 )
    {
          onData_ModelEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Data_Records) == 0 )
    {
          onData_RecordsEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Data_Root) == 0 )
    {
          onData_RootEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Dimension) == 0 )
    {
          onDimensionEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::File_Root) == 0 )
    {
          onFile_RootEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Meta_Data) == 0 )
    {
          onMeta_DataEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Required_MD) == 0 )
    {
          onRequired_MDEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Signal) == 0 )
    {
          onSignalEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Signal_Group) == 0 )
    {
          onSignal_GroupEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::UserDefined_MD) == 0 )
    {
          onUserDefined_MDEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::UserMetaData::Tag) == 0 )
    {
          onUserMetaDataEndTag(name);
    }
    else if ( currentTag.compare(MXA::SupportFiles) == 0 )
    {
          onSupportFilesEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Support_File) == 0 )
    {
          onSupportFileEndTag(name);
    }
} // End OnEndElement(...)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::OnCharacterData(const XML_Char* data, int32_t len)
{
 // std::cout << "------------------------------------------------" << std::endl;
 // std::cout << "Character Data: " << data << std::endl;
 // std::cout << "------------------------------------------------" << std::endl;
  if (this->_parseData)
  {
    this->_userAttributeData.append(data, len);
  }
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <File_Root>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onFile_RootStartTag(const XML_Char* name, const XML_Char** attrs)
{
    // printf("Starting %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Model>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onData_ModelStartTag(const XML_Char* name, const XML_Char** attrs)
{
  for (int i = 0; attrs[i]; i += 2)
  {
    //printf("\n\t %s='%s'", attrs[i], attrs[i + 1]);
    if (MXA_XML::Model_Type.compare(attrs[i]) == 0)
    {
      this->m_DataModel->setModelType ( attrs[i+1] );
    }
    else if (MXA_XML::Model_Version.compare(attrs[i]) == 0)
    {
      float fileVersion;
      StringUtils::stringToNum<float>(fileVersion, attrs[i+1]);
      this->m_DataModel->setModelVersion(fileVersion);
    }
    else
    {
      std::cout << "Ignoring Unknown Attribute in <"<< MXA_XML::Data_Model << "> " << attrs[i] << "=" << attrs[i+1] << std::endl;
    }
  }
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Root>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onData_RootStartTag(const XML_Char* name, const XML_Char** attrs)
{
  if ( NULL != attrs[0] && NULL != attrs[1])
  {
    this->m_DataModel->setDataRoot(attrs[1]);
  }
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Dimensions>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onData_DimensionsStartTag(const XML_Char* name, const XML_Char** attrs)
{
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Dimension>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onDimensionStartTag(const XML_Char* name, const XML_Char** attrs)
{
    XMLAttributeMap attrMap;
    for (int i = 0; attrs[i]; i += 2) {
      attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
    }
    int32_t  start, end, increment;
  start = end = increment = std::numeric_limits<int32_t>::max();
    int32_t  index, count, uniform;
  index = count = uniform = std::numeric_limits<int32_t>::min();

    //Check for each Attribute. If the attribute was in the list then parse its value
    if ( attrMap.find(MXA::MXA_INDEX_TAG) != attrMap.end() ) { StringUtils::stringToNum(index, attrMap[MXA::MXA_INDEX_TAG], std::dec); }
    if ( attrMap.find(MXA::MXA_COUNT_TAG) != attrMap.end() ) { StringUtils::stringToNum(count, attrMap[MXA::MXA_COUNT_TAG], std::dec);  }
    if ( attrMap.find(MXA::MXA_START_VALUE_TAG) != attrMap.end() ) { StringUtils::stringToNum(start, attrMap[MXA::MXA_START_VALUE_TAG], std::dec);  }
    if ( attrMap.find(MXA::MXA_END_VALUE_TAG) != attrMap.end() ) { StringUtils::stringToNum(end, attrMap[MXA::MXA_END_VALUE_TAG], std::dec);  }
    if ( attrMap.find(MXA::MXA_INCREMENT_TAG) != attrMap.end() ) { StringUtils::stringToNum(increment, attrMap[MXA::MXA_INCREMENT_TAG], std::dec);  }
    if ( attrMap.find(MXA::MXA_UNIFORM_TAG) != attrMap.end() ) { StringUtils::stringToNum(uniform, attrMap[MXA::MXA_UNIFORM_TAG], std::dec); }

    if (uniform != 0 && uniform != 1)
    {
      // std::cout << "uniform value is NOT valid for Dimension " << attrMap[MXA::MXA_NAME_TAG] << std::endl;
      uniform = 1;
    }
    MXADataDimension::Pointer dim = MXADataDimension::New( attrMap[MXA::MXA_NAME_TAG], attrMap[MXA::MXA_ALT_NAME_TAG], index, count, start, end, increment, uniform);
    if (dim.get() == NULL)
    {
      std::cout << logTime() << "Attempting to add a NULL MXA Data Dimension to the Model." << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
    }
    else
    {
//    std::string message;
//    bool isValid = dim->isValid(message);
//    if (isValid == true)
//    {
//      std::cout << "DataModelXMLEvtHandler::onDimensionStartTag: Dim " << attrMap[MXA::MXA_NAME_TAG] << " was valid" << std::endl;
//    }
//    else
//    {
//      std::cout << "DataModelXMLEvtHandler::onDimensionStartTag: Dim " << attrMap[MXA::MXA_NAME_TAG] << " wasÊNOT valid" << std::endl;
//    }
      this->m_DataModel->insertDataDimension(dim, index);
    }
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Records>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onData_RecordsStartTag(const XML_Char* name, const XML_Char** attrs)
{
  MXADataRecord* nullRecord = NULL;
  _currentParentRecord.reset(nullRecord); // Make the parent NULL since we are at the top level
  //std::cout << "DataModelXMLEvtHandler::onData_RecordsStartTag Data Records Size: " << this->m_DataModel->getDataRecords().size() << std::endl;
  this->_indent = 1;
  // printf("Starting %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Signal_Group>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onSignal_GroupStartTag(const XML_Char* name, const XML_Char** attrs)
{
  ++_indent;
  std::map<std::string, std::string> attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
  //std::cout << StringUtils::indent(_indent) << "DataModelXMLEvtHandler::onSignal_GroupStartTag: " << attrMap[MXA::MXA_NAME_TAG] << " " << this->m_DataModel->getDataRecords().size() << std::endl;

  int32_t luid = -1;
  if (NULL == _currentParentRecord.get() )
  {
    //std::cout << StringUtils::indent(_indent) << "_currentParentRecord was NULL" << std::endl;
    luid = static_cast<int32_t>( this->m_DataModel->getDataRecords().size() ); // This is actually an UNSIGNED int..
  }
  else
  {
    luid = _currentParentRecord->getNumChildren();
  }
  MXADataRecord::Pointer record = MXADataRecord::New( luid, attrMap[MXA::MXA_NAME_TAG], attrMap[MXA::MXA_ALT_NAME_TAG] );

  if (_currentParentRecord.get() != NULL)
  {
    m_DataModel->addDataRecord(record, _currentParentRecord);
  }
  else
  {
    m_DataModel->addDataRecord(record);
  }
  _currentParentRecord = record; // Set this as the current Parent Record

}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Signal>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onSignalStartTag(const XML_Char* name, const XML_Char** attrs)
{
  ++_indent;
  std::map<std::string, std::string> attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
 // std::cout << StringUtils::indent(_indent) << "DataModelXMLEvtHandler::onSignalStartTag: " << attrMap[MXA::MXA_NAME_TAG] << " " << this->m_DataModel->getDataRecords().size() << std::endl;

  int32_t luid = -1;
  if (NULL == _currentParentRecord.get() )
  {
  //  std::cout << StringUtils::indent(_indent) << "_currentParentRecord was NULL" << std::endl;
    luid = static_cast<int32_t>( this->m_DataModel->getDataRecords().size() ); // This is actually an UNSIGNED int..
  }
  else
  {
    luid = _currentParentRecord->getNumChildren();
  }

  MXADataRecord::Pointer record = MXADataRecord::New( luid, attrMap[MXA::MXA_NAME_TAG], attrMap[MXA::MXA_ALT_NAME_TAG] );
  if (_currentParentRecord.get() != NULL)
  {
    m_DataModel->addDataRecord(record, _currentParentRecord);
  }
  else
  {
    m_DataModel->addDataRecord(record);
  }
}


// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Meta_Data>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onMeta_DataStartTag(const XML_Char* name, const XML_Char** attrs)
{

}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Required_MD>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onRequired_MDStartTag(const XML_Char* name, const XML_Char** attrs)
{
  std::map<std::string, std::string> attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
  //Verify that each piece of the Required MetaData is present
  if (this->m_DataModel->setRequiredMetaData(attrMap) < 0 )
  {
    std::cout << "Error: Not all Required Meta Data was found in the file." << std::endl;
    this->m_ParseError = -1;
  }
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <UserDefined_MD>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onUserDefined_MDStartTag(const XML_Char* name, const XML_Char** attrs)
{

}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <UserMetaData>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onUserMetaDataStartTag(const XML_Char* name, const XML_Char** attrs)
{
  // Attributes are: 'key', 'dims', 'type'
  this->_userMDKey.clear();
  this->_userMDDims.clear();
  this->_userMDType.clear();
   //   std::cout << "---------------UserMetaDataStart----------------------" << std::endl;
   //   std::cout << "Tag: " << name << std::endl;
  for (int i = 0; attrs[i]; i += 2)
  {
   // printf("\n\t %s='%s'", attrs[i], attrs[i + 1]);
    if (MXA_XML::UserMetaData::Key.compare(attrs[i]) == 0)
    {
      this->_userMDKey = attrs[i+1];
    }
    else if (MXA_XML::UserMetaData::Dims.compare(attrs[i]) == 0)
    {
      this->_userMDDims = attrs[i+1];
    }
    else if (MXA_XML::UserMetaData::Type.compare(attrs[i]) == 0)
    {
      this->_userMDType = attrs[i+1];
    }
    else
    {
      std::cout << "Unknown Attribute in "<< MXA_XML::UserMetaData::Tag
          << " tag."<< std::endl;
    }
  }
//      std::cout << "\n   Data Start   " << std::endl;
  // Store the key, dims, and type in iVars for use when the tag closes
  // Check all three required Attributes are valid
  if (this->_userMDKey.empty() || this->_userMDDims.empty()
      || this->_userMDType.empty() )
  {
    this->m_ParseError = -1;
    std::cout << "One of the Required Attributes for tag " << MXA_XML::UserMetaData::Tag << " is missing ";
    if (NULL != m_Parser) {
        std::cout << "at line "
        << this->m_Parser->GetCurrentLineNumber() << ", column "
        << this->m_Parser->GetCurrentColumnNumber() << std::endl;
    }
  }

  this->_userAttributeData.clear(); //Clear the User MetaData String
  this->_parseData = true; // We want to parse all character from now until this is set to false
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <UserMetaData>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onUserMetaDataEndTag(const XML_Char* name)
{
  if (this->m_ParseError < 0)
  {
    return;
  }
  if ( this->_userMDType.compare("H5T_STRING") == 0 )
  {
    IMXAArray::Pointer attr = MXAAsciiStringData::Create(this->_userAttributeData);
    this->m_DataModel->addUserMetaData(this->_userMDKey, attr);
  }
  else
  {
    std::vector<uint64_t> dims;
    uint64_t temp = 0;
    std::istringstream istream (this->_userMDDims);
    while(istream.good() )
    {
      istream >> temp;
      dims.push_back(temp);
    }
   // int32_t typeId = H5Lite::HDFTypeFromString(this->_userMDType);
    if ( this->_userMDType.compare("H5T_NATIVE_UINT8") == 0) {
      this->m_ParseError = readPrimitiveAttribute<uint8_t>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_UINT16") == 0) {
      this->m_ParseError = readPrimitiveAttribute<uint16_t>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_UINT32") == 0) {
      this->m_ParseError = readPrimitiveAttribute<uint32_t>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_UINT64") == 0) {
      this->m_ParseError = readPrimitiveAttribute<uint64_t>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_INT8") == 0) {
      this->m_ParseError = readPrimitiveAttribute<int8_t>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_INT16") == 0) {
      this->m_ParseError = readPrimitiveAttribute<int16_t>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_INT32") == 0) {
      this->m_ParseError = readPrimitiveAttribute<int32_t>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_INT64") == 0 ) {
      this->m_ParseError = readPrimitiveAttribute<int64_t>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_FLOAT") == 0) {
      this->m_ParseError = readPrimitiveAttribute<float>( dims);
    } else if ( this->_userMDType.compare("H5T_NATIVE_DOUBLE") == 0) {
      this->m_ParseError = readPrimitiveAttribute<double>( dims);
    } else {
      std::cout << "Unknown Type: " << this->_userMDType << " for a read value of " << this->_userMDType << std::endl;
      this->m_ParseError = -1;
    }
  }

  this->_parseData = false; // Stop parsing character data
}



//______________________________________________________________________________
//------------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Dimensions>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onData_DimensionsEndTag(const XML_Char* name)
{
  this->m_DataModel->squeezeDataDimensions();
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Model>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onData_ModelEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Records>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onData_RecordsEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Root>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onData_RootEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Dimension>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onDimensionEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <File_Root>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onFile_RootEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Meta_Data>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onMeta_DataEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Required_MD>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onRequired_MDEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Signal>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onSignalEndTag(const XML_Char* name)
{
  --_indent;
    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Signal_Group>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onSignal_GroupEndTag(const XML_Char* name)
{
  --_indent;
  _currentParentRecord = _currentParentRecord->getParent().lock();
  //printf("Ending %s\n", name);
}


// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <UserDefined_MD>
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onUserDefined_MDEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onSupportFilesStartTag(const XML_Char* name, const XML_Char** attrs)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onSupportFilesEndTag(const XML_Char* name)
{
  // Increment our counter

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onSupportFileStartTag(const XML_Char* name, const XML_Char** attrs)
{

  ISupportFile::Pointer sfile = MXASupportFile::New();
  for (int i = 0; attrs[i]; i += 2)
  {
    if (MXA::MXA_FILESYSTEM_PATH_TAG.compare(attrs[i]) == 0)
    {
      sfile->setFileSystemPath(attrs[i + 1] );
    }
    else if (MXA::MXA_FILETYPE_TAG.compare(attrs[i]) == 0)
    {
      sfile->setFileType(attrs[i + 1] );
    }
  }
  // Adding the support file will set it's index correctly
  this->m_DataModel->addSupportFile(sfile);
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataModelXMLEvtHandler::onSupportFileEndTag(const XML_Char* name)
{

}


