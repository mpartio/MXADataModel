/*
 * MXASupportFile.h
 *
 *  Created on: Sep 23, 2008
 *      Author: mjackson
 */

#ifndef MXASUPPORTFILE_H_
#define MXASUPPORTFILE_H_

#include <Common/MXATypeDefs.h>
#include <Base/ISupportFile.h>

#include <string>

#include <hdf5.h>

/**
* @class MXASupportFile MXASupportFile.h src/Core/MXASupportFile.h
* @brief
* @author  mjackson
* @date Sep 23, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT MXASupportFile: public ISupportFile
{
  public:
    
    virtual ~MXASupportFile();

    static ISupportFilePtr NewFromFileSystem( const std::string &filesystempath, 
                                              const std::string &filetype,
                                              bool cacheFile = false);


    static ISupportFilePtr NewFromMXAFile(hid_t dataFile, 
                                               const std::string &index,
                                               bool cacheFile);


    virtual void setFileSystemPath(const std::string &filesystemPath);
    virtual std::string getFileSystemPath();

    virtual void setFileType(const std::string &fileType);
    virtual std::string getFileType();

    virtual uint8*  getFilePointer(uint64 offset = 0);
    virtual uint64  getFileSize();
    virtual bool isFileCached();


    virtual int32 readFromFileSystem();
    virtual void flushCachedFile();
 
      
    virtual void setIndex(int index);
    virtual int getIndex();
    virtual int32 readFromMXAFile();


  protected:
    MXASupportFile();
    MXASupportFile(hid_t datafile, int index);

  private:
    std::string  _filesystemPath;
    std::string  _fileType;
    IMXAArrayPtr _fileContents;
    int          _index;
    hid_t        _fileId;

    MXASupportFile(const MXASupportFile&);    // Copy Constructor Not Implemented
    void operator=(const MXASupportFile&);  // Operator '=' Not Implemented
};

#endif /* MXASUPPORTFILE_H_ */
