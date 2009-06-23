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
#ifndef IDATAIMPORT_H_
#define IDATAIMPORT_H_

#include <MXA/Common/DLLExport.h>
#include <MXA/Base/IDataFile.h>

/**
 * @brief Abstract class that formally declares the methods used when implementing
 * a class to import data into the underlying data storage.
 * @author Mike Jackson
 * @date April 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT IDataImport
{
  public:
    IDataImport() {};
    virtual ~IDataImport() {};
#if 0
    /**
     * @brief Set the value of m_outputFilePath
     * @param outputFilePath the new value of m_outputFilePath
     */
    virtual void setOutputFilePath ( std::string outputFilePath ) = 0;

    /**
     * Get the value of m_outputFilePath
     * @return the value of m_outputFilePath
     */
    virtual std::string getOutputFilePath ( ) = 0;
#endif

    /**
     * @brief Set the value of datafile
     * @param datafile the new value of datafile
     */
    virtual void setDataFile ( IDataFile::Pointer datafile ) = 0;

    /**
     * Get the value of datafile
     * @return the value of datafile
     */
    virtual IDataFile::Pointer getDataFile ( ) = 0;


    /**
     * Set the value of m_dataSources
     * @param datasources the new value of m_dataSources
     */
    virtual void setDataSources ( IDataSources &datasources ) = 0;

    /**
     * Get the value of m_dataSources
     * @return the value of m_dataSources
     */
    virtual IDataSources getDataSources ( ) = 0;

    /**
     * @param  dataSource The datasource to add to the list
     */
    virtual void addDataSource (IDataSourcePtr dataSource ) = 0;

    /** @brief Imports the data into the data file
    * @return Error Condition. Zero or Positive is Success
    */
    virtual int32 import() = 0;

  private:
    IDataImport(const IDataImport&);    //Not Implemented
    void operator=(const IDataImport&); //Not Implemented
};

#endif /*IDATAIMPORT_H_*/
