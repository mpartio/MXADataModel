///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) Mike Jackson,
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef IIMPORTDELEGATE_H_
#define IIMPORTDELEGATE_H_

#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Base/IImportProperty.h>
#include <string>


/**
 * @brief Formally defines the methods that subclasses need to implement to create
 * a delegate to import data sources into HDF5 data files.
 * @author Mike Jackson
 * @date April 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT IImportDelegate : public IImportProperty
{

public:
  IImportDelegate(){};
  virtual ~IImportDelegate(){};

  /**
  * @brief Imports a Data source into the underlying data storage mechanism. This is the method that subclasses
  * should over ride and implement.
  * @param dataSource A Valid IDataSource Object
  * @param dataFile The IDataFile object
  * @return Zero or Positive on Success
  */
  virtual int32 importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile) = 0;
  #if 0
  virtual int32 setProperty(const std::string &key, const std::string &value)
    { return 0; }

  virtual int32 getProperty(const std::string &key,  std::string &value)
    { return 0; }
#endif

};




#endif /*IIMPORTDELEGATE_H_*/
