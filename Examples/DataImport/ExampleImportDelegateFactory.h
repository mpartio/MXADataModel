///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef EXAMPLEIMPORTDELEGATEFACTORY_H_
#define EXAMPLEIMPORTDELEGATEFACTORY_H_

#include <MXA/DataImport/AbstractImportDelegateFactory.h>
#include <MXA/Base/IImportDelegate.h>
#include <Examples/DataImport/ExampleImportDelegate.h>


DEFINE_IMPORT_DELEGATE_NAMESPACE(ExampleImport)


/**
* @class ExampleImportDelegateFactory ExampleImportDelegateFactory.h Examples/DataImport/ExampleImportDelegateFactory.h
* @brief Factory class to generate ExampleImportDelegateFactory objects
* @author Mike Jackson
* @date Sep 5, 2008
* @version $Revision: 1.2 $
*/
class ExampleImportDelegateFactory : public AbstractImportDelegateFactory
{
  public:
    ExampleImportDelegateFactory();
    virtual ~ExampleImportDelegateFactory();

    /**
     * @brief This method will return a new instance of ExampleImportDelegateFactory provided
     * the className matches.
     * @param className The name of the Delegate class that will be returned
     * @return A new boost shared pointer to H5ImportTestDelegate
     */
    IImportDelegate::Pointer newDataImportDelegate (const std::string &className );

    /**
     * @brief Returns the Classname of the delegate that this factory can create.
     */
    std::string delegateClassName();

    /**
    * @brief Sets a property of this class with the given value.
    * @param key The name of the property.
    * @param value  The value to set the property to.
    */
    virtual int32_t setProperty(const std::string &key, const std::string &value)
    {
      return 0;
    }

  private:

    ExampleImportDelegateFactory(const ExampleImportDelegateFactory&);    //Not Implemented
    void operator=(const ExampleImportDelegateFactory&);  //Not Implemented

};

#endif /* EXAMPLEIMPORTDELEGATEFACTORY_H_ */
