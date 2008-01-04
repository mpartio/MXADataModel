#ifndef _MXAAbstractAttribute_h_
#define _MXAAbstractAttribute_h_

#include <Common/DLLExport.h>
#include <Core/MXAAbstractData.h>

#include <string>

/**
* @class MXAAbstractAttribute MXAAbstractAttribute.h PathToHeader/MXAAbstractAttribute.h
* @brief 
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT MXAAbstractAttribute : public MXAAbstractData
{
  public:
    MXAAbstractAttribute();
    virtual ~MXAAbstractAttribute();
      
    virtual void setAttributeKey (const std::string &attributeKey) = 0;
    virtual std::string getAttributeKey () = 0;
    
  protected:  
  
  private:
      MXAAbstractAttribute(const MXAAbstractAttribute&);    //Not Implemented
      void operator=(const MXAAbstractAttribute&); //Not Implemented
  
};

#endif //_MXAAbstractAttribute_h_





