

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
ImportDelegateManager::Pointer ImportDelegateManager::instance()
{
  static ImportDelegateManager::Pointer singleton;

  if (singleton.get() == NULL)
  {
    singleton.reset (new ImportDelegateManager() );
  }
  return singleton;
}

// -----------------------------------------------------------------------------
//  Static Method
// -----------------------------------------------------------------------------
void ImportDelegateManager::registerKnownImportDelegateFactories()
{
#if MXA_TIFF_SUPPORT
    //Register to be able to import Tiff images
    AbstractImportDelegateFactory::Pointer h5TiffImportDelegateFactory ( new H5TiffImportDelegateFactory() );
    ImportDelegateManager::registerImportDelegateFactory(h5TiffImportDelegateFactory);
#endif

#if MXA_HDF5_SUPPORT
    //Register to be able to import BMP images
    AbstractImportDelegateFactory::Pointer h5BmpImportDelegateFactory ( new H5BmpImportDelegateFactory() );
    ImportDelegateManager::registerImportDelegateFactory(h5BmpImportDelegateFactory);
#endif
}

// -----------------------------------------------------------------------------
//  Static Method
// -----------------------------------------------------------------------------
void ImportDelegateManager::registerImportDelegateFactory(AbstractImportDelegateFactory::Pointer importer)
{
  if (NULL != importer.get() )
  {
    // Instantiate the Instance Manager for import delegates
    ImportDelegateManager::Pointer idManager = ImportDelegateManager::instance();
    idManager->addImportDelegateFactory( importer );
  }
}

// -----------------------------------------------------------------------------
//  Static Method
// -----------------------------------------------------------------------------
IImportDelegate::Pointer ImportDelegateManager::createNewImportDelegate(const std::string &classname)
{
  ImportDelegateManager::Pointer idManager = ImportDelegateManager::instance();
  return idManager->createDataImportDelegate( classname );
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IImportDelegate::Pointer ImportDelegateManager::createDataImportDelegate(const std::string &classname)
{
  //std::cout << "ImportDelegateManager::newDataImportDelegate -> Trying to create ImportDelegate for '" << classname << "'" << std::endl;
  IImportDelegate::Pointer importDelegate;
  for (AbstractImportDelegateFactory::Collection::iterator iter = _factories.begin(); iter != _factories.end(); ++iter ) {
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
void ImportDelegateManager::addImportDelegateFactory(AbstractImportDelegateFactory::Pointer factoryPtr)
{
  this->_factories.push_back(factoryPtr);
}

// -----------------------------------------------------------------------------
// public Method
// -----------------------------------------------------------------------------
AbstractImportDelegateFactory::Pointer ImportDelegateManager::getImportDelegateFactory(const std::string &classname)
{
  for (AbstractImportDelegateFactory::Collection::iterator iter = _factories.begin(); iter != _factories.end(); ++iter ) {
      std::string cn = (*(iter)).get()->delegateClassName();
      if ( cn.compare(classname) == 0)
      {
        return *(iter);
      }
    }
  AbstractImportDelegateFactory::Pointer nullPointer;
  return nullPointer;
}

