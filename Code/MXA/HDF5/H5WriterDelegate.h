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
#ifndef _HDFWRITER_H_
#define _HDFWRITER_H_

// MXA Includes
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IDataModel.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Base/IMXADataModelWriterDelegate.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/HDF5/H5Lite.h>

//-- STL Headers
#include <iostream>
#include <string>

//-- HDF Headers
#include <hdf5.h>

//Forward Declare classes
class MXANode;


/**
 * @brief Writes the DataModel to an HDF5 file
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT H5WriterDelegate : public IMXADataModelWriterDelegate
{

public:
    MXA_SHARED_POINTERS(H5WriterDelegate);
    MXA_TYPE_MACRO(H5WriterDelegate)

    static Pointer New(hid_t fileId)
    {
      Pointer sharedPtr (new H5WriterDelegate(fileId));
      return sharedPtr;
    }

  virtual ~H5WriterDelegate();

  /**
   * @brief Writes the Data Model to the HDF5 file
  * @param fileId HDF5 FileId to write to
   * @return true on success
   */
  int32_t writeModel(IDataModel::Pointer model);


protected:
  H5WriterDelegate() {};
  H5WriterDelegate(hid_t fileId);


  /**
   * @brief Writes the Data Model skeleton groups
  * @param uniqueId HDF5 FileId to write to
   * @return
   */
  int32_t writeDataModelTemplate(hid_t uniqueId);

  /**
   * @brief Writes the Data Dimensions from the Model
  * @param fileId HDF5 FileId to write to
  * @return
  */
  int32_t writeDataDimensions(hid_t fileId);

  /**
  * @brief Writes the Data Records
  * @param fileId HDF5 FileId to write to
  * @return
  */
  int32_t writeDataRecords(hid_t fileId);

  /**
   * @brief Writes the Required MetaData
  * @param fileId HDF5 FileId to write to
  * @return
  */
  int32_t writeRequiredMetaData(hid_t fileId);

  /**
   * @brief Writes the User Meta Data
  * @param fileId HDF5 FileId to write to
  * @return
  */
  int32_t writeUserMetaData(hid_t fileId);

  /**
  * @brief Writes the support files to the hdf5 data file
  * @param fileId HDF5 file id to write to
  * @return error code
  */
  int32_t writeSupportFiles(hid_t fileId);


  /**
   * @brief Walks the Data records hierarchy
* @param gid
* @param records
* @return
*/
  int32_t _traverseDataRecords(hid_t gid,  IDataRecord::Container &records);

  /**
   * @brief Writes a std::string as a HDF Dataset.
   * @param loc_id The Parent location to write the dataset
   * @param dsetName The Name to use for the dataset
   * @param data The actual data to write as a null terminated string
   * @param overwrite Overwrite the existing data
   * @return Standard HDF5 error conditions
   */
  herr_t  _writeStringDataset (hid_t loc_id,
                               const std::string& dsetName,
                               const std::string& data,
                               bool overwrite = false);

/**
 * @brief Creates a Dataset with the given name at the location defined by loc_id.
 * This version of writeDataset should be used with a single scalar value. If you
 * need to write an array of values, use the form that takes an std::vector<>
 *
 * @param loc_id The Parent location to store the data
 * @param dsetName The name of the dataset
 * @param value The value to write to the HDF5 dataset
 * @return Standard HDF5 error conditions
 */
  template <typename T>
  static herr_t _writeScalarDataset (hid_t loc_id,
                              const std::string& dsetName,
                              T &value)
  {
    herr_t err = -1;
    hid_t did = -1;
    hid_t sid = -1;
    herr_t retErr = 0;
    hsize_t dims = 1;
    hid_t rank = 1;
    hid_t dataType = H5Lite::HDFTypeForPrimitive(value);
    if (dataType == -1)
    {
      return -1;
    }
    //Create the DataSpace
    sid = H5Screate_simple( rank, &(dims), NULL );
    if (sid < 0)
    {
      return sid;
    }
    HDF_ERROR_HANDLER_OFF
    did = H5Dopen(loc_id, dsetName.c_str(), H5P_DEFAULT );
    HDF_ERROR_HANDLER_ON

    if ( did < 0 ) // dataset does not exist
    {
      did = H5Dcreate (loc_id, dsetName.c_str(), dataType, sid, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    if ( did >= 0 )
    {
      err = H5Dwrite( did, dataType, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value );
      if (err < 0 ) {
        std::cout << "Error Writing Data" << std::endl;
        retErr = err;
      }
      err = H5Dclose( did );
      if (err < 0) {
        std::cout << "Error Closing Dataset." << std::endl;
        retErr = err;
      }
    } else {
      retErr = did;
    }
    /* Terminate access to the data space. */
    err = H5Sclose( sid );
    if (err< 0) {
      std::cout << "Error Closing Dataspace" << std::endl;
      retErr = err;
    }
    return retErr;
  }

private:
  hid_t m_FileId;
  IDataModel::Pointer m_DataModel;

  H5WriterDelegate(const H5WriterDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5WriterDelegate&); //Copy Assignment Not Implemented
};



#endif /* _HDFWRITER_H_ */

