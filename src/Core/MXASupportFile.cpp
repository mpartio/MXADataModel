///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#include "MXASupportFile.h"
#include <Common/IO/Reader64.h>
#include <Core/MXAConstants.h>
#include <HDF5/H5MXADataFile.h>
#include <DataWrappers/MXAArrayTemplate.hpp>

#include <boost/iostreams/device/file.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>

//--- Convenience code -----------------
typedef boost::filesystem::path FilePath;
namespace FileSystem = boost::filesystem;


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
  return spPtr;
}

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
_index (0)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXASupportFile::MXASupportFile(hid_t fileId, int index) :
_index(index),
_fileId(fileId)
{
}

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


  FileSystem::path p( this->_filesystemPath, FileSystem::native );

  // Make sure the file exists
  if ( !FileSystem::exists( p ) )
    {
      std::cout << "Error at " << __FILE__ << "(" << __LINE__ << ")  ";
      std::cout << this->_filesystemPath << " not found." << std::endl;
      return 0;
    }

  // Check to make sure the file is regular
  if ( !FileSystem::is_regular( p ) )
    {
      std::cout << "Error at " << __FILE__ << "(" << __LINE__ << ")  ";
      std::cout << this->_filesystemPath << "not a regular file." << std::endl;
      return 0;
    }

    return FileSystem::file_size(p);
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

  FileSystem::path p( this->_filesystemPath, FileSystem::native );

  // Make sure the file exists
  if ( !FileSystem::exists( p ) )
    {
      std::cout << this->_filesystemPath << " not found." << std::endl;
      return -1;
    }

  // Check to make sure the file is regular
  if ( !FileSystem::is_regular( p ) )
    {
      std::cout << this->_filesystemPath << "not a regular file." << std::endl;
      return -1;
    }

   // std::cout << "size of " << argv[1] << " is " << fs::file_size( p )  << std::endl;
    boost::intmax_t fileSize = FileSystem::file_size(p);

    boost::iostreams::file_source sourceFile (this->_filesystemPath, BOOST_IOS::in);
    if (sourceFile.is_open() == true )
    {
      _fileContents = MXAArrayTemplate<char>::CreateArray(fileSize);
      char* front = (char*)(_fileContents->getVoidPointer(0));
      sourceFile.read(front, fileSize);
      sourceFile.close();
    }

  return err;
}

#if 0
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 MXASupportFile::writeToFileSystem()
{
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 MXASupportFile::readFromMXAFile()
{

 // this->_fileContents = MXAArrayTemplate<uint8>::CreateArray(989800);
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32 MXASupportFile::writeToMXAFile()
{
  return -1;
}

#endif
