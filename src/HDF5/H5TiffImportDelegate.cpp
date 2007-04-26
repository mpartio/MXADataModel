

#include "H5TiffImportDelegate.h"

//-- MXA Includes
#include "Headers/MXAErrorDefinitions.h"
#include "Headers/LogTime.h"
#include "Interfaces/IFileIODelegate.h"
#include "MXADataModel/MXADataModel.h"
#include "MXADataModel/MXADataSource.h"
#include "HDF5/H5Lite.h"
#include "HDF5/H5Utilities.h"
#include "HDF5/H5TiffIO.h"

//-- STL Includes
#include <iostream>

//-- HDF5 Includes
#include <hdf5.h>

//-- Boost Filesystem Headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

// -----------------------------------------------------------------------------
//  Convenience macros
// -----------------------------------------------------------------------------
#define CONST_CAST_STRING(arg)\
  const_cast<std::string&>(arg)

#define CPP_CHECK_ATTRIBUTE_ADDITION(ds, attr, value) \
if ( err < 0 ) {\
  std::cout << logTime() << "Error adding attribute " << attr << " with value " << value \
  <<   " to data set " << ds << std::endl; } 

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5TiffImportDelegate::H5TiffImportDelegate() :
  _modelPtr(),
  _fileNotFoundIsError(true),
  _importAsGrayScale(false)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5TiffImportDelegate::~H5TiffImportDelegate()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5TiffImportDelegate::setFileNotFoundIsError(bool value)
{
  this->_fileNotFoundIsError = value;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5TiffImportDelegate::setImportAsGrayScale(bool value)
{
  this->_importAsGrayScale = value;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5TiffImportDelegate::setDataModel(MXADataModelPtr model) {
  this->_modelPtr = model; 
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 H5TiffImportDelegate::importDataSource(MXADataSourcePtr dataSource, MXADataModelPtr model)
{
  herr_t err = -1;
  // Make sure the file Exists first before we go much further
  FileSystem::path sourcePath ( dataSource->getSourcePath() ); //create a boost::filesystem::path object
  if ( !FileSystem::exists(sourcePath) )
  {
    std::cout << logTime() << "Error: Tiff image not found: " << sourcePath.native_file_string() << std::endl;
    if (_fileNotFoundIsError)
    {
      return MXA_ERROR_FILE_NOT_FOUND;
    }
    else 
    {
      return 1;
    }
  }
  
  //Check for valid TIFF file extension - tif or tiff. 
  std::string fileExt( boost::filesystem::extension(sourcePath) );
  std::transform ( fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower );
  if ( fileExt != TiffIOConst::TiffExtension )
  {
    if (fileExt != TiffIOConst::TifExtension ) 
    {
      std::cout << logTime() << "Only import of Tiff images with extensions of 'tiff' or 'tif' are supported." << std::endl;
      return MXA_ERROR_IMAGE_FORMAT_NOT_SUPPORTED; 
    }
  }
  
  //Basic Checks are OK. Now Import the Tiff image into the HDF5 File
  std::cout << logTime() << "Importing Tiff Image: " << dataSource->getSourcePath() << std::endl;
  // Close then reopen the file to get around an HDF5 performance issue
  std::string filename = model->getIODelegate()->getOpenFileName();
  model->getIODelegate()->closeMXAFile(); //Close the file
  model->getIODelegate()->openMXAFile(filename, false); // Reopen the file
  hid_t fileId = model->getIODelegate()->getOpenFileId();
  if (fileId < 0) 
  {
    std::cout << "FileId was not Valid [" << fileId << "]. Data was NOT imported" << std::endl;
    return -1;
  }
   
  // Generate the internal HDF dataset path and create all the groups necessary to write the dataset
  uint32 pos = 0;
  std::string datasetPath( dataSource->generateInternalPath() );;
  pos = datasetPath.find_last_of("/");
  std::string parentPath ( datasetPath.substr(0, pos)  );
  
  //Make sure all the intermediary groups are in place in the HDF5 File
  err = H5Utilities::createGroupsFromPath(parentPath, fileId);
  if (err < 0)
  {
    std::cout << logTime() << "Error: Could not create HDF5 Group paths for path: " << parentPath << std::endl;
    return -1;
  }
  
  //Read the Tiff and Import it into the HDF5 File
  H5TiffIO tiffIO(fileId);
  err = tiffIO.importTiff(dataSource->getSourcePath(), fileId, datasetPath, this->_importAsGrayScale);
  if (err < 0) 
  {
    std::cout << logTime() << "Error Importing Tiff Image: " << dataSource->getSourcePath() << std::endl;
    return -1;
  }
  
  err = H5Lite::writeAttributeStr(fileId, datasetPath, CONST_CAST_STRING(MXA::FieldVariable), CONST_CAST_STRING(MXA::DataTypeImage) );
  CPP_CHECK_ATTRIBUTE_ADDITION(datasetPath, FIELD_VARIABLE, DATA_TYPE_IMAGE);
  
  return 1;
}



