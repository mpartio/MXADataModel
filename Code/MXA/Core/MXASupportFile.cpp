///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#include "MXASupportFile.h"
#include <MXA/Common/IO/MXAFileReader64.h>
#include <MXA/Core/MXAConstants.h>
#ifdef MXA_HDF5_SUPPORT
#include <MXA/HDF5/H5MXADataFile.h>
#endif
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/Utilities/MXAFileInfo.h>

//-- Boost Headers
//#include <boost/iostreams/device/file.hpp>

//-- C++ Includes
#include <iostream>
#include <fstream>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFile::Pointer MXASupportFile::NewFromFileSystem(const std::string &filesystempath,
                                                  const std::string &filetype,
                                                  bool cacheFile)
{
  ISupportFile* sp = new MXASupportFile();
  ISupportFile::Pointer spPtr(sp);
  sp->setFileSystemPath(filesystempath);
  sp->setFileType(filetype);
  if (cacheFile)
  {
    if (sp->readFromFileSystem() < 0 )  // the read failed
    {
      ISupportFile::Pointer nullPtr;
      spPtr.swap(nullPtr);
    }
  }

  return spPtr;
}

#ifdef MXA_HDF5_SUPPORT
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFile::Pointer MXASupportFile::NewFromMXAFile(int32_t dataFile,
                                               const std::string &index,
                                               bool cacheFile)
{
  int i = 0;
  StringUtils::stringToNum<int>(i, index);
  ISupportFile* sp = new MXASupportFile(dataFile, i);
  ISupportFile::Pointer spPtr(sp);
  sp->setIndex(i);
  if (cacheFile)
  {
    if (sp->readFromMXAFile() < 0)
    {
      ISupportFile::Pointer nullPtr;
      spPtr.swap(nullPtr);
    }
  }

  {

  }
  return spPtr;
}
#endif

