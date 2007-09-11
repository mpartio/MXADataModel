///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MXADATASOURCE_H
#define MXADATASOURCE_H

//-- MXA Headers
#include "Common/DLLExport.h"
#include "Common/MXATypeDefs.h"
#include "Common/MXATypes.h"
#include "Base/IDataSource.h"

//-- STL Headers
#include <string>
#include <vector>

//Include Boost Headers
#include "boost/shared_ptr.hpp"


// Forward declare classes
class MXADataRecord;

/**
  * @class MXADataSource MXADataSource.h src/Core/MXADataSource.h
  * 
  * @brief This class holds the information needed to read data from a source and store
  * that data into the underlying data storage for the MXA Data Model (HDF5 or XML)
  */

class MXA_EXPORT MXADataSource : public IDataSource
{
public:
  /**
   * @brief Empty Constructor
   */
  MXADataSource ( );

  /**
   * @brief Empty Destructor
   */
  virtual ~MXADataSource ( );


  /**
   * @brief Generates the internal path that is used by the underlying data storage
   * mechanism to place the data into the hierarchy
   * @return std::string
   */
  std::string generateInternalPath ( );
  
  /**
   * @brief Generates the internal path that is used by the underlying data storage
   * mechanism to place the data into the hierarchy
   * @param indices A new set of indices to use to generate the paths
   * @return The generated internal path. Empty if there was an error.
   */
  std::string generateInternalPath ( std::vector<int32> &indices);
  
  /**
   * @brief Set the value of _dataRecord
   * @param new_var the new value of _dataRecord
   */
  void setDataRecord ( MXADataRecordPtr dataRecord );

  /**
   * @brief Get the value of _dataRecord
   * @return the value of _dataRecord
   */
  MXADataRecordPtr getDataRecord ( );


  /**
   * @brief Set the value of _sourcePath
   * @param new_var the new value of _sourcePath
   */
  void setSourcePath ( std::string sourcePath );

  /**
   * @brief Get the value of _sourcePath
   * @return the value of _sourcePath
   */
  std::string getSourcePath ( );


  /**
   * @brief Set the value of _importDelegate
   * @param new_var the new value of _importDelegate
   */
  void setImportDelegate ( IDataImportDelegatePtr dataParser );

  /**
   * @brief Get the value of _importDelegate
   * @return the value of _importDelegate
   */
  IDataImportDelegatePtr getImportDelegate ( );


  /**
   * @brief Set the value of mdataModel
   * @param new_var the new value of _dataModel
   */
  void setDataModel ( IDataModelPtr dataModel );

  /**
   * @brief Get the value of _dataModel
   * @return the value of _dataModel
   */
  IDataModelPtr getDataModel ( );


  /**
   * @brief Set the value of _dimensionValues
   * @param new_var the new value of _dimensionValues
   */
  void setDimensionValues ( std::vector<int32> new_var );

  /**
   * @brief Get the value of m_dimensionValues
   * @return the value of m_dimensionValues
   */
  std::vector<int32> getDimensionValues ( );
  
private:

  MXADataRecordPtr              _dataRecord;
  std::string                   _sourcePath;
  IDataImportDelegatePtr        _dataImportDelegate;
  IDataModelPtr                 _dataModel;
  std::vector<int32>            _dimensionValues;

  MXADataSource(const MXADataSource&);   //Copy Constructor Not Implemented
  void operator=(const MXADataSource&); //Copy Assignment Not Implemented

};

#endif // MXADATASOURCE_H
