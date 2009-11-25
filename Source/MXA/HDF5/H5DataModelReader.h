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
#ifndef _HDFREADER_H_
#define _HDFREADER_H_

//MXA Includes
#include <MXA/Common/DLLExport.h>
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Base/IDataModelReader.h>
//#include <MXA/HDF5/H5IODelegate.h>
//#include <MXA/HDF5/H5AttributeArrayTemplate.hpp>

// C++ Headers
#include <string>

// HDF Headers
#include <hdf5.h>


/**
 * @brief This class is responsible for the actual reading of the data model
 * from the HDF5 data file
 * @author Mike Jackson
 * @date Mar 2007
 * @version $Revision: 1.2 $
 *   H5DataModelReader.h
 */
class MXA_EXPORT H5DataModelReader : public IDataModelReader
{

public:
  /**
  * @brief Constructor for H5DataModelReader
  * @param dataModel The DataModel to use. Note that settings in the datamodel will
  * be over written with those from the file.
  */
  H5DataModelReader(IDataModel::Pointer dataModel);

  virtual ~H5DataModelReader();


  // Loading Methods
  /**
  * @brief Reads the Data model
  * @param locId The HDF5 file or group id
  * @return Standard HDF5 Error Condition
  */
  herr_t readDataModel(hid_t locId);

  /**
  * @brief Reads the model type
  * @param locId The HDF5 file or group id
  * @return Standard HDF5 Error Condition
  */
  herr_t readModelType(hid_t locId);

  /**
  * @brief reads the model version
  * @param locId The HDF5 file or group id
  * @return Standard HDF5 Error Condition
  */
  herr_t readModelVersion(hid_t locId);

  /**
  * @brief reads the data root
  * @param locId The HDF5 file or group id
  * @return Standard HDF5 Error Condition
  */
  herr_t readDataRoot(hid_t locId);

  /**
  * @brief reads the data dimensions
  * @param locId The HDF5 file or group id
  * @return Standard HDF5 Error Condition
  */
  herr_t readDataDimensions(hid_t locId);

  /**
  * @brief reads the data records
  * @param locId
  * @return Standard HDF5 Error Condition
  */
  herr_t readDataRecords(hid_t locId);

  /**
  * @brief reads the required meta data
  * @param locId The HDF5 file or group id
  * @return Standard HDF5 Error Condition
  */
  herr_t readRequiredMetaData(hid_t locId);

  /**
  * @brief reads the user meta data
  * @param locId The HDF5 file or group id
  * @return Standard HDF5 Error Condition
  */
  herr_t readUserMetaData(hid_t locId);

 /**
  * @brief reads the support files meta data from the MXA file without
  * actually reading the file data
  * @param locId The HDF5 file or group id
  * @return Standard HDF5 Error Condition
  */  
  herr_t readSupportFiles(hid_t locId);


protected:
  // ----- Helper methods to read data dimensions or data records --------------
  /**
  * @brief Reads a single Data Dimension from the HDF5 data file
  * @param locId The HDF5 file or group id
  * @param name The name of the dimension to read
  * @return IDataDimension Shared Pointer
  */
  MXADataDimension::Pointer _loadDataDimension(hid_t locId, std::string name);

  /**
  * @brief Recursive method that traverses the tree of Data Records
  * @param gid The Group ID
  * @param parent The Parent Data Record
  * @return Standard HDF5 Error Condition
  */
  herr_t _traverseDataRecords( hid_t gid, MXADataRecord::Pointer parent);

  /**
  * @brief Reads a Data Record from the HDF5 data file
  * @param gid The GUID to assign to the Data Record
  * @param name The Name of the data record
  * @return IDataRecord shared pointer
  */
  MXADataRecord::Pointer _loadDataRecord(hid_t gid, std::string name);


private:

    IDataModel::Pointer _dataModel;

    H5DataModelReader(const H5DataModelReader&);   //Copy Constructor Not Implemented
    void operator=(const H5DataModelReader&); //Copy Assignment Not Implemented
};



#endif /* _HDFREADER_H_ */

