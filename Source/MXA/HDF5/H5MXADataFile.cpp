
#include "H5MXADataFile.h"
#include <MXA/Base/IFileWriter.h>
#include <MXA/Base/IFileReader.h>
#include <MXA/Base/IDataset.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/HDF5/H5DataModelWriter.h>
#include <MXA/HDF5/H5DataModelReader.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/Utilities/MXAFileSystemPath.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataFile::Pointer H5MXADataFile::OpenFile(const std::string &filename, bool readOnly)
{
  H5MXADataFile* dataFile = new H5MXADataFile(filename);
  IDataFile::Pointer filePtr (dynamic_cast<IDataFile*>(dataFile) );
  int32 err = filePtr->openFile(readOnly);
  if (err < 0)
  { // Something went wrong - Return a null wrapped pointer
    H5MXADataFile* nullDataFile = 0x0;
    filePtr.reset(nullDataFile);
  }
  else
  {
    dataFile->_setWeakPointer(filePtr);
  }
  return filePtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataFile::Pointer H5MXADataFile::CreateFileWithModel(const std::string &filename, IDataModel::Pointer model)
{
  H5MXADataFile* dataFile = new H5MXADataFile(filename, model);
  IDataFile::Pointer filePtr (dynamic_cast<IDataFile*>(dataFile) );
  int32 err = filePtr->createFile();
  if (err < 0)
  { // Something went wrong - Return a null wrapped pointer
    std::cout << logTime() << "H5MXADataFile::CreateFileWithModel - Error Creating file.\t[" << filePtr->getFilename() << "]" << std::endl;
    H5MXADataFile* nullDataFile = 0x0;
    filePtr.reset(nullDataFile);
    return filePtr;
  }
  dataFile->_setWeakPointer(filePtr);
  // Write the supplied data model to the data file
  err = filePtr->saveDataModel();
  if (err < 0)
  { // Something went wrong - Return a null wrapped pointer
    std::cout << logTime() << "H5MXADataFile::CreateFileWithModel - Error Writing Data Model to file.\t[" << filePtr->getFilename() << "]" << std::endl;
    H5MXADataFile* nullDataFile = 0x0;
    filePtr.reset(nullDataFile);
  }

  return filePtr;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5MXADataFile::H5MXADataFile(const std::string &filename) :
  IDataFile(filename),
  _filename(filename),
  _fileId(-1),
  _isFileOpen(false),
  _isReadOnly(true)
{
  this->_dataModel = MXADataModel::New();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5MXADataFile::H5MXADataFile(const std::string &filename, IDataModel::Pointer model) :
  IDataFile(filename),
  _filename(filename),
  _fileId(-1),
  _isFileOpen(false),
  _isReadOnly(true)
{
  if (NULL == model.get()) // Model is NUll, so create a new one.
  {
    IDataModel::Pointer modelPtr = MXADataModel::New();
    model.swap(modelPtr);
  }
  this->_dataModel = model;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5MXADataFile::~H5MXADataFile()
{
  // std::cout << logTime() << "H5MXADataFile::~H5MXADataFile()" << std::endl;
  if (this->_isFileOpen == true)
  {
    closeFile(false);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5MXADataFile::_setWeakPointer(boost::weak_ptr<IDataFile> weakPtr)
{
  this->_weakPtr = weakPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataFile::Pointer H5MXADataFile::CreateEmptyFile(const std::string &filename)
{
  IDataModel::Pointer modelPtr = MXADataModel::New();
  IDataFile::Pointer filePtr (new H5MXADataFile(filename, modelPtr));
  int32 err = filePtr->createFile();
  if (err < 0)
  { // Something went wrong - Return a null wrapped pointer
    std::cout << logTime() << "H5MXADataFile::CreateFileWithModel - Error Creating file.\t[" << filePtr->getFilename() << "]" << std::endl;
    H5MXADataFile* nullDataFile = 0x0;
    filePtr.reset(nullDataFile);
  }
  return filePtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string H5MXADataFile::getFilename()
{
  return this->_filename;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataModel::Pointer H5MXADataFile::getDataModel()
{
  return this->_dataModel;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5MXADataFile::createFile()
{
  int32 err = 1;
  if (_fileId > 0)
  {
    err = this->closeFile(false); // Close the file first if it is open.
    if (err < 0)
    {
      std::cout << logTime() << "H5MXADataFile::createFile - Error closing currently open file." << std::endl;
      return err;
    }
  }
#if 1
  //bool didDeleteFile = 1;
  // Now delete the file if it really exists
  if (true == MXAFileSystemPath::exists(this->_filename) )
  {
    static_cast<void>(MXAFileSystemPath::remove(this->_filename) );
    if (true == MXAFileSystemPath::exists(this->_filename) )
    {
      std::cout << "H5IODelegate::writeModelToFile: The file could not be deleted\n  " << this->_filename << std::endl;
      return -20;
    }
  }
  // Model file is NOT open and the filenames do NOT match
  if (_fileId < 0)
  {
    // Named file is not open, try opening it first
    HDF_ERROR_HANDLER_OFF
    //Create the HDF File
    _fileId = H5Fcreate(this->_filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (_fileId < 0)
    {
      std::cout << "Error Creating new MXA File" << std::endl;
      this->_isFileOpen = false;
      HDF_ERROR_HANDLER_ON
      return _fileId;
    }
    HDF_ERROR_HANDLER_ON
    this->_isFileOpen = true;
    this->_isReadOnly = false;
  }
#endif
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5MXADataFile::openFile(bool readOnly)
{
  if (this->_filename.empty() == true)
  {
    std::cout << logTime() << "Error: H5MXADataFile::openFile() - variable _filename is empty but is required to be set." << std::endl;
    return -1;
  }
  this->_isReadOnly = readOnly;

  HDF_ERROR_HANDLER_OFF

  if (this->_isReadOnly) {
    this->_fileId = H5Fopen(this->_filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  } else {
    this->_fileId = H5Fopen(this->_filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
  }

  if (this->_fileId < 0)
  {
    std::cout << logTime() << "The mxa file could not be opened.\t[" << this->_filename << "]" << std::endl;
    HDF_ERROR_HANDLER_ON
    return this->_fileId;
  }

  // Now load the model from the file
  int32 err = this->_readDataModel();
  if (err < 0)
  {
    std::cout << logTime() << "The data model could NOT be loaded from the file.\t[" << this->_filename << "]" << std::endl;
    HDF_ERROR_HANDLER_ON
    return err;
  }

  HDF_ERROR_HANDLER_ON

  this->_isFileOpen = true;
  return _fileId;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5MXADataFile::closeFile(bool saveModel)
{
  herr_t err = 1;
  if (this->_fileId < 0) {  // fileId isn't open
    this->_isFileOpen = false;
    return 1;
  }

  //Save the model first if requested
  if (true == saveModel && false == this->_isReadOnly)
  {
    err = this->saveDataModel();
    if (err < 0)
    {
      std::cout << logTime() << "Error saving model to file.\t[" << this->_filename << "]" << std::endl;
      return err;
    }
  }

  // Get the number of open identifiers of all types
  //  except files
  int32 num_open = H5Fget_obj_count(_fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |
            H5F_OBJ_DATATYPE | H5F_OBJ_ATTR);
  if (num_open > 0) {
    std::cout << "WARNING: Some IDs weren't closed. Closing them."  << std::endl;
    std::vector<hid_t> attr_ids(num_open, 0);
    H5Fget_obj_ids(this->_fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |
       H5F_OBJ_DATATYPE | H5F_OBJ_ATTR,
       num_open, &(attr_ids.front()) );
    for (int i=0; i<num_open; i++)
    {
      H5Utilities::closeHDF5Object(attr_ids[i]);
    }
  }

  err = H5Fclose(_fileId);
  if (err < 0) {
    std::cout << logTime() << "H5IODelegate::closeMXAFile(): H5Fclose() caused error " << err << std::endl;
  }
  this->_fileId = -1;
  this->_filename = "";
  this->_isFileOpen = false;
  this->_isReadOnly = true;
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
float H5MXADataFile::supportedMXAFileVersion()
{
  return MXA::MXACurrentFileVersion;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool H5MXADataFile::isFileOpen()
{
  return this->_isFileOpen;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool H5MXADataFile::isReadOnly()
{
 return this->_isReadOnly;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5MXADataFile::saveDataModel()
{
  return this->_writeDataModel();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5MXADataFile::_writeDataModel()
{
  H5DataModelWriter writer( this->_dataModel, this->_weakPtr.lock() );
  return writer.writeModelToFile(_fileId);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5MXADataFile::_readDataModel()
{
  H5DataModelReader reader(this->_dataModel);
  return reader.readDataModel(this->_fileId);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5MXADataFile::writeData(const IDataset::Pointer dataset)
{
  return dataset->writeToFile(this->_weakPtr.lock() );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5MXADataFile::readData(const IDataset::Pointer dataset)
{
  return dataset->writeToFile(this->_weakPtr.lock() );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
hid_t H5MXADataFile::getFileId()
{
  return this->_fileId;
}
