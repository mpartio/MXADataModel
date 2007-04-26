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
#include "Headers/DLLExport.h"
#include "Headers/MXATypes.h"
#include "Headers/MXATypeDefs.h"
#include "Interfaces/IDataImportDelegate.h"

// Include Boost Headers
#include <boost/filesystem/path.hpp>


//-- Forward Declare the MXADataModel Class
class MXADataModel;

//Alias the namespace to cut down on line width and typing
namespace FileSystem = boost::filesystem;

/**
 * @brief
 * @author
 * @date April 2007
 * @header
 */
class MXA_EXPORT H5TiffImportDelegate : public IDataImportDelegate
{
public:
	H5TiffImportDelegate();
	virtual ~H5TiffImportDelegate();
  
  /**
  * @brief Implementation from the IDataImportDelegate class
  */
  int32 importDataSource(MXADataSourcePtr dataSource, MXADataModelPtr model);
  
  /**
   * @brief Sets the pointer to the DataModel
   * @param model A Pointer to the DataModel
   */
  void setDataModel(MXADataModelPtr model);
  
  void setFileNotFoundIsError(bool value);
  void setImportAsGrayScale(bool value);
  
private:
  MXADataModelPtr _modelPtr;
  bool _fileNotFoundIsError;
  bool _importAsGrayScale;
  
  H5TiffImportDelegate(const H5TiffImportDelegate&);   //Copy Constructor Not Implemented
  void operator=(const H5TiffImportDelegate&); //Copy Assignment Not Implemented

  
  
};

#endif /*OFIMPORTDELEGATE_H_*/
