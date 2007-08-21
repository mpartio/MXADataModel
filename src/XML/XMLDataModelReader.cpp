

#include "Common/LogTime.h"
#include "XML/XMLDataModelReader.h"

#include <sstream>

#include <boost/shared_ptr.hpp>

#define BUFFER_SIZE 1024


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
XMLDataModelReader::XMLDataModelReader(IFileIODelegate* ioDelegate, MXADataModel* dataModel, const std::string &fileName) :
_dataModel(dataModel),
_fileName(fileName),
_xmlParseError(0),
_userAttributeData(""),
_parseData(false),
_userMDKey(""),
_userMDDims(""),
_userMDType("")
{
  _ioDelegate  = static_cast<XMLIODelegate*>(ioDelegate);
  _parser = NULL;
}
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
XMLDataModelReader::~XMLDataModelReader()
{
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 XMLDataModelReader::readDataModel(int32 locId)
{
  char buf[BUFFER_SIZE];

  // Create and initialise an instance of the parser.
  ExpatParser parser( dynamic_cast<ExpatEvtHandler*>( this ) );
  this->_parser = &parser;
  parser.Create(NULL, NULL);
  parser.EnableElementHandler();
  parser.EnableCharacterDataHandler();


  // Load the XML file.
  FILE*  fh    = fopen(_fileName.c_str(), "r");
  if (NULL == fh)
  {
    std::cout << "XMLDataModelReader: Could Not Open XML File for reading:" << std::endl;
    return -1;
  }
  bool   atEnd = false;
  size_t nRead;
  while (!atEnd && this->_xmlParseError >= 0)
  {
    // Read a block from the XML file and pass it to the parser
    nRead = fread(buf, 1, BUFFER_SIZE, fh);
    atEnd = feof(fh);
    parser.Parse(buf, nRead, atEnd);
  }
  fclose(fh);
  std::string message;
  if (this->_dataModel->isValid(message) == false )
  {
    std::cout << message << std::endl;
  }
  
  return this->_xmlParseError;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 XMLDataModelReader::getParseError() 
{
  return this->_xmlParseError;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLDataModelReader::OnStartElement(const XML_Char* name, const XML_Char** attrs)
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
} // End OnStartElement(...)


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLDataModelReader::OnEndElement(const XML_Char* name)
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
} // End OnEndElement(...)

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLDataModelReader::OnCharacterData(const XML_Char* data, int len)
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
void XMLDataModelReader::onFile_RootStartTag(const XML_Char* name, const XML_Char** attrs)
{
    // printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Model> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_ModelStartTag(const XML_Char* name, const XML_Char** attrs)
{
  for (int i = 0; attrs[i]; i += 2)
  {
    //printf("\n\t %s='%s'", attrs[i], attrs[i + 1]);
    if (MXA_XML::Model_Type.compare(attrs[i]) == 0)
    {
      this->_dataModel->setModelType ( attrs[i+1] );
    }
    else if (MXA_XML::Model_Version.compare(attrs[i]) == 0)
    {
      float fileVersion;
      StringUtils::stringToNum<float>(fileVersion, attrs[i+1]);
      this->_dataModel->setModelVersion(fileVersion);
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
void XMLDataModelReader::onData_RootStartTag(const XML_Char* name, const XML_Char** attrs)
{
  if ( NULL != attrs[0] && NULL != attrs[1])
  {
    this->_dataModel->setDataRoot(attrs[1]);
  }
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Dimensions> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_DimensionsStartTag(const XML_Char* name, const XML_Char** attrs)
{
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Dimension> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onDimensionStartTag(const XML_Char* name, const XML_Char** attrs)
{
    std::map<std::string, std::string> attrMap;
    for (int i = 0; attrs[i]; i += 2) {
      attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
    }
    int32 index, count, start, end, increment, uniform = 0;
    //FIXME: This code is no longer valid because these are now OPTIONAL tags
#warning All this needs to be fixed
    StringUtils::stringToNum(index, attrMap[MXA::MXA_INDEX_TAG], std::dec);
    StringUtils::stringToNum(count, attrMap[MXA::MXA_COUNT_TAG], std::dec);
    StringUtils::stringToNum(start, attrMap[MXA::MXA_START_VALUE_TAG], std::dec);
    StringUtils::stringToNum(end, attrMap[MXA::MXA_END_VALUE_TAG], std::dec);
    StringUtils::stringToNum(increment, attrMap[MXA::MXA_INCREMENT_TAG], std::dec);
    StringUtils::stringToNum(uniform, attrMap[MXA::MXA_UNIFORM_TAG], std::dec);
    
    MXADataDimensionPtr dim = MXADataDimension::New( attrMap[MXA::MXA_NAME_TAG], attrMap[MXA::MXA_ALT_NAME_TAG], index, count, start, end, increment, uniform);
    this->_dataModel->addDataDimension(dim);
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Records> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_RecordsStartTag(const XML_Char* name, const XML_Char** attrs)
{
  MXADataRecord* nullRecord = NULL;
  _currentParentRecord.reset(nullRecord); // Make the parent NULL since we are at the top level
  // printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Signal_Group> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onSignal_GroupStartTag(const XML_Char* name, const XML_Char** attrs)
{
   std::map<std::string, std::string> attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
  int32 luid = -1;
  if (NULL == _currentParentRecord.get() )
  {
    luid = static_cast<int32>( this->_dataModel->getDataRecords().size() ); // This is actually an UNSIGNED int.. 
  }
  else 
  {
    luid = _currentParentRecord->getNumChildren();
  }
  MXADataRecordPtr record = MXADataRecord::New( luid, attrMap[MXA::MXA_NAME_TAG], attrMap[MXA::MXA_ALT_NAME_TAG] );
  this->_dataModel->addDataRecord(record, _currentParentRecord); 
  _currentParentRecord = record; // Set this as the current Parent Record
  
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Signal> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onSignalStartTag(const XML_Char* name, const XML_Char** attrs)
{
  std::map<std::string, std::string> attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
  int32 luid = -1;
  if (NULL == _currentParentRecord.get() )
  {
    luid = static_cast<int32>( this->_dataModel->getDataRecords().size() ); // This is actually an UNSIGNED int.. 
  }
  else 
  {
    luid = _currentParentRecord->getNumChildren();
  }
  MXADataRecordPtr record = MXADataRecord::New( luid, attrMap[MXA::MXA_NAME_TAG], attrMap[MXA::MXA_ALT_NAME_TAG] );
  this->_dataModel->addDataRecord(record, _currentParentRecord);
}


// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Meta_Data> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onMeta_DataStartTag(const XML_Char* name, const XML_Char** attrs)
{

}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Required_MD> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onRequired_MDStartTag(const XML_Char* name, const XML_Char** attrs)
{
  std::map<std::string, std::string> attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
  //Verify that each piece of the Required MetaData is present
  if (this->_dataModel->setRequiredMetaData(attrMap) < 0 ) 
  {
    std::cout << "Error: Not all Required Meta Data was found in the file." << std::endl;
    this->_xmlParseError = -1;
  }
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <UserDefined_MD> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onUserDefined_MDStartTag(const XML_Char* name, const XML_Char** attrs)
{

}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <UserMetaData> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onUserMetaDataStartTag(const XML_Char* name, const XML_Char** attrs)
{
  // Attributes are: 'key', 'dims', 'type'
  this->_userMDKey.clear();
  this->_userMDDims.clear();
  this->_userMDType.clear();
  //    std::cout << "---------------UserMetaDataStart----------------------" << std::endl;
  //    std::cout << "Tag: " << name << std::endl;
  for (int i = 0; attrs[i]; i += 2)
  {
    //printf("\n\t %s='%s'", attrs[i], attrs[i + 1]);
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
  //    std::cout << "\n   Data Start   " << std::endl;
  // Store the key, dims, and type in iVars for use when the tag closes
  // Check all three required Attributes are valid
  if (this->_userMDKey.empty() || this->_userMDDims.empty()
      || this->_userMDType.empty() )
  {
    this->_xmlParseError = -1;
    std::cout << "One of the Required Attributes for tag "
        << MXA_XML::UserMetaData::Tag << " is missing at line "
        <<this->_parser->GetCurrentLineNumber() << ", column "
        << this->_parser->GetCurrentColumnNumber() << std::endl;
  }

  this->_userAttributeData.clear(); //Clear the User MetaData String
  this->_parseData = true; // We want to parse all character from now until this is set to false
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <UserMetaData> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onUserMetaDataEndTag(const XML_Char* name)
{
  if (this->_xmlParseError < 0) return;
 //  std::cout << this->_userAttributeData << std::endl;
//  std::cout << "   Data End   " << std::endl;
//  std::cout << "---------------UserMetaDataEnd----------------------" << std::endl;
  if ( this->_userMDType.compare("H5T_STRING") == 0 )
  {
    this->_dataModel->addUserMetaData(this->_userMDKey, this->_userAttributeData);
  }
  else 
  {
    std::vector<uint64> dims;
    uint64 temp = 0;
    std::istringstream istream (this->_userMDDims);
    while(istream.good() )
    {
      istream >> temp;
      dims.push_back(temp);
    }    
    hid_t typeId = H5Lite::HDFTypeFromString(this->_userMDType);
    if ( H5Tequal(typeId, H5T_STD_U8BE) || H5Tequal(typeId,H5T_STD_U8LE) ) {
      this->_xmlParseError = readPrimitiveAttribute<uint8>( dims);
    } else if ( H5Tequal(typeId, H5T_STD_U16BE) || H5Tequal(typeId,H5T_STD_U16LE) ) {
      this->_xmlParseError = readPrimitiveAttribute<uint16>( dims);
    } else if ( H5Tequal(typeId, H5T_STD_U32BE) || H5Tequal(typeId,H5T_STD_U32LE) ) {
      this->_xmlParseError = readPrimitiveAttribute<uint32>( dims);
    } else if ( H5Tequal(typeId, H5T_STD_U64BE) || H5Tequal(typeId,H5T_STD_U64LE) ) {
      this->_xmlParseError = readPrimitiveAttribute<uint64>( dims);
    } else if ( H5Tequal(typeId, H5T_STD_I8BE) || H5Tequal(typeId,H5T_STD_I8LE) ) {
      this->_xmlParseError = readPrimitiveAttribute<int8>( dims);
    } else if ( H5Tequal(typeId, H5T_STD_I16BE) || H5Tequal(typeId,H5T_STD_I16LE) ) {
      this->_xmlParseError = readPrimitiveAttribute<int16>( dims);
    } else if ( H5Tequal(typeId, H5T_STD_I32BE) || H5Tequal(typeId,H5T_STD_I32LE) ) {
      this->_xmlParseError = readPrimitiveAttribute<int32>( dims); 
    } else if ( H5Tequal(typeId, H5T_STD_I64BE) || H5Tequal(typeId,H5T_STD_I64LE) ) {
      this->_xmlParseError = readPrimitiveAttribute<int64>( dims);
    } else if ( H5Tequal(typeId, H5T_NATIVE_FLOAT)  ) {
      this->_xmlParseError = readPrimitiveAttribute<float>( dims);
    } else if ( H5Tequal(typeId, H5T_NATIVE_DOUBLE)  ) {
      this->_xmlParseError = readPrimitiveAttribute<double>( dims);
    } else {
      std::cout << "Unknown Type: " << typeId << " for a read value of " << this->_userMDType << std::endl;
      this->_xmlParseError = -1;
    }
  }
  
  this->_parseData = false; // Stop parsing character data
}



//______________________________________________________________________________
//------------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Dimensions> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_DimensionsEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Model> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_ModelEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Records> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_RecordsEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Data_Root> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_RootEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Dimension> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onDimensionEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <File_Root> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onFile_RootEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Meta_Data> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onMeta_DataEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Required_MD> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onRequired_MDEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Signal> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onSignalEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <Signal_Group> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onSignal_GroupEndTag(const XML_Char* name)
{
  MXADataRecord* rec = static_cast<MXADataRecord*>( _currentParentRecord->getParent().lock().get() );
  _currentParentRecord.reset( rec ); // Set this as the current Parent Record
  //printf("Ending %s\n", name); 
}


// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <UserDefined_MD> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onUserDefined_MDEndTag(const XML_Char* name)
{

    // printf("Ending %s\n", name); 
}



