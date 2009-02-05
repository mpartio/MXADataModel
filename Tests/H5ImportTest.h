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

#include <MXA/Base/IDataFile.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/DataImport/AbstractImportDelegateFactory.h>

#include <string>

/**
*  @brief This class is used to show simply how to write some data into an HDF5 file.
*  Some of the features of the IDataImportDelegate are not shown. This class
*  will simply write a single value to the HDF5 file.
* @author Mike Jackson
* @date April 2007
* @version $Revision: 1.2 $
*/
class H5ImportTestDelegate: public IImportDelegate
{
public:
  H5ImportTestDelegate() :
    _value(10)
    {};

  virtual ~H5ImportTestDelegate(){};

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
    IDataModelPtr model = dataFile->getDataModel();
    std::string path ( dataSource->generateInternalPath() );
    uint32 pos = path.find_last_of("/");
    std::string parentPath ( path.substr(0, pos)  );

    hid_t fileId = dataFile->getFileId();
    H5Utilities::createGroupsFromPath(parentPath,  fileId);
    //Write the Data to the HDF5 File
    return H5Lite::writeScalarDataset(fileId, path, this->_value);
  }

  /**
   * @brief Sets the value to write to the h5 file
   * @param value The value to write to the file
   */
  void setValue(int32 value)
  {
    this->_value = value;
  }

  /* Need these implemented but we are not going to make use of the functionality */
  virtual int32 setProperty(const std::string &key, const std::string &value)
    { return -1; }

  virtual int32 getProperty(const std::string &key, std::string &value)
    { return -1; }

private:
  int32 _value;

  H5ImportTestDelegate(const H5ImportTestDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5ImportTestDelegate&); //Copy Assignment Not Implemented

};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

// Declare our constant in a namespace
DEFINE_IMPORT_DELEGATE_NAMESPACE(H5ImportTest);

/**
* @class H5ImportTestDelegateFactory H5ImportTest.h Testing/H5ImportTest.h
* @brief Factory class to generate H5ImportTestDelegate objects
* @author Mike Jackson
* @date Sep 12, 2007
* @version $Revision: 1.2 $
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
   * @return A new boost shared pointer to H5ImportTestDelegate
   */
  IImportDelegatePtr newDataImportDelegate (const std::string &className)
  {
    IImportDelegatePtr delegate; // Creates a Null Shared Pointer
    if ( className.compare( H5ImportTest::Detail::ClassName ) == 0)
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
    return H5ImportTest::Detail::ClassName;
  }

  private:
    H5ImportTestDelegateFactory(const H5ImportTestDelegateFactory&);    //Not Implemented
    void operator=(const H5ImportTestDelegateFactory&);  //Not Implemented
};


#endif /*H5IMPORTTEST_H_*/
