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
#include <Common/DLLExport.h>
#include <Core/MXADataModel.h>
#include <Base/IDataModelReader.h>
//#include <HDF5/H5IODelegate.h>
#include <HDF5/H5AttributeArrayTemplate.hpp>

// C++ Headers
#include <string>

// HDF Headers
#include <hdf5.h>


/**
 * @brief This class is responsible for the actual reading of the data model
 * from the HDF5 data file
 * @author Mike Jackson
 * @date Mar 2007
 * @version $Revision: 1.18 $
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
  H5DataModelReader(IDataModelPtr dataModel);
  
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
  * @brief Reads a Primitive Value that will be assigned to a named attribute
  * @param locId The HDF5 file or group id
  * @param datasetPath The path to the dataset to read
  * @param key The attribute key to read
  * @param dims The dimensions of the attribute
  * @return Standard HDF5 Error Condition 
  */
  template<typename T>
  int32 readPrimitiveAttribute(hid_t locId, 
                               const std::string &datasetPath, 
                               const std::string &key, 
                               const std::vector<uint64> &dims)
    {
      herr_t err = -1;
      if (dims.size() == 1 && dims.at(0) == 1) // One Dimensional Array with 1 element
      {
        T data;
        err = H5Lite::readScalarAttribute(locId, datasetPath, key, data);
        if (err >= 0)
        {
          MXAAbstractAttributePtr attr = H5AttributeArrayTemplate<T>::CreateScalarAttribute(datasetPath, key, data);
          this->_dataModel->addUserMetaData(attr);
        }
      }
      else // Multi-Dimensional Data 
      {
        MXAAbstractAttributePtr attr =
            H5AttributeArrayTemplate<T>::CreateAbstractAttributeMultiDimensionalArray(datasetPath, key, dims.size(), &(dims.front()));
        if (attr.get() != NULL)
        {
          T* data = static_cast<T*>(attr->getVoidPointer(0) );
          err = H5Lite::readPointerAttribute(locId, datasetPath, key, data);

          if (err >= 0)
          {
            this->_dataModel->addUserMetaData(attr);
          }
        }
      }
      return err;
    }
  
protected:
  // ----- Helper methods to read data dimensions or data records --------------
  /**
  * @brief Reads a single Data Dimension from the HDF5 data file
  * @param locId The HDF5 file or group id
  * @param name The name of the dimension to read
  * @return IDataDimension Shared Pointer
  */
  MXADataDimensionPtr _loadDataDimension(hid_t locId, std::string name);

  /**
  * @brief Recursive method that traverses the tree of Data Records
  * @param gid The Group ID
  * @param parent The Parent Data Record
  * @return Standard HDF5 Error Condition
  */
  herr_t _traverseDataRecords( hid_t gid, MXADataRecordPtr parent);

  /**
  * @brief Reads a Data Record from the HDF5 data file
  * @param gid The GUID to assign to the Data Record
  * @param name The Name of the data record
  * @return IDataRecord shared pointer
  */
  MXADataRecordPtr _loadDataRecord(hid_t gid, std::string name);
  
  
private:
  
    IDataModelPtr _dataModel;
  
    H5DataModelReader(const H5DataModelReader&);   //Copy Constructor Not Implemented
    void operator=(const H5DataModelReader&); //Copy Assignment Not Implemented
};



#endif /* _HDFREADER_H_ */

