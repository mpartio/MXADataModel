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
#ifndef ABSTRACTFACTORY_H_
#define ABSTRACTFACTORY_H_

#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>

#include <string>


#define DEFINE_IMPORT_DELEGATE_NAMESPACE(name)\
namespace name \
{\
  namespace Detail\
  {\
    const std::string ClassName(#name);\
  }\
}



/**
* @class AbstractImportDelegateFactory AbstractImportDelegateFactory.h DataImport/AbstractImportDelegateFactory.h
* @brief Abstract base class to inherit from when creating custom ImportDelegateFactories
* @author Mike Jackson
* @date Sept 2007
* @version $Revision: 1.5 $
*/
class MXA_EXPORT AbstractImportDelegateFactory
{
  public:
    virtual ~AbstractImportDelegateFactory() {}

    /**
     * @brief Creates a new DataImportDelegate based on a class name
     * @param className The name of the class to create
     * @return Returns an instance of IImportDelegatePtr. This method can return
     * a null ImportDelegate so check the return value with the boost::shared_ptr.get()
     * method to check the value of the wrapped pointer.
     */
    virtual IImportDelegatePtr newDataImportDelegate (const std::string &className) = 0;

    /**
     * @brief Returns the ClassName of the delegate that this factory will create.
     */
    virtual std::string delegateClassName() = 0;

  protected:
    AbstractImportDelegateFactory() {};

  private:
    AbstractImportDelegateFactory(const AbstractImportDelegateFactory&);    //Not Implemented
    void operator=(const AbstractImportDelegateFactory&); //Not Implemented

};


#endif /*ABSTRACTFACTORY_H_*/
