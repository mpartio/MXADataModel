#ifndef ABSTRACTFACTORY_H_
#define ABSTRACTFACTORY_H_

#include "Common/DLLExport.h"
#include "Common/MXATypeDefs.h"
#include <string>


/**
* @class AbstractImportDelegateFactory AbstractImportDelegateFactory.h DataImport/AbstractImportDelegateFactory.h
* @brief 
* @author
* @date
* @version
*/
class MXA_EXPORT AbstractImportDelegateFactory 
{
  public:

    virtual IImportDelegatePtr newDataImportDelegate (const std::string &className) = 0;
    
    virtual std::string delegateClassName() = 0;
    
    virtual ~AbstractImportDelegateFactory() {}
    
    
};


#endif /*ABSTRACTFACTORY_H_*/
