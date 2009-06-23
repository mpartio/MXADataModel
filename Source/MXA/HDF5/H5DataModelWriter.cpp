//MXA Includes
#include <MXA/HDF5/H5DataModelWriter.h>

#include <MXA/Common/LogTime.h>
#include <MXA/Base/IRequiredMetaData.h>
#include <MXA/Base/ISupportFile.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/HDF5/H5AttributeWriter.h>
#include <MXA/Utilities/StringUtils.h>
#include <MXA/Utilities/MXAFileSystemPath.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5DataModelWriter::H5DataModelWriter(IDataModel::Pointer dataModel, IDataFile::Pointer dataFile) :
  _dataModel(dataModel),
  _dataFile(dataFile)
{

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
int32 H5DataModelWriter::writeModelToFile(hid_t fileId)
{
//  std::cout << "Writing File Type/Version, DataRoot, and DataModel Groups..." << std::endl;printf("\n");
  if ( writeDataModelTemplate(fileId) < 0 ) return -1;
//  std::cout << "Writing Data Dimensions..." << std::endl;printf("\n");
  if ( writeDataDimensions(fileId) < 0) return -1;
//  std::cout << "Writing Data Records..." << std::endl;printf("\n");
  if ( writeDataRecords(fileId) < 0) return -1;
//  std::cout << "Writing Required MetaData..." << std::endl;  printf("\n");
  if ( writeRequiredMetaData(fileId) < 0) return -1;
//  std::cout << "Writing User MetaData...." << std::endl;printf("\n");
  if ( writeUserMetaData(fileId) < 0) return -1;
//  std::cout << "Done Writing Model" << std::endl;printf("\n");
  if ( writeSupportFiles(fileId) < 0) return -1;
  // std::cout << "Done writing support files" << std::endl;printf("\n");

  return 1;
}

// -----------------------------------------------------------------------------
//  Writes the support files to the HDF5 file
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeSupportFiles(hid_t fileId)
{
  int32 err = 0;
  uint64 fileSize[1] = { 0 };
  uint8* fileContents = NULL;
  int32 rank = 1;
  ISupportFile* file = NULL;
  ISupportFile::Container files = this->_dataModel->getSupportFiles();

  for (ISupportFile::Container::iterator iter = files.begin(); iter != files.end(); ++iter)
  {
    file = (*iter).get();
    if (NULL != file)
    {
      err = file->readFromFileSystem();
      if (err >= 0)
      {
        fileContents = file->getFilePointer(0);
        fileSize[0] = file->getFileSize();
        std::string dsetName = MXA::SupportFilesPath + "/" + StringUtils::numToString(file->getIndex());
        if (dsetName.empty() == false) {
          err = H5Lite::writePointerDataset<uint8>(fileId, dsetName, rank, fileSize, fileContents);
          err = H5Lite::writeStringAttribute(fileId, dsetName, MXA::MXA_FILESYSTEM_PATH_TAG, file->getFileSystemPath() );
          err = H5Lite::writeStringAttribute(fileId, dsetName, MXA::MXA_FILETYPE_TAG, file->getFileType() );
          std::string path = MXAFileSystemPath::filename( file->getFileSystemPath() );
          err = H5Lite::writeStringAttribute(fileId, dsetName, MXA::MXA_FILENAME_TAG, path);

        }
        else
        { err = -1; }
      }
    }
    else
    {
      err = -1;
    }
    if (err < 0) { break; } // Bail out of the loop if there is an error
  }
  return err;
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
  err = H5Utilities::createGroupsFromPath( MXA::SupportFilesPath, fileId);
  if (err < 0)
  {
    std::cout << logTime() << "Error Creating SupportFiles Group" << std::endl;
    return err;
  }

  err = 0;
  //Write the File Version
  hid_t modelGroupId = H5Gopen(fileId, MXA::DataModelPath.c_str());
  if (modelGroupId < 0)
  {
    err = -1;
    std::cout << logTime() << "Error Opening 'Data Model' Group." << std::endl;
    return err;
  }
  else
  {
    std::string fType = _dataModel->getModelType();
    err = H5Lite::writeStringAttribute(fileId, MXA::DataModel.c_str(), MXA::ModelType, fType);
    if (err < 0) {
      std::cout << logTime() << "Error Writing Model Type to Data Model Group as Attribute." << std::endl;
    }
    float32 version = _dataModel->getModelVersion();
    err = H5Lite::writeScalarAttribute(fileId, MXA::DataModel, MXA::ModelVersion, version);
    if (err < 0)
    {
      std::cout << logTime() << "Error Writing Model Version to Data Model Group as Attribute." << std::endl;
    }
  }
  err = H5Gclose(modelGroupId);
  if (err < 0) { std::cout << DEBUG_OUT(logTime) << "Error Closing 'Data Model' Group" << std::endl; return err; }

  //Write the data root
  std::string dataRoot = _dataModel->getDataRoot();
  err = this->_writeStringDataset(fileId, const_cast<std::string&>(MXA::DataRootPath), dataRoot, true);
  if (err < 0)
  {
    std::cout << "Error Writing Data Root to HDF File" << std::endl;
    return err;
  }

  // Create the HDF5 Group structure for the actual Data Root
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

  IDataDimension::Container dimensions = _dataModel->getDataDimensions();
  MXADataDimension* dim;
  for (IDataDimension::Container::iterator iter = dimensions.begin(); iter < dimensions.end(); ++iter )
  {
    dim = static_cast<MXADataDimension*> ( (*(iter)).get() ); //Get a reference to the raw pointer
    //std::cout << "Writing DataDimension: " << dim->getDimensionName() << std::endl;
    dsetName = StringUtils::numToString( dim->getIndex() );

    // Create the dimension dataset
    int32 i = dim->getIndex();
    err = this->_writeScalarDataset(gid, dsetName,  i);
    if (err<0) { std::cout << "Error Writing DatasetName for DataDimension: " << dim->getDimensionName() << std::endl;;break;}

    i = dim->getCount();
    err = H5Lite::writeScalarAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_COUNT_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_COUNT_TAG << std::endl; break;}

    i = dim->getStartValue();
    err = H5Lite::writeScalarAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_START_VALUE_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_START_VALUE_TAG << std::endl; break;}

    i = dim->getUniform();
    err = H5Lite::writeScalarAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_UNIFORM_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_UNIFORM_TAG << std::endl; break;}

    i = dim->getEndValue();
    err = H5Lite::writeScalarAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_END_VALUE_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_END_VALUE_TAG << std::endl; break;}

    i = dim->getIncrement();
    err = H5Lite::writeScalarAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_INCREMENT_TAG), i);
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_INCREMENT_TAG << std::endl; break;}

    std::string s(dim->getDimensionName() );
    err = H5Lite::writeStringAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_NAME_TAG), s );
    if (err<0) { std::cout << "Error writing attribute " << MXA::MXA_NAME_TAG << std::endl; break;}

    s = dim->getAltName();
    err = H5Lite::writeStringAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_ALT_NAME_TAG), s);
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
  int32 retErr = 0;
  herr_t err = 0;
  if (gid < 0) {
    std::cout << "ERROR getting data records group" << std::endl;
    return gid;
  }

  IDataRecord::Container cc =  _dataModel->getDataRecords();
  err = _traverseDataRecords(gid, cc );
  if (err < 0) { retErr = err; }
  err = H5Utilities::closeHDF5Object(gid);
  if (err < 0) { retErr = err; }
  return retErr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::_traverseDataRecords(hid_t gid, IDataRecord::Container &records)
{
//#error Start here and test the writing of the file so far.....
  MXADataRecord* rec;
  std::string dsetName;
  hid_t recGrpId;
  herr_t err = 0;
  int32 i = 0;
  //std::cout << "Traversing Data Record" << std::endl;
  for ( IDataRecord::Container::iterator iter = records.begin(); iter < records.end(); ++iter )
  {
    rec = dynamic_cast<MXADataRecord*> ( (*(iter)).get() ); //get the Raw pointer to the object
    //std::cout << "Writing '" << rec->getRecordName()  << "'" << std::endl;
    dsetName = StringUtils::numToString( rec->getLuid() );
   // std::cout << "Writing '" << rec->getRecordName()  << "' as '" << dsetName << "'" << std::endl;
    if (rec->hasChildren() ) {
     // std::cout << "Creating Group for '" << rec->getRecordName()  << "' as '" << dsetName << "'" << std::endl;
      recGrpId = H5Utilities::createGroup(gid, dsetName);
      if (recGrpId < 0)
      {
        std::cout << "Error Creating Group for Writing HDF5 file" << std::endl;
        err = recGrpId;
        break;
      }
      IDataRecord::Container children = rec->getChildren();

      err = _traverseDataRecords( recGrpId, children );
      H5Utilities::closeHDF5Object(recGrpId);
      if (err < 0) {
        break;
      }

    } else {
   //   std::cout << "Writing Data Record for " << rec->getRecordName() << std::endl;
      i = rec->getLuid();
      err = this->_writeScalarDataset(gid, dsetName, i);
      if (err<0) {std::cout << "Error Writing Data Model Record " << dsetName << std::endl; break;}
    }

    // Set the attributes
    i = rec->getLuid();
    err = H5Lite::writeScalarAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_LUID_TAG), i);
    if (err<0){std::cout << "Error Writing Attribute " << MXA::MXA_LUID_TAG << std::endl; break;}

    i = rec->getGuid();
    err = H5Lite::writeScalarAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_GUID_TAG), i);
    if (err<0) {std::cout << "Error Writing Attribute " << MXA::MXA_GUID_TAG << std::endl; break;}

    std::string s = rec->getRecordName();
    err = H5Lite::writeStringAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_NAME_TAG), s);
    if (err<0) {std::cout << "Error Writing Attribute " << MXA::MXA_NAME_TAG << std::endl; break;}

    s = rec->getAltName();
    err = H5Lite::writeStringAttribute(gid, dsetName, const_cast<std::string&>(MXA::MXA_ALT_NAME_TAG), s );
    if (err<0) {std::cout << "Error Writing Attribute " << MXA::MXA_ALT_NAME_TAG << std::endl; break;}
