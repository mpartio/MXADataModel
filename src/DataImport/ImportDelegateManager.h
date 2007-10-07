#ifndef IMPORTDELEGATEMANAGER_H_
#define IMPORTDELEGATEMANAGER_H_

#include <Common/MXATypeDefs.h>



/**
* @class ImportDelegateManager ImportDelegateManager.h DataImport/ImportDelegateManager.h
* @brief 
* @author
* @date
* @version $Revision: 1.3 $
*/
class ImportDelegateManager
{
  public:
     // Static instance method
     static ImportDelegateManagerPtr instance();

     virtual IImportDelegatePtr newDataImportDelegate (const std::string &className);
     
     virtual void registerDataImportFactory(AbstractImportDelegateFactoryPtr factoryPtr);
     
     virtual ~ImportDelegateManager();
     
   protected:  
     ImportDelegateManager();

     
     AbstractImportDelegateFactories  _factories;

     
   private:
     ImportDelegateManager(const ImportDelegateManager&);    //Not Implemented
     void operator=(const ImportDelegateManager&); //Not Implemented
  
};

#endif /*IMPORTDELEGATEMANAGER_H_*/
