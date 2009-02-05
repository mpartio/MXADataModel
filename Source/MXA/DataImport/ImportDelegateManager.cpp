#include <MXA/DataImport/ImportDelegateManager.h>
#include <MXA/DataImport/AbstractImportDelegateFactory.h>
#if MXA_TIFF_SUPPORT
#include <MXA/HDF5/H5TiffImportDelegateFactory.h>
#endif

#if MXA_HDF5_SUPPORT
#include <MXA/HDF5/H5BmpImportDelegateFactory.h>
#endif

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
//  Static Method
// -----------------------------------------------------------------------------
void ImportDelegateManager::registerKnownImportDeletegateFactories()
{
#if MXA_TIFF_SUPPORT
    //Register to be able to import Tiff images
    AbstractImportDelegateFactoryPtr h5TiffImportDelegateFactory ( new H5TiffImportDelegateFactory() );
    ImportDelegateManager::registerImportDelegateFactory(h5TiffImportDelegateFactory);
#endif

#if MXA_HDF5_SUPPORT
    //Register to be able to import BMP images
    AbstractImportDelegateFactoryPtr h5BmpImportDelegateFactory ( new H5BmpImportDelegateFactory() );
    ImportDelegateManager::registerImportDelegateFactory(h5BmpImportDelegateFactory);
#endif
}

// -----------------------------------------------------------------------------
//  Static Method
// -----------------------------------------------------------------------------
void ImportDelegateManager::registerImportDelegateFactory(AbstractImportDelegateFactoryPtr importer)
{
  if (NULL != importer.get() )
  {
    // Instantiate the Instance Manager for import delegates
    ImportDelegateManagerPtr idManager = ImportDelegateManager::instance();
    idManager->addImportDelegateFactory( importer );
  }
}

// -----------------------------------------------------------------------------
//  Static Method
// -----------------------------------------------------------------------------
IImportDelegatePtr ImportDelegateManager::createNewImportDelegate(const std::string &classname)
{
  ImportDelegateManagerPtr idManager = ImportDelegateManager::instance();
  return idManager->createDataImportDelegate( classname );
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IImportDelegatePtr ImportDelegateManager::createDataImportDelegate(const std::string &classname)
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
void ImportDelegateManager::addImportDelegateFactory(AbstractImportDelegateFactoryPtr factoryPtr)
{
  this->_factories.push_back(factoryPtr);
}

// -----------------------------------------------------------------------------
// public Method
// -----------------------------------------------------------------------------
AbstractImportDelegateFactoryPtr ImportDelegateManager::getImportDelegateFactory(const std::string &classname)
{
  for (AbstractImportDelegateFactories::iterator iter = _factories.begin(); iter != _factories.end(); ++iter ) {
      std::string cn = (*(iter)).get()->delegateClassName();
      if ( cn.compare(classname) == 0)
      {
        return *(iter);
      }
    }
  AbstractImportDelegateFactoryPtr nullPointer;
  return nullPointer;
}

