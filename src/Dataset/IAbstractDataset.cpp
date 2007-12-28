#include "IAbstractDataset.h"

#include <Dataset/IAbstractAttribute.h>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IAbstractDataset::IAbstractDataset(const std::string &datasetPath, std::vector<uint64> datasetDimensions) :
  _datasetPath(datasetPath), 
  _datasetDimensions(datasetDimensions)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IAbstractDataset::~IAbstractDataset() 
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string IAbstractDataset::getDatasetPath()
{
  return _datasetPath;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 IAbstractDataset::getRank()
{
  return this->_datasetDimensions.size();
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::vector<uint64> IAbstractDataset::getDatasetDimensions()
{
  return this->_datasetDimensions;
}

