#include "Headers/LogTime.h"
#include "HDF5/H5IODelegate.h"
#include "HDF5/H5Lite.h"
#include "HDF5/H5DataModelReader.h"
#include "HDF5/H5DataModelWriter.h"
#include "HDF5/H5Utilities.h"

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5IODelegate::H5IODelegate() :
  _fileId(-1),
  _openFile("")
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5IODelegate::~H5IODelegate()
{
  if (_fileId > 0) {
   // std::cout << "H5IODelegate::~H5IODelegate()  Closing file.." << std::endl;   
    this->closeMXAFile();
  }
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXATypes::MXAError H5IODelegate::writeFromModel(std::string fileName, MXADataModel* model, bool closeWhenFinished)
{
  
  int32 success = -1;
  _fileId = -1;
  //Create the HDF File
  _fileId = this->createMXAFile(fileName);
  if (_fileId < 0)
  {
    std::cout << "Error Creating new MXA File" << std::endl;
    return _fileId;
  }
  
  H5DataModelWriter writer(this, model );
  success = writer.writeModelToFile(_fileId);
  
  //Close the file as we are done with it.
  if (closeWhenFinished) 
  {  closeMXAFile(); }
  else 
  {
    this->_openFile = fileName;
  }
  return success;
}

// -----------------------------------------------------------------------------
//  Opens the file and then reads the data model from the file. Then closes the
//  when reading is complete.
// -----------------------------------------------------------------------------
MXATypes::MXAError H5IODelegate::readIntoModel(std::string fileName, MXADataModel* model, bool closeWhenFinished)
{
  //Open the HDF File
  _fileId = openMXAFile(fileName, true);
  if (_fileId < 0) {
    return _fileId;
  }
  //Instantiate the H5DataModelReader object
  H5DataModelReader reader(this, model);
  herr_t error = reader.readDataModel(_fileId);
  //Close the file as we are done with it.
  if (closeWhenFinished) 
  {  closeMXAFile(); }
  else 
  {
    this->_openFile = fileName;
  }
  return error;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool H5IODelegate::supportedMXAFileVersion(float version)
{
  //FIXME: Change this to a constant
  if (version == MXA::MXACurrentFileVersion) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Returns true if the file referred to by fileId is an HDF5
//  compliant file (has the correct file type and version)
// -----------------------------------------------------------------------------
//TODO: Clean this up
bool H5IODelegate::isMXAFile(hid_t fileId)
{
  herr_t err=0;
  
  // Check the Type
  std::string ftype;
  err = H5Lite::readDataset(fileId, const_cast<std::string&>(MXA::FileTypePath), ftype);
  if (err < 0 || (ftype != "MHD")) {
    std::cout << "NOT an HDF5 File (wrong type): " << ftype << std::endl;
    return false;
  }

  // Check the Version
  float32 fileVersion;
  err = H5Lite::readDataset(fileId, MXA::FileVersionPath, fileVersion, H5T_NATIVE_FLOAT);
  
  if (err < 0 || ! supportedMXAFileVersion(fileVersion)) {
    std::cout << "Not a supported HDF5 File (wrong version): "  << fileVersion << "  Error: " << err << std::endl;
    return false;
  }

  //std::cout << "Supported FILE TYPE: " << ftype << " Version: " << fileVersion << std::endl;
  return true;
}

// -----------------------------------------------------------------------------
// Returns true if the file referred to by filename is an HDF5
//  compliant file (has the correct file type and version)
//TODO: Clean this up
bool H5IODelegate::isMXAFile(std::string filename)
{
  hid_t fileId = openMXAFile(filename, true);
  if (fileId < 0) {
    return false;
  }
  closeMXAFile();
  return true;
}


// -----------------------------------------------------------------------------
// Opens and returns an hdf file identifier
//TODO: Clean this up
hid_t H5IODelegate::openMXAFile(std::string filename, bool readOnly)
{
  // Suspend the HDF Error Handlers
  herr_t (*_oldHDF_error_func)(void *);
  void *_oldHDF_error_client_data;

  // Store the current HDF error handlers
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);

  // Turn off error handling
  H5Eset_auto(NULL, NULL);

  if (readOnly) {
    _fileId = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  } else {
    _fileId = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
  }

  // Turn the HDF Error handlers back on
  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);

  if (_fileId < 0) {
    return _fileId;
  }

  //TODO: Inline the File Type and File Version Checks.
  //TODO: Sanity Check the DataModel for FileType/Version and Data Model Groups and subgroups.
  // Make sure this is an HDF5 file
  if (! isMXAFile(_fileId)) {
    closeMXAFile();
    return (hid_t) -1;
  } 
  this->_openFile = filename;
  return _fileId;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
hid_t H5IODelegate::createMXAFile(std::string filename)
{
  return H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5IODelegate::closeMXAFile() 
{
  if (_fileId <= 0) {  // fileId isn't open
    return;
  }
  
  // Get the number of open identifiers of all types
  //  except files
  int num_open = H5Fget_obj_count(_fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |
			      H5F_OBJ_DATATYPE | H5F_OBJ_ATTR);
  if (num_open > 0) {
    std::cout << "WARNING: Some IDs weren't closed. Closing them."  << std::endl;
 //   hid_t *attr_ids;
//    attr_ids = (hid_t *)malloc(num_open * sizeof(hid_t));
    std::vector<hid_t> attr_ids(num_open, 0);
    H5Fget_obj_ids(_fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |
		   H5F_OBJ_DATATYPE | H5F_OBJ_ATTR, 
		   num_open, &(attr_ids.front()) );
    for (int i=0; i<num_open; i++) {
      
      H5Utilities::closeHDF5Object(attr_ids[i]);
    }

  }
    
  herr_t err = H5Fclose(_fileId);
  if (err < 0) {
    std::cout << "H5IODelegate::closeMXAFile(): H5Fclose() caused error " << err << std::endl;
  }
  this->_openFile = "";
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5IODelegate::createGroupsFromPath(std::string path, hid_t parent)
{
  
  hid_t gid = 1;
  herr_t err = -1;
  std::string first;
  std::string second;
  
  if (parent <= 0) {
    std::cout << "Bad parent Id. Returning from createGroupsFromPath" << std::endl;
    return -1;
  }
  // remove any front slash
  uint32 pos = path.find_first_of("/", 0);
  if ( 0 == pos ) 
  {
    path = path.substr(1, path.size());
  } else if (pos == std::string::npos) // Path contains only one element
  {
    gid = _createGroup(parent, path);
    if (gid < 0)
    {
      std::cout << "Error creating group: " << path << " err:" << gid << std::endl;
      return gid;
    }
    err = H5Gclose(gid);
    if (err < 0) { std::cout << logTime() << "Error closing group during group creation." << std::endl; return err; }
    return err; //Now return here as this was a special case.
  }
  
  //Remove any trailing slash
  pos = path.find_last_of("/");
  if ( pos == (path.size() - 1) ) // slash was in the last position
  {
    path = path.substr(0, pos);
  }
  
  if (path.size() == 0) 
  {
    return -1; // The path that was passed in was only a slash.. 
  }

  
  pos = path.find_first_of("/", 0);
  while (pos != std::string::npos )
  {
    first = path.substr(0, pos);
    second = path.substr(pos+1, path.length() );
    gid = _createGroup(parent,first);
    if (gid < 0)
    {
      std::cout << "Error creating group:" << gid << std::endl;
      return gid;
    }
    err = H5Gclose(gid);
    pos = path.find_first_of("/", pos+1);
    if (pos == std::string::npos) 
    {
      first += "/" + second;
      gid = _createGroup(parent,first);
      if (gid < 0)
      {
        std::cout << "Error creating group:" << gid << std::endl;
        return gid;
      }
      err = H5Gclose(gid);
    }
  }
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
hid_t H5IODelegate::_createGroup(hid_t loc_id, std::string group) 
{
  hid_t grp_id;
  herr_t err = -1;

  // Suspend the HDF Error Handlers
  herr_t (*_oldHDF_error_func)(void *);
  void *_oldHDF_error_client_data;

  // Store the current HDF error handlers
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);

  // Turn off error handling
  H5Eset_auto(NULL, NULL);

  err = H5Gget_objinfo(loc_id, group.c_str(), 0, NULL);
//  std::cout << "H5Gget_objinfo = " << err << " for " << group << std::endl;  
  if (err == 0)
  {
    grp_id = H5Gopen(loc_id, group.c_str());
  } else 
  {
    grp_id = H5Gcreate(loc_id, group.c_str(), 0);
  }
  // Turn the HDF Error handlers back on
  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);
  return grp_id;
}
