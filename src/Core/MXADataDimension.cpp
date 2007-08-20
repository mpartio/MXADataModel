#include "MXADataDimension.h"

// -----------------------------------------------------------------------------
//  Constructor
// -----------------------------------------------------------------------------
MXADataDimension::MXADataDimension() :
MXANode(MXANode::Dimension, std::string("-1")),
_index(std::numeric_limits<int32>::max()),
_count(std::numeric_limits<int32>::max()),
_startValue(std::numeric_limits<int32>::max()),
_endValue(std::numeric_limits<int32>::max()),
_increment(std::numeric_limits<int32>::max()),
_uniform(1),
_dimensionName(""),
_altName("")
{
  
}

// -----------------------------------------------------------------------------
//  Static New Method that is used to construct a new DataDimension Node
// -----------------------------------------------------------------------------
MXADataDimensionPtr MXADataDimension::New(std::string name, std::string altName,
    int32 index, int32 count, int32 startValue, 
    int32 endValue, int32 increment, int32 uniform)
{
  MXADataDimension* dim = new MXADataDimension();
  MXADataDimensionPtr dimension(dim);
  dimension->_setWeakPtr(boost::weak_ptr<MXANode>(dimension)); // Set the Weak Pointer
  dimension->setNodeType(MXANode::Dimension);
  dim->setIndex(index);
  dim->setCount(count);
  dim->setStartValue(startValue);
  dim->setUniform(uniform);
  dim->setEndValue(endValue);
  dim->setIncrement(increment);
  dim->setDimensionName(name);
  dim->setAltName(altName);
  dim->setDimensionName(name);
  return dimension;
}

// -----------------------------------------------------------------------------
//  Destructor
// -----------------------------------------------------------------------------
MXADataDimension::~MXADataDimension()
{
  
}

// -----------------------------------------------------------------------------
//  Setter for Index
// -----------------------------------------------------------------------------
void MXADataDimension::setIndex(int32 aValue)
{
    _index = aValue;
    this->_nodeName = StringUtils::numToString(this->_index);
}

// -----------------------------------------------------------------------------
//  Getter for Index
// -----------------------------------------------------------------------------
int32 MXADataDimension::getIndex()
{
    return _index;
}

// -----------------------------------------------------------------------------
//  Over ride from Superclass so we can keep the index and nodename in sync
// -----------------------------------------------------------------------------
void MXADataDimension::setNodeName(std::string nodeName)
{
  this->_nodeName = nodeName;
  int32 i = 0;
  if ( StringUtils::stringToNum<int>(i, this->_nodeName, std::dec) )
  {
    this->_index = i;
  } else {
    std::cout << "ERROR: A NodeName for a DataDimension was set using a value that can not be converted to an integer. DataDimension Node names are represented by integers. You supplied " << nodeName << std::endl;
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataDimension::printNode(std::ostream& os, int32 indent)
{
  os << _indent(indent) << "*-Dimension Name: " << this->_dimensionName << std::endl;
  os << _indent(indent) << " |-Alternate Name: " << this->_altName << std::endl;
  os << _indent(indent) << " |-Display Name: " << this->_nodeName << std::endl;
  os << _indent(indent) << " |-Type: " << this->getNodeTypeString() << std::endl;
  os << _indent(indent) << " |-Index: " << this->_index << std::endl;
  os << _indent(indent) << " |-StartValue: " << this->_startValue << std::endl;
  os << _indent(indent) << " |-EndValue: " << this->_endValue << std::endl;
  os << _indent(indent) << " |-Count: " << this->_count << std::endl;
  os << _indent(indent) << " |-Increment: " << this->_increment << std::endl;
  os << _indent(indent) << " |-Uniform: " << this->_uniform << std::endl;

}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataDimension::writeDimension(IDataDimensionWriter* writer)
{
  return writer->writeDataDimension(this);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXADataDimension::isValid(std::string &message)
{
  int32 limit = std::numeric_limits<int32>::max();
  bool valid = true;
  if (limit == this->_startValue || 
      limit == this->_endValue ||
      limit == this->_increment ||
      limit == this->_count)
  {
    valid = false;
    message.append("One of [start value, end value, increment or count] was not initialized.\n");
  }
  
  if (this->_endValue < this->_startValue )
  {
    valid = false;
    message.append("End Value is Less than the Start Value.\n");
  }
  
  if (this->_dimensionName.empty() || this->_altName.empty())
  {
    valid = false;
    message.append("Either the Dimension Name or the Alt Name is empty. Both are required to have a value.\n");
  }
  
  return valid;
}

