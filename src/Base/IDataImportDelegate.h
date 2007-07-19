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
#ifndef _IDATAIMPORTDELEGATE_H_
#define _IDATAIMPORTDELEGATE_H_


#include "Common/DLLExport.h"


/**
 * @brief Formally defines the methods that subclasses need to implement to create
 * a delegate to import data sources into HDF5 data files.
 * @author Mike Jackson
 * @date April 2007
 * @version 1.0
 *  
 */
class MXA_EXPORT IDataImportDelegate
{

public:
  IDataImportDelegate(){};
  virtual ~IDataImportDelegate(){};
  
  /**
  * @brief
  * @param dataSource
  * @param model
  * @return Zero or Positive on Success
  */
  virtual int32 importDataSource(MXADataSourcePtr dataSource, MXADataModelPtr model) = 0;
  

};



#endif /* _IDATAIMPORTDELEGATE_H_ */

