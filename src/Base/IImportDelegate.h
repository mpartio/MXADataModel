#ifndef IIMPORTDELEGATE_H_
#define IIMPORTDELEGATE_H_


#include <Common/DLLExport.h>
#include <Common/MXATypeDefs.h>


/**
 * @brief Formally defines the methods that subclasses need to implement to create
 * a delegate to import data sources into HDF5 data files.
 * @author Mike Jackson
 * @date April 2007
 * @version $Revision: 1.3 $
 *  
 */
class MXA_EXPORT IImportDelegate
{

public:
  IImportDelegate(){};
  virtual ~IImportDelegate(){};
  
  /**
  * @brief Imports a Data source into the underlying data storage mechanism. This is the method that subclasses
  * should over ride and implement.
  * @param dataSource A Valid IDataSource Object
  * @param model A Valid IDataModel object
  * @return Zero or Positive on Success
  */
  virtual int32 importDataSource(IDataSourcePtr dataSource, IDataModelPtr model) = 0;
  

};




#endif /*IIMPORTDELEGATE_H_*/
