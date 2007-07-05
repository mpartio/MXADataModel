#include "XMLDataModelReader.h"
#include "XML/XMLDataModelReader.h"


#include <boost/shared_ptr.hpp>

#define BUFFER_SIZE 1024


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
XMLDataModelReader::XMLDataModelReader(IFileIODelegate* ioDelegate, MXADataModel* dataModel, const std::string &fileName) :
_dataModel(dataModel),
_fileName(fileName),
_xmlParseError(0),
_cdata(""),
_parseData(false)
{
  _ioDelegate  = static_cast<XMLIODelegate*>(ioDelegate);
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
    else if ( currentTag.compare(MXA_XML::UserMetaData) == 0 )
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
    else if ( currentTag.compare(MXA_XML::UserMetaData) == 0 )
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
    this->_cdata.append(data, len);
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
  this->_dataModel->setFileVersion(MXA::MXACurrentFileVersion);
  this->_dataModel->setFileType(MXA::MXACurrentFileType);
    // printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Root> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_RootStartTag(const XML_Char* name, const XML_Char** attrs)
{
// There should only be 1 attribute: Name="lskdfsldkfj"
  if ( NULL != attrs[0] && NULL != attrs[1])
  {
    this->_dataModel->setDataRoot(attrs[1]);
  }
    // printf("Starting %s\n", name); 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <Data_Dimensions> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onData_DimensionsStartTag(const XML_Char* name, const XML_Char** attrs)
{
    // printf("Starting %s\n", name); 
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
  //FIXME: Finish Implementing this 
   //  printf("Starting %s\n", name); 
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
  //TODO: Not sure we need to actually implement anything here
//     std::cout << "Starting " << name << std::endl;
//     for (int i = 0; attrs[i]; i += 2) {
//       printf("\n\t %s='%s'", attrs[i], attrs[i + 1]);
//     }
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag <UserMetaData> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onUserMetaDataStartTag(const XML_Char* name, const XML_Char** attrs)
{
  //FIXME: Finish Implementing this
  // Attributes are: 'key', 'dims', 'type'
    std::cout << "---------------UserMetaDataStart----------------------" << std::endl;
    std::cout << "Tag: " << name << std::endl;
     for (int i = 0; attrs[i]; i += 2) {
       printf("\n\t %s='%s'", attrs[i], attrs[i + 1]);
     }
     std::cout << "\n   Data Start   " << std::endl;
     // Store the key, dims, and type in iVars for use when the tag closes
     
     this->_cdata.clear();
     this->_parseData = true;
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag <UserMetaData> 
// -----------------------------------------------------------------------------
void XMLDataModelReader::onUserMetaDataEndTag(const XML_Char* name)
{
  
  //FIXME: Start Here
  // Use the iVars where the key, dims and type were stored to create a new 
  // MXAAttribute.
  // Use a StringStream to read in the data based on the type
  // Probably some sort of template for this
  // Maybe an IAttributeReader Class
  std::cout << this->_cdata << std::endl;
  std::cout << "   Data End   " << std::endl;
  std::cout << "---------------UserMetaDataEnd----------------------" << std::endl;
  this->_parseData = false;
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



