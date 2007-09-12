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
#ifndef OFIMPORTDELEGATE_H_
#define OFIMPORTDELEGATE_H_


//-- MXA Includes
#include "Common/DLLExport.h"
#include "Common/MXATypes.h"
#include "Common/MXATypeDefs.h"
#include "Base/IImportDelegate.h"
#include "DataImport/AbstractImportDelegateFactory.h"

// Include Boost Headers
#include <boost/filesystem/path.hpp>
// C++ STL headers
#include <string>

//-- Forward Declare the MXADataModel Class
class MXADataModel;

//Alias the namespace to cut down on line width and typing
namespace FileSystem = boost::filesystem;

/**
 * @brief A generic Import Delegate that imports TIFF images into an HDF5 data file
 * @author Mike Jackson
 * @date April 2007
 * @header HDF5/H5TiffImportDelegate.h
 */
class MXA_EXPORT H5TiffImportDelegate : public IImportDelegate
{
public:
	H5TiffImportDelegate();
	virtual ~H5TiffImportDelegate();
  
  /**
  * @brief Implementation from the IDataImportDelegate class
  */
  int32 importDataSource(IDataSourcePtr dataSource, IDataModelPtr model);
  
  /**
   * @brief Sets the pointer to the DataModel
   * @param model A Pointer to the DataModel
   */
  void setDataModel(IDataModelPtr model);
  
  /**
  * @brief 
  * @param value
  */
  void setFileNotFoundIsError(bool value);

  /**
  * @brief 
  * @param value
  */
  void setImportAsGrayScale(bool value);
  
private:
  IDataModelPtr      _modelPtr;
  bool _fileNotFoundIsError;
  bool _importAsGrayScale;
  
  H5TiffImportDelegate(const H5TiffImportDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5TiffImportDelegate&); //Copy Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------

// Declare our constant in a namespace
namespace H5TiffImportDelegateFactory_Detail
{
  const std::string ClassName("H5TiffImportDelegate");
}

// -----------------------------------------------------------------------------
//  Factory Class to generate H5TiffImportDelegate Objects
// -----------------------------------------------------------------------------
/**
* @class H5TiffImportDelegateFactory H5TiffImportDelegateFactory.h HDF5/H5TiffImportDelegateFactory.h
* @brief Factory Class to generate H5TiffImportDelegate Objects
* @author Mike Jackson
* @date Sept 2007
* @version $Revision: 1.5 $
*/
class H5TiffImportDelegateFactory : public AbstractImportDelegateFactory
{
  public:
    H5TiffImportDelegateFactory() {}
    virtual ~H5TiffImportDelegateFactory() {}
    
    
  /**
   * @brief This method will return a new instance of H5TiffImportDelegate provided
   * the className matches.
   * @param className The name of the Delegate class that will be returned
   * @param dataModel The data model to use
   * @param dataSource The data source to import
   * @return A new boost shared pointer to H5TiffImportDelegate
   */
  IImportDelegatePtr newDataImportDelegate (const std::string &className,
                                                IDataModelPtr dataModel,
                                                IDataSourcePtr dataSource)
  {
    IImportDelegatePtr delegate; // Creates a Null Shared Pointer
    if ( className.compare( H5TiffImportDelegateFactory_Detail::ClassName ) == 0)
    {
      delegate.reset ( new H5TiffImportDelegate() );
    }
    return delegate;
  }
  
  /**
   * @brief Returns the Classname of the delegate that this factory can create.
   */
  std::string delegateClassName()
  {
    return H5TiffImportDelegateFactory_Detail::ClassName;
  }
  
  private:
    H5TiffImportDelegateFactory(const H5TiffImportDelegateFactory&);    //Not Implemented
    void operator=(const H5TiffImportDelegateFactory&);  //Not Implemented
};



#endif /*OFIMPORTDELEGATE_H_*/
