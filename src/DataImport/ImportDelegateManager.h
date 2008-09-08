///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef IMPORTDELEGATEMANAGER_H_
#define IMPORTDELEGATEMANAGER_H_

#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>

//-- C++ Includes
#include <string>


/**
* @class ImportDelegateManager ImportDelegateManager.h DataImport/ImportDelegateManager.h
* @brief This is a Singleton class that exists to register Data Import Factory classes.
*
* This is part of the Factory design pattern where Specific Data Importers are created
* from a factory class
* @author Mike Jackson
* @date Sept 2007
* @version $Revision: 1.6 $
*/
class MXA_EXPORT ImportDelegateManager
{
  public:
    /**
     * @brief public destructor
     */
    virtual ~ImportDelegateManager();

// -----------------------------------------------------------------------------
//  Static Methods
     /**
      * @brief Use this method to get the instance of this class
      * @return Boost Shared pointer to the singleton instance
      */
     static ImportDelegateManagerPtr instance();

     /**
      * @brief This method ensures the ImportDelegateManager is instantiated and
      * then registers built-in import delegates. To date these are:
      *  H5Tiff importer
      *  H5BMP Importer
      */
     static void registerKnownImportDeletegateFactories();

     /**
      * @brief This method ensures the ImportDelegateManager is instantiated and
      * then registers the passed in ImportDelegate
      * @param importer A Valid AbstractImportDelegateFactoryPtr subclass
      */
     static void registerImportDelegateFactory(AbstractImportDelegateFactoryPtr importer);

     /**
      * @brief Creates a new ImportDelegate based on a class name
      * @param className The name of the class to create
      * @return Returns an instance of IImportDelegatePtr. This method can return
      * a null ImportDelegate so check the return value with the boost::shared_ptr.get()
      * method to check the value of the wrapped pointer.
      */
     static IImportDelegatePtr createNewImportDelegate (const std::string &className);


// -----------------------------------------------------------------------------
//  Public Methods

     /**
      * @brief Creates a new DataImportDelegate based on a class name. Usually you should
      * use the static method createNewImportDelegate() instead of this although
      * there should not be a problem if you do use this method.
      * @param className The name of the class to create
      * @return Returns an instance of IImportDelegatePtr. This method can return
      * a null ImportDelegate so check the return value with the boost::shared_ptr.get()
      * method to check the value of the wrapped pointer.
      */
     virtual IImportDelegatePtr createDataImportDelegate (const std::string &className);

     /**
      * @brief Registers a Specific DataImportFactory. Usually you would use the
      * static method registerImportDelegateFactory instead of this method although
      * it is perfectly legal to use this method.
      * @param factoryPtr The ImportDelegateFactory to register
      */
     virtual void addImportDelegateFactory(AbstractImportDelegateFactoryPtr factoryPtr);

     /**
      * @brief Attempts to return the ImportDelegateFactory for a given class name.
      * @param classname The name of the class to find the factory for.
      */
     virtual AbstractImportDelegateFactoryPtr getImportDelegateFactory(const std::string &classname);


   protected:
     ImportDelegateManager();


   private:

     AbstractImportDelegateFactories  _factories;


     ImportDelegateManager(const ImportDelegateManager&);    //Not Implemented
     void operator=(const ImportDelegateManager&); //Not Implemented

};

#endif /*IMPORTDELEGATEMANAGER_H_*/
