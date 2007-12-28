#include "IAbstractAttribute.h"
#include <Dataset/IAbstractDataset.h>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IAbstractAttribute::IAbstractAttribute(const std::string &datasetPath, const std::string &key, std::vector<uint64> datasetDimensions) :
  _key(key), 
  _datasetPath(datasetPath), 
  _datasetDimensions(datasetDimensions)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IAbstractAttribute::~IAbstractAttribute()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string IAbstractAttribute::getKey()
{
  return this->_key;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void IAbstractAttribute::setKey(const std::string &key)
{
  this->_key = key;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string IAbstractAttribute::getDatasetPath()
{
  return _datasetPath;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int64 IAbstractAttribute::getRank()
{
  return this->_datasetDimensions.size();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::vector<uint64> IAbstractAttribute::getAttributeDimensions()
{
  return this->_datasetDimensions;
}

