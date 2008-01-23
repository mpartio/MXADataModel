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
#ifndef _H5MXARequiredMetaData_h_
#define _H5MXARequiredMetaData_h_


#include <Base/IRequiredMetaData.h>


#include <iostream>


/**
* @class H5MXARequiredMetaData H5MXARequiredMetaData.h HDF5/H5MXARequiredMetaData.h
* @brief Concrete implmentation of the IRequiredMetaData virtual class.
* @author Mike Jackson  for IMTS.us
* @date Jan 15, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT H5MXARequiredMetaData : public IRequiredMetaData 
{
  public:
    /**
     * @brief Static method to create a new IRequiredMetaData object that is 
     * wrapped in a Boost Shared Pointer.
     */
    static IRequiredMetaDataPtr New( std::string researcherName, 
                                     std::string dateCreated, 
                                     std::string datasetName, 
                                     std::string description,
                                     std::string distributionRights,
                                     std::string releaseNumber,
                                     std::string pedigree, 
                                     std::string derivedSrcFile);

    virtual ~H5MXARequiredMetaData();
      
    void setResearcherName(const std::string &researcherName);
    std::string getResearcherName();
    
    void setDateTimeCreated(const std::string &dateTime);
    std::string getDateTimeCreated();
    
    void setDatasetName(const std::string &datasetName);
    std::string getDatasetName(const std::string &datasetName);
    
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
    H5MXARequiredMetaData( std::string researcherName, 
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
    
    H5MXARequiredMetaData(const H5MXARequiredMetaData&);    //Not Implemented
    void operator=(const H5MXARequiredMetaData&); //Not Implemented
  
};

#endif //_H5MXARequiredMetaData_h_