#if 0
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFile::Pointer MXASupportFile::NewFromXMLModel(const std::string &filesystempath,
                                       const std::string &filetype,
                                       int index)
{

    ISupportFile::Pointer spPtr(new MXASupportFile());
    spPtr->setFileSystemPath(filesystempath);
    spPtr->setFileType(filetype);
    spPtr->setIndex(index);
    return spPtr;
}
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFile::Pointer MXASupportFile::New()
{
  ISupportFile::Pointer spPtr(new MXASupportFile());
  return spPtr;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXASupportFile::MXASupportFile() :
_index (0),
_fileId(0)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
#ifdef MXA_HDF5_SUPPORT
MXASupportFile::MXASupportFile(int32_t fileId, int index) :
_index(index),
_fileId(fileId)
{
}
#endif
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXASupportFile::~MXASupportFile()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXASupportFile::setIndex(int index)
{
  this->_index = index;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

int MXASupportFile::getIndex()
{
  return this->_index;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXASupportFile::readFromMXAFile()
{

  int32_t err = -1;
#ifdef MXA_HDF5_SUPPORT
  hid_t fileId = this->_fileId;
  std::string dsetpath = MXA::SupportFilesPath + "/" + StringUtils::numToString(this->_index);
  // Read the contents of the file into an array
  IMXAArray* contents = H5Lite::readMXAArray(fileId, dsetpath);
  if (NULL != contents)
  {
    err = 0;
    this->_fileContents.reset(contents);
  }

  err = H5Lite::readStringAttribute(fileId, dsetpath, MXA::MXA_FILESYSTEM_PATH_TAG, this->_filesystemPath);
  if (err < 0)
  {
    std::cout << "MXASupportFile could not read attribute '" << MXA::MXA_FILESYSTEM_PATH_TAG <<
      "' for Support file with index '" << this->_index << "'" << std::endl;
    return err;
  }

  err = H5Lite::readStringAttribute(fileId, dsetpath, MXA::MXA_FILETYPE_TAG, this->_fileType);
  if (err < 0)
  {
    std::cout << "MXASupportFile could not read attribute '" << MXA::MXA_FILETYPE_TAG <<
      "' for Support file with index '" << this->_index << "'" << std::endl;
    return err;
  }
#endif
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXASupportFile::setFileSystemPath(const std::string &filesystemPath)
{
  // Check if the file path is different, if it is different, then flush the file cache
  if (this->_filesystemPath.compare(filesystemPath) != 0)
  {
    this->flushCachedFile();
  }
  this->_filesystemPath = filesystemPath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXASupportFile::getFileSystemPath()
{
  return this->_filesystemPath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXASupportFile::setFileType(const std::string &fileType)
{
  this->_fileType = fileType;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXASupportFile::getFileType()
{
  return this->_fileType;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint8_t*  MXASupportFile::getFilePointer(uint64_t offset)
{
  uint8_t* ptr = NULL;
  if ( NULL != this->_fileContents.get() )
  {
    ptr = static_cast<uint8_t*>(_fileContents->getVoidPointer(offset));
  }
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint64_t MXASupportFile::getFileSize()
{
  if ( NULL != this->_fileContents.get() )
  {
    return _fileContents->getNumberOfElements();
  }

  if (this->_fileId > 0)
  {
#ifdef MXA_HDF5_SUPPORT
    std::vector<hsize_t> dims;
    H5T_class_t type_class;
    size_t type_size;
    std::string dsetpath = MXA::SupportFilesPath + "/" + StringUtils::numToString(this->_index);
    herr_t err = H5Lite::getDatasetInfo(this->_fileId, dsetpath, dims, type_class, type_size);
    if (err < 0)
    {
      return 0;
    }
    hsize_t fileSize = dims[0];
    return (uint64_t)(fileSize);
#endif
  }

  std::string p = MXAFileInfo::toNativeSeparators(this->_filesystemPath);
  // Make sure the file exists
  if ( !MXAFileInfo::exists( p ) )
    {
      std::cout << "Error at " << __FILE__ << "(" << __LINE__ << ")  ";
      std::cout << "File '" << this->_filesystemPath << "' not found." << std::endl;
      return 0;
    }

  // Check to make sure the file is regular
  if ( !MXAFileInfo::isFile( p ) )
    {
      std::cout << "Error at " << __FILE__ << "(" << __LINE__ << ")  ";
      std::cout << "File '" << this->_filesystemPath << "' not a regular file." << std::endl;
      return 0;
    }

    return MXAFileInfo::fileSize(p);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXASupportFile::flushCachedFile()
{
  IMXAArray::Pointer nullPtr;
  this->_fileContents.swap(nullPtr);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXASupportFile::isFileCached()
{
  if (NULL == this->_fileContents.get() ) return false;
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXASupportFile::readFromFileSystem()
{
  int32_t err = 0;

  std::string p = MXAFileInfo::toNativeSeparators(this->_filesystemPath);

  // Make sure the file exists
  if ( !MXAFileInfo::exists( p ) )
    {
      std::cout << this->_filesystemPath << " not found." << std::endl;
      return -1;
    }

  // Check to make sure the file is regular
  if ( !MXAFileInfo::isFile( p ) )
    {
      std::cout << this->_filesystemPath << "not a regular file." << std::endl;
      return -1;
    }

   // std::cout << "size of " << argv[1] << " is " << fs::file_size( p )  << std::endl;
    uint64_t fileSize = MXAFileInfo::fileSize(p);

  //  boost::iostreams::file_source sourceFile (p, BOOST_IOS::in);
		std::ifstream sourceFile(p.c_str(), std::ifstream::in | std::ifstream::binary );
    if (sourceFile.is_open() == true )
    {
      _fileContents = MXAArrayTemplate<char>::CreateArray(fileSize);
      char* front = (char*)(_fileContents->getVoidPointer(0));
      sourceFile.read(front, fileSize);
      sourceFile.close();
    }

  return err;
}
