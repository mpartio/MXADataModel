#ifndef _IDataset_h_
#define _IDataset_h_


#include <Base/IFileWriter.h>
#include <Base/IFileReader.h>
#include <Dataset/IDataArray.h>

#include <string>

#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<IDataArray> IDataArrayPtr;

/**
* @class IDataset IDataset.h Dataset/IDataset.h
* @brief 
* @author Mike Jackson
* @date Dec 31, 2007
* @version $Revision: 1.1 $
*/
class IDataset : public IFileWriter, public IFileReader
{
  public:
    static IDatasetPtr New(const std::string &datasetPath, IDataArrayPtr data)
    {
      IDataset* ds = new IDataset (datasetPath, data);
      IDatasetPtr ptr (ds);
      return ptr;
    }
    
    virtual ~IDataset(){}

    void setDatasetPath(const std::string &datasetPath)
    {
      this->_datasetPath = datasetPath;
    }
    
    std::string getDatasetPath()
    {
      return this->_datasetPath;
    }
    
    void setData(IDataArrayPtr data)
    {
      this->_data = data;
    }
     
    IDataArrayPtr getData()
    {
      return this->_data;
    }
    
    void addAttribute (const std::string &key, IDataArrayPtr attributeData)
    {
      this->_attributes[key] = attributeData;
    }
    
    void removeAttribute(const std::string &key)
    {
      std::map<std::string, IDataArrayPtr>::iterator cur  = _attributes.find(key);
      if (cur != _attributes.end() )
      {
        _attributes.erase(cur);
      }
      
    }
    
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
    virtual int32 writeToFile(IDataFilePtr dataFile)
    {
      this->_data->writeDataToFile(dataFile, this->_datasetPath);
      //Iterate over attributes
      for (std::map<std::string, IDataArrayPtr>::iterator iter = _attributes.begin(); iter != _attributes.end(); ++iter ) {
        (*iter).second->writeAttributeToFile(dataFile, this->_datasetPath, (*iter).first);
      }
      return -1;
    }

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
    virtual int32 readFromFile(IDataFilePtr dataFile)
    {
      return -1;
    }
        
        
  protected:
    
    IDataset(const std::string &datasetPath, IDataArrayPtr data)
    {
      this->_datasetPath = datasetPath;
      this->_data = data;
    }
    
  private:
    std::string _datasetPath;
    IDataArrayPtr _data;
    std::map<std::string, IDataArrayPtr> _attributes;
    
    IDataset(const IDataset&);    //Not Implemented
    void operator=(const IDataset&); //Not Implemented
  
};

#endif //_IDataset_h_

