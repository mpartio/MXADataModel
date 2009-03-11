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
class MXA_EXPORT MXAFileSystemPath
{
  public:
#if __WIN32
    static const char Separator = '\\';
#else
    static const char Separator = '/';
#endif

    static bool isDirectory(const std::string &path);
    static bool isFile(const std::string &path);
    static bool exists(const std::string &path);
    static std::string extension(const std::string &path);
    static std::string filename(const std::string &path);

    static bool mkdir(const std::string &name, bool createParentDirectories);
    static bool rmdir(const std::string &name, bool recurseParentDirectories);

    static bool remove(const std::string &path);

    static std::string cleanPath(const std::string &path);

    static std::string fromNativeSeparators(const std::string  &pathName);
    static std::string toNativeSeparators(const std::string &pathName);


  protected:
     MXAFileSystemPath();
    virtual ~MXAFileSystemPath();

  private:

      MXAFileSystemPath(const MXAFileSystemPath&);    //Not Implemented
      void operator=(const MXAFileSystemPath&); //Not Implemented

};

#endif //_MXAFileSystemPath_h_



