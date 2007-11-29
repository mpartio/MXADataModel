#include <Core/MXADataDimension.h>
#include <Utilities/StringUtils.h>


// -----------------------------------------------------------------------------
//  Constructor
// -----------------------------------------------------------------------------
MXADataDimension::MXADataDimension() :
_index(std::numeric_limits<int32>::min()),
_count(std::numeric_limits<int32>::min()),
_startValue(std::numeric_limits<int32>::max()),
_endValue(std::numeric_limits<int32>::max()),
_increment(std::numeric_limits<int32>::max()),
_uniform(std::numeric_limits<int32>::min()),
_dimensionName(""),
_altName(""),
_nodeType("Dimension")
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
  os << StringUtils::indent(indent) << "*-Dimension Name: " << this->_dimensionName << std::endl;
  os << StringUtils::indent(indent) << " |-Alternate Name: " << this->_altName << std::endl;
  os << StringUtils::indent(indent) << " |-Display Name: " << this->_nodeName << std::endl;
  os << StringUtils::indent(indent) << " |-Type: " << this->_nodeType << std::endl;
  os << StringUtils::indent(indent) << " |-Index: " << this->_index << std::endl;
  os << StringUtils::indent(indent) << " |-StartValue: " << this->_startValue << std::endl;
  os << StringUtils::indent(indent) << " |-EndValue: " << this->_endValue << std::endl;
  os << StringUtils::indent(indent) << " |-Count: " << this->_count << std::endl;
  os << StringUtils::indent(indent) << " |-Increment: " << this->_increment << std::endl;
  os << StringUtils::indent(indent) << " |-Uniform: " << this->_uniform << std::endl;

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
  int32 limitMax = std::numeric_limits<int32>::max();
  int32 limitMin = std::numeric_limits<int32>::min();
  
  bool valid = true;
  if (limitMax == this->_startValue || 
      limitMax == this->_endValue ||
      limitMax == this->_increment)
  {
    valid = false;
    message.append("One of [start value, end value, increment] was not initialized.\n");
  }
  
  if (limitMin == this->_count ||
      limitMin == this->_uniform ||
      limitMin == this->_index)
  {
    valid = false;
    message.append("One of [count, index or uniform] was not initialized.\n");
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

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataDimension::calculateCount()
{
  // translate to 0
  int32 end = _endValue - _startValue;
  int32 count = end;
  int32 inc = _increment;
  count = (count/inc) + 1;
  _count = count;
  return _count;
}




