#include <Core/MXADataRecord.h>
#include <Common/LogTime.h>

// Set the initial value of the _uniqueGUIDValue
int32 MXADataRecord::_uniqueGUIDValue(0);

// -----------------------------------------------------------------------------
//  Constructor
// -----------------------------------------------------------------------------
MXADataRecord::MXADataRecord() :
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
  record->_setWeakPtr(boost::weak_ptr<IDataRecord>(record)); // Set the Weak Pointer
  rec->setLuid(luid);
  rec->setGuid( MXADataRecord::nextGUIDValue() );
  rec->setRecordName(name);
  rec->setAltName(altName);
  return record;
}

// -----------------------------------------------------------------------------
//  Destructor
// -----------------------------------------------------------------------------
MXADataRecord::~MXADataRecord()
{
  //std::cout << logTime() << "~MXADataRecord {" << this->_uniqueId << "}" << std::endl;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataRecord::generateLUT(IDataRecordLookupTable &lut, IDataRecords &nodes)
{
  for ( IDataRecords::iterator iter = nodes.begin(); iter < nodes.end(); ++iter )
  {
    //  rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
    IDataRecordPtr node =  ( *(iter) );
    lut[node->getUniqueId()] = node;
    if (node->hasChildren() ) {
      // std::cout << "Creating Group for " << rec->getRecordName() << std::endl;
      IDataRecords children = node->getChildren();
      MXADataRecord::generateLUT( lut, children );
    } 
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------

void MXADataRecord::printDataRecord(std::ostream& os, int32 indent)
{
  os << StringUtils::indent(indent) << "*-Record Name: " << this->_recordName << std::endl;
  os << StringUtils::indent(indent) << " |-Alternate Name: " << this->_altName << std::endl;
  os << StringUtils::indent(indent) << " |-Display Name: " << this->_nodeName << std::endl;
  os << StringUtils::indent(indent) << " |-LUID: " << this->_luid << std::endl;
  os << StringUtils::indent(indent) << " |-GUID: " << this->getGuid() << std::endl;
#if 0
  for (MXAAttributes::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter )
  {
    os << StringUtils::indent(indent) << " |- Attribute: " << std::endl;
  }
#endif
  if (this->hasChildren() ) { indent++;}
  for (IDataRecords::iterator iter = _children.begin(); iter != _children.end(); ++iter)
  {
    (*(iter))->printDataRecord(os, indent);
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataRecord::printDataRecordTree(int32 depth)
{
  if ( this->hasChildren() ) {
    IDataRecords::const_iterator iter;
    for (iter=this->_children.begin(); iter!=this->_children.end(); iter++) {
      (*iter)->printDataRecordTree(depth+1);
    }
  }
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataRecord::generatePath()
{
  std::string path;
  path =  StringUtils::numToString(this->_luid);
  MXADataRecord* parent = dynamic_cast<MXADataRecord*>( this->_parent.lock().get() );
  while( NULL != parent )
  {
    path =  StringUtils::numToString(parent->getLuid() ) + "/" + path;
    parent = dynamic_cast<MXADataRecord*>(parent->getParent().lock().get() );
  }
  return path;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataRecord::generateParentPath()
{
  std::string path;

  MXADataRecord* parent = dynamic_cast<MXADataRecord*>( this->_parent.lock().get() );
  while( NULL != parent )
  {
    path =  StringUtils::numToString(parent->getLuid() ) + "/" + path;
    parent = dynamic_cast<MXADataRecord*>(parent->getParent().lock().get() );
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


// -----------------------------------------------------------------------------
//  This is used to set a weak ptr to itself. The weak ptr is then used when 
//  another node needs to set this object as its parent. If we did NOT use a weak
//  ptr and used a shared ptr then we would end up with a cyclic reference and the
//  object would never get deleted because the ref count would never hit zero
// -----------------------------------------------------------------------------
void MXADataRecord::_setWeakPtr(IDataRecordWeakPtr weakPtr)
{
  this->_selfPtr = weakPtr; 
}

// -----------------------------------------------------------------------------
//  Used for Unique Ids
// -----------------------------------------------------------------------------
void MXADataRecord::resetGUIDValue()
{
  _uniqueGUIDValue = 0;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataRecord::nextGUIDValue()
{
  return MXADataRecord::_uniqueGUIDValue++;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataRecord::setParent(IDataRecordWeakPtr parent)
{
  this->_parent = parent;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataRecordWeakPtr MXADataRecord::getParent()
{
  return this->_parent;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataRecord::getNumChildren() const
{
  return this->_children.size();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXADataRecord::hasChildren() const
{
  return (this->_children.size() != 0);
}

// -----------------------------------------------------------------------------
//  Adds a child ONLY if it is NOT already inserted into the children vector
// -----------------------------------------------------------------------------
void MXADataRecord::addChild(IDataRecordPtr child)
{
  child->setParent(this->_selfPtr);
  bool alreadyInserted = false;
  for (IDataRecords::iterator iter = this->_children.begin(); iter != this->_children.end(); ++iter)
  {
    if ( (*(iter)).get() == child.get() ) //Compare the raw pointers
    {
      alreadyInserted = true;
      break;
    }
  }
  if (alreadyInserted == false) {
    this->_children.push_back(child);
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataRecord::removeChild(int index)
{
  IDataRecords::iterator iter = this->_children.begin() + index;
  this->_children.erase(iter);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataRecord::removeChild(IDataRecord* child)
{
  for (IDataRecords::iterator iter = this->_children.begin(); iter != this->_children.end(); ++iter)
  {
    if ( (*(iter)).get() == child )
    {
      this->_children.erase(iter);
      break;
    }
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataRecord::removeAllChildren()
{
  this->_children.clear();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataRecord::indexOfChild(IDataRecord* child)
{
  int32 retVal = -1;
  for (IDataRecords::iterator iter = this->_children.begin(); iter != this->_children.end(); ++iter)
  {
    if ( (*(iter)).get() == child )
    {
      retVal = iter - this->_children.begin();
      break;
    }
  }
  return retVal;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataRecordPtr MXADataRecord::getChildAt(int32 index)
{
  if (static_cast<uint32>(index) < this->_children.size())
    return this->_children[index];
    else {
      //std::cout << "MXADataRecord(GUID): " << this->getUniqueId() << " Num Children: " << this->_children.size() << "  Index:" << index << std::endl;
    }
   MXADataRecordPtr ptr;
   return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataRecords& MXADataRecord::getChildren()
{
  return this->_children;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataRecord::removeAttribute(std::string label)
{
  _nodeAttributes.erase(label);
}






