#include <Core/MXAAttribute.h>



// Boost Includes
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAttribute::MXAAttribute() 
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAttribute::MXAAttribute(const std::string &key, const Any &value)
{
  _key = key;
  _value = value;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAttribute::~MXAAttribute()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXAAttribute::getKey()
{
  return _key;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
Any MXAAttribute::getValue()
{
  return _value;
}

// -----------------------------------------------------------------------------
//  Converts Value to an int32
// -----------------------------------------------------------------------------
int MXAAttribute::valueToInt32()
{
  return boost::any_cast<int32>(_value);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
float32 MXAAttribute::valueToFloat32() { 
  //FIXME: Add a try catch here otherwise this will terminate when trying to convert a std::vector or something else
  return boost::any_cast<float32>(_value); 
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
float64 MXAAttribute::valueToFloat64() { 
	//FIXME: Add a try catch here otherwise this will terminate when trying to convert a std::vector or something else
	return boost::any_cast<float64>(_value); 
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXAAttribute::valueToString() 
{ 
  if ( boost::any_cast<std::string>( &_value ) ) 
  {
   // std::cout << "Value is already a string. No Conversion needed" << std::endl;
    return boost::any_cast<std::string>(_value);
  } 
  else if ( _value.empty() ) 
  {
    return std::string("***Empty Value****");
  }
  
  std::stringstream strstream;
  this->_attrHelper->convert(strstream);
  return strstream.str();
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXAAttribute::isNumeric()
{
  return true;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXAAttribute::isString()
{
  return false;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXAAttribute::canConvertToString()
{
  return true;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXAAttribute::canConvertToNumber()
{
  return true;
}
