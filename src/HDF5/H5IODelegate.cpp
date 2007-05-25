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
    std::cout << "H5IODelegate::~H5IODelegate()  Closing file.." << std::endl;   
    this->closeMXAFile();
  }
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXATypes::MXAError H5IODelegate::writeModelToFile(std::string fileName, MXADataModel* model, bool closeWhenFinished)
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
MXATypes::MXAError H5IODelegate::readModelFromFile(std::string fileName, MXADataModel* model, bool closeWhenFinished)
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
  err = H5Lite::readStringDataset(fileId, const_cast<std::string&>(MXA::FileTypePath), ftype);
  //std::cout << logTime() << "Err: " << err << std::endl;
  if ( (ftype != MXA::MXACurrentFileType) ) {
    std::cout << "NOT an MXA formatted HDF5 File. Type should be " << MXA::MXACurrentFileType << " but I found:" << ftype << std::endl;
    return false;
  }
  if (err < 0)
  {
    std::cout << logTime() << "H5IODelegate::isMXAFile: Error Getting the FileType. Look for other log messages" << err << std::endl;
    return false;
  }
  // Check the Version
  float32 fileVersion;
  err = H5Lite::readScalarDataset(fileId, MXA::FileVersionPath, fileVersion);
  
  if (! supportedMXAFileVersion(fileVersion)) {
    std::cout << "Not a supported MXA Formatted HDF5 File. File Version should be " << MXA::MXACurrentFileVersion << " but I found: "  << fileVersion << std::endl;
    return false;
  }
  if (err < 0)
  {
    std::cout << logTime() << "H5IODelegate::isMXAFile: Error Getting the File Version. Look for other log messages" << err << std::endl;
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
  //std::cout << logTime() << "H5IODelegate::closeMXAFile  _fileId: " << _fileId << std::endl;
  if (_fileId < 0) {  // fileId isn't open
    return;
  }
  
  // Get the number of open identifiers of all types
  //  except files
  int32 num_open = H5Fget_obj_count(_fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |
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
  this->_fileId = -1;
  this->_openFile = "";
}



