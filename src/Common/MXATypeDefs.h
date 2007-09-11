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

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "Common/MXATypes.h"

/** @file MXATypeDefs.h
 * @brief This file contains many common typdefs used through out the MXA Source code
 */


class MXADataModel;
class MXANode;
class MXAAttribute;
class MXADataRecord;
class MXADataDimension;
class MXADataSource;
class MXADataImport;
class IDataImport;
class IDataImportDelegate;
class IFileIODelegate;
class IDataSource;
class IDataModel;
class IDataDimension;
class IDataRecord;


typedef  boost::shared_ptr<MXADataModel>           MXADataModelPtr; /**< MXADataModel wrapped in a Boost Shared Pointer*/
typedef  boost::shared_ptr<IDataModel>             IDataModelPtr;
typedef  boost::shared_ptr<MXANode>                MXANodePtr;
typedef  boost::shared_ptr<MXAAttribute>           MXAAttributePtr;
typedef  boost::shared_ptr<MXADataRecord>          MXADataRecordPtr;
typedef  boost::shared_ptr<IDataRecord>            IDataRecordPtr;
typedef  boost::shared_ptr<MXADataDimension>       MXADataDimensionPtr;
typedef  boost::shared_ptr<IDataDimension>         IDataDimensionPtr;
typedef  boost::shared_ptr<IFileIODelegate>        IODelegatePtr;
typedef  std::vector<MXAAttributePtr>              MXAUserMetaData;
typedef  std::vector<MXAAttributePtr>              MXAAttributes;

typedef  boost::shared_ptr<MXADataSource>          MXADataSourcePtr;
typedef  boost::shared_ptr<IDataSource>            IDataSourcePtr;
typedef  std::vector<MXADataSourcePtr>             MXADataSources;
typedef  std::vector<IDataSourcePtr>               IDataSources;
typedef  boost::shared_ptr<IDataImportDelegate>    IDataImportDelegatePtr;
typedef  boost::shared_ptr<MXADataImport>          MXADataImportPtr;
typedef  boost::shared_ptr<IDataImport>            IDataImportPtr;


typedef  std::vector<MXADataDimensionPtr>          MXADataDimensions;
typedef  std::vector<IDataDimensionPtr>            IDataDimensions;
typedef  std::vector<MXANodePtr>                   MXADataRecords;
typedef  std::vector<MXANodePtr>                   MXANodes;

typedef  std::map<std::string, MXAAttributePtr>    Attributes;


typedef  boost::weak_ptr<MXANode>                  MXANodeWeakPtr;
typedef  std::vector<MXANodePtr>                   MXANodeChildren;
typedef  std::map<int32, MXANodePtr>               NodeLookupTable;
typedef  std::map<std::string, std::string>        MXARequiredMetaData;



