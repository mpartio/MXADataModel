#include <XML/XMLDataModelWriter.h>
#include <XML/XMLUserMetaDataWriter.h>
#include <XML/XMLConstants.h>
#include <Base/IRequiredMetaData.h>
#include <XML/XMLMXAAttributeWriter.hpp>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
XMLDataModelWriter::XMLDataModelWriter( IDataModelPtr dataModel, const std::string &fileName) :
  _dataModel(dataModel),
  _fileName(fileName),
  _dataRecordIndentation(2)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
XMLDataModelWriter::~XMLDataModelWriter()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeModelToFile(int32 NOT_USED)
{
  // Open the file in the proper mode.
  _ofstreamPtr.reset( new std::ofstream ( _fileName.c_str() ) );
  std::ofstream &stream = *(_ofstreamPtr.get());

  // Check.
  if ( NULL == _ofstreamPtr.get() || false == stream.is_open() )
  {
    std::cout << "Could not open file '" << this->_fileName << "' for writing XML Model." << std::endl;
    return -1;
  }


  stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       << "<!DOCTYPE File_Root SYSTEM \"http://titanium.imts.us/viewvc/Task_7/MXADataModel/Resources/mxa_0.4.dtd\">\n";

  std::map<std::string, std::string> attrs;
  attrs[MXA_XML::Model_Type] = _dataModel->getModelType();
  std::stringstream sstream;
  sstream << _dataModel->getModelVersion();
  attrs[MXA_XML::Model_Version] = sstream.str();
 // attrs["Something"] = "Bad bad bad";
  _openTag(MXA_XML::File_Root, 0);

  _openTag(MXA_XML::Data_Model, 1, true, attrs);
  _writeDataRoot(2);
  writeDataDimensions(2);
  writeDataRecords(2);
  _closeGroupTag(MXA_XML::Data_Model, 1);

  _openTag(MXA_XML::Meta_Data, 1);
  writeRequiredMetaData(2);
  writeUserMetaData(2);
  _closeGroupTag(MXA_XML::Meta_Data, 1);
  _closeGroupTag(MXA_XML::File_Root, 0);
  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeDataModelTemplate(int32 uniqueId)
{
  std::cout << "XMLDataModelWriter::writeDataModelTemplate NOT IMPLEMENTED" << std::endl;
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_replaceAll(std::string &str, std::string search_str,
				 std::string replace_str)
{
  int32 search_len = search_str.length();
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
std::string XMLDataModelWriter::escapedText(const std::string &str)
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
std::string XMLDataModelWriter::escapedAttribute(const std::string &str)
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
std::string XMLDataModelWriter::_toUpper(const std::string str)
{
  std::string res = str;
  int32 (*pf)(int)= toupper;
  transform(res.begin(), res.end(), res.begin(), pf);
  return res;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_openTag(const std::string &tagName,
                                  int32 depth,
                                  bool group,
                                  std::map<std::string, std::string> &attributes )
{
  int32 numAttributes = attributes.size();
  std::ofstream &stream = *(_ofstreamPtr.get());
  stream << StringUtils::indent(depth) << "<" << tagName;

  if (numAttributes > 0) {
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
void XMLDataModelWriter::_openTag(const std::string &tagName, int32 depth, bool group)
{
  std::map<std::string, std::string> attrs;
  _openTag(tagName, depth, group, attrs);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_closeGroupTag(const std::string &tagName, int32 depth)
{
  std::ofstream &stream = *(_ofstreamPtr.get());
  stream << StringUtils::indent(depth) << "</" << tagName << ">" << "\n";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_writeDataRoot(int32 depth)
{
  std::map<std::string, std::string> attrs;
  attrs[MXA_XML::Attribute::Name] = _dataModel->getDataRoot();
  _openTag(MXA_XML::Data_Root, depth, false, attrs);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeDataDimensions(int32 depth)
{
  _openTag(MXA_XML::Data_Dimensions, depth);
  IDataDimensions dimensions = _dataModel->getDataDimensions();
  MXADataDimension* dim;
  for (IDataDimensions::iterator iter = dimensions.begin(); iter < dimensions.end(); ++iter )
  {
    dim = static_cast<MXADataDimension*> ( (*(iter)).get() );
    dim->writeDimension(this);
  }
  _closeGroupTag(MXA_XML::Data_Dimensions, depth);
  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeDataRecords(int32 depth)
{

  _openTag(MXA_XML::Data_Records, this->_dataRecordIndentation);
  IDataRecords records =  _dataModel->getDataRecords();
  MXADataRecord* rec;
  int32 err = 0;
  for ( IDataRecords::iterator iter = records.begin(); iter < records.end(); ++iter )
  {
    rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
    err = rec->writeRecord(this);
  }
  _closeGroupTag(MXA_XML::Data_Records, depth);
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeRequiredMetaData(int32 depth)
{
  std::map<std::string, std::string> meta;
  _dataModel->getRequiredMetaData();
  IRequiredMetaDataPtr metaData = _dataModel->getRequiredMetaData();
  metaData->generateKeyValueMap(meta);
  _openTag(MXA_XML::Required_MD, depth, false, meta);
  return 1;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeUserMetaData(int32 depth)
{
  std::map<std::string, std::string> meta;
  _openTag(MXA_XML::UserDefined_MD, depth, true, meta);

  int32 err = 0;
  //int32 fileId = 0;

  MXAAbstractAttributes metadata = _dataModel->getUserMetaData();
  IMXAArray* attr = NULL;

  //XMLUserMetaDataWriter writer(_ofstreamPtr);
  for (MXAAbstractAttributes::iterator iter = metadata.begin(); iter!=metadata.end(); iter++) {
    IMXAArrayPtr attr = (*iter).second;
    std::string attributeKey = (*iter).first;
    XMLMXAAttributeWriter writer(_ofstreamPtr);
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
int32 XMLDataModelWriter::writeDataDimension(IDataDimension* dim)
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
int32 XMLDataModelWriter::writeDataRecord(IDataRecord* record)
{
  //int32 depth = 3;
  int32 err = -1;
  std::map<std::string, std::string> attrs;
  attrs[MXA_XML::Attribute::Name] = record->getRecordName();
  attrs[MXA_XML::Attribute::AltName] = record->getAltName();
  this->_dataRecordIndentation += 2;
  if ( dynamic_cast<IDataRecord*>(record)->hasChildren() ) {
    _openTag(MXA_XML::Signal_Group, this->_dataRecordIndentation, true, attrs);
    MXADataRecord* rec;
    IDataRecords records = dynamic_cast<IDataRecord*>(record)->getChildren();
    for ( IDataRecords::iterator iter = records.begin(); iter < records.end(); ++iter )
    {
      rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
      err = rec->writeRecord(this);
    }
    _closeGroupTag(MXA_XML::Signal_Group, this->_dataRecordIndentation);
  } else {
    _openTag(MXA_XML::Signal, this->_dataRecordIndentation, false, attrs);
  }
  this->_dataRecordIndentation -= 2;
  return err;
}

