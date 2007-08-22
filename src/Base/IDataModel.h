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
#ifndef _IMXADATAMODEL_H
#define _IMXADATAMODEL_H

//TODO: Add moveDataDimension method to swap Data Dimensions
//TODO: Add removeDataRecord method


//-- MXA Includes
#include "Common/DLLExport.h"
#include "Common/MXATypes.h"

//-- STL Includes
#include <string>
#include <map>

/**
 * @brief Formally defines the methods that subclasses will need to implement when 
 * creating new MXADataModel implementations and those implementations want to be
 * compatible with this code base
 * @author Mike Jackson
 * @date March 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT IDataModel
{
  public:
    IDataModel() {}
    virtual ~IDataModel() {}

    /**
     * @brief Returns the MXA File version that the model adheres to
     * @return MXA API Version
     */
    virtual float getModelVersion() = 0;
    
    /**
    * @brief Sets the File Version of the MXA data file
    * @param version The API Version we are going to write
    */
    virtual void setModelVersion(float version) = 0;

    /**
     * @brief Returns the type of file, should be MHD or MXA
     */
    virtual std::string getModelType() = 0;

    /**
    * @brief Returns the file type for the MXA Data File
    * @param fileType The filetype (should be MXA)
    */
    virtual void setModelType(const std::string &fileType) = 0;

    /**
     * @brief This will set the default model version and type
     */
    virtual void setDefaultTypeAndVersion() = 0;
    //------------------------------------------------------------
    /**
    * @brief Sets the Data Root Value
    * @param dataRoot The data root to write to the model
    */
    virtual void setDataRoot(const std::string &dataRoot) = 0;
    
    /**
    * @brief Returns the Data Root from the model
    */
    virtual std::string getDataRoot() = 0;

    //------------- Data Dimension Methods ---------------------------------------
    /**
    * @brief Adds a Data Dimension to the data model
    * @param dimension The IDataDimension to add to the model
    * @param setIndex Automatically set the index for this Dimension. Default is false
    */
    virtual void addDataDimension(MXADataDimensionPtr dimension, bool setIndex=false) = 0;
    
    /**
    * @brief Adds a Data Dimension by declaring all the values of the data dimension
    * @param name Name of the data dimension
    * @param altName Alternate name of the data dimension
    * @param index The index of the dimension
    * @param count The Number of entries this data dimension will have in the data file
    * @param startValue THe starting index of the entries
    * @param endValue The ending value (inclusive) of the entries
    * @param increment The increment of the index
    * @param uniform Are the indices uniform
    * @return A boost::shared_ptr to the newly created DataDimension Object
    */
    virtual MXADataDimensionPtr addDataDimension(std::string name, std::string altName,
                                    int32 index, int32 count, int32 startValue, 
                                    int32 endValue, int32 increment, int32 uniform) = 0;

    /**
    * @brief Removes a Data dimension from the model
    * @param dimension The Data Dimension to remove
    * @return Standard HDF5 Error Condition
    */
    virtual int32 removeDataDimension(MXADataDimension* &dimension) = 0;
    /**
    * @brief Removes a Data dimension using the index of the dimension
    * @param index The index of the data dimension to remove
    * @return Standard HDF5 Error Condition
    */
    virtual int32 removeDataDimension(int32 index) = 0;
    
    /**
    * @brief Returns the data dimensions of the Model in a std::vector
    * @return 
    */
    virtual MXADataDimensions& getDataDimensions() = 0;
    /**
    * @brief Returns a specific Data Dimension from the model
    * @param index The index to return
    * @return A Pointer to the data dimension or NULL if there is an error
    */
    virtual MXADataDimension* getDataDimension(int32 index) = 0;
    /**
    * @brief Returns a specific Data Dimension from the model. Technically Data
    * dimensions could have the same names as Data Dimensions are only differentiated
    * by index. This will return the FIRST occurance of a Data Dimension with the 
    * given name
    * @param dimName The name of the data dimension
    * @return A Pointer to the data dimension or NULL if there is an error
    */
    virtual MXADataDimension* getDataDimension(std::string dimName) = 0;
    
    /**
     * @brief Returns the number of data dimensions in the model
     */
    virtual int32 getNumberOfDataDimensions() = 0;
    
    //------------- Data Records Methods ---------------------------------------
    /**
    * @brief Adds a data Record to the model
    * @param record The record to add to the model
    */
    virtual void addDataRecord(MXADataRecordPtr record) = 0;
    /**
    * @brief Adds a Data record to the model using the given parent argument as
    * the record's parent object
    * @param record The record to add to the model
    * @param parent The parent of the record. 
    * Note that this is equivelent to just setting the parnet of the record manually.
    * this is provided for convenience and consistancy.
    */
    virtual void addDataRecord(MXADataRecordPtr record, MXADataRecordPtr parent) = 0;
    
    /**
    * @brief Returns the Data Records from the model. The natural form of the 
    * records is in a Tree structure.
    */
    virtual MXADataRecords& getDataRecords() = 0;
    
    /**
    * @brief Returns a Data Record that is found by giving the full path using the actual names of the data record(s)
    * @param path The path to the data record to return
    * @param parent The parent of the Data Record to return. If this is NULL then the 
    * search will start at the top level of the data records.
    * @return Boost::shared_ptr to the data record object
    */
    virtual MXADataRecordPtr getDataRecordByNamedPath(std::string path, MXADataRecord* parent=NULL) = 0;
    
    /**
    * @brief Returns a Data Record that is found by giving the full path using the internal names of the data record(s)
    * @param path The path to the data record to return
    * @param parent The parent of the Data Record to return. If this is NULL then the 
    * search will start at the top level of the data records.
    * @return Boost::shared_ptr to the data record object
    */
    virtual MXADataRecordPtr getDataRecordByInternalPath(std::string path, MXADataRecord* parent=NULL) = 0;
    
    //------------- Required Meta Data Methods -----------------------------------
    /**
    * @brief Sets the Required Meta Data for the Data Model
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
    virtual MXATypes::MXAError setRequiredMetaData( std::string researcherName, std::string dateCreated, 
                              std::string datasetName, std::string description,
                              std::string distributionRights,
                              std::string releaseNumber,
                              std::string pedigree, std::string derivedSrcFile) = 0;
    /**
    * @brief Sets the required meta data using a std::map of keys/values
    * @param metadata The meta data
    */
    virtual MXATypes::MXAError setRequiredMetaData(std::map<std::string, std::string> &metadata) = 0;
    /**
    * @brief Returns the meta data for the data model
    * @param requiredMetaData
    */
    virtual void getRequiredMetaData(std::map<std::string, std::string> &requiredMetaData) = 0;
    
    
    /**
    * @brief Adds User defined Meta data to the model
    * @param umd The Key/Value pair to append to the model
    */
    virtual void addUserMetaData( MXAAttributePtr umd) = 0;
    /**
    * @brief Adds a User defined meta data to the model
    * @param key The key to use
    * @param value The value to use
    */
    virtual void addUserMetaData(const std::string &key, const std::string &value) = 0;
    
    //---------------- Methods to Set/Get the IODelegate Object -------------------
    /**
    * @brief Sets the IODelegate object for the model
    * @param ioDelegate
    */
    virtual void setIODelegate(IODelegatePtr ioDelegate) = 0;
    /**
    * @brief The IODelegate object being used by this Data model
    * @return 
    */
    virtual IODelegatePtr getIODelegate() = 0;
    
    //-------------- Methods to Print the DataModel to an outputstream ----------
    /**
    * @brief Prints the data model in ASCI to the given ostream
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printModel(std::ostream &os, int32 indent) = 0;
    /**
    * @brief Prints the data record from the model
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printDataRecords(std::ostream &os, int32 indent) = 0;
    /**
    * @brief Prints the data dimensions from the model
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printDataDimensions(std::ostream &os, int32 indent) = 0;
    /**
    * @brief prints the required Meta data from the model
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printRequiredMetaData(std::ostream &os, int32 indent) = 0;
    /**
    * @brief Prints the user meta data from the model
    * @param os The output stream to print to
    * @param indent The level of indentation to start with
    */
    virtual void printUserMetaData(std::ostream &os, int32 indent) = 0;
    
    //--------------- Methods to Write the DataModel to a File -------------------
    /**
    * @brief Writes the model to the data file
    * @param fileName The path to the data file
    * @param closeWhenFinished Terminates access to the file when complete. Default is false
    * @return Standard HDF5 Error condition
    */
    virtual MXATypes::MXAError writeModel(const std::string &fileName, bool closeWhenFinished=false) = 0;
    /**
    * @brief Writes the model to the data file using a specific IODelegate
    * @param fileName The path to the data file
    * @param ioDelegate The IODelegate to employ when writing the model
    * @param closeWhenFinished Terminates access to the file when complete. Default is false
    * @return Standard HDF5 Error condition
    */
    virtual MXATypes::MXAError writeModel(const std::string &fileName, IODelegatePtr ioDelegate, bool closeWhenFinished=false) = 0;
    
    //--------------- Methods to Read the DataModel from a File -------------------
    /**
    * @brief Reads the model from a file
    * @param fileName The path to the data file
    * @param closeWhenFinished Terminates access to the file when complete. Default is false
    * @return Standard HDF5 Error condition
    */
    virtual MXATypes::MXAError readModel(const std::string &fileName, bool closeWhenFinished=false) = 0;
    /**
    * @brief Reads the model from a file
    * @param fileName The path to the data file
    * @param ioDelegate The IODelegate to employ when reading the model
    * @param closeWhenFinished Terminates access to the file when complete. Default is false
    * @return Standard HDF5 Error condition
    */
    virtual MXATypes::MXAError readModel(const std::string &fileName, IODelegatePtr ioDelegate, bool closeWhenFinished=false) = 0;
   
    
    /**
     * @brief Performs some basic checks to make sure the model is valid.
     * @param message String to store messages relating to errors/omissions about the model
     */
    virtual bool isValid(std::string &message) = 0;

  private:
    IDataModel(const IDataModel&); //Not Implemented
    void operator=(const IDataModel&); //Not Implemented
};

#endif /*_IMXADATAMODEL_H*/
