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

//TODO: Read compliant Test File into a Data Model


//MXA Includes
#include "Headers/DLLExport.h"
#include "MXADataModel/MXADataModel.h"
#include "Interfaces/IDataModelReader.h"
#include "HDF5/H5IODelegate.h"


// C++ Headers
#include <string>

//-- HDF Headers
#include "hdf5.h"


/**
 * @brief This class is responsible for the actual reading of the data model
 * from the HDF5 data file
 * @author Mike Jackson
 * @date Mar 2007
 * @version 1.0
 *   H5DataModelReader.h
 */
class MXA_EXPORT H5DataModelReader : public IDataModelReader
{

public:
  /**
  * @brief Constructor for H5DataModelReader
  * @param ioDelegate The IFileIODelegate object to use
  * @param dataModel The DataModel to use. Note that settings in the datamodel will
  * be over written with those from the file.
  */
  H5DataModelReader(IFileIODelegate* ioDelegate, MXADataModel* dataModel);
  
  virtual ~H5DataModelReader();
  
  
  // Loading Methods
  /**
  * @brief 
  * @param locId
  * @return 
  */
  herr_t readDataModel(hid_t locId);
 
  /**
  * @brief 
  * @param locId
  * @return 
  */
  herr_t readFileType(hid_t locId);
 
  /**
  * @brief 
  * @param locId
  * @return 
  */
  herr_t readFileVersion(hid_t locId);
 
  /**
  * @brief 
  * @param locId
  * @return 
  */
  herr_t readDataRoot(hid_t locId);
 
  /**
  * @brief 
  * @param locId
  * @return 
  */
  herr_t readDataDimensions(hid_t locId);
 
  /**
  * @brief 
  * @param fileId
  * @return 
  */
  herr_t readDataRecords(hid_t locId);
 
  /**
  * @brief 
  * @param locId
  * @return 
  */
  herr_t readRequiredMetaData(hid_t locId);
 
  /**
  * @brief 
  * @param locId
  * @return 
  */
  herr_t readUserMetaData(hid_t locId);
  
  /**
  * @brief 
  * @param locId
  * @param datasetPath
  * @param key
  * @param dims
  * @return 
  */
  template<typename T>
  int32 readPrimitiveAttribute( hid_t locId, const std::string &datasetPath, 
                                 const std::string &key, 
                                 const std::vector<uint64> &dims)
  {
    herr_t err = -1;
    if (dims.size() == 1 && dims.at(0) == 1) // One Dimensional Array with 1 element
    {
      T data;
      err = H5Lite::readScalarAttribute(locId, datasetPath, key, data);
      if (err >= 0) {   
        MXAAttributePtr attr = MXAAttribute::createAttribute(key, data);
        this->_dataModel->addUserMetaData(attr);
      }
    } 
    else // Multi-Dimensional Data 
    {
      std::vector<T> data;
      err = H5Lite::readVectorAttribute(locId, datasetPath, key, data);
      if (err >= 0) {   
        MXAAttributePtr attr = MXAAttribute::createAttribute(key, data, dims);
        this->_dataModel->addUserMetaData(attr);
      }
    }
    return err;
  }
  
protected:
  // ----- Helper methods to read data dimensions or data records --------------
  /**
  * @brief 
  * @param locId
  * @param name
  * @return 
  */
  MXADataDimensionPtr _loadDataDimension(hid_t locId, std::string name);

  /**
  * @brief 
  * @param gid
  * @param parent
  * @return 
  */
  herr_t _traverseDataRecords( hid_t gid, MXADataRecordPtr parent);

  /**
  * @brief 
  * @param gid
  * @param name
  * @return 
  */
  MXADataRecordPtr _loadDataRecord(hid_t gid, std::string name);
  
  
private:
    H5IODelegate* _ioDelegate;
    MXADataModel* _dataModel;
  
    H5DataModelReader(const H5DataModelReader&);   //Copy Constructor Not Implemented
    void operator=(const H5DataModelReader&); //Copy Assignment Not Implemented
};



#endif /* _HDFREADER_H_ */

