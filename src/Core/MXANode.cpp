//-- MHD Headers
#include <MXANode.h>
#include <Core/MXAAttribute.h>

//-- STL Headers
#include <iostream>
#include <sstream>

// Set the initial value of the _uniqueGUIDValue
int MXANode::_uniqueGUIDValue(0);

// -----------------------------------------------------------------------------
//  Protected 2 Argument constructor
// -----------------------------------------------------------------------------
MXANode::MXANode(int32 nodeType, std::string nodeName) :
_nodeName(nodeName),
_nodeType(nodeType),
_uniqueId(0)
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANode::~MXANode()
{
}

// -----------------------------------------------------------------------------
//  Static New Method that should be used to create a MXANode
// -----------------------------------------------------------------------------
MXANodePtr MXANode::New(int32 nodeType, std::string nodeName)
{
  MXANodePtr node(new MXANode(nodeType, nodeName));
  node->_setWeakPtr(boost::weak_ptr<MXANode>(node));
  return node;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXANode::generateLUT(std::map<int32, MXANodePtr> &lut, MXANodes &nodes)
{
    for ( MXANodes::iterator iter = nodes.begin(); iter < nodes.end(); ++iter )
    {
    //  rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
      MXANodePtr node = static_cast<MXANodePtr> ( *(iter) );
      lut[node->getUniqueId()] = node;
      if (node->hasChildren() ) {
       // std::cout << "Creating Group for " << rec->getRecordName() << std::endl;
        MXANodes children = node->getChildren();
        MXANode::generateLUT( lut, children );
      } 
    }
}

// -----------------------------------------------------------------------------
//  This is used to set a weak ptr to itself. The weak ptr is then used when 
//  another node needs to set this object as its parent. If we did NOT use a weak
//  ptr and used a shared ptr then we would end up with a cyclic reference and the
//  object would never get deleted because the ref count would never hit zero
// -----------------------------------------------------------------------------
void MXANode::_setWeakPtr(MXANodeWeakPtr weakPtr)
{
  this->_selfPtr = weakPtr; 
}

// -----------------------------------------------------------------------------
//  Setter for nodeName
// -----------------------------------------------------------------------------
void MXANode::setNodeName(std::string aValue)
{
    _nodeName = aValue;
}

// -----------------------------------------------------------------------------
//  Getter for nodeName
// -----------------------------------------------------------------------------
std::string MXANode::getNodeName()
{
    return _nodeName;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXANode::setNodeType(int32 newtype) 
{
  this->_nodeType = newtype;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXANode::getNodeType()
{
  return this->_nodeType;
}

// -----------------------------------------------------------------------------
//  Used for Unique Ids
// -----------------------------------------------------------------------------
void MXANode::resetGUIDValue()
{
  _uniqueGUIDValue = 0;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXANode::nextGUIDValue()
{
  return MXANode::_uniqueGUIDValue++;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXANode::getNodeTypeString()
{
  return MXANode::getNodeTypeAsString(this->_nodeType);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXANode::setParent(MXANodeWeakPtr parent)
{
  this->_parent = parent;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANodeWeakPtr MXANode::getParent()
{
  return this->_parent;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int MXANode::getNumChildren() const
{
  return this->_children.size();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXANode::hasChildren() const
{
  return (this->_children.size() != 0);
}

// -----------------------------------------------------------------------------
//  Adds a child ONLY if it is NOT already inserted into the children vector
// -----------------------------------------------------------------------------
void MXANode::addChild(MXANodePtr child)
{
  child->setParent(this->_selfPtr);
  bool alreadyInserted = false;
  for (MXANodeChildren::iterator iter = this->_children.begin(); iter != this->_children.end(); ++iter)
  {
    if ( (*(iter)).get() == child.get() ) //Compare the raw pointers
    {
      alreadyInserted = true;
      std::cout << "Node " << child->getNodeName() << " was already inserted as child of this Node: " << this->getNodeName() << std::endl;
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
void MXANode::removeChild(int index)
{
  MXANodeChildren::iterator iter = this->_children.begin() + index;
  this->_children.erase(iter);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXANode::removeChild(MXANodePtr child)
{
  for (MXANodeChildren::iterator iter = this->_children.begin(); iter != this->_children.end(); ++iter)
  {
    if ( *(iter) == child )
    {
      this->_children.erase(iter);
      break;
    }
  }
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXANode::indexOfChild(MXANodePtr child)
{
  int32 retVal = -1;
  for (MXANodeChildren::iterator iter = this->_children.begin(); iter != this->_children.end(); ++iter)
  {
    if ( *(iter) == child )
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
int32 MXANode::indexOfChild(MXANode* child)
{
  int32 retVal = -1;
  for (MXANodeChildren::iterator iter = this->_children.begin(); iter != this->_children.end(); ++iter)
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
MXANodePtr MXANode::getChildAt(int32 index)
{
  if (static_cast<uint32>(index) < this->_children.size())
    return this->_children[index];
    else {
      //std::cout << "MXANode(GUID): " << this->getUniqueId() << " Num Children: " << this->_children.size() << "  Index:" << index << std::endl;
    }
   MXANodePtr ptr;
   return ptr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXANodeChildren& MXANode::getChildren()
{
  return this->_children;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXANode::removeAttribute(std::string label)
{
  _nodeAttributes.erase(label);
}



#if 0
MXANodePtr MXANode::findChildIf(BPRED pred, const void *arg1)
{
  std::vector<MXANode *>::iterator iter;
  for (iter=this->_children.begin(); iter!=this->_children.end(); ++iter) {
    if (pred(arg1, (*iter))) {
      return (*iter);
    }
  }  
  return NULL;
}



// Finds a node that is below this node (searches all 
//  nodes below this one, not just this node's children)
MXANode * MXANode::findSubNode(int id)
{
  std::vector<MXANode *>::iterator iter;
  MXANode *child;
  for (iter=this->_children.begin(); iter!=this->_children.end(); ++iter) {
    if ((*iter)->uniqueId == id) {
      return (*iter);
    } else if ((*iter)->hasChildren()) {
      child = (*iter)->findSubNode(id);
      if (child != NULL) {
	return child;
      }
    }
  }
  return NULL;
}



MXANode * MXANode::findSubNodeIf(BPRED pred, const void *arg1)
{
  std::vector<MXANode *>::iterator iter;
  MXANode *child;
  for (iter=this->_children.begin(); iter!=this->_children.end(); ++iter) {
    if (pred(arg1, (*iter))) {
      return (*iter);
    } else if ((*iter)->hasChildren()) {
      child = (*iter)->findSubNodeIf(pred, arg1);
      if (child != NULL) {
	return child;
      }
    }
  }
  return NULL;  
}


#endif

// Data
#if 0
void MXANode::setData(DatasetData &node_data)
{
  data = node_data;
}

void MXANode::setData(std::string node_data)
{
  string_data = node_data;
}

void MXANode::setData(int node_data)
{
  int_data = node_data;
}

void MXANode::setData(float node_data)
{
  float_data = node_data;
}

#endif


// Utilities
std::string MXANode::_indent(int depth)
{
  const int32 indentSize = 2;
  return std::string(indentSize * depth, ' ');
}

std::string MXANode::getNodeTypeAsString(int node_type)
{
  switch(node_type) {
  case Root:
    return "Root";
  case FileVersion:
    return "File Version";
  case FileType:
    return "File Type";
  case DataModel:
    return "Data Model";
  case DataRoot:
    return "Data Root";
  case DataDimensions:
    return "Data Dimensions";
  case Dimension:
    return "Dimension";
  case DataRecords:
    return "Data Records";
  case Record:
    return "Record";
  case MetaData:
    return "Meta Data";
  case Required:
    return "Required Meta Data";
  case UserDefined:
    return "User Defined Meta Data";
  case DataPath:
    return "Data Path";
  case Data:
    return "Data";
  case Dataset:
    return "Unknown Dataset";
  default:
    return "Unknown";
  }
}


void MXANode::printNodeTree(int32 depth)
{
  std::cout << _indent(depth) << "Node(" << this->getNodeTypeString() << "): " << this->_nodeName << std::endl;

  if ( this->hasChildren() ) {
    MXANodeChildren::const_iterator iter;
    for (iter=this->_children.begin(); iter!=this->_children.end(); iter++) {
      (*iter)->printNodeTree(depth+1);
    }
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXANode::printNode(std::ostream& os, int32 indentSize)
{
  os << _indent(indentSize) << "Name: " << this->_nodeName << std::endl;
  os << _indent(indentSize) << "Type: " << this->getNodeTypeString() << std::endl;
  os << _indent(indentSize) << "Attributes:" << std::endl;
  MXAAttribute* attr;
  for (Attributes::iterator iter = _nodeAttributes.begin(); iter != _nodeAttributes.end(); ++iter)
  {
    attr = (*iter).second.get();
      os << _indent(indentSize + 1) << "  Key: " << attr->getKey() << "  \t\tValue: " << attr->valueToString() << std::endl;
  }
}


#if 0
std::string MXANode::getAttributeString()
{
  std::string result("");
  std::map<std::string, std::string>::const_iterator iter;
  for (iter = this->data.attributes.begin(); 
       iter != this->data.attributes.end(); iter++) {
    result += iter->first + ":  " + iter->second + "\n";
  }
  return result;
}
#endif


