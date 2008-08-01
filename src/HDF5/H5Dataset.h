///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _H5DATASET_H_
#define _H5DATASET_H_

#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>
#include <Base/IDataset.h>


/**
 * @brief This class encapsulates an HDF5 dataset as it appears in a data file. The
 * actual data is read into an IMXAArray instance and each attribute is its own
 * IMXAArray instance.
 * @class H5Dataset H5Dataset.h HDF5/H5Dataset.h
 * @author Mike Jackson
 * @date Jan 3, 2008
 * @version $Revision: 1.2 $
 */
class MXA_EXPORT H5Dataset : public IDataset
{

public:

  /**
   * @brief Loads a dataset from an HDf5 file into this class.
   * @param dataFile Instance of IDataFile class
   * @param datasetPath The path to the dataset
   * @return Boost::shared_ptr wrapped H5Dataset instance
   */
  static IDatasetPtr LoadFromFile(IDataFilePtr dataFile, const std::string &datasetPath);

  /**
   * @brief Creates an H5Dataset object with the given dataset path and data
   * @param datasetPath The path to the data set
   * @param data The data that will be stored in hdf5 file
   */
  static IDatasetPtr CreateDatasetPtr(const std::string &datasetPath, IMXAArrayPtr data);

  /**
   * @brief Creates an H5Dataset object with the given dataset path and data
   * @param datasetPath The path to the data set
   * @param data The data that will be stored in hdf5 file
   */
  H5Dataset(const std::string &datasetPath, IMXAArrayPtr data);


  virtual ~H5Dataset();

  // -----------------------------------------------------------------------------
  //  IDataset Methods
  // -----------------------------------------------------------------------------

  /**
   * @brief Sets the dataset path and name
   * @param path + name of the dataset
   */
  void setDatasetPath(const std::string &path);

  /**
   * @brief Returns the path+name of the dataset
   */
  std::string getDatasetPath();

  /**
   * @brief Sets the data for the dataset
   * @param data Instance of IMXAArray
   */
  void setData(IMXAArrayPtr data);

  /**
   * @brief Returns the data for this class
   */
  IMXAArrayPtr getData();
  /**
   * @brief Adds an attribute to this dataset
   * @param attributeKey The name of the attribute
   * @param attribute The data for the attribute
   */
  void addAttribute (const std::string &attributeKey, IMXAArrayPtr attribute);
  /**
   * @brief Removes an attribute from the data set
   * @param attributeKey The name of the attribute to remove
   */
  void removeAttribute (const std::string &attributeKey);
  /**
   * @brief Returns an attribute or NULL if not found
   * @param attributeKey The name of the attribute to retrieve
   * @return
   */
  IMXAArrayPtr getAttribute(const std::string &attributeKey);

  // -----------------------------------------------------------------------------
  // IDataFileIO Methods
  // -----------------------------------------------------------------------------
  virtual int32 writeToFile (IDataFilePtr dataFile);
  virtual int32 readFromFile (IDataFilePtr dataFile);

protected:
  H5Dataset() {}  // Protected constructor

private:
  IMXAArrayPtr _data;
  MXAAbstractAttributes _attributes;
  std::string _datasetPath;


  H5Dataset(const H5Dataset&);   //Copy Constructor Not Implemented
  void operator=(const H5Dataset&); //Copy Assignment Not Implemented
};



#endif /* _H5DATASET_H_ */

