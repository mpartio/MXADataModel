#ifndef _MXAAbstractData_h_
#define _MXAAbstractData_h_

#include <Base/IAbstractDataArray.h>


/**
* @class MXAAbstractData MXAAbstractData.h PathToHeader/MXAAbstractData.h
* @brief 
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT MXAAbstractData : public IAbstractDataArray
{
  public:
    MXAAbstractData();
      virtual ~MXAAbstractData();
    
    virtual void setDatasetPath(const std::string &datasetPath) = 0;
    virtual std::string getDatasetPath() = 0;
    
  protected:  
  
  private:
      MXAAbstractData(const MXAAbstractData&);    //Not Implemented
      void operator=(const MXAAbstractData&); //Not Implemented
  
};

#endif //_MXAAbstractData_h_