#if 0
    H5AttributeWriter attrWriter;
    MXAAttributes attributes = rec->getUserDefinedAttributes();
    for (std::vector<MXAAttributePtr>::iterator iter = attributes.begin(); iter != attributes.end(); ++iter )
    {
      (*(iter))->write(gid, dsetName, attrWriter);
    }
    std::cout << logTime() << "Done writing Record: " << rec->getRecordName() << std::endl;
#endif
  }
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeRequiredMetaData(hid_t fileId)
{
  int32 data = 0;
  herr_t err = 0;
  err = this->_writeScalarDataset(fileId, const_cast<std::string&>(MXA::RequiredMetaDataPath), data);
  if (err < 0)
  {
    std::cout << logTime() << "Error Creating Dataset for RequiredMetaData." << err << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")\n" << std::endl;
    return err;
  }
  std::map<std::string, std::string> meta;
  IRequiredMetaData::Pointer metaData = _dataModel->getRequiredMetaData();
  metaData->generateKeyValueMap(meta);

  for (std::map<std::string, std::string>::iterator iter=meta.begin(); iter!=meta.end(); iter++) {
    err = H5Lite::writeStringAttribute(fileId, const_cast<std::string&>(MXA::RequiredMetaDataPath), const_cast<std::string&>( iter->first), iter->second);
    if(err<0) {std::cout << logTime() << "Error Writing Required MetaData: " << iter->first << "=" << iter->second << " at path " << MXA::RequiredMetaDataPath << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")\n" << std::endl;
    break;}
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5DataModelWriter::writeUserMetaData(hid_t fileId)
{
  int32 data = 0;
  herr_t err = 0;
  this->_writeScalarDataset(fileId, const_cast<std::string&>(MXA::UserMetaDataPath), data);
  MXAAbstractAttributes metadata = _dataModel->getUserMetaData();
  IMXAArray::Pointer attr;
  //H5AttributeWriter writer;
  for (MXAAbstractAttributes::iterator iter = metadata.begin(); iter!=metadata.end(); iter++) {
    std::string key = (*iter).first;
    attr = (*iter).second;


    err = H5Lite::writeMXAAttribute(fileId, MXA::UserMetaDataPath, key, attr.get() );
    if(err<0) {
      std::cout << logTime() << "Error Writing User MetaData at HDF5 Path '" << MXA::UserMetaDataPath  << "' using Key:" << key << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")\n" << std::endl;
      break;
    }
  }
  return err;
}


// -----------------------------------------------------------------------------
//  Writes a string to a HDF5 dataset
// -----------------------------------------------------------------------------
herr_t H5DataModelWriter::_writeStringDataset (hid_t loc_id,
                                            const std::string& dsetName,
                                            const std::string &data,
                                            bool overwrite)
{
  hid_t   did=-1;
  hid_t   sid=-1;
  hid_t   tid = -1;
  size_t  size = 0;
  herr_t err = -1;
  herr_t retErr = 0;
  /* create a string data type */
  if ( (tid = H5Tcopy( H5T_C_S1 )) >= 0 )
  {
    size = data.size() + 1;
    if ( H5Tset_size(tid, size) >= 0 )
    {
      if ( H5Tset_strpad(tid, H5T_STR_NULLTERM ) >= 0 )
      {
      /* Create the data space for the dataset. */
        if ( (sid = H5Screate( H5S_SCALAR )) >= 0 )
        {
          /* Create the dataset. */
          if (overwrite == true)
          {
            HDF_ERROR_HANDLER_OFF
            did = H5Dopen(loc_id, dsetName.c_str() );
            if (did > 0)
            {
              hsize_t storageSize = H5Dget_storage_size(did);
              if (did > 0 && storageSize != data.size() + 1)
              { // Data sizes are NOT the same. Delete current data set
                err = H5Gunlink(loc_id, dsetName.c_str() );
                if (err < 0 )
                {
                  retErr = err;
                }
                err = H5Dclose(did);
                did = -1;
              }
            }
            HDF_ERROR_HANDLER_ON
          }
          if (did < 0 && retErr >= 0) // dataset does not exist
          {
            did = H5Dcreate(loc_id, dsetName.c_str(), tid, sid, H5P_DEFAULT);
          }
          if ( did >= 0 && retErr >= 0)
          {
             if ( !data.empty() )
              {
                  err = H5Dwrite(did, tid, H5S_ALL, H5S_ALL,H5P_DEFAULT, data.c_str() );
                  if (err<0 ) {
                    std::cout << "Error Writing String Data" << std::endl;
                    retErr = err;
                  }
              }
          } else {
            retErr = did;
          }
          CloseH5D(did, err, retErr);
        }
        CloseH5S(sid, err, retErr);
      }
    }
    CloseH5T(tid, err, retErr);
  }
  return retErr;
}

