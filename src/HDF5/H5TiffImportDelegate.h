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
#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Base/IImportDelegate.h>
#include <DataImport/AbstractImportDelegateFactory.h>

// C++ STL headers
#include <string>
#include <iostream>

// Include Boost Headers
#include <boost/filesystem/path.hpp>

//-- Forward Declare the MXADataModel Class
class MXADataModel;

//Alias the namespace to cut down on line width and typing
namespace FileSystem = boost::filesystem;

namespace H5TiffImport
{
MXA_PROPERTY_CONSTANT(FileNotFoundIsError);
MXA_PROPERTY_CONSTANT(ImportAsGrayScale);
}

/**
 * @brief A generic Import Delegate that imports TIFF images into an HDF5 data file.
 * @author Mike Jackson
 * @date April 2007
 * @class H5TiffImportDelegate HDF5/H5TiffImportDelegate.h HDF5/H5TiffImportDelegate.h
 * @version $Revision: 1.13 $
 */
class MXA_EXPORT H5TiffImportDelegate : public IImportDelegate
{
public:
	H5TiffImportDelegate();
	virtual ~H5TiffImportDelegate();

  /**
  * @brief Implementation from the IDataImportDelegate class
  */
  int32 importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile);

  /**
   * @brief Sets the pointer to the DataModel
   * @param model A Pointer to the DataModel
   */
  void setDataModel(IDataModelPtr model);

  /**
  * @brief Sets the 'FileNotFoundIsError' flag.
  * @param _fileNotFoundIsError If this is true then the import will abort if an input file can not be found
  */
  //void setFileNotFoundIsError(bool value);
  MXA_PROPERTY(bool, FileNotFoundIsError, _fileNotFoundIsError);

  /**
  * @brief Sets the 'ImportAsGrayScale' value
  * @param _importAsGrayScale If TRUE, then the tiff file will be converted to Grayscale
  */
  //void setImportAsGrayScale(bool value);
  MXA_PROPERTY(bool, ImportAsGrayScale, _importAsGrayScale);

  /**
  * @brief Sets a property of this class with the given value.
  * @param key The name of the property.
  * @param value  The value to set the property to.
  */
  virtual int32 setProperty(const std::string &key, const std::string &value);

  virtual int32 getProperty(const std::string &key, const std::string &value)
  {
    std::cout << "H5TiffImportDelegate::getProperty is NOT implemented" << std::endl;
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  template<typename T>
  int32 getProperty(const std::string &key, T &value)
  {
    GET_PROPERTY_BODY(H5TiffImport, T, FileNotFoundIsError, _fileNotFoundIsError, key, value);
    GET_PROPERTY_BODY(H5TiffImport, T, ImportAsGrayScale, _importAsGrayScale, key, value);
    return 0;
  }

private:
  //IDataFilePtr      _dataFilePtr;
  bool              _fileNotFoundIsError;
  bool              _importAsGrayScale;

  H5TiffImportDelegate(const H5TiffImportDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5TiffImportDelegate&); //Copy Assignment Not Implemented
};


#endif /*OFIMPORTDELEGATE_H_*/
