


#include "MXADataSource.h"
#include "Core/MXADataModel.h"

#include <iostream>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataSource::MXADataSource ( ) {

}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataSource::~MXADataSource ( ) { }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataSource::setDataRecord ( IDataRecordPtr new_var ) {
  _dataRecord = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataRecordPtr MXADataSource::getDataRecord ( ) {
  return _dataRecord;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataSource::setSourcePath ( std::string new_var ) {
  _sourcePath = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataSource::getSourcePath ( ) {
  return _sourcePath;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataSource::setImportDelegate ( IImportDelegatePtr new_var ) {
  _dataImportDelegate = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IImportDelegatePtr MXADataSource::getImportDelegate ( ) {
  return _dataImportDelegate;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataSource::setDataModel ( IDataModelPtr new_var ) {
  _dataModel = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataModelPtr MXADataSource::getDataModel ( ) {
  return _dataModel;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataSource::setDimensionValues ( std::vector<int32> new_var ) {
  _dimensionValues = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::vector<int32> MXADataSource::getDimensionValues ( ) {
  return _dimensionValues;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataSource::generateInternalPath ( ) 
{
  std::string path;
  //Put the data root on first
  path += this->_dataModel->getDataRoot();
  //Put the Data Dimensions on Next
  for (std::vector<int32>::iterator iter = this->_dimensionValues.begin(); iter != this->_dimensionValues.end(); ++iter ) {
    path += StringUtils::numToString(*iter);
    path += "/";
  }
   // Now build up the DataRecord path
  path = path + _dataRecord->generatePath();
  return path;
}

// -----------------------------------------------------------------------------
//  Use a new set of indices to generate a path
// -----------------------------------------------------------------------------
std::string MXADataSource::generateInternalPath ( std::vector<int32> &indices) 
{
  std::string path;
  //make sure the new set of indices is _atleast_ as large as the original set
  if (indices.size() < this->_dimensionValues.size() )
  {
    return path;
  }
  //Put the data root on first
  path += this->_dataModel->getDataRoot();
  //Put the Data Dimensions on Next
  for (std::vector<int32>::iterator iter = indices.begin(); iter != indices.end(); ++iter ) {
    path += StringUtils::numToString(*iter);
    path += "/";
  }
   // Now build up the DataRecord path
  path = path + _dataRecord->generatePath();
  return path;
}
