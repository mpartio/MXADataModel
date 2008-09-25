///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ISUPPORTFILE_H_
#define ISUPPORTFILE_H_

#include <string>

#include <Common/MXATypeDefs.h>
#include <Core/MXAConstants.h>
#include <DataWrappers/MXAArrayTemplate.hpp>


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
* @brief
* @author  mjackson
* @date Sep 23, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT ISupportFile
{
public:
  ISupportFile(){};
  virtual ~ISupportFile(){};

	virtual void setFileSystemPath(const std::string &filesystemPath) = 0;
	
  virtual std::string getFileSystemPath() = 0;

  virtual void setFileType(const std::string &fileType) = 0;
  virtual std::string getFileType() = 0;

	virtual uint8*  getFilePointer(uint64 offset) = 0;
  virtual uint64  getFileSize() = 0;
  virtual bool isFileCached() = 0;
  virtual void flushCachedFile() = 0;
 	virtual int32 readFromFileSystem() = 0;

  virtual void setIndex(int index) = 0;
  virtual int getIndex() = 0;
  virtual int32 readFromMXAFile() = 0;


protected:


private:
  ISupportFile(const ISupportFile&);    // Copy Constructor Not Implemented
  void operator=(const ISupportFile&);  // Operator '=' Not Implemented

};

#endif /* ISUPPORTFILE_H_ */
