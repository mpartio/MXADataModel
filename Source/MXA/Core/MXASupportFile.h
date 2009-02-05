///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef MXASUPPORTFILE_H_
#define MXASUPPORTFILE_H_

#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Base/ISupportFile.h>

#include <string>
#if MXA_HDF5_SUPPORT
#include <hdf5.h>
#endif

/**
* @class MXASupportFile MXASupportFile.h src/Core/MXASupportFile.h
* @brief This class serves as a container to hold the necessary information
* to read either the meta data about a support file from the HDF5 file or read the
* file from the filesystem.
* @author  Mike Jackson for BlueQuartz Software
* @date Sep 23, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT MXASupportFile: public ISupportFile
{
  public:

    virtual ~MXASupportFile();

    /**
     * @brief Creates a new MXASupportFile object based on the path to a file on
     * the local filesystem.
     * @param filesystempath The path to the file on the local filesystem
     * @param filetype Either "Text" or "Binary"
     * @param cacheFile False by default which means the actual reading of the file
     * is put off until it is explicitly asked for. If you set this argument to "true"
     * then the file is read immediately.
     * @return A boost shared_ptr wrapping an instance of MXASupportFile. If something
     * goes wrong during the initialization then it is possible to get a NULL pointer
     * returned.
     */
    static ISupportFilePtr NewFromFileSystem( const std::string &filesystempath,
                                              const std::string &filetype,
                                              bool cacheFile = false);

    /**
     * @brief Creates a new MXASupportFile object based on the meta data from the HDF5 file
     * @param dataFile The hdf5 id value of an opened HDF5 file
     * @param index The internal index that the MXA model assigned to it.
     * @param cacheFile False by default which means the actual reading of the file
     * is put off until it is explicitly asked for. If you set this argument to "true"
     * then the file is read immediately.
     * @return A boost shared_ptr wrapping an instance of MXASupportFile. If something
     * goes wrong during the initialization then it is possible to get a NULL pointer
     * returned.
     */
#if MXA_HDF5_SUPPORT
    static ISupportFilePtr NewFromMXAFile(int32 dataFile,
                                               const std::string &index,
                                               bool cacheFile);
#endif
    /**
     * @brief Creates a new MXASupportFile object with default values.
     */
    static ISupportFilePtr New();


    /**
     * @brief Sets the path to the file on the local filesystem.
     * @param filesystemPath The path to the file on the local filesystem
     */
    virtual void setFileSystemPath(const std::string &filesystemPath);

    /**
     * @brief Returns the path to the file on the local filesystem.
     */
    virtual std::string getFileSystemPath();

    /**
     * @brief Sets the File type as a hint for viewers on how to render the file if
     * possible. The default is "Binary". You may also use "Text".
     * @param fileType "Binary" or "Text"
     */
    virtual void setFileType(const std::string &fileType);

    /**
     * @brief Returns the file type
     */
    virtual std::string getFileType();

    /**
     * @brief Returns a pointer to the specified offset in the cached file. Note that
     * if the file has NOT been read then this will return a NULL pointer.
     * @param offset The offset into the cached file to read.
     * @return Pointer to the data at the specified offset.
     */
    virtual uint8*  getFilePointer(uint64 offset = 0);

    /**
     * @brief Returns the size of the file which could be Zero if the file has NOT been read
     * or can not be read.
     * @return Size of the file.
     */
    virtual uint64  getFileSize();

    /**
     * @brief Has the file been read into memory.
     * @return
     */
    virtual bool isFileCached();

    /**
     * @brief Read from the filesystem and cache the file into memory. Careful when
     * doing this with large files.
     * @return Error code. Negative value indicates error.
     */
    virtual int32 readFromFileSystem();

    /**
     * @brief If the file is residing in memory, free the memory by unloading the file
     * from memory
     */
    virtual void flushCachedFile();

    /**
     * @brief Sets the internal value used by the MXA API to store the file in teh HDF5 file.
     * @param index The index to use.
     */
    virtual void setIndex(int index);
    /**
     * @brief Returns the internal index used by the MXA API to store the file in the HDF5 file.
     */
    virtual int getIndex();
    /**
     * @brief Reads file contents from the HDF5 file into memory. From this point you
     * can write the file out to the local filesystem if needed.
     * @return Error code. Negative is an error.
     */
    virtual int32 readFromMXAFile();


  protected:
    MXASupportFile();
    MXASupportFile(int32 datafile, int index);

  private:
    std::string  _filesystemPath;
    std::string  _fileType;
    IMXAArrayPtr _fileContents;
    int          _index;
    int32        _fileId;
    int          _source;

    MXASupportFile(const MXASupportFile&);    // Copy Constructor Not Implemented
    void operator=(const MXASupportFile&);  // Operator '=' Not Implemented
};

#endif /* MXASUPPORTFILE_H_ */
