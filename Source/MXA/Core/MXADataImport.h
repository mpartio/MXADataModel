///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
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
#include <MXA/Common/DLLExport.h>
#include <MXA/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IDataImport.h>

//-- STL Headers
#include <string>
#include <vector>

//Include Boost Headers
#include <boost/shared_ptr.hpp>



/**
 * @brief Concrete implementation of the IDataImport abstract class and controls the main
 * import loop.
 *
 * @author Mike Jackson
 * @date April 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT MXADataImport : public IDataImport
{
public:
  MXA_SHARED_POINTERS(MXADataImport);
  MXA_TYPE_MACRO(MXADataImport);

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
  static IDataImport::Pointer New()
  {
    return IDataImport::Pointer( new MXADataImport );
  }

  /**
   * Set the value of m_dataModel
   * @param dataModel the new value of m_dataModel
   */
  void setDataFile ( IDataFile::Pointer dataModel );

  /**
   * Get the value of m_dataModel
   * @return the value of m_dataModel
   */
  IDataFile::Pointer getDataFile ( );


  /**
   * Set the value of m_dataSources
   * @param datasources the new value of m_dataSources
   */
  void setDataSources ( IDataSource::Collection &datasources );

  /**
   * Get the value of m_dataSources
   * @return the value of m_dataSources
   */
  IDataSource::Collection getDataSources ( );

  /**
   * @param  dataSource The datasource to add to the list
   */
  void addDataSource (IDataSource::Pointer dataSource );

  /** @brief Imports the data into the data file
  * @return Error Condition. Zero or Positive is Success
  */
  int32_t import();

private:
  std::string                 _outputFilePath;
  IDataFile::Pointer                _dataFile;
  IDataSource::Collection                _dataSources;

  MXADataImport(const MXADataImport&);   //Copy Constructor Not Implemented
  void operator=(const MXADataImport&); //Copy Assignment Not Implemented

};

#endif // MXADATAIMPORT_H
