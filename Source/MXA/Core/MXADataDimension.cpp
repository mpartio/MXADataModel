#include <MXA/Core/MXADataDimension.h>
#include <MXA/Utilities/StringUtils.h>
#include <MXA/Common/LogTime.h>

// -----------------------------------------------------------------------------
//  Constructor
// -----------------------------------------------------------------------------
MXADataDimension::MXADataDimension() :
_index(std::numeric_limits<int32_t>::min()),
_count(std::numeric_limits<int32_t>::min()),
_startValue(std::numeric_limits<int32_t>::max()),
_endValue(std::numeric_limits<int32_t>::max()),
_increment(std::numeric_limits<int32_t>::max()),
_uniform(std::numeric_limits<int32_t>::min()),
_dimensionName(""),
_altName(""),
_nodeType("Dimension")
{
  
}

// -----------------------------------------------------------------------------
//  Static New Method that is used to construct a new DataDimension Node
// -----------------------------------------------------------------------------
MXADataDimension::Pointer MXADataDimension::New(std::string name, std::string altName,
    int32_t index, int32_t count, int32_t startValue, 
    int32_t endValue, int32_t increment, int32_t uniform)
{
  MXADataDimension* dim = new MXADataDimension();
  MXADataDimension::Pointer dimension(dim);

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
void MXADataDimension::setIndex(int32_t aValue)
{
    _index = aValue;
    this->_nodeName = StringUtils::numToString(this->_index);
}

// -----------------------------------------------------------------------------
//  Getter for Index
// -----------------------------------------------------------------------------
int32_t MXADataDimension::getIndex()
{
    return _index;
}

// -----------------------------------------------------------------------------
//  Over ride from Superclass so we can keep the index and nodename in sync
// -----------------------------------------------------------------------------
void MXADataDimension::setNodeName(std::string nodeName)
{
  this->_nodeName = nodeName;
  int32_t i = 0;
  if ( StringUtils::stringToNum<int32_t>(i, this->_nodeName, std::dec) )
  {
    this->_index = i;
  } else {
    std::cout << "ERROR: A NodeName for a DataDimension was set using a value that can not be converted to an integer. DataDimension Node names are represented by integers. You supplied " << nodeName << std::endl;
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataDimension::printNode(std::ostream& os, int32_t indent)
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
int32_t MXADataDimension::writeDimension(IDataDimensionWriter* writer)
{
  return writer->writeDataDimension(this);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXADataDimension::isValid(std::string &message)
{
  int32_t limitMax = std::numeric_limits<int32_t>::max();
  int32_t limitMin = std::numeric_limits<int32_t>::min();
  
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
int32_t MXADataDimension::calculateCount()
{
  // translate to 0
  int32_t end = _endValue - _startValue;
  int32_t count = end;
  int32_t inc = _increment;
  count = (count/inc) + 1;
  _count = count;
  return _count;
}




