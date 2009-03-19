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
#ifndef _MXATYPEDEFS_H__
#define _MXATYPEDEFS_H__


#include <string>
#include <vector>
#include <map>

//-- Boost Includes
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_array.hpp>

#include <MXA/Common/MXATypes.h>

/** @file MXATypeDefs.h
 * @brief This file contains many common typdefs used through out the MXA Source code
 */

#if defined (MXA_USE_SHARED_STRINGS)
typedef boost::shared_ptr<std::string>    MXAString;
#else
typedef std::string MXAString;
#endif


class IDataFile;
class IFileReader;
class IFileWriter;
class MXADataModel;
class MXANode;
class MXADataRecord;
class MXADataDimension;
class MXADataSource;
class MXADataImport;
class RequiredMetaData;
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

class IDataArray;
class ISupportFile;
//class ISupportFileContents;

// Data Encapsulation
class IDataset;
class IMXAArray;
class MXASupportFileContents;

// MXA Meta Data
class IRequiredMetaData;
class H5MXARequiredMetaData;

class DataExportProperties;

typedef boost::shared_ptr<DataExportProperties>     DataExportPropertiesPtr;

typedef boost::shared_ptr<ISupportFile>             ISupportFilePtr;
typedef std::vector<ISupportFilePtr>                ISupportFiles;
//typedef boost::shared_ptr<ISupportFileContents>   ISupportFileContentsPtr;
typedef boost::shared_ptr<IMXAArray>                MXASupportFileContentsPtr;

typedef boost::shared_ptr<IDataset>                 IDatasetPtr;
typedef boost::shared_ptr<IMXAArray>                IMXAArrayPtr;
//typedef boost::shared_ptr<MXAAbstractData>          MXAAbstractDataPtr;
//typedef boost::shared_ptr<MXAAbstractAttribute>     MXAAbstractAttributePtr;
typedef std::map<std::string, IMXAArrayPtr>         MXAAbstractAttributes;

typedef boost::shared_ptr<IRequiredMetaData>        IRequiredMetaDataPtr;
typedef boost::shared_ptr<H5MXARequiredMetaData>    H5MXARequiredMetaDataPtr;
typedef boost::shared_ptr<RequiredMetaData>         RequiredMetaDataPtr;

typedef boost::shared_ptr<MXADataModel>           MXADataModelPtr;
typedef boost::shared_ptr<IDataModel>             IDataModelPtr;

typedef boost::shared_ptr<IDataFile>                IDataFilePtr;
typedef boost::shared_ptr<IFileReader>              IFileReaderPtr;
typedef boost::shared_ptr<IFileWriter>              IFileWriterPtr;

typedef std::map<std::string, std::string>        MXARequiredMetaData;
typedef boost::shared_ptr<IFileIODelegate>        IFileIODelegatePtr;

// Importing of Data related
typedef boost::shared_ptr<IDataSource>                     IDataSourcePtr;
typedef std::vector<IDataSourcePtr>                        IDataSources;
typedef boost::shared_ptr<IDataImport>                     IDataImportPtr;
typedef boost::shared_ptr<IImportDelegate>                 IImportDelegatePtr;
typedef boost::shared_ptr<AbstractImportDelegateFactory>   AbstractImportDelegateFactoryPtr;
typedef std::vector<AbstractImportDelegateFactoryPtr>      AbstractImportDelegateFactories;
typedef boost::shared_ptr<ImportDelegateManager>           ImportDelegateManagerPtr;
typedef boost::shared_ptr<MXADataSource>                   MXADataSourcePtr;
typedef std::vector<MXADataSourcePtr>                      MXADataSources;
typedef boost::shared_ptr<MXADataImport>                   MXADataImportPtr;


// Data Dimension typedefs
typedef boost::shared_ptr<MXADataDimension>       MXADataDimensionPtr;
typedef boost::shared_ptr<IDataDimension>         IDataDimensionPtr;
typedef std::vector<MXADataDimensionPtr>          MXADataDimensions;
typedef std::vector<IDataDimensionPtr>            IDataDimensions;

// Data Record Typedefs
typedef boost::shared_ptr<IDataRecord>            IDataRecordPtr;
typedef boost::shared_ptr<MXADataRecord>          MXADataRecordPtr;
typedef boost::weak_ptr<MXADataRecord>            MXADataRecordWeakPtr;
typedef boost::weak_ptr<IDataRecord>              IDataRecordWeakPtr;
typedef std::vector<IDataRecordPtr>               IDataRecords;
typedef std::vector<MXADataRecordPtr>             MXADataRecords;
typedef std::map<int32, IDataRecordPtr>           IDataRecordLookupTable;

// HDF Typedefs
typedef boost::shared_ptr<H5TiffImportDelegateFactory>  H5TiffImportDelegateFactoryPtr;


typedef boost::shared_ptr<IStringSection>          IStringSectionPtr;
typedef std::vector<IStringSectionPtr>             IStringSections;

