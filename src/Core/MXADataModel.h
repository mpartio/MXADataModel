//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, Mike Jackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _MXADATAMODEL_H_
#define _MXADATAMODEL_H_


//TODO: Remove DataRecord using Pointer or String Name

//TODO: openFile (Filename, delegate)
//TODO: options to overwrite file, bail on file existance or overwrite current model


//------- Utility Methods
//TODO: Generate Paths for each Element (unix/hdf style)
//TODO: Find Dimension/Record based on Criteria
//TODO: Method to copy the model from a file and write it to a new file
//TODO: Static Method to Read from the various types of files (text, xml, H5) and create the model
//TODO: Use Usul as a template to implement typeid macros that each class can use


//-- MXA Headers
#include "Common/MXATypeDefs.h"
#include "Common/MXATypeDefs.h"
#include "Base/IDataModel.h"
#include "Core/MXAConstants.h"
#include "Core/MXANode.h"
#include "Core/MXADataDimension.h"
#include "Core/MXADataRecord.h"


//-- STL Headers
#include <list>
#include <map>

//Include Boost Headers
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"

/**
 * @brief Main class used to work with the DataModel paradigm
 * @author Mike Jackson
 * @date March 2007
 * @class MXADataModel MXADataModel.h MXADataModel/MXADataModel.h
 */
class MXA_EXPORT MXADataModel : public IDataModel
{

 public:
   MXADataModel();
  virtual ~MXADataModel();

  /**
   * @brief Static method to create a new blank model.
   * @return A boost shard_ptr to the newly created model
   */
  static MXADataModelPtr New();
  
  /**
   * @brief Validates that all the required Meta data is not empty
   * @param requiredMetaData
   * @return Error - Negative is error. Zero or Positive is success
   */
  static MXATypes::MXAError validateRequiredMetaData(std::map<std::string, std::string> &requiredMetaData);
  
  /**
   * @brief Returns the MXA File version that the model adheres to
   * @return MXA API Version
   */
  float getFileVersion();
  void setFileVersion(float version);

  /**
   * @brief Returns the type of file, should be MHD or MXA
   */
  std::string getFileType();

  /**
   * @brief Sets the file type. Should be MHD or MXA
   * @param 
   * @return
   */
  void setFileType(const std::string &fileType);

  /**
   * @brief Sets the "Data Root"
   * @param  dataRoot
   */
  void setDataRoot(const std::string &dataRoot);
  /**
   * @brief Returns the Data Root Value
   * @return
   */
  std::string getDataRoot();

  /**
   * @brief Adds a Data Dimension to the Model
   * @param dimension
   * @param setIndex Sets the Index for the Dimension. Default is false.
   */
  void addDataDimension(MXADataDimensionPtr dimension, bool setIndex=false);

  /**
   * @brief Creates and adds a new Data Dimension to the Model
   * @return A boost::shared_ptr to the newly created Data Dimension
   */
  MXADataDimensionPtr addDataDimension(std::string name, std::string altName,
      int32 index, int32 count, int32 startValue,
      int32 endValue, int32 increment, int32 uniform);

  /**
   * @brief Removes a Data Dimension from the Model using pointer comparison. The
   * passed in dimension shared_ptr will have its wrapped pointer set to NULL
   * if the deletion succeeds.
   * @param  dimension The Data Dimension object to remove
   */
  int32 removeDataDimension(MXADataDimension* &dimension);

  /**
   * @brief Removes a data dimension by index
   * @param index
   */
  int32 removeDataDimension(int32 index);
  
  /**
   * @brief Removed a Data Dimension by Name
   * @param dimensionName The 'name' property of the Data Dimension to remove
   * @return Error: Negative is error condition
   */
  int32 removeDataDimension(const std::string &dimensionName);
  
  /**
   * @brief Returns the vector of Data Dimenions
   * @return
   */
  MXADataDimensions& getDataDimensions();

  /**
   * @brief Returns a Data dimension object given an index
   * @param index
   * @return
   */
  MXADataDimension* getDataDimension(int32 index);

  /**
   * @brief Returns a Data Dimension object given the name of the data dimension
   * @param dimName
   * @return
   */
  MXADataDimension* getDataDimension(std::string dimName);
  
  /**
   * @brief
   * @return The Number of data dimensions
   */
  int32 getNumberOfDataDimensions();

  /**
   * @brief Adds a Data Record given a DataRecord shared pointer
   * @param record
   */
  void addDataRecord(MXADataRecordPtr record);

  /**
   * @brief Adds a Data Record to the model with the given parent record as the data
   * records parent
   * @param record The data record to add to the model
   * @param parent The parent of the data record
   * @return
   */
  void addDataRecord(MXADataRecordPtr record, MXADataRecordPtr parent);

  /**
   * @brief Returns the Hierarchy of Data Records
   * @return
   */
  MXADataRecords& getDataRecords();

  /**
   * @brief Returns a Data Record object from the given path
   * @param path The path to the data record
   * @param parent The parent data record
   * @return
   */
  MXADataRecordPtr getDataRecordByPath(std::string path, MXADataRecord* parent=NULL);
  
  /**
   * @brief Creates a lookup table using the GUID of the Data Record as the Key
   * @param lut The Lookup Table to populate
   */
  void createDataRecordLUT(std::map<int64, MXADataRecordPtr> &lut);
  
  /**
   * @brief Sets the required meta data fields
   * @param 
   * @return
   */
  MXATypes::MXAError setRequiredMetaData( std::string researcherName, std::string dateCreated,
      std::string datasetName, std::string description,
      std::string distributionRights,
      std::string releaseNumber,
      std::string pedigree, std::string derivedSrcFile);

