///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006, Shawn Nicholson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////

//MXA Includes
#include "HDF5/H5DataModelWriter.h"
#include "HDF5/H5Lite.h"
#include "HDF5/H5Utilities.h"
#include "HDF5/H5AttributeWriter.h"
#include "Headers/LogTime.h"
#include "MXADataModel/MXADataModel.h"
#include "Utilities/StringUtils.h"

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5DataModelWriter::H5DataModelWriter(IFileIODelegate* ioDelegate, MXADataModel* dataModel)
{
  _ioDelegate  = static_cast<H5IODelegate*>(ioDelegate);
  _dataModel = dataModel;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5DataModelWriter::~H5DataModelWriter()
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool H5DataModelWriter::writeModelToFile(hid_t fileId)
{
//  std::cout << "Writing File Type/Version, DataRoot, and DataModel Groups..." << std::endl;printf("\n");
  if ( writeDataModelTemplate(fileId) < 0 ) return false;
//  std::cout << "Writing Data Dimensions..." << std::endl;printf("\n");
  if ( writeDataDimensions(fileId) < 0) return false;
//  std::cout << "Writing Data Records..." << std::endl;printf("\n");
  if ( writeDataRecords(fileId) < 0) return false;
//  std::cout << "Writing Required MetaData..." << std::endl;  printf("\n");
  if ( writeRequiredMetaData(fileId) < 0) return false;
//  std::cout << "Writing User MetaData...." << std::endl;printf("\n");
  if ( writeUserMetaData(fileId) < 0) return false;
//  std::cout << "Done Writing Model" << std::endl;printf("\n");
  return true;
}


// -----------------------------------------------------------------------------
//  Writes the basic groups of the data model and the file type/version
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeDataModelTemplate(hid_t fileId)
{
  herr_t err = -1;
  err = H5Utilities::createGroupsFromPath( MXA::DataDimensionsPath, fileId);
  if (err < 0)
  {
    std::cout << logTime() << "Error Creating Data Dimension Parent Group" << std::endl;
    return err;
  }
  err = H5Utilities::createGroupsFromPath( MXA::DataRecordsPath, fileId);
  if (err < 0)
  {
    std::cout << logTime() << "Error Creating Data Records Parent Group" << std::endl;
    return err;
  }
  err = H5Utilities::createGroupsFromPath( MXA::MetaDataPath, fileId);
  if (err < 0)
  {
    std::cout << logTime() << "Error Creating MetaData Group" << std::endl;
    return err;
  }
  
  err = 0;
  //Write the File Version
  float32 version = _dataModel->getFileVersion();
  err = H5Lite::writeDataset(fileId, const_cast<std::string&>(MXA::FileVersionPath), version, H5T_NATIVE_FLOAT);
  if (err < 0) {
    std::cout << "Error Writing File Version to HDF File" << std::endl;
    return err;
  }
  //Write the File Type
  std::string fType = _dataModel->getFileType();
  err = H5Lite::writeDataset(fileId, const_cast<std::string&>(MXA::FileTypePath), fType );
  if (err < 0) {
    std::cout << "Error Writing File Type to HDF File" << std::endl;
    return err;
  }
  
  //Write the data root
  std::string dataRoot = _dataModel->getDataRoot();
  err = H5Lite::writeDataset(fileId, const_cast<std::string&>(MXA::DataRootPath), dataRoot);
  if (err < 0)
  {
    std::cout << "Error Writing Data Root to HDF File" << std::endl;
    return err;
  }
  
  // Create the HDF5 Group structure for the actual Data Root
//  H5Utilities::createPathGroups(fileId, _dataModel->getDataRoot());
  err = H5Utilities::createGroupsFromPath(_dataModel->getDataRoot(), fileId);
  if (err < 0) 
  {
    std::cout << logTime() << "Error Creating Group structure for the Data Root" << std::endl;
  }

  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeDataDimensions(hid_t fileId)
{
//  std::cout << "Writing Data Dimensions" << std::endl;
  hid_t gid = H5Utilities::openHDF5Object(fileId, MXA::DataDimensionsPath);
  if (gid < 0) {
    std::cout << "ERROR getting data dimensions group" << std::endl;
    return gid;
  }
  herr_t err = 0;
  
  std::string dsetName;
  
  MXADataDimensions dimensions = _dataModel->getDataDimensions();
  MXADataDimension* dim;
  for (MXADataDimensions::iterator iter = dimensions.begin(); iter < dimensions.end(); ++iter )
  {
    dim = static_cast<MXADataDimension*> ( (*(iter)).get() ); //Get a reference to the raw pointer
    dsetName = StringUtils::numToString( dim->getIndex() );

    // Create the dimension dataset
    int32 i = dim->getIndex();
    H5Lite::writeDataset(gid, dsetName,  i, H5T_NATIVE_INT32);
    
    i = dim->getCount();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_COUNT_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_COUNT_TAG << std::endl; break;}
    
    i = dim->getStartValue();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_START_VALUE_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_START_VALUE_TAG << std::endl; break;}
      
    i = dim->getUniform();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_UNIFORM_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_UNIFORM_TAG << std::endl; break;}
      
    i = dim->getEndValue();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_END_VALUE_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_END_VALUE_TAG << std::endl; break;}
      
    i = dim->getIncrement();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_INCREMENT_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_INCREMENT_TAG << std::endl; break;}
    
    std::string s(dim->getDimensionName() );
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_NAME_TAG), s );
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_NAME_TAG << std::endl; break;}
    
    s = dim->getAltName();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_ALT_NAME_TAG), s);   
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_ALT_NAME_TAG << std::endl; break;}
  }
  
  H5Utilities::closeHDF5Object(gid);
  //std::cout << "Writing Data Dimensions DONE" << std::endl;
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeDataRecords(hid_t fileId)
{
  hid_t gid = H5Utilities::openHDF5Object(fileId, MXA::DataRecordsPath);
  if (gid < 0) {
    std::cout << "ERROR getting data records group" << std::endl;
    return gid;
  } 
  
  MXADataRecords cc =  _dataModel->getDataRecords();
  _traverseDataRecords(gid, cc );
  H5Utilities::closeHDF5Object(gid);
  return gid;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::_traverseDataRecords(hid_t gid, MXADataRecords &records) 
{
//#error Start here and test the writing of the file so far.....
  MXADataRecord* rec;
  std::string dsetName;
  hid_t recGrpId;
  herr_t err = 0;
  int32 i = 0;
  //std::cout << "Traversing Data Record" << std::endl;
  for ( MXADataRecords::iterator iter = records.begin(); iter < records.end(); ++iter )
  {

    rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
    dsetName = StringUtils::numToString( rec->getLuid() );
    
    if (rec->hasChildren() ) {
     // std::cout << "Creating Group for " << rec->getRecordName() << std::endl;
      recGrpId = H5Utilities::createGroup(gid, dsetName);
      MXANodeChildren children = rec->getChildren();
      
      _traverseDataRecords( recGrpId, children );
      H5Utilities::closeHDF5Object(recGrpId);
    } else {
     // std::cout << "Writing Data Record for " << rec->getRecordName() << std::endl;
      i = rec->getLuid();
      err = H5Lite::writeDataset(gid, dsetName, i, H5T_NATIVE_INT32);
      if (err<0) {std::cout << "Error Writing Data Model Record " << dsetName << std::endl; break;}
    }

    // Set the attributes
    i = rec->getLuid();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_LUID_TAG), i);
    if (err<0){std::cout << "Error Writing Attribute " << MXA::MXA_LUID_TAG << std::endl; break;}
    
    i = rec->getGuid();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_GUID_TAG), i);
    if (err<0) {std::cout << "Error Writing Attribute " << MXA::MXA_GUID_TAG << std::endl; break;}

    std::string s = rec->getRecordName();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_NAME_TAG), s);
    if (err<0) {std::cout << "Error Writing Attribute " << MXA::MXA_NAME_TAG << std::endl; break;}
    
    s = rec->getAltName();
    err = H5Lite::writeAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_ALT_NAME_TAG), s );
    if (err<0) {std::cout << "Error Writing Attribute " << MXA::MXA_ALT_NAME_TAG << std::endl; break;}
  }
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeRequiredMetaData(hid_t fileId)
{
  int data = 0;
  herr_t err = 0;
  err = H5Lite::writeDataset(fileId, const_cast<std::string&>(MXA::RequiredMetaDataPath), data, H5T_NATIVE_INT32);
  if (err < 0)
  {
    std::cout << logTime() << "Error Creating Dataset for RequiredMetaData." << err << std::endl;
    return err;
  }
  std::map<std::string, std::string> metadata;
  _dataModel->getRequiredMetaData(metadata);

  for (std::map<std::string, std::string>::iterator iter=metadata.begin(); iter!=metadata.end(); iter++) {
    err = H5Lite::writeAttribute(fileId, const_cast<std::string&>(MXA::RequiredMetaDataPath), const_cast<std::string&>( iter->first), iter->second);
    if(err<0) {std::cout << "Error Writing Required MetaData: " << iter->first << "=" << iter->second << " at path " << MXA::RequiredMetaDataPath << std::endl; break;}
  }
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeUserMetaData(hid_t fileId)
{ 
  int data = 0;
  herr_t err = 0;
  H5Lite::writeDataset(fileId, const_cast<std::string&>(MXA::UserMetaDataPath), data, H5T_NATIVE_INT32);
  std::vector<MXAAttributePtr> metadata = _dataModel->getUserMetaData();
  MXAAttribute* attr = NULL;
  H5AttributeWriter writer;
  for (std::vector<MXAAttributePtr>::iterator iter = metadata.begin(); iter!=metadata.end(); iter++) {
    attr = (*(iter)).get();
   // std::cout << "Writing User MetaData for Key: " << attr->getKey() << std::endl;
    err = attr->write( fileId, const_cast<std::string&>(MXA::UserMetaDataPath), writer);
    if(err<0) {std::cout << "Error Writing User MetaData Attribute " << MXA::UserMetaDataPath  << " Key:" << attr->getKey() << std::endl; break;}
  }
  return err;
}


#if 0
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeSources(hid_t fileId)
{
  if (_num_sources == 0) {
    // don't indicate an error - just nothing to write
    return true;
  }

  DataSource source;
  std::string src_path, dset_name;
  std::string file_path, file_ext;
  std::list<std::string> splitFilePath;
  std::vector<DataSource>::const_iterator iter;
  herr_t error = 0; // < 0 is fail, >= 0 is success
  for (iter=_dataSources.begin(); iter!=_dataSources.end(); iter++) {
    src_path = generateDataSourcePath(*iter);
    dset_name = StringUtils::numToString(iter->record_id);
    file_path = iter->source_path;
    
    splitFilePath = StringUtils::splitString(".", file_path);
    file_ext = splitFilePath.back();
    #if DEBUG
    std::cout << "WRITING RECORD ID: " << dset_name
	            << "\n  FROM: " << src_path 
	            << "\n  SOURCE FILE: " << file_path << std::endl;
    #endif
     
    // Make sure the group path exists
    MXAHDFInterface::createPathGroups(fileId, src_path);
    if ( NULL == iter->parseDelegate) {
      std::cout << ">>>> ParseDelegate was NULL.. No data will be imported. <<<<<" << std::endl;
      return false; 
    }
    error = iter->parseDelegate->encodeSourceToHDF5 (fileId, src_path, dset_name, file_path);
    if (error < 0) {
      std::cout << "Error loading data from file " << file_path << " into the hdf file" << std::endl; 
    }
  }
  return true;
}
#endif