//-- Define some Shared Arrays to use
typedef boost::shared_array<uint8>                 UCharArray;
typedef boost::shared_array<int8>                  CharArray;
typedef boost::shared_array<uint16>                UShortArray;
typedef boost::shared_array<int16>                 ShortArray;
typedef boost::shared_array<uint32>                UIntArray;
typedef boost::shared_array<int32>                 IntArray;
typedef boost::shared_array<uint64>                ULongLongArray;
typedef boost::shared_array<int64>                 LongLongArray;


#define NEW_UCHAR_ARRAY(VarName, Size)\
  UCharArray VarName(new uint8[Size]);

/* If we are compiling WITHOUT HDF5 support then we need some enumerated types */
#if  MXA_HDF5_SUPPORT == 0

#include <MXA/Common/LogTime.h>
#define H5T_NATIVE_INT8 1
#define H5T_NATIVE_INT8  1
#define H5T_NATIVE_UINT8  2
#define H5T_NATIVE_INT16  3
#define H5T_NATIVE_UINT16  4
#define H5T_NATIVE_INT32  5
#define H5T_NATIVE_UINT32  6
#define H5T_NATIVE_INT64  7
#define H5T_NATIVE_UINT64  8
#define H5T_NATIVE_FLOAT  9
#define H5T_NATIVE_DOUBLE  10
#define H5T_STRING  11


namespace H5Lite
{



/**
 *
 * @param value
 * @return
 */
template<typename T>
 MXA_EXPORT std::string HDFTypeForPrimitiveAsStr(T value)
{
  if (typeid(value) == typeid(int8)) return "H5T_NATIVE_INT8";
  if (typeid(value) == typeid(uint8)) return "H5T_NATIVE_UINT8";

  if (typeid(value) == typeid(int16)) return "H5T_NATIVE_INT16";
  if (typeid(value) == typeid(uint16)) return "H5T_NATIVE_UINT16";

  if (typeid(value) == typeid(int32)) return "H5T_NATIVE_INT32";
  if (typeid(value) == typeid(uint32)) return "H5T_NATIVE_UINT32";

  if (typeid(value) == typeid(int64)) return "H5T_NATIVE_INT64";
  if (typeid(value) == typeid(uint64)) return "H5T_NATIVE_UINT64";

  if (typeid(value) == typeid(float32)) return "H5T_NATIVE_FLOAT";
  if (typeid(value) == typeid(float64)) return "H5T_NATIVE_DOUBLE";

  //if (typeid(value) == typeid(bool)) return "H5T_NATIVE_UINT8";

  std::cout << DEBUG_OUT(logTime) << "Error: HDFTypeForPrimitiveAsStr - Unknown Type: " << typeid(value).name() << std::endl;
  return "";
}

/**
 *
 * @param value
 * @return
 */
template<typename T>
 MXA_EXPORT int32 HDFTypeForPrimitive(T value)
{
  if (typeid(value) == typeid(float32)) return H5T_NATIVE_FLOAT;
  if (typeid(value) == typeid(float64)) return H5T_NATIVE_DOUBLE;

  if (typeid(value) == typeid(int8)) return H5T_NATIVE_INT8;
  if (typeid(value) == typeid(uint8)) return H5T_NATIVE_UINT8;

  if (typeid(value) == typeid(int16)) return H5T_NATIVE_INT16;
  if (typeid(value) == typeid(uint16)) return H5T_NATIVE_UINT16;

  if (typeid(value) == typeid(int32)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(uint32)) return H5T_NATIVE_UINT32;

  if (typeid(value) == typeid(int64)) return H5T_NATIVE_INT64;
  if (typeid(value) == typeid(uint64)) return H5T_NATIVE_UINT64;

  if (typeid(value) == typeid(bool)) return H5T_NATIVE_UINT8;

  return -1;
}

static int32 HDFTypeFromString(const std::string &value)
{
  if (value.compare("H5T_NATIVE_INT8") == 0) return H5T_NATIVE_INT8;
  if (value.compare("H5T_NATIVE_UINT8") == 0) return H5T_NATIVE_UINT8;

  if (value.compare("H5T_NATIVE_INT16") == 0) return H5T_NATIVE_INT16;
  if (value.compare("H5T_NATIVE_UINT16") == 0) return H5T_NATIVE_UINT16;

  if (value.compare("H5T_NATIVE_INT32") == 0) return H5T_NATIVE_INT32;
  if (value.compare("H5T_NATIVE_UINT32") == 0) return H5T_NATIVE_UINT32;

  if (value.compare("H5T_NATIVE_INT64") == 0) return H5T_NATIVE_INT64;
  if (value.compare("H5T_NATIVE_UINT64") == 0) return H5T_NATIVE_UINT64;

  if (value.compare("H5T_NATIVE_FLOAT") == 0) return H5T_NATIVE_FLOAT;
  if (value.compare("H5T_NATIVE_DOUBLE") == 0) return H5T_NATIVE_DOUBLE;

  if (value.compare("H5T_STRING") == 0) return H5T_STRING;

  std::cout << DEBUG_OUT(logTime) << "Error: HDFTypeFromString - Unknown Type: " << value << std::endl;
  return -1;
}

} /* End H5Lite namespace */

#endif /* MXA_HDF5_SUPPORT == 0 */


#endif /* _MXATYPEDEFS_H__ */
