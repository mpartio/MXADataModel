#include "MXADataRecord.h"



// -----------------------------------------------------------------------------
//  Constructor
// -----------------------------------------------------------------------------
MXADataRecord::MXADataRecord() :
MXANode(MXANode::Record, std::string("-1")),
_luid(0),
_recordName(""),
_altName("")
{
  
}

// -----------------------------------------------------------------------------
//  Static New Method that is used to construct a new DataRecord Node
// -----------------------------------------------------------------------------
MXADataRecordPtr MXADataRecord::New(int luid, std::string name, std::string altName)
{
  MXADataRecord* rec = new MXADataRecord();
  MXADataRecordPtr record(rec);
  record->_setWeakPtr(boost::weak_ptr<MXANode>(record)); // Set the Weak Pointer
  //rec->setNodeName(name);
  rec->setLuid(luid);
  rec->setGuid( MXANode::nextGUIDValue() );
  rec->setRecordName(name);
  rec->setAltName(altName);
  return record;
}

// -----------------------------------------------------------------------------
//  Destructor
// -----------------------------------------------------------------------------
MXADataRecord::~MXADataRecord()
{
}

// -----------------------------------------------------------------------------
//  Over ride from Superclass so we can keep the index and nodename in sync
// -----------------------------------------------------------------------------
void MXADataRecord::setNodeName(std::string nodeName)
{
  this->_nodeName = nodeName;
  int32 i = 0;
  if ( StringUtils::stringToNum<int>(i, this->_nodeName, std::dec) )
  {
    this->_luid = i;
  } else {
    std::cout << "ERROR: A NodeName for a DataRecord was set using a value that can not be converted to an integer. DataRecord Node names are represented by integers. You supplied " << nodeName << std::endl;
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------

void MXADataRecord::printNode(std::ostream& os, int32 indent)
{
  os << _indent(indent) << "*-Record Name: " << this->_recordName << std::endl;
  os << _indent(indent) << " |-Alternate Name: " << this->_altName << std::endl;
  os << _indent(indent) << " |-Display Name: " << this->_nodeName << std::endl;
  os << _indent(indent) << " |-LUID: " << this->_luid << std::endl;
  os << _indent(indent) << " |-GUID: " << this->getGuid() << std::endl;
#if 0
  for (MXAAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter )
  {
    os << _indent(indent) << " |- Attribute: " << std::endl;
  }
#endif
  if (this->hasChildren() ) { indent++;}
  for (MXANodeChildren::iterator iter = _children.begin(); iter != _children.end(); ++iter)
  {
    (*(iter))->printNode(os, indent);
  }
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataRecord::generatePath()
{
  std::string path;
  path =  StringUtils::numToString(this->_luid);
  MXANodePtr parent = this->_parent.lock();
  MXADataRecordPtr parentPtr = boost::dynamic_pointer_cast<MXADataRecord>(parent);
  while( parent.get() != NULL )
  {
    path =  StringUtils::numToString(parentPtr->getLuid() ) + "/" + path;
    parent = parentPtr->getParent().lock();
    parentPtr = boost::dynamic_pointer_cast<MXADataRecord>(parent);
  }
  return path;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataRecord::generateParentPath()
{
  std::string path;
  //path =  StringUtils::numToString(this->_luid);
  MXANodePtr parent = this->_parent.lock();
  MXADataRecordPtr parentPtr = boost::dynamic_pointer_cast<MXADataRecord>(parent);
  while( parent.get() != NULL )
  {
    path =  StringUtils::numToString(parentPtr->getLuid() ) + "/" + path;
    parent = parentPtr->getParent().lock();
    parentPtr = boost::dynamic_pointer_cast<MXADataRecord>(parent);
  }
  return path;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataRecord::writeRecord(IDataRecordWriter* writer)
{
  return writer->writeDataRecord(this);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXADataRecord::isValid(std::string &message)
{
  bool valid = true;
  if (this->_recordName.empty() || this->_altName.empty())
  {
    valid = false;
    message.append("Either the Data Record Name or the Alt Name is empty. Both are required to have a value.\n");
  }
  return valid;
}