  /**
   * @brief Sets the Required Meta Data Fields given a std::map object
   * @param metadata
   * @return
   */
  MXATypes::MXAError setRequiredMetaData(std::map<std::string, std::string> &metadata);

  /**
   * @brief Returns the RequiredMeta Data in the provided std::map
   * @param requiredMetaData
   * @return
   */
  void getRequiredMetaData(std::map<std::string, std::string> &requiredMetaData);

  /**
   * @brief Adds an entry in the User Meta Data record
   * @param  umd
   * @return
   */
  void addUserMetaData ( MXAAttributePtr umd);
  
  /**
   * @brief Adds a Meta Data entry to the User Meta Data Record
* @param 
* @return
*/
  void addUserMetaData(const std::string &key, const std::string &value);
  
  /**
   * @brief Adds a user meta data entry
* @param key The key for the entry
* @param value The value of the entry
*/
  template<typename T>
  void addUserMetaData(std::string key, T value)
  {
    MXAAttributePtr umd = MXAAttribute::createAttribute<T>(key, value);
    addUserMetaData(umd);
  }

  /** @brief Returns the Data Structure used to hold the user defined Meta Data */
  MXAUserMetaData& getUserMetaData();
  
  /** @brief Sets the IODelegate for this model. Default delegate is an HDF5 delegate */
  void setIODelegate(IODelegatePtr ioDelegate);
  /** @brief Returns the IOdelegate for this model */
  IODelegatePtr getIODelegate();
  
  /**
   * @brief Prints the Data Model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printModel(std::ostream &os, int32 indent);

  /**
   * @brief Prints the Data Records section of the model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printDataRecords(std::ostream &os, int32 indent);

  /**
   * @brief Prints the Data Dimension section of the model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printDataDimensions(std::ostream &os, int32 indent);

  /**
   * @brief Prints the Required Meta Data section of the model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printRequiredMetaData(std::ostream &os, int32 indent);

  /**
   * @brief Prints the User Meta Data section of the model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printUserMetaData(std::ostream &os, int32 indent);

  /**
   * @brief Writes the data model using the internal IODelegate to an external file
   * @param fileName The filename of the file to write the model to
   * @param closeWhenFinished Tells the underlying IODelegate to close the file cutting off
   * access to the file. If you need to write more to the file, set this to 'true'
   * @return
   */
  MXATypes::MXAError writeModel(const std::string &fileName, bool closeWhenFinished=false);

  /**
   * @brief Writes the data model using the internal IODelegate to an external file
   * @param fileName The filename of the file to write the model to
   * @param ioDelegate Use the provided IODelegate instead of the internal IODelegate
   * @param closeWhenFinished Tells the underlying IODelegate to close the file cutting off
   * access to the file. If you need to write more to the file, set this to 'true'
   * @return
   */
  MXATypes::MXAError writeModel(const std::string &fileName, IODelegatePtr ioDelegate, bool closeWhenFinished=false);
  
  /**
   * @brief Reads the data model from the file using the internal IODelegate object
   * @param fileName The filename of the file to write the model to
   * @param ioDelegate Use the provided IODelegate instead of the internal IODelegate
   * @param closeWhenFinished Tells the underlying IODelegate to close the file cutting off
   * access to the file. If you need to write more to the file, set this to 'true'
   * @return
   */
  MXATypes::MXAError readModel(const std::string &fileName, bool closeWhenFinished=false);
  
  /**
   * @brief Reads the data model from the file using the provided IODelegate object
   * @param fileName The filename of the file to write the model to
   * @param ioDelegate Use the provided IODelegate instead of the internal IODelegate
   * @param closeWhenFinished Tells the underlying IODelegate to close the file cutting off
   * access to the file. If you need to write more to the file, set this to 'true'
   * @return
   */
  MXATypes::MXAError readModel(const std::string &fileName, IODelegatePtr ioDelegate, bool closeWhenFinished=false);

  /**
   * @brief Creates an absolute path suitable for create an HDF5 data set.
   * @param indices The indices to use for the Data dimensions
   * @param record The MXADataRecord to generate the path to
   * @return The hdf5 path
   */
   std::string generatePathToDataset ( std::vector<int32> &indices, MXADataRecord* record);

  private:
   MXADataModel(const MXADataModel&);   //Copy Constructor Not Implemented
   void operator=(const MXADataModel&); //Copy Assignment Not Implemented

   float _fileVersion;
   std::string _fileType;
   
   //Holds the 'path' to the root of the actual data in the data portion of the file
   std::string    _dataRoot;
   // Holds a vector of data dimensions
   MXADataDimensions _dataDimensions;
   //Holds a vector of hierarchacally ordered Data Records
   MXADataRecords    _dataRecords;

   //Fields to hold the Required Meta Data Fields
   std::string _researcherName ;
   std::string _datasetDateCreated ;
   std::string _derivedSourceFile;
   std::string _datasetDescription;
   std::string _distributionRights;
   std::string _datasetName;
   std::string _datasetPedigree;
   std::string _datasetPublicReleaseNumber;

   // Holds the arbitrary User Meta Data
   MXAUserMetaData _userMetaData;
   
   // The default delegate to use to write the model to a file
   IODelegatePtr _ioDelegate;
   
  /**
   * @brief Creates indentation spaces for printing output
   * @param indent The number of spaces to use for the indentation
   * @return The String containing the spaces
   */
   std::string _indent(int indent);
   
   /**
    * @brief
    * @param lut
    * @param records
    */
   void _traverseForLUT(std::map<int64, MXADataRecordPtr> &lut, MXADataRecords &records);
  
};


#endif /* _MXADATAMODEL_H_ */
