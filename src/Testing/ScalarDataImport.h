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
#ifndef SCALARDATAIMPORT_H_
#define SCALARDATAIMPORT_H_


#include <Common/MXATypeDefs.h>
#include <DataImport/AbstractImportDelegateFactory.h>

#include <string>

/**
*  @brief This class is used to show simply how to write some data into an HDF5 file.
*  Some of the features of the IDataImportDelegate are not shown. This class
*  will simply write a single value to the HDF5 file.
* @author Mike Jackson
* @date April 2007
* @version $Revision: 1.5 $
*/
class ScalarDataDelegate: public IImportDelegate
{
public:
  ScalarDataDelegate(){};
  virtual ~ScalarDataDelegate(){};

// -----------------------------------------------------------------------------
//  Implemented Method from the IDataImportDelegate interface 
// -----------------------------------------------------------------------------
  /**
   * @brief Imports the datasource to the data file
   * @param dataSource The source of the data
   * @param dataFile The IDataFile object
   * @return 
   */
  int32 importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile)
  {
    
    std::string path ( dataSource->generateInternalPath() );
  //  std::cout << logTime() << "ScalarDataDelegate::importDataSource() " << path << std::endl;
    uint32 pos = path.find_last_of("/");
    std::string parentPath ( path.substr(0, pos)  );
    int32 value = 55;
    hid_t fileId = dataFile->getFileId();
    H5Utilities::createGroupsFromPath(parentPath,  fileId);
    //Write the Data to the HDF5 File
    return H5Lite::writeScalarDataset(fileId, path, value);
  }
  
private:
  ScalarDataDelegate(const ScalarDataDelegate&);   //Copy Constructor Not Implemented
  void operator=(const ScalarDataDelegate&); //Copy Assignment Not Implemented
  
};

// -----------------------------------------------------------------------------
//  The Factory Object that will create ScalarDataDelegate Objects
// -----------------------------------------------------------------------------

// Declare our constant in a namespace
namespace ScalarDataDelegateFactory_Detail
{
  const std::string ClassName("ScalarDataDelegate");
}

/**
* @class ScalarDataDelegateFactory ScalarDataIMport.h Testing/ScalarDataIMport.h
* @brief 
* @author
* @date
* @version
*/
class ScalarDataDelegateFactory : public AbstractImportDelegateFactory
{
  public:
    ScalarDataDelegateFactory() {}
    virtual ~ScalarDataDelegateFactory() {}
    
    
  /**
   * @brief This method will return a new instance of ScalarDataDelegate provided
   * the className matches.
   * @param className The name of the Delegate class that will be returned
   * @param dataModel The data model to use
   * @param dataSource The data source to import
   * @return A new boost shared pointer to ScalarDataDelegate
   */
  IImportDelegatePtr newDataImportDelegate (const std::string &className,
                                                IDataModelPtr dataModel,
                                                IDataSourcePtr dataSource)
  {
    IImportDelegatePtr delegate; // Creates a Null Shared Pointer
    if ( className.compare( ScalarDataDelegateFactory_Detail::ClassName ) == 0)
    {
      delegate.reset ( new ScalarDataDelegate() );
    }
    return delegate;
  }
  
  /**
   * @brief Returns the Classname of the delegate that this factory can create.
   */
  std::string delegateClassName()
  {
    return ScalarDataDelegateFactory_Detail::ClassName;
  }
  
  private:
    ScalarDataDelegateFactory(const ScalarDataDelegateFactory&);    //Not Implemented
    void operator=(const ScalarDataDelegateFactory&);  //Not Implemented
};



#endif /*SCALARDATAIMPORT_H_*/
