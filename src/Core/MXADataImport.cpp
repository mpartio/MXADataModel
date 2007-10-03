#include "Core/MXADataImport.h"
#include "Common/LogTime.h"
#include "Base/IImportDelegate.h"
#include "Core/MXADataSource.h"
#include "Core/MXADataModel.h"


#include <iostream>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataImport::MXADataImport ( ) 
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataImport::~MXADataImport ( ) 
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataImport::import()
{
  int32 err = 1;
  for (IDataSources::iterator iter = _dataSources.begin(); iter != _dataSources.end(); ++iter)
  {
    err = (*(iter))->getImportDelegate()->importDataSource( *(iter), this->_dataModel );
    if ( err < 0 )
    {
      break;
    }
  }
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataImport::addDataSource (IDataSourcePtr dataSource ) {
  this->_dataSources.push_back(dataSource);
}

 
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataImport::setOutputFilePath ( std::string new_var ) {
  _outputFilePath = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string MXADataImport::getOutputFilePath ( ) {
  return _outputFilePath;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataImport::setDataModel ( boost::shared_ptr<IDataModel> new_var ) {
  _dataModel = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataModelPtr MXADataImport::getDataModel ( ) {
  return _dataModel;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataImport::setDataSources ( IDataSources &new_var ) {
  _dataSources = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataSources MXADataImport::getDataSources ( ) {
  return _dataSources;
}


