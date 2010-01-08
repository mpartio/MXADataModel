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
//#include <MXA/Common/DLLExport.h>
//#include <MXA/Common/MXATypeDefs.h>
#include <MXA/MXATypes.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IDataSource.h>
#include <MXA/Base/IImportDelegate.h>

//-- STL Headers
#include <string>
#include <vector>

// Forward declare classes
class MXADataRecord;

/**
  * @class MXADataSource MXADataSource.h src/Core/MXADataSource.h
  *
  * @brief This class holds the information needed to read data from a source and store
  * that data into the underlying data storage for the MXA Data Model (HDF5 or XML)
  * @version $Revision: 1.2 $
  */

class MXA_EXPORT MXADataSource : public IDataSource
{
public:

  MXA_SHARED_POINTERS(MXADataSource);
  MXA_TYPE_MACRO(MXADataSource);
  typedef std::vector<Pointer>               Container;

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
  std::string generateInternalPath ( std::vector<int32_t> &indices);

  /**
   * @brief Set the value of _dataRecord
   * @param dataRecord the new value of _dataRecord
   */
  void setDataRecord ( IDataRecord::Pointer dataRecord );

  /**
   * @brief Get the value of _dataRecord
   * @return the value of _dataRecord
   */
  IDataRecord::Pointer getDataRecord ( );


  /**
   * @brief Set the value of _sourcePath
   * @param sourcePath the new value of _sourcePath
   */
  void setSourcePath ( std::string sourcePath );

  /**
   * @brief Get the value of _sourcePath
   * @return the value of _sourcePath
   */
  std::string getSourcePath ( );

  /**
   * @brief Does the datasource exist on the filesystem
   * @return true if it does exist.
   */
  bool dataSourceExists();

  /**
   * @brief Set the value of _importDelegate
   * @param dataParser the new value of _importDelegate
   */
  void setImportDelegate ( IImportDelegate::Pointer dataParser );

  /**
   * @brief Get the value of _importDelegate
   * @return the value of _importDelegate
   */
  IImportDelegate::Pointer getImportDelegate ( );


  /**
   * @brief Set the value of mdataModel
   * @param dataModel the new value of _dataModel
   */
  void setDataModel ( IDataModel::Pointer dataModel );

  /**
   * @brief Get the value of _dataModel
   * @return the value of _dataModel
   */
  IDataModel::Pointer getDataModel ( );


  /**
   * @brief Set the value of _dimensionValues
   * @param new_var the new value of _dimensionValues
   */
  void setDimensionValues ( std::vector<int32_t> new_var );

  /**
   * @brief Get the value of m_dimensionValues
   * @return the value of m_dimensionValues
   */
  std::vector<int32_t> getDimensionValues ( );

private:

  IDataRecord::Pointer                _dataRecord;
  std::string                   _sourcePath;
  IImportDelegate::Pointer            _dataImportDelegate;
  IDataModel::Pointer                 _dataModel;
  std::vector<int32_t>            _dimensionValues;

  MXADataSource(const MXADataSource&);   //Copy Constructor Not Implemented
  void operator=(const MXADataSource&); //Copy Assignment Not Implemented

};

#endif // MXADATASOURCE_H
