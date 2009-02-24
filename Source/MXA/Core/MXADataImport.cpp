#include <MXA/Core/MXADataImport.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Base/IImportDelegate.h>
#include <MXA/Core/MXADataSource.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Base/IDataFile.h>

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
  // Ensure the output file is open
  if (this->_dataFile->isFileOpen() == false)
  {
    err = _dataFile->openFile(false);
    if (err < 0)
    {
      std::cout << DEBUG_OUT(logTime) << "MXADataImport::import - Error Opening file: " << this->_dataFile->getFilename() << std::endl;
      return err;
    }
  }

  for (IDataSources::iterator iter = _dataSources.begin(); iter != _dataSources.end(); ++iter)
  {
    err = (*(iter))->getImportDelegate()->importDataSource( *(iter), this->_dataFile );
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

#if 0
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
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXADataImport::setDataFile ( IDataFilePtr new_var ) {
  _dataFile = new_var;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataFilePtr MXADataImport::getDataFile ( ) {
  return _dataFile;
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


