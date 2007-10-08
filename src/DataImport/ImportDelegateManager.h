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

/**
* @class ImportDelegateManager ImportDelegateManager.h DataImport/ImportDelegateManager.h
* @brief This is a Singleton class that exists to register Data Import Factory classes.
* 
* This is part of the Factory design pattern where Specific Data Importers are created
* from a factory class
* @author Mike Jackson
* @date Sept 2007
* @version $Revision: 1.4 $
*/
class ImportDelegateManager
{
  public:
     /**
      * @brief Use this method to get the instance of this class
      * @return Boost Shared pointer to the singleton instance
      */
     static ImportDelegateManagerPtr instance();
     
     /**
      * @brief Creates a new DataImportDelegate based on a class name
      * @param className The name of the class to create
      * @return Returns an instance of IImportDelegatePtr. This method can return
      * a null ImportDelegate so check the return value with the boost::shared_ptr.get() 
      * method to check the value of the wrapped pointer.
      */
     virtual IImportDelegatePtr newDataImportDelegate (const std::string &className);
     
     /**
      * @brief Registers a Specific DataImportFactory
      * @param factoryPtr The ImportDelegateFactory to register
      */
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
