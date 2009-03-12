///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#include "MXASupportFile.h"
#include <MXA/Common/IO/MXAFileReader64.h>
#include <MXA/Core/MXAConstants.h>
#if MXA_HDF5_SUPPORT
#include <MXA/HDF5/H5MXADataFile.h>
#endif
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/Utilities/MXAFileSystemPath.h>

//-- Boost Headers
#include <boost/iostreams/device/file.hpp>

//-- C++ Includes
#include <iostream>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFilePtr MXASupportFile::NewFromFileSystem(const std::string &filesystempath,
                                                  const std::string &filetype,
                                                  bool cacheFile)
{
  ISupportFile* sp = new MXASupportFile();
  ISupportFilePtr spPtr(sp);
  sp->setFileSystemPath(filesystempath);
  sp->setFileType(filetype);
  if (cacheFile)
  {
    if (sp->readFromFileSystem() < 0 )  // the read failed
    {
      ISupportFilePtr nullPtr;
      spPtr.swap(nullPtr);
    }
  }

  return spPtr;
}

#if MXA_HDF5_SUPPORT
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFilePtr MXASupportFile::NewFromMXAFile(hid_t dataFile,
                                               const std::string &index,
                                               bool cacheFile)
{
  int i = 0;
  StringUtils::stringToNum<int>(i, index);
  ISupportFile* sp = new MXASupportFile(dataFile, i);
  ISupportFilePtr spPtr(sp);
  sp->setIndex(i);
  if (cacheFile)
  {
    if (sp->readFromMXAFile() < 0)
    {
      ISupportFilePtr nullPtr;
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
ISupportFilePtr MXASupportFile::NewFromXMLModel(const std::string &filesystempath,
                                       const std::string &filetype,
                                       int index)
{

    ISupportFilePtr spPtr(new MXASupportFile());
    spPtr->setFileSystemPath(filesystempath);
    spPtr->setFileType(filetype);
    spPtr->setIndex(index);
    return spPtr;
}
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ISupportFilePtr MXASupportFile::New()
{
  ISupportFilePtr spPtr(new MXASupportFile());
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
#if MXA_HDF5_SUPPORT
MXASupportFile::MXASupportFile(hid_t fileId, int index) :
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
  // TODO Auto-generated destructor stub
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
int32 MXASupportFile::readFromMXAFile()
{

  int32 err = -1;
#if MXA_HDF5_SUPPORT
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
uint8*  MXASupportFile::getFilePointer(uint64 offset)
{
  uint8* ptr = NULL;
  if ( NULL != this->_fileContents.get() )
  {
    ptr = static_cast<uint8*>(_fileContents->getVoidPointer(offset));
  }
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint64 MXASupportFile::getFileSize()
{
  if ( NULL != this->_fileContents.get() )
  {
    return _fileContents->getNumberOfElements();
  }

  if (this->_fileId > 0)
  {
#if MXA_HDF5_SUPPORT
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
    return (uint64)(fileSize);
#endif
  }

  std::string p = MXAFileSystemPath::toNativeSeparators(this->_filesystemPath);
  //FileSystem::path p( this->_filesystemPath, FileSystem::native );

  // Make sure the file exists
  if ( !MXAFileSystemPath::exists( p ) )
    {
      std::cout << "Error at " << __FILE__ << "(" << __LINE__ << ")  ";
      std::cout << "File '" << this->_filesystemPath << "' not found." << std::endl;
      return 0;
    }

  // Check to make sure the file is regular
  if ( !MXAFileSystemPath::isFile( p ) )
    {
      std::cout << "Error at " << __FILE__ << "(" << __LINE__ << ")  ";
      std::cout << "File '" << this->_filesystemPath << "' not a regular file." << std::endl;
      return 0;
    }

    return MXAFileSystemPath::fileSize(p);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MXASupportFile::flushCachedFile()
{
  IMXAArrayPtr nullPtr;
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
int32 MXASupportFile::readFromFileSystem()
{
  int32 err = 0;

  std::string p = MXAFileSystemPath::toNativeSeparators(this->_filesystemPath);

  // Make sure the file exists
  if ( !MXAFileSystemPath::exists( p ) )
    {
      std::cout << this->_filesystemPath << " not found." << std::endl;
      return -1;
    }

  // Check to make sure the file is regular
  if ( !MXAFileSystemPath::isFile( p ) )
    {
      std::cout << this->_filesystemPath << "not a regular file." << std::endl;
      return -1;
    }

   // std::cout << "size of " << argv[1] << " is " << fs::file_size( p )  << std::endl;
    uint64 fileSize = MXAFileSystemPath::fileSize(p);

    boost::iostreams::file_source sourceFile (p, BOOST_IOS::in);
    if (sourceFile.is_open() == true )
    {
      _fileContents = MXAArrayTemplate<char>::CreateArray(fileSize);
      char* front = (char*)(_fileContents->getVoidPointer(0));
      sourceFile.read(front, fileSize);
      sourceFile.close();
    }

  return err;
}
