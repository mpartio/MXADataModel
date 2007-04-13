#include "XMLDataModelWriter.h"
#include "XML/XMLUserMetaDataWriter.h"

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
XMLDataModelWriter::XMLDataModelWriter(MXADataModel* dataModel, const std::string &fileName)
{
  _dataModel = dataModel;
  _fileName = fileName;
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
bool XMLDataModelWriter::writeModelToFile(int32 fileId)
{
  // Open the file in the proper mode.
  _ofstreamPtr.reset( new std::ofstream ( _fileName.c_str() ) );
  std::ofstream &stream = *(_ofstreamPtr.get());
  
  // Check.
  if ( NULL == _ofstreamPtr.get() || false == stream.is_open() )
  {
    std::cout << "Could not open file '" << this->_fileName << "' for writing XML Model." << std::endl;
    return false;
  }
  
  
  stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       << "<!DOCTYPE File_Root SYSTEM \"http://materials.cmu.edu/degraef/mhd_0.4.dtd\">\n";

  _openTag(MXA::MXA_XML_FILE_ROOT, 0);
  _openTag(MXA::MXA_XML_DATA_MODEL, 1);
  _saveDataRoot(2);
  writeDataDimensions(2);
  writeDataRecords(2);
  _closeGroupTag(MXA::MXA_XML_DATA_MODEL, 1);
  _openTag(MXA::MXA_XML_META_DATA, 1);
  writeRequiredMetaData(2);
  writeUserMetaData(2);
  _closeGroupTag(MXA::MXA_XML_META_DATA, 1);
  _closeGroupTag(MXA::MXA_XML_FILE_ROOT, 0);
  return true;
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
std::string XMLDataModelWriter::indent(int depth)
{
  const int indentSize = 2;
  return std::string(indentSize * depth, ' ');
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_replaceAll(std::string &str, std::string search_str,
				 std::string replace_str)
{
  int search_len = search_str.length();
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
  int (*pf)(int)= toupper;
  transform(res.begin(), res.end(), res.begin(), pf);
  return res;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_openTag(std::string tagName, int depth, bool group,
                                  std::map<std::string, std::string> attributes,
			                            bool allupper)
{
  int numAttributes = attributes.size();
  std::ofstream &stream = *(_ofstreamPtr.get());
  stream << indent(depth) << "<" << tagName;

  if (numAttributes > 0) {
    std::string label, value;
    std::map<std::string, std::string>::iterator iter;
    for (iter=attributes.begin(); iter!=attributes.end(); iter++) {
      if (allupper) {
        label = _toUpper(iter->first);
      } else {
        label = iter->first;
      }
      value = escapedAttribute(iter->second);
      stream << " " << label << "=" << value; 
    }
  }

  stream  << (group ? ">" : " />") << "\n";
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_openTag(std::string tagName, int depth, bool group)
{
  std::map<std::string, std::string> attrs;
  _openTag(tagName, depth, group, attrs);
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_closeGroupTag(std::string tagName, int depth)
{
  std::ofstream &stream = *(_ofstreamPtr.get());
  stream << indent(depth) << "</" << tagName << ">" << "\n";
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void XMLDataModelWriter::_saveDataRoot(int depth)
{
  std::map<std::string, std::string> attrs;
  attrs[MXA::MXA_XML_NAME_ATTR] = _dataModel->getDataRoot();
  _openTag(MXA::MXA_XML_DATA_ROOT, depth, false, attrs);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeDataDimensions(int depth)
{
  _openTag(MXA::MXA_XML_DATA_DIMENSIONS_GROUP, depth);
  MXADataDimensions dimensions = _dataModel->getDataDimensions();
  MXADataDimension* dim;
  for (MXADataDimensions::iterator iter = dimensions.begin(); iter < dimensions.end(); ++iter )
  {
    dim = static_cast<MXADataDimension*> ( (*(iter)).get() );
    dim->writeDimension(this);
  }
  _closeGroupTag(MXA::MXA_XML_DATA_DIMENSIONS_GROUP, depth);
  return 1;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeDataRecords(int depth)
{
  _openTag(MXA::MXA_XML_DATA_RECORDS_GROUP, depth);
  MXADataRecords records =  _dataModel->getDataRecords();
  MXADataRecord* rec;
  int32 err = 0;
  for ( MXADataRecords::iterator iter = records.begin(); iter < records.end(); ++iter )
  {
    rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
    err = rec->writeRecord(this);
  }
  _closeGroupTag(MXA::MXA_XML_DATA_RECORDS_GROUP, depth);
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeRequiredMetaData(int depth)
{
  std::map<std::string, std::string> meta;
  _dataModel->getRequiredMetaData(meta);
  _openTag(MXA::MXA_XML_REQUIRED_META_DATA, depth, false, meta);
  return 1;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeUserMetaData(int depth)
{
  std::map<std::string, std::string> meta;
  _openTag(MXA::MXA_XML_USER_META_DATA, depth, true, meta);

  int32 err = 0;
  int32 fileId = 0;
 
  std::vector<MXAAttributePtr> metadata = _dataModel->getUserMetaData();
  MXAAttribute* attr = NULL;
  
  XMLUserMetaDataWriter writer(_ofstreamPtr);
  for (std::vector<MXAAttributePtr>::iterator iter = metadata.begin(); iter!=metadata.end(); iter++) {
    attr = (*(iter)).get();
    err = attr->write( fileId, const_cast<std::string&>(MXA::UserMetaDataPath), writer);
    if(err<0) {std::cout << "Error Writing User MetaData Attribute " << MXA::UserMetaDataPath  << " Key:" << attr->getKey() << std::endl; break;}
  }
  _closeGroupTag(MXA::MXA_XML_USER_META_DATA, depth - 1);
  return err;
}

// -----------------------------------------------------------------------------
//  IDataDimensionWriter Interface
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeDataDimension(IDataDimension* dim)
{
  std::map<std::string, std::string> attrs;
  attrs[MXA::MXA_XML_NAME_ATTR] = dim->getDimensionName();
  attrs[MXA::MXA_XML_ALT_NAME_ATTR] = dim->getAltName();
  _openTag(MXA::MXA_XML_DATA_DIMENSION, 1, false, attrs);
  return 1;
}

// -----------------------------------------------------------------------------
//  IDataRecordWriter Interface
// -----------------------------------------------------------------------------
int32 XMLDataModelWriter::writeDataRecord(IDataRecord* record)
{
  int32 depth = 3;
  int32 err = -1;
  std::map<std::string, std::string> attrs;
  attrs[MXA::MXA_XML_NAME_ATTR] = record->getRecordName();
  attrs[MXA::MXA_XML_ALT_NAME_ATTR] = record->getAltName();

  if ( dynamic_cast<INode*>(record)->hasChildren() ) {
    _openTag(MXA::MXA_XML_SIGNAL_GROUP, depth - 1, true, attrs);
    MXADataRecord* rec;
    MXANodeChildren records = dynamic_cast<INode*>(record)->getChildren();
    for ( MXADataRecords::iterator iter = records.begin(); iter < records.end(); ++iter )
    {
      rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
      err = rec->writeRecord(this);
    }
    _closeGroupTag(MXA::MXA_XML_SIGNAL_GROUP, depth - 1);
  } else {
    _openTag(MXA::MXA_XML_SIGNAL, depth, false, attrs);
  }
  return err;
} 

