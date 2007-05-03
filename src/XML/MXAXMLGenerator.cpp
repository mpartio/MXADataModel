
#include "MHDXMLGenerator.h"

#include <cctype>
#include <algorithm>
#include <iostream>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MHDXMLGenerator::MHDXMLGenerator(MXADataModel *model)
  : _model(model)
{
}

// I would like to add a new tag to the XML support:
//  <MHD version=\"0.4\">
//   </MHD>
//  so that we can know if we support a particular
//  xml data model version
bool MHDXMLGenerator::write(string filename)
{
  _out.open(filename.c_str(), ios::out);
  if (! _out) {
    std::cout << "Error opening file for writing" << std::endl;
    return false;
  }

  _out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       << "<!DOCTYPE File_Root SYSTEM \"http://materials.cmu.edu/degraef/mhd_0.4.dtd\">\n";


  _openTag(MXA_XML_FILE_ROOT, 0);
  _openTag(MXA_XML_DATA_MODEL, 1);
  _saveDataRoot(2);
  _saveDataDimensions(2);
  _saveDataRecords(2);
  _closeGroupTag(MXA_XML_DATA_MODEL, 1);
  _openTag(MXA_XML_META_DATA, 1);
  _saveRequiredMetaData(2);
  _saveUserDefinedMetaData(2);
  _closeGroupTag(MXA_XML_META_DATA, 1);
  _closeGroupTag(MXA_XML_FILE_ROOT, 0);


  _out.close();
  return true;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
string MHDXMLGenerator::indent(int depth)
{
  const int32 indentSize = 2;
  return string(indentSize * depth, ' ');
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_replaceAll(string &str, string search_str,
				 string replace_str)
{
  int32 search_len = search_str.length();
  string::const_iterator iter;

  string::size_type start=0;
  bool found=true;

  for (;found;) {
    start = str.find(search_str, start);
    if (start != string::npos) {
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
string MHDXMLGenerator::escapedText(const string &str)
{
  string result = str;
  _replaceAll(result, "&", "&amp;");
  _replaceAll(result, "<", "&lt;");
  _replaceAll(result, ">", "&gt;");
  return result;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
string MHDXMLGenerator::escapedAttribute(const string &str)
{
  string result = escapedText(str);
  _replaceAll(result, "\"", "&quot;");
  result.insert(0, "\"");
  result.append("\"");
  return result;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
string MHDXMLGenerator::_toUpper(const string str)
{
  string res = str;
  int32 (*pf)(int)= toupper;
  transform(res.begin(), res.end(), res.begin(), pf);
  return res;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_openTag(string tagName, int32 depth, bool group,
			       map<string, string> attributes,
			       bool allupper)
{
  int32 numAttributes = attributes.size();

  _out << indent(depth) << "<" << tagName;

  if (numAttributes > 0) {
    string label, value;
    map<string, string>::iterator iter;
    for (iter=attributes.begin(); iter!=attributes.end(); iter++) {
      if (allupper) {
	label = _toUpper(iter->first);
      } else {
	label = iter->first;
      }
      value = escapedAttribute(iter->second);
      _out << " " << label << "=" << value; 
    }
  }

  _out  << (group ? ">" : " />") << "\n";
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_openTag(string tagName, int32 depth, bool group)
{
  map<string, string> attrs;
  _openTag(tagName, depth, group, attrs);
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_closeGroupTag(string tagName, int32 depth)
{
  _out << indent(depth) << "</" << tagName << ">" << "\n";
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_saveDataRoot(int depth)
{
  map<string, string> attrs;
  attrs[MXA_XML_NAME_ATTR] = _model->getDataRoot();
  _openTag(MXA_XML_DATA_ROOT, depth, false, attrs);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_saveDataDimensions(int depth)
{
  map<string, string> attrs;
  
  vector<pDimensionData> dims = _model->getDataDimensions();
  vector<pDimensionData>::const_iterator iter;

  _openTag(MXA_XML_DATA_DIMENSIONS_GROUP, depth);

  for (iter=dims.begin(); iter!=dims.end(); iter++) {
    attrs.clear();
    attrs[MXA_XML_NAME_ATTR] = (*iter)->name;
    attrs[MXA_XML_ALT_NAME_ATTR] = (*iter)->alt_name;
    _openTag(MXA_XML_DATA_DIMENSION, depth+1, false, attrs);
  }

  _closeGroupTag(MXA_XML_DATA_DIMENSIONS_GROUP, depth);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_saveSignal(MXANode *signal, int32 depth)
{
  map<string, string> attrs;
  pRecordData rec = _model->getRecordData(signal);
  attrs[MXA_XML_NAME_ATTR] = rec->name;
  attrs[MXA_XML_ALT_NAME_ATTR] = rec->alt_name;
  
  if (signal->hasChildren()) {
    _openTag(MXA_XML_SIGNAL_GROUP, depth, true, attrs);
    vector<MXANode *>::const_iterator iter;
    for (iter=signal->children.begin(); iter!=signal->children.end(); iter++) {
      _saveSignal(*iter, depth+1);
    }
    _closeGroupTag(MXA_XML_SIGNAL_GROUP, depth);
  } else {
    _openTag(MXA_XML_SIGNAL, depth, false, attrs);
  }
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_saveDataRecords(int depth)
{
  _openTag(MXA_XML_DATA_RECORDS_GROUP, depth);
  
  MXANode *recroot = _model->getDataRecordsRoot();
  vector<MXANode *>::const_iterator iter;
  for (iter=recroot->children.begin(); iter!=recroot->children.end(); iter++) {
    _saveSignal(*iter, depth+1);
  }

  _closeGroupTag(MXA_XML_DATA_RECORDS_GROUP, depth);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_saveRequiredMetaData(int depth)
{
  map<string, string>& meta = _model->getRequiredMetaData();
  _openTag(MXA_XML_REQUIRED_META_DATA, depth, false, meta);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MHDXMLGenerator::_saveUserDefinedMetaData(int depth)
{
  map<string, string>& meta = _model->getUserMetaData();
  _openTag(MXA_XML_USER_META_DATA, depth, false, meta, false);
}
  
