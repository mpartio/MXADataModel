#ifndef _IDataset_h_
#define _IDataset_h_

#include <Base/IDataFileIO.h>


/**
* @class IDataset IDataset.h PathToHeader/IDataset.h
* @brief 
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT IDataset : public IDataFileIO 
{
  public:
    IDataset(){}
    virtual ~IDataset(){}
    
    virtual std::string getDatasetPath() = 0;
    virtual void setData(MXAAbstractDataPtr data) = 0;
    virtual MXAAbstractDataPtr getData() = 0;
    
    virtual void addAttribute (MXAAbstractAttributePtr attribute) = 0;
    virtual void removeAttribute (const std::string &attributeKey) = 0;
    virtual MXAAbstractAttributePtr getAttribute(const std::string &attributeKey) = 0;
    
  protected:  
  
  private:
      IDataset(const IDataset&);    //Not Implemented
      void operator=(const IDataset&); //Not Implemented
  
};

#endif //_IDataset_h_

