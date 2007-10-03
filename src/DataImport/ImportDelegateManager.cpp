#include "ImportDelegateManager.h"
#include "DataImport/AbstractImportDelegateFactory.h"
#include <iostream>


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
ImportDelegateManager::ImportDelegateManager()  
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
ImportDelegateManager::~ImportDelegateManager()
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
ImportDelegateManagerPtr ImportDelegateManager::instance()
{
  static ImportDelegateManagerPtr singleton;
  
  if (singleton.get() == NULL)
  {
    singleton.reset (new ImportDelegateManager() );
  }
  return singleton;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IImportDelegatePtr ImportDelegateManager::newDataImportDelegate(const std::string &classname)
{
  //std::cout << "ImportDelegateManager::newDataImportDelegate -> Trying to create ImportDelegate for '" << classname << "'" << std::endl;
  IImportDelegatePtr importDelegate;
  for (AbstractImportDelegateFactories::iterator iter = _factories.begin(); iter != _factories.end(); ++iter ) {
    std::string cn = (*(iter)).get()->delegateClassName();
    if ( cn.compare(classname) == 0)
    {
      return (*(iter)).get()->newDataImportDelegate(classname);
    }
  }
  
  return importDelegate;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void ImportDelegateManager::registerDataImportFactory(AbstractImportDelegateFactoryPtr factoryPtr)
{
  this->_factories.push_back(factoryPtr);
}

