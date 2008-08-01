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
#ifndef _IRequiredMetaData_h_
#define _IRequiredMetaData_h_

//-- MXA includes
#include <Base/IDataFileIO.h>


/**
* @class IRequiredMetaData IRequiredMetaData.h Base/IRequiredMetaData.h
* @brief This class holds all the required meta data that is used by the
* MXADataModel and is included in each data file.
* @author Mike Jackson for IMTS.us
* @date Jan 15, 2008
* @version $Revision: 1.4 $
*/
class MXA_EXPORT IRequiredMetaData : public IDataFileIO
{
  public:
    IRequiredMetaData() {}
    virtual ~IRequiredMetaData() {}

    /**
     * @brief Sets all the required meta data objects
     * @param researcherName The name of the researcher
     * @param dateCreated The date the data was created
     * @param datasetName The name of the dataset or experiment
     * @param description A short description of the experiment
     * @param distributionRights How is the data to be distributed.
     * @param releaseNumber If the data is publicly releasable was there a tracking number for the release
     * @param pedigree More detailed information about the data/experiment
     * @param derivedSrcFile Did the data come from a data file
     *
     * The distributionRights should be one of the following:
     * const std::string MXA_RIGHTS_UNLIMITED_VALUE("Unlimited");
     * const std::string MXA_RIGHTS_LIMITED_VALUE("Limited");
     * const std::string MXA_RELEASE_LIMITATION_TAG("Release_Limitation");
     */
    virtual void setAll(std::string researcherName,
                std::string dateCreated,
                std::string datasetName,
                std::string description,
                std::string distributionRights,
                std::string releaseNumber,
                std::string pedigree,
                std::string derivedSrcFile) = 0;

    /**
     * @brief Sets the Researcher's name or who is generating the data set
     * @param researcherName
     */
    virtual void setResearcherName(const std::string &researcherName) = 0;

    /**
     * @brief Returns the Researcher name value
     * @return
     */
    virtual std::string getResearcherName() = 0;

    /**
     * @brief Sets the date and time the data set was created. The preferable format
     * for the date time is "yyyy/MM/dd HH:mm:ss"
     * @param dateTime
     */
    virtual void setDateTimeCreated(const std::string &dateTime) = 0;

    /**
     * @brief Returns the date/time the dataset was created
     * @return
     */
    virtual std::string getDateTimeCreated() = 0;

    /**
     * @brief Provides a descriptive name for the dataset
     * @param datasetName The name of the dataset
     */
    virtual void setDatasetName(const std::string &datasetName) = 0;

    /**
     * @brief Returns the name given to the data set.
     * @return
     */
    virtual std::string getDatasetName(const std::string &datasetName) = 0;

    /**
     * @brief Provides a description for the data in the data file
     * @param description The description of the data
     */
    virtual void setDescription(const std::string &description) = 0;

    /**
     * @brief Returns the description of the dataset.
     * @return
     */
    virtual std::string getDescription() = 0;

    /**
     * @brief Sets the Distribution rights for the data and should be either
     * "Unlimited" or "Limited".
     * @param distributionRights The distribution Rights for the dataset.
     */
    virtual void setDistributionRights(const std::string &distributionRights) = 0;

    /**
     * @brief Returns the distribution rights of the dataset
     * @return
     */
    virtual std::string getDistributionRights() = 0;

    /**
     * @brief Sets the release number or other tracking number/code that is used
     * to track the dataset
     * @param releaseNumber The release number/code or "Not Applicable"
     */
    virtual void setReleaseNumber(const std::string &releaseNumber) = 0;

    /**
     * @brief Returns the Release number or tracking number for the dataset.
     * @return
     */
    virtual std::string getReleaseNumber() = 0;

    /**
     * @brief Sets the Pedigree of the data which is either one of "Derived" or "Original"
     * @param pedigree The pedigree of the data
     */
    virtual void setPedigree(const std::string &pedigree) = 0;

    /**
     * @brief Returns the pedigree of the dataset and is either "Derived" or "Original"
     * @return
     */
    virtual std::string getPedigree() = 0;

    /**
     * @brief Sets the Derived Source file meta data. Use "Not Applicable" if that
     * is appropriate to your data set
     * @param derivedSourceFile Value of the Derived Source File
     */
    virtual void setDerivedSourceFile(const std::string &derivedSourceFile) = 0;

    /**
     * @brief Returns the value of the Derived source file meta data. Returns
     * "Not Applicable" if no derived source file was used.
     * @return
     */
    virtual std::string getDerivedSourceFile() = 0;

    /**
     * @brief Sets the value for a given meta data key.
     * @param key The meta data key to set
     * @param value The value of meta data to set
     * @return Zero or positive on success.
     */
    virtual int32 setValueForKey(const std::string &key, const std::string &value) = 0;

    /**
     * @brief Returns the value for a given key
     * @param key The key to retrieve the value for
     * @return The meta data value associated with the key. An empty string is a valid
     * return value.
     */
    virtual std::string getValueForKey (const std::string &key) = 0;

    /**
     * @brief Validates the Required Meta Data
     * @param message A std::string to store error messages
     * @return If this is a valid set of Required Meta Data
     */
    virtual bool isValid(std::string message) = 0;

    /**
     * @brief prints a debugging type diagnostic description of the contents of this class
     * @param os An output stream to write to.
     * @param indent The number of spaces to use for indentation
     */
    virtual void printSelf(std::ostream& os, int32 indent) = 0;

    /**
     * @brief Generates a std::map of the meta data key to its associated value.
     * @param mdMap The map to store the values into.
     */
    virtual void generateKeyValueMap(std::map<std::string, std::string> &mdMap) = 0;

  protected:


  private:
      IRequiredMetaData(const IRequiredMetaData&);    //Not Implemented
      void operator=(const IRequiredMetaData&); //Not Implemented

};

#endif //_IRequiredMetaData_h_





