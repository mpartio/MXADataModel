///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _REQUIREDMETADATA_H_
#define _REQUIREDMETADATA_H_



#include <Base/IRequiredMetaData.h>


#include <iostream>


/**
* @class RequiredMetaData RequiredMetaData.h HDF5/RequiredMetaData.h
* @brief Concrete implmentation of the IRequiredMetaData virtual class.
* @author Mike Jackson  for IMTS.us
* @date Jan 15, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT RequiredMetaData : public IRequiredMetaData
{
  public:
    /**
     * @brief Static method to create a new IRequiredMetaData object that is
     * wrapped in a Boost Shared Pointer.
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
    static IRequiredMetaDataPtr New( std::string researcherName,
                                     std::string dateCreated,
                                     std::string datasetName,
                                     std::string description,
                                     std::string distributionRights,
                                     std::string releaseNumber,
                                     std::string pedigree,
                                     std::string derivedSrcFile);

    /**
     * @brief Creates a new IRequiredMetaDataPtr object that has all empty values.
     * Note that this will make it INVALID. You are required to fill in the appropriate values
     * to make it valid. This is provided as a convenience.
     */
    static IRequiredMetaDataPtr DefaultMetaData();

    virtual ~RequiredMetaData();

    /**
     * @brief Sets all the required meta data
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
    void setAll(std::string researcherName,
                std::string dateCreated,
                std::string datasetName,
                std::string description,
                std::string distributionRights,
                std::string releaseNumber,
                std::string pedigree,
                std::string derivedSrcFile);


    void setResearcherName(const std::string &researcherName);
    std::string getResearcherName();

    void setDateTimeCreated(const std::string &dateTime);
    std::string getDateTimeCreated();

    void setDatasetName(const std::string &datasetName);
    std::string getDatasetName();

    void setDescription(const std::string &description);
    std::string getDescription();

    void setDistributionRights(const std::string &distributionRights);
    std::string getDistributionRights();

    void setReleaseNumber(const std::string &releaseNumber);
    std::string getReleaseNumber();

    void setPedigree(const std::string &pedigree);
    std::string getPedigree();

    void setDerivedSourceFile(const std::string &derivedSourceFile);
    std::string getDerivedSourceFile();

    int32 setValueForKey(const std::string &key, const std::string &value);
    std::string getValueForKey (const std::string &key);

    int32 writeToFile (IDataFilePtr dataFile);
    int32 readFromFile (IDataFilePtr dataFile);

    bool isValid(std::string message);

    void printSelf(std::ostream& os, int32 indent);

    void generateKeyValueMap(std::map<std::string, std::string> &mdMap);

  protected:
    RequiredMetaData( std::string researcherName,
                           std::string dateCreated,
                           std::string datasetName,
                           std::string description,
                           std::string distributionRights,
                           std::string releaseNumber,
                           std::string pedigree,
                           std::string derivedSrcFile);

  private:
    //Fields to hold the Required Meta Data Fields
    std::string _researcherName;
    std::string _dateCreated;
    std::string _datasetName;
    std::string _description;
    std::string _distributionRights;
    std::string _releaseNumber;
    std::string _pedigree;
    std::string _derivedSrcFile;

    RequiredMetaData(const RequiredMetaData&);    //Not Implemented
    void operator=(const RequiredMetaData&); //Not Implemented

};

#endif //_RequiredMetaData_h_







