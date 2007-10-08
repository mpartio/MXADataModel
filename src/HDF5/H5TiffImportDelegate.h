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
#include <Common/DLLExport.h>
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

/**
 * @brief A generic Import Delegate that imports TIFF images into an HDF5 data file.
 * @author Mike Jackson
 * @date April 2007
 * @class H5TiffImportDelegate HDF5/H5TiffImportDelegate.h HDF5/H5TiffImportDelegate.h
 * @version $Revision: 1.10 $
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
  * @brief Sets the 'FileNotFoundIsError' flag.
  * @param value If this is true then the import will abort if an input file can not be found
  */
  void setFileNotFoundIsError(bool value);

  /**
  * @brief Sets the 'ImportAsGrayScale' value
  * @param value If TRUE, then the tiff file will be converted to Grayscale
  */
  void setImportAsGrayScale(bool value);
  
private:
  IDataModelPtr      _modelPtr;
  bool _fileNotFoundIsError;
  bool _importAsGrayScale;
  
  H5TiffImportDelegate(const H5TiffImportDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5TiffImportDelegate&); //Copy Assignment Not Implemented
};


#endif /*OFIMPORTDELEGATE_H_*/
