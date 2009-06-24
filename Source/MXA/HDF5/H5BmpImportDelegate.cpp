#include <MXA/HDF5/H5BmpImportDelegate.h>

//-- MXA Includes
#include <MXA/Common/MXAErrorDefinitions.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Utilities/MXAFileSystemPath.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXADataSource.h>
#include <MXA/BMPIO/MXABmpIO.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/HDF5/H5BmpIO.h>

//-- STL Includes
#include <iostream>

//-- HDF5 Includes
#include <hdf5.h>

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
H5BmpImportDelegate::H5BmpImportDelegate() :
  _modelPtr(),
  _fileNotFoundIsError(true),
  _importAsGrayScale(false)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5BmpImportDelegate::~H5BmpImportDelegate()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5BmpImportDelegate::setProperty(const std::string &key, const std::string &value)
{
  SET_PROPERTY_BODY(H5BmpImport, bool, FileNotFoundIsError, key, value);
  SET_PROPERTY_BODY(H5BmpImport, bool, ImportAsGrayScale, key, value);
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 H5BmpImportDelegate::importDataSource(IDataSource::Pointer dataSource, IDataFile::Pointer dataFile)
{
  //std::cout << "H5BmpImportDelegate::importDataSource:  Importing as grayscale->" << this->_importAsGrayScale << std::endl;
  herr_t err = -1;
  // Make sure the file Exists first before we go much further
  std::string sourcePath =  dataSource->getSourcePath();
  if ( MXAFileSystemPath::exists(sourcePath) == false)
  {
    std::cout << logTime() << "Error: BMP image not found: " << MXAFileSystemPath::toNativeSeparators(sourcePath) << std::endl;
    if (_fileNotFoundIsError)
    {
      return MXA_ERROR_FILE_NOT_FOUND;
    }
    else
    {
      return 1;
    }
  }

  //Check for valid BMP file extension - BMP or BMP.
  std::string fileExt = MXAFileSystemPath::extension(sourcePath);
  std::transform ( fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower );
  if (fileExt != MXA::Bmp::FileExtension )
  {
    std::cout << logTime() << "Only import of BMP images with extensions of 'bmp' are supported." << std::endl;
    return MXA_ERROR_IMAGE_FORMAT_NOT_SUPPORTED;
  }


  //Basic Checks are OK. Now Import the BMP image into the HDF5 File
  std::cout << logTime() << "Importing BMP Image: " << dataSource->getSourcePath() << std::endl;
  hid_t fileId = dataFile->getFileId();
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

  //Read the BMP and Import it into the HDF5 File
  H5BmpIO bmpIO(fileId);
  err = bmpIO.importBmp(dataSource->getSourcePath(), fileId, datasetPath, this->_importAsGrayScale);
  if (err < 0)
  {
    std::cout << logTime() << "Error Importing BMP Image: " << dataSource->getSourcePath() << std::endl;
    return -1;
  }

  err = H5Lite::writeStringAttribute(fileId, datasetPath, CONST_CAST_STRING(MXA::FieldVariable), CONST_CAST_STRING(MXA::DataTypeImage) );
  CPP_CHECK_ATTRIBUTE_ADDITION(datasetPath, FIELD_VARIABLE, DATA_TYPE_IMAGE);

  return 1;
}



