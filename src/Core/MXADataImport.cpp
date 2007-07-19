#include "MXADataImport.h"
#include "Common/LogTime.h"
#include "Base/IDataImportDelegate.h"
#include "Core/MXADataSource.h"
#include <iostream>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataImport::MXADataImport ( ) {

}
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataImport::~MXADataImport ( ) { }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXADataImport::import()
{
  //std::cout << "MXADataImport: Importing Data...." << std::endl;
  int32 err = 1;
  for (DataSources::iterator iter = _dataSources.begin(); iter != _dataSources.end(); ++iter)
  {
    err = (*(iter)).get()->getImportDelegate()->importDataSource( *iter, this->getDataModel() );
    if ( err < 0 )
    {
      break;
    }
  }
  //std::cout << logTime() << "...MXADataImport: Finished Importing Data: " << err << std::endl;
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataImport::addDataSource (MXADataSourcePtr dataSource ) {
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
void MXADataImport::setDataModel ( boost::shared_ptr<MXADataModel> new_var ) {
  _dataModel = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXADataModelPtr MXADataImport::getDataModel ( ) {
  return _dataModel;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXADataImport::setDataSources ( std::vector<MXADataSourcePtr> &new_var ) {
  _dataSources = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::vector<MXADataSourcePtr> MXADataImport::getDataSources ( ) {
  return _dataSources;
}


