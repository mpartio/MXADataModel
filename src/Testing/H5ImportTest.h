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
#ifndef H5IMPORTTEST_H_
#define H5IMPORTTEST_H_

#include "HDF5/H5Lite.h"
#include "HDF5/H5Utilities.h"
#include "DataImport/AbstractImportDelegateFactory.h"

#include <string>

/**
*  @brief This class is used to show simply how to write some data into an HDF5 file.
*  Some of the features of the IDataImportDelegate are not shown. This class
*  will simply write a single value to the HDF5 file.
* @author Mike Jackson
* @date April 2007
*/
class H5ImportTestDelegate: public IImportDelegate
{
public:
  H5ImportTestDelegate(){};
  virtual ~H5ImportTestDelegate(){};

// -----------------------------------------------------------------------------
//  Implemented Method from the IDataImportDelegate interface 
// -----------------------------------------------------------------------------
  int32 importDataSource(IDataSourcePtr dataSource, IDataModelPtr model)
  {
    std::string path ( dataSource->generateInternalPath() );
    uint32 pos = path.find_last_of("/");
    std::string parentPath ( path.substr(0, pos)  );
    int32 value = 55;
    hid_t fileId = model->getIODelegate()->getOpenFileId();
    H5Utilities::createGroupsFromPath(parentPath,  fileId);
    //Write the Data to the HDF5 File
    return H5Lite::writeScalarDataset(fileId, path, value);
  }
  
private:
  H5ImportTestDelegate(const H5ImportTestDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5ImportTestDelegate&); //Copy Assignment Not Implemented
  
};



// Declare our constant in a namespace
namespace H5ImportTestDelegateFactory_Detail
{
  const std::string ClassName("H5ImportTestDelegate");
}

/**
* @class H5ImportTestDelegateFactory H5ImportTestDelegateFactory.h Testing/H5ImportTestDelegateFactory.h
* @brief Factory class to generate H5ImportTestDelegate objects
* @author Mike Jackson
* @date Sep 12, 2007
* @version $Revision: 1.1 $
*/
class H5ImportTestDelegateFactory : public AbstractImportDelegateFactory
{
  public:
    H5ImportTestDelegateFactory() {}
    virtual ~H5ImportTestDelegateFactory() {}
    
    
  /**
   * @brief This method will return a new instance of H5ImportTestDelegate provided
   * the className matches.
   * @param className The name of the Delegate class that will be returned
   * @param dataModel The data model to use
   * @param dataSource The data source to import
   * @return A new boost shared pointer to H5ImportTestDelegate
   */
  IImportDelegatePtr newDataImportDelegate (const std::string &className,
                                                IDataModelPtr dataModel,
                                                IDataSourcePtr dataSource)
  {
    IImportDelegatePtr delegate; // Creates a Null Shared Pointer
    if ( className.compare( H5ImportTestDelegateFactory_Detail::ClassName ) == 0)
    {
      delegate.reset ( new H5ImportTestDelegate() );
    }
    return delegate;
  }
  
  /**
   * @brief Returns the Classname of the delegate that this factory can create.
   */
  std::string delegateClassName()
  {
    return H5ImportTestDelegateFactory_Detail::ClassName;
  }
  
  private:
    H5ImportTestDelegateFactory(const H5ImportTestDelegateFactory&);    //Not Implemented
    void operator=(const H5ImportTestDelegateFactory&);  //Not Implemented
};


#endif /*H5IMPORTTEST_H_*/
