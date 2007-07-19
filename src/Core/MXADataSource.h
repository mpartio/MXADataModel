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
  * class MXADataSource
  * This class holds the information needed to read data from a source and store
  * that data into the underlying data storage for the MXA Data Model (HDF5 or XML)
  */

class MXA_EXPORT MXADataSource : public IDataSource
{
public:
  /**
   * Empty Constructor
   */
  MXADataSource ( );

  /**
   * Empty Destructor
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
   * Set the value of m_dataRecord
   * @param new_var the new value of m_dataRecord
   */
  void setDataRecord ( MXADataRecordPtr dataRecord );

  /**
   * Get the value of m_dataRecord
   * @return the value of m_dataRecord
   */
  MXADataRecordPtr getDataRecord ( );


  /**
   * Set the value of m_sourcePath
   * @param new_var the new value of m_sourcePath
   */
  void setSourcePath ( std::string sourcePath );

  /**
   * Get the value of m_sourcePath
   * @return the value of m_sourcePath
   */
  std::string getSourcePath ( );


  /**
   * Set the value of m_importDelegate
   * @param new_var the new value of m_importDelegate
   */
  void setImportDelegate ( IDataImportDelegatePtr dataParser );

  /**
   * Get the value of m_importDelegate
   * @return the value of m_importDelegate
   */
  IDataImportDelegatePtr getImportDelegate ( );


  /**
   * Set the value of m_dataModel
   * @param new_var the new value of m_dataModel
   */
  void setDataModel ( MXADataModelPtr dataModel );

  /**
   * Get the value of m_dataModel
   * @return the value of m_dataModel
   */
  MXADataModelPtr getDataModel ( );


  /**
   * Set the value of m_dimensionValues
   * @param new_var the new value of m_dimensionValues
   */
  void setDimensionValues ( std::vector<int32> new_var );

  /**
   * Get the value of m_dimensionValues
   * @return the value of m_dimensionValues
   */
  std::vector<int32> getDimensionValues ( );
  
private:

  MXADataRecordPtr              _dataRecord;
  std::string                   _sourcePath;
  IDataImportDelegatePtr        _dataImportDelegate;
  MXADataModelPtr               _dataModel;
  std::vector<int32>            _dimensionValues;


private:
  MXADataSource(const MXADataSource&);   //Copy Constructor Not Implemented
  void operator=(const MXADataSource&); //Copy Assignment Not Implemented

};

#endif // MXADATASOURCE_H
