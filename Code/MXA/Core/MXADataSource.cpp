


#include <MXA/Core/MXADataSource.h>
//#include <MXA/Core/MXADataModel.h>
//#include <MXA/Utilities/StringUtils.h>
#include <MXA/Utilities/MXAFileInfo.h>
//
//#include <iostream>

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
void MXADataSource::setDataRecord ( IDataRecord::Pointer new_var ) {
  _dataRecord = new_var;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataRecord::Pointer MXADataSource::getDataRecord ( ) {
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
bool MXADataSource::dataSourceExists()
{
  return MXAFileInfo::exists(this->_sourcePath);
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataSource::setImportDelegate ( IImportDelegate::Pointer dataParser ) {
  _dataImportDelegate = dataParser;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IImportDelegate::Pointer MXADataSource::getImportDelegate ( ) {
  return _dataImportDelegate;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataSource::setDataModel ( IDataModel::Pointer dataModel ) {
  _dataModel = dataModel;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataModel::Pointer MXADataSource::getDataModel ( ) {
  return _dataModel;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataSource::setDimensionValues ( std::vector<int32_t> new_var ) {
  _dimensionValues = new_var;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<int32_t> MXADataSource::getDimensionValues ( ) {
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
  for (std::vector<int32_t>::iterator iter = this->_dimensionValues.begin(); iter != this->_dimensionValues.end(); ++iter ) {
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
std::string MXADataSource::generateInternalPath ( std::vector<int32_t> &indices)
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
  for (std::vector<int32_t>::iterator iter = indices.begin(); iter != indices.end(); ++iter ) {
    path += StringUtils::numToString(*iter);
    path += "/";
  }
   // Now build up the DataRecord path
  path = path + _dataRecord->generatePath();
  return path;
}
