///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ISUPPORTFILE_H_
#define ISUPPORTFILE_H_

#include <string>

//#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Core/MXAConstants.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>


namespace SupportFile
{
  namespace FileType
  {
    const std::string Text (MXA::MXA_TEXT_TAG);
    const std::string Binary (MXA::MXA_BINARY_TAG);
  }

}


/**
* @class ISupportFile ISupportFile.h src/Base/ISupportFile.h
* @brief Formal Interface for managing arbitrary files that will be added to the HDF5 file.
* @author  Mike Jackson for BlueQuartz Software
* @date Sep 23, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT ISupportFile
{
public:

  MXA_SHARED_POINTERS(ISupportFile);
  typedef std::vector<ISupportFile::Pointer> Container;

  ISupportFile(){};
  virtual ~ISupportFile(){};

  /**
   * @brief Sets the path to the file on the local filesystem.
   * @param filesystemPath The path to the file on the local filesystem
   */
	virtual void setFileSystemPath(const std::string &filesystemPath) = 0;

	/**
	 * @brief Returns the path to the file on the local filesystem.
	 */
  virtual std::string getFileSystemPath() = 0;

  /**
   * @brief Sets the File type as a hint for viewers on how to render the file if
   * possible. The default is "Binary". You may also use "Text".
   * @param fileType "Binary" or "Text"
   */
  virtual void setFileType(const std::string &fileType) = 0;

  /**
   * @brief Returns the file type
   */
  virtual std::string getFileType() = 0;

  /**
   * @brief Returns a pointer to the specified offset in the cached file. Note that
   * if the file has NOT been read then this will return a NULL pointer.
   * @param offset The offset into the cached file to read.
   * @return Pointer to the data at the specified offset.
   */
	virtual uint8_t*  getFilePointer(uint64_t offset) = 0;

	/**
	 * @brief Returns the size of the file which could be Zero if the file has NOT been read
	 * or can not be read.
	 * @return Size of the file.
	 */
  virtual uint64_t  getFileSize() = 0;

  /**
   * @brief Has the file been read into memory.
   * @return
   */
  virtual bool isFileCached() = 0;

  /**
   * @brief If the file is residing in memory, free the memory by unloading the file
   * from memory
   */
  virtual void flushCachedFile() = 0;

  /**
   * @brief Read from the filesystem and cache the file into memory. Careful when
   * doing this with large files.
   * @return Error code. Negative value indicates error.
   */
 	virtual int32_t readFromFileSystem() = 0;

 	/**
 	 * @brief Sets the internal value used by the MXA API to store the file in teh HDF5 file.
 	 * @param index The index to use.
 	 */
  virtual void setIndex(int index) = 0;

  /**
   * @brief Returns the internal index used by the MXA API to store the file in the HDF5 file.
   */
  virtual int getIndex() = 0;

  /**
   * @brief Reads file contents from the HDF5 file into memory. From this point you
   * can write the file out to the local filesystem if needed.
   * @return Error code. Negative is an error.
   */
  virtual int32_t readFromMXAFile() = 0;


protected:


private:
  ISupportFile(const ISupportFile&);    // Copy Constructor Not Implemented
  void operator=(const ISupportFile&);  // Operator '=' Not Implemented

};

#endif /* ISUPPORTFILE_H_ */
