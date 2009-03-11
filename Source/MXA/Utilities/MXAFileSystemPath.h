///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _MXAFileSystemPath_h_
#define _MXAFileSystemPath_h_

#include <MXA/Common/MXATypes.h>
#include <string>



/**
* @class MXAFileSystemPath MXAFileSystemPath.h PathToHeader/MXAFileSystemPath.h
* @brief
* @author mjackson
* @date Mar 10, 2009
* @version $Revision$
*/
class MXAFileSystemPath
{
  public:
#if _WIN32
    static MXA_EXPORT const char Separator = '\\';
#else
    static MXA_EXPORT const char Separator = '/';
#endif

    static MXA_EXPORT bool isDirectory(const std::string &path);
    static MXA_EXPORT bool isFile(const std::string &path);
    static MXA_EXPORT bool exists(const std::string &path);
    static MXA_EXPORT std::string extension(const std::string &path);
    static MXA_EXPORT std::string filename(const std::string &path);

    static MXA_EXPORT bool mkdir(const std::string &name, bool createParentDirectories);
    static MXA_EXPORT bool rmdir(const std::string &name, bool recurseParentDirectories);

    static MXA_EXPORT bool remove(const std::string &path);

    static MXA_EXPORT std::string cleanPath(const std::string &path);

    static MXA_EXPORT std::string fromNativeSeparators(const std::string  &pathName);
    static MXA_EXPORT std::string toNativeSeparators(const std::string &pathName);

    static MXA_EXPORT bool isDirPath(const std::string &dirPath, bool *existed);

  protected:
     MXAFileSystemPath();
    virtual ~MXAFileSystemPath();

  private:

      MXAFileSystemPath(const MXAFileSystemPath&);    //Not Implemented
      void operator=(const MXAFileSystemPath&); //Not Implemented

};

#endif //_MXAFileSystemPath_h_



