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

//-- Boost Includes
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <Common/MXATypes.h>

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
class IImportDelegate;
class IFileIODelegate;
class IDataSource;
class IDataModel;
class IDataDimension;
class IDataRecord;
class IAttribute;
class AbstractImportDelegateFactory;
class ImportDelegateManager;
class H5TiffImportDelegateFactory;
class IStringSection;

typedef  boost::shared_ptr<MXADataModel>           MXADataModelPtr;
typedef  boost::shared_ptr<IDataModel>             IDataModelPtr;

typedef  boost::shared_ptr<MXAAttribute>           MXAAttributePtr;
typedef  boost::shared_ptr<IAttribute>             IAttributePtr;
typedef  std::vector<MXAAttributePtr>              MXAAttributes;
typedef  std::vector<IAttributePtr>                IAttributes;
typedef  std::map<std::string, MXAAttributePtr>    MXAAttributeMap;
typedef  std::vector<MXAAttributePtr>              MXAUserMetaData;
typedef  std::map<std::string, std::string>        MXARequiredMetaData;

typedef  boost::shared_ptr<IFileIODelegate>        IODelegatePtr;

// Importing of Data related 
typedef  boost::shared_ptr<MXADataSource>          MXADataSourcePtr;
typedef  boost::shared_ptr<IDataSource>            IDataSourcePtr;
typedef  std::vector<MXADataSourcePtr>             MXADataSources;
typedef  std::vector<IDataSourcePtr>               IDataSources;
typedef  boost::shared_ptr<MXADataImport>          MXADataImportPtr;
typedef  boost::shared_ptr<IDataImport>            IDataImportPtr;
typedef  boost::shared_ptr<IImportDelegate>                IImportDelegatePtr;
typedef  boost::shared_ptr<AbstractImportDelegateFactory>  AbstractImportDelegateFactoryPtr;
typedef  boost::shared_ptr<ImportDelegateManager>          ImportDelegateManagerPtr;
//typedef  boost::shared_ptr<ImportDelegateFactory>          ImportDelegateFactoryPtr;
typedef  std::vector<AbstractImportDelegateFactoryPtr>     AbstractImportDelegateFactories;

// Data Dimension typedefs
typedef  boost::shared_ptr<MXADataDimension>       MXADataDimensionPtr;
typedef  boost::shared_ptr<IDataDimension>         IDataDimensionPtr;
typedef  std::vector<MXADataDimensionPtr>          MXADataDimensions;
typedef  std::vector<IDataDimensionPtr>            IDataDimensions;

// Data Record Typedefs
typedef  boost::shared_ptr<IDataRecord>            IDataRecordPtr;
typedef  boost::shared_ptr<MXADataRecord>          MXADataRecordPtr;
typedef  boost::weak_ptr<MXADataRecord>            MXADataRecordWeakPtr;
typedef  boost::weak_ptr<IDataRecord>              IDataRecordWeakPtr;
typedef  std::vector<IDataRecordPtr>               IDataRecords;
typedef  std::vector<MXADataRecordPtr>             MXADataRecords;
typedef  std::map<int32, IDataRecordPtr>           IDataRecordLookupTable;

// HDF Typedefs
typedef boost::shared_ptr<H5TiffImportDelegateFactory>  H5TiffImportDelegateFactoryPtr;


typedef boost::shared_ptr<IStringSection>          IStringSectionPtr;
typedef std::vector<IStringSectionPtr>     IStringSections;

