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
#ifndef _IDATAMODEL_H_
#define _IDATAMODEL_H_


//-- MXA Includes
//#include "MXA/MXADLLExport.h"
//#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IDataDimension.h>
#include <MXA/Base/IDataRecord.h>
#include <MXA/Base/IRequiredMetaData.h>
#include <MXA/Base/IMXAArray.h>
#include <MXA/Base/ISupportFile.h>

//-- STL Includes
#include <string>
#include <map>

typedef std::map<std::string, IMXAArray::Pointer>         MXAAbstractAttributes;

/**
 * @brief Formally defines the methods that subclasses will need to implement when
 * creating new MXADataModel implementations and those implementations want to be
 * compatible with this code base
 * @author Mike Jackson
 * @date March 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT IDataModel
{
  public:
    MXA_SHARED_POINTERS(IDataModel);

    IDataModel() {}
    virtual ~IDataModel() {}



    /**
     * @brief Returns the MXA File version that the model adheres to.
     * @return MXA API Version
     */
    virtual float getModelVersion() = 0;

    /**
    * @brief Sets the File Version of the MXA data file.
    * @param version The API Version we are going to write
    */
    virtual void setModelVersion(float version) = 0;

    /**
     * @brief Returns the type of file, should be MHD or MXA.
     */
    virtual std::string getModelType() = 0;

    /**
    * @brief Returns the file type for the MXA Data File.
    * @param fileType The filetype (should be MXA)
    */
    virtual void setModelType(const std::string &fileType) = 0;

    /**
     * @brief This will set the default model version and type.
     */
    virtual void setDefaultTypeAndVersion() = 0;
    //------------------------------------------------------------
    /**
    * @brief Sets the Data Root Value.
    * @param dataRoot The data root to write to the model
    */
    virtual void setDataRoot(const std::string &dataRoot) = 0;

    /**
    * @brief Returns the Data Root from the model.
    */
    virtual std::string getDataRoot() = 0;

    //------------- Data Dimension Methods ---------------------------------------
    /**
    * @brief Adds a Data Dimension to the data model.
    * @param dimension The IDataDimension to add to the model
    */
    virtual void addDataDimension(IDataDimension::Pointer dimension ) = 0;

    /**
    * @brief Adds a Data Dimension by declaring all the values of the data dimension.
    * @param name Name of the data dimension
    * @param altName An Alternate name for the data dimension
    * @param count The Number of entries this data dimension will have in the data file
    * @param startValue THe starting index of the entries
    * @param endValue The ending value (inclusive) of the entries
    * @param increment The increment of the index
    * @param uniform Are the indices uniform
    * @return A boost::shared_ptr to the newly created DataDimension Object
    */
    virtual IDataDimension::Pointer addDataDimension(std::string name, std::string altName,
                                    int32_t count, int32_t startValue,
                                    int32_t endValue, int32_t increment, int32_t uniform) = 0;


    /**
     * @brief Inserts a Data Dimension at a given index position. If Needed the
     * internal data structure holding the Data Dimenions will be expanded.
     * @param dimension The Data Dimension to Insert
     * @param index The index to insert the Data Dimension at
     * @return Error condition
     */
    virtual int32_t insertDataDimension(IDataDimension::Pointer dimension, int32_t index) = 0;


    /**
     * @brief This method will remove any NULL Data Dimensions from the internal
     * data structure that holds the list of Data Dimensions.
     */
    virtual void squeezeDataDimensions() = 0;

    /**
    * @brief Removes a Data dimension using the index of the dimension.
    * @param index The index of the data dimension to remove
    * @return Standard HDF5 Error Condition
    */
    virtual int32_t removeDataDimension(int32_t index) = 0;

    /**
     * @brief moves the data dimension at index 'fromIndex' to another index
     * 'newIndex'.
     * @param fromIndex The current index of the data dimension to move
     * @param toIndex The new index to place the data dimension at
     * @return Error condition
     */
    virtual int32_t moveDataDimension(int32_t fromIndex, int32_t toIndex) = 0;

    /**
     * @brief Swaps a pair of Data Dimensions in the index list.
     * @param index1 The first Data Dimension
     * @param index2 The second Data Dimension to swap with the first
     * @return Error Condition
     */
    virtual int32_t swapDataDimensions(int32_t index1, int32_t index2) = 0;

    /**
    * @brief Returns the data dimensions of the Model in a std::vector.
    * @return
    */
    virtual IDataDimension::Container& getDataDimensions() = 0;
    /**
    * @brief Returns a specific Data Dimension from the model.
    * @param index The index to return
    * @return A Pointer to the data dimension or NULL if there is an error
    */
    virtual IDataDimension::Pointer getDataDimension(int32_t index) = 0;
    /**
    * @brief Returns a specific Data Dimension from the model. Technically Data
    * dimensions could have the same names as Data Dimensions are only differentiated
    * by index. This will return the FIRST occurance of a Data Dimension with the
    * given name
    * @param dimName The name of the data dimension
    * @return A Pointer to the data dimension or NULL if there is an error
    */
    //virtual IDataDimension* getDataDimension(std::string dimName) = 0;

    virtual IDataDimension::Pointer getDataDimension(std::string dimName) = 0;

    /**
     * @brief Returns the number of data dimensions in the model.
     */
    virtual int32_t getNumberOfDataDimensions() = 0;

    //------------- Data Records Methods ---------------------------------------
    /**
    * @brief Adds a data Record to the model.
    * @param record The record to add to the model
    */
    virtual void addDataRecord(IDataRecord::Pointer record) = 0;
    /**
    * @brief Adds a Data record to the model using the given parent argument as
    * the record's parent object.
    * @param record The record to add to the model
    * @param parent The parent of the record.
    * Note that this is equivelent to just setting the parnet of the record manually.
    * this is provided for convenience and consistancy.
    */
    virtual void addDataRecord(IDataRecord::Pointer record, IDataRecord::Pointer parent) = 0;

    /**
     * @brief Removes the Data Record from the Model.
     * @param record The record to remove from the Data model
     * @return Error Condition
     */
    virtual int32_t removeDataRecord(IDataRecord::Pointer record) = 0;

    /**
    * @brief Returns the Data Records from the model. The natural form of the
    * records is in a Tree structure.
    */
    virtual IDataRecord::Container& getDataRecords() = 0;

    /**
    * @brief Returns a Data Record that is found by giving the full path using the "RecordName" of the data record(s).
    *
    * For example, if you have a nested structure of data records with a top level DataRecord having
    * a RecordName of "TopLevel" and child DataRecord with a RecordName of "ChildRecord", then using this
    * method in the following way will retrieve the child DataRecord:
    * <br><code><br>
    * std::string path ("TopLevel/ChildRecord");<br>
    * IDataRecord::Pointer datarecord = myDataModel.getDataRecordByNamedPath(path);<br>
    * </code><br>
    * You can also look at the Unit Testing Code for more examples.
    *
    * @param path The path to the data record to return
    * @param parent The parent of the Data Record to return. If this is NULL then the
    * search will start at the top level of the data records.
    * @return Boost::shared_ptr to the data record object
    */
    virtual IDataRecord::Pointer getDataRecordByNamedPath(const std::string &path, IDataRecord* parent=NULL) = 0;

    /**
    * @brief Returns a Data Record that is found by giving the full path using the
    * internal names of the data record(s). Internal names are given by the LUID
    * value of each data record.
    * @param path The path to the data record to return
    * @param parent The parent of the Data Record to return. If this is NULL then the
    * search will start at the top level of the data records.
    * @return Boost::shared_ptr to the data record object
    */
    virtual IDataRecord::Pointer getDataRecordByInternalPath(const std::string &path, IDataRecord* parent=NULL) = 0;

    //------------- Required Meta Data Methods -----------------------------------
    /**
    * @brief Sets the Required Meta Data for the Data Model.
    * @param researcherName The name of the researcher doing to experiment
    * @param dateCreated The date the data was created
    * @param datasetName The name of the experiment/dataset
    * @param description A short description of the data/experiment
    * @param distributionRights How can the data be distributed.
    * @param releaseNumber If the data is public, did your organization apply a public release number or
    * identifier to the data
    * @param pedigree More detailed information regarding the data, its collection methods, the instrumentation, and the research
    * @param derivedSrcFile Is this data derived from another data set or set of data files?
    */
    virtual int32_t setRequiredMetaData( std::string researcherName,
                                                    std::string dateCreated,
                                                    std::string datasetName,
                                                    std::string description,
                                                    std::string distributionRights,
                                                    std::string releaseNumber,
                                                    std::string pedigree,
                                                    std::string derivedSrcFile) = 0;
    /**
    * @brief Sets the required meta data using a std::map of keys/values
    * @param metadata The meta data
    */
    virtual int32_t setRequiredMetaData(std::map<std::string, std::string> &metadata) = 0;

    /**
     * @brief Sets the required meta data
     * @param metaData IRequiredMetaData::Pointer object
     */
    virtual int32_t setRequiredMetaData(IRequiredMetaData::Pointer metaData) = 0;

    /**
    * @brief Returns the meta data for the data model
    * @return IRequiredMetaData::Pointer object
    */
    virtual IRequiredMetaData::Pointer getRequiredMetaData() = 0;

    /**
     * @brief Sets all the user defined meta data for this model. Any previously
     * defined meta data will be erased.
     * @param attributes Vector of MXAAbstractAttributePtr objects
     */
    virtual void setUserMetaData( MXAAbstractAttributes &attributes) = 0;

    /** @brief Returns the Data Structure used to hold the user defined Meta Data */
    virtual MXAAbstractAttributes getUserMetaData() = 0;

    /**
    * @brief Adds User defined Meta data to the model
    * @param key The attribute key name
    * @param umd The Key/Value pair to append to the model
    */
    virtual void addUserMetaData( const std::string &key, IMXAArray::Pointer umd) = 0;

  /**
   * @brief Removes specific user meta-data entry
   * @param key The user meta-data to remove
   */
    virtual void removeUserMetaData(const std::string &key) = 0;

    /**
     * @brief Returns a specific User meta data item
     * @param key The value of the attribute key
     */
    virtual IMXAArray::Pointer getUserMetaData(const std::string &key) = 0;

    /**
    * @brief Prints the user meta data from the model
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printUserMetaData(std::ostream &os, int32_t indent) = 0;

    //-------------- Methods to add arbitrary support files to the model ----------
    /**
     * @brief Adds a SupportFile Reference to the model
     * @param supportFile An ISupportFile object or subclass
     * @param updateIndex Should the "index" property of the supportFile object be updated
     */
    virtual void addSupportFile(ISupportFile::Pointer supportFile, bool updateIndex = false) = 0;

    /**
     * @brief Returns the list of SupportFile objects from the model.
     * @return The list of Support files for this model.
     */
    virtual ISupportFile::Container getSupportFiles() = 0;

    /**
     * @brief Returns a specific ISupportFile instance from the model or a NULL
     * wrapped pointer in case nothing was found or index is out of bounds.
     * @param index The index (zero based) of the ISupportFile instance to return
     * @return Boost Shared Pointer wrapping an ISupportFile subclass instance or NULL
     * if nothing was found.
     */
    virtual ISupportFile::Pointer getSupportFile(int index) = 0;

    //-------------- Methods to Print the DataModel to an outputstream ----------
    /**
    * @brief Prints the data model in ASCI to the given ostream
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printModel(std::ostream &os, int32_t indent) = 0;
    /**
    * @brief Prints the data record from the model
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printDataRecords(std::ostream &os, int32_t indent) = 0;
    /**
    * @brief Prints the data dimensions from the model
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printDataDimensions(std::ostream &os, int32_t indent) = 0;
    /**
    * @brief prints the required Meta data from the model
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printRequiredMetaData(std::ostream &os, int32_t indent) = 0;


    /**
     * @brief Performs some basic checks to make sure the model is valid.
     * @param message String to store messages relating to errors/omissions about the model
     */
    virtual bool isValid(std::string &message) = 0;

  private:
    IDataModel(const IDataModel&); //Not Implemented
    void operator=(const IDataModel&); //Not Implemented
};

#endif /*_IDATAMODEL_H_*/
