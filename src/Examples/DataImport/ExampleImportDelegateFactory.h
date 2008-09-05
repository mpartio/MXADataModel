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

#include <DataImport/AbstractImportDelegateFactory.h>
#include <Examples/DataImport/ExampleImportDelegate.h>


// Declare our constant in a namespace
namespace ExampleImport
{
  namespace Detail
  {
    const std::string ClassName("ExampleImport");
  }
}

/**
* @class ExampleImportDelegateFactory ExampleImportDelegateFactory.h Examples/DataImport/ExampleImportDelegateFactory.h
* @brief Factory class to generate ExampleImportDelegateFactory objects
* @author Mike Jackson
* @date Sep 5, 2008
* @version $Revision: 1.1 $
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
     * @param dataModel The data model to use
     * @param dataSource The data source to import
     * @return A new boost shared pointer to H5ImportTestDelegate
     */
    IImportDelegatePtr newDataImportDelegate (const std::string &className );

    /**
     * @brief Returns the Classname of the delegate that this factory can create.
     */
    std::string delegateClassName();

  private:

    ExampleImportDelegateFactory(const ExampleImportDelegateFactory&);    //Not Implemented
    void operator=(const ExampleImportDelegateFactory&);  //Not Implemented

};

#endif /* EXAMPLEIMPORTDELEGATEFACTORY_H_ */
