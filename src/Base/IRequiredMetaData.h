#ifndef _IRequiredMetaData_h_
#define _IRequiredMetaData_h_


#include <Base/IDataFileIO.h>



/**
* @class IRequiredMetaData IRequiredMetaData.h Base/IRequiredMetaData.h
* @brief 
* @author mjackson
* @date Jan 15, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT IRequiredMetaData : public IDataFileIO 
{
  public:
    IRequiredMetaData() {}
    virtual ~IRequiredMetaData() {}
    
    virtual void setResearcherName(const std::string &researcherName) = 0;
    virtual std::string getResearcherName() = 0;
    
    virtual void setDateTimeCreated(const std::string &dateTime) = 0;
    virtual std::string getDateTimeCreated() = 0;
    
    virtual void setDatasetName(const std::string &datasetName) = 0;
    virtual std::string getDatasetName(const std::string &datasetName) = 0;
    
    virtual void setDescription(const std::string &description) = 0;
    virtual std::string getDescription() = 0;
    
    virtual void setDistributionRights(const std::string &distributionRights) = 0;
    virtual std::string getDistributionRights() = 0;
    
    virtual void setReleaseNumber(const std::string &releaseNumber) = 0;
    virtual std::string getReleaseNumber() = 0;
    
    virtual void setPedigree(const std::string &pedigree) = 0;
    virtual std::string getPedigree() = 0;
    
    virtual void setDerivedSourceFile(const std::string &derivedSourceFile) = 0;
    virtual std::string getDerivedSourceFile() = 0;
    
    virtual int32 setValueForKey(const std::string &key, const std::string &value) = 0;
    virtual std::string getValueForKey (const std::string &key) = 0;
    
    virtual bool isValid(std::string message) = 0;
    
    virtual void printSelf(std::ostream& os, int32 indent) = 0;
    
    virtual void generateKeyValueMap(std::map<std::string, std::string> &mdMap) = 0;
    
  protected:  
    
    
  private:
      IRequiredMetaData(const IRequiredMetaData&);    //Not Implemented
      void operator=(const IRequiredMetaData&); //Not Implemented
  
};

#endif //_IRequiredMetaData_h_


  


