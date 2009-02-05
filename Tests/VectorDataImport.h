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
#ifndef VECTORDATAIMPORT_H_
#define VECTORDATAIMPORT_H_


#include <MXA/Common/MXATypeDefs.h>
#include <MXA/DataImport/AbstractImportDelegateFactory.h>

#include <string>

/**
* @class VectorDataDelegate VectorDataImport.h Testing/VectorDataImport.h
* @brief
* @author
* @date
* @version $Revision: 1.2 $
*/
class VectorDataDelegate: public IImportDelegate
{
public:
  VectorDataDelegate(){};
  virtual ~VectorDataDelegate(){};

// -----------------------------------------------------------------------------
//  Implemented Method from the IDataImportDelegate interface
// -----------------------------------------------------------------------------
  int32 importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile)
  {
    std::string path ( dataSource->generateInternalPath() );
  //  std::cout << logTime() << "VectorDataDelegate::importDataSource() " << path << std::endl;
    uint32 pos = path.find_last_of("/");
    std::string parentPath ( path.substr(0, pos)  );
    hid_t fileId = dataFile->getFileId();
    H5Utilities::createGroupsFromPath(parentPath, fileId);
    //Create data in a 2x5 table
    std::vector<hsize_t> dims;
    dims.push_back(128);
    dims.push_back(128);
    dims.push_back(128);
    std::vector<uint8> data;
    for (int i = 0; i < 128; ++i)
    {
      for (int j = 0; j < 128; ++j)
      {
        for (int k = 0; k < 128; ++k)
        data.push_back( (i*j *k) * 255);
      }
    }

    return H5Lite::writeVectorDataset(fileId, path, dims, data);
  }

  virtual int32 setProperty(const std::string &key, const std::string &value)
  {
    std::cout << "VectorDataDelegate::setProperty is NOT implemented" << std::endl;
    return -1;
  }

  virtual int32 getProperty(const std::string &key, std::string &value)
  {
    std::cout << "VectorDataDelegate::getProperty is NOT implemented" << std::endl;
    return -1;
  }

private:
  VectorDataDelegate(const VectorDataDelegate&);   //Copy Constructor Not Implemented
  void operator=(const VectorDataDelegate&); //Copy Assignment Not Implemented

};

// -----------------------------------------------------------------------------
//  VectorDataDelegate Factory Class
// -----------------------------------------------------------------------------

// Declare our constant in a namespace
namespace VectorDataDelegateFactory_Detail
{
  const std::string ClassName("VectorDataDelegate");
}

/**
* @class VectorDataDelegateFactory VectorDataImport.h Testing/VectorDataImport.h
* @brief
* @author
* @date
* @version
*/
class VectorDataDelegateFactory : public AbstractImportDelegateFactory
{
  public:
    VectorDataDelegateFactory() {}
    virtual ~VectorDataDelegateFactory() {}


  /**
   * @brief This method will return a new instance of VectorDataDelegate provided
   * the className matches.
   * @param className The name of the Delegate class that will be returned
   * @param dataModel The data model to use
   * @param dataSource The data source to import
   * @return A new boost shared pointer to VectorDataDelegate
   */
  IImportDelegatePtr newDataImportDelegate (const std::string &className,
                                                IDataModelPtr dataModel,
                                                IDataSourcePtr dataSource)
  {
    IImportDelegatePtr delegate; // Creates a Null Shared Pointer
    if ( className.compare( VectorDataDelegateFactory_Detail::ClassName ) == 0)
    {
      delegate.reset ( new VectorDataDelegate() );
    }
    return delegate;
  }

  /**
   * @brief Returns the Classname of the delegate that this factory can create.
   */
  std::string delegateClassName()
  {
    return VectorDataDelegateFactory_Detail::ClassName;
  }

  private:
    VectorDataDelegateFactory(const VectorDataDelegateFactory&);    //Not Implemented
    void operator=(const VectorDataDelegateFactory&);  //Not Implemented
};

#endif /*VECTORDATAIMPORT_H_*/
