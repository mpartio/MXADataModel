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

#ifndef MXADATAIMPORT_H
#define MXADATAIMPORT_H

//-- MXA Includes
#include "Common/DLLExport.h"
#include "Common/MXATypes.h"
#include "Common/MXATypeDefs.h"
#include "Base/IDataImport.h"

//-- STL Headers
#include <string>
#include <vector>

//Include Boost Headers
#include "boost/shared_ptr.hpp"



/**
 * @brief Concrete implementation of the IDataImport abstract class and controls the main 
 * import loop.
 * 
 * @author Mike Jackson
 * @date April 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT MXADataImport : public IDataImport
{
public:

  
  /**
   * Empty Constructor
   */
  MXADataImport ( );

  /**
   * Empty Destructor
   */
  virtual ~MXADataImport ( );

  /**
   * @brief Static method to create a new MXADataImport Object
   * @return boost shared pointer wrapping an MXADataImport Object pointer
   */
  static IDataImportPtr New()
  {
    return IDataImportPtr( new MXADataImport );
  }
  
  /**
   * Set the value of m_outputFilePath
   * @param new_var the new value of m_outputFilePath
   */
  void setOutputFilePath ( std::string outputFilePath );

  /**
   * Get the value of m_outputFilePath
   * @return the value of m_outputFilePath
   */
  std::string getOutputFilePath ( );


  /**
   * Set the value of m_dataModel
   * @param new_var the new value of m_dataModel
   */
  void setDataModel ( IDataModelPtr dataModel );

  /**
   * Get the value of m_dataModel
   * @return the value of m_dataModel
   */
  IDataModelPtr getDataModel ( );


  /**
   * Set the value of m_dataSources
   * @param new_var the new value of m_dataSources
   */
  void setDataSources ( IDataSources &datasources );

  /**
   * Get the value of m_dataSources
   * @return the value of m_dataSources
   */
  IDataSources getDataSources ( );

  /**
   * @param  dataSource The datasource to add to the list
   */
  void addDataSource (IDataSourcePtr dataSource );

  /** @brief Imports the data into the data file
  * @return Error Condition. Zero or Positive is Success
  */
  int32 import();

private:
  std::string                 _outputFilePath;
  IDataModelPtr               _dataModel;
  IDataSources                _dataSources;

  MXADataImport(const MXADataImport&);   //Copy Constructor Not Implemented
  void operator=(const MXADataImport&); //Copy Assignment Not Implemented
 
};

#endif // MXADATAIMPORT_H
