///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
/* Note that some of this implementation was inspired by the Qt source code,
 * in particular the QDir and QFileEngine source codes. The idea is to keep the
 * API the same between my implementation and the Qt Implementation so that
 * switching between then is easy.
 *
 */


#include "MXAFileSystemPath.h"

#include <iostream>
#include <vector>

#include <stdlib.h>
#include <ctype.h>

#if defined (WIN32)
#include <direct.h>
#define UNLINK _unlink
#else
#define UNLINK ::unlink
#endif



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXAFileSystemPath::MXAFileSystemPath()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXAFileSystemPath::~MXAFileSystemPath()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::isDirectory(const std::string &fsPath)
{
#if defined (WIN32)
  bool existed = false;
  return MXAFileSystemPath::isDirPath(fsPath, &existed);
#else
  int error;
  MXA_STATBUF st;
  error = MXA_STAT(fsPath.c_str(), &st);
  if (!error && (st.st_mode & S_IFMT) == S_IFDIR)
  {
    return true;
  }
  return false;
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::isFile(const std::string &fsPath)
{
  int error;
  MXA_STATBUF st;
  error = MXA_STAT(fsPath.c_str(), &st);
  if (!error && (st.st_mode & S_IFMT) == S_IFREG)
  {
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint64 MXAFileSystemPath::fileSize(const std::string &path)
{
  int error;
  MXA_STATBUF st;
  error = MXA_STAT(path.c_str(), &st);
  if (!error && (st.st_mode & S_IFMT) == S_IFREG)
  {
    return (uint64)(st.st_size);
  }
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::exists(const std::string &fsPath)
{
  int error;
  std::string dirName(fsPath);
  // Both windows and OS X both don't like trailing slashes so just get rid of them
  // for all Operating Systems.
  if (dirName[dirName.length() - 1] == MXAFileSystemPath::Separator) {
        dirName = dirName.substr(0, dirName.length() - 1);
  }
  MXA_STATBUF st;
  error = MXA_STAT(dirName.c_str(), &st);
  return (error == 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::extension(const std::string &fsPath)
{
  std::string::size_type pos = fsPath.find_last_of('.');
  std::string::size_type slashPos = fsPath.find_last_of(MXAFileSystemPath::Separator);
  if (pos > 0
      && pos != std::string::npos
      && fsPath[pos-1] != MXAFileSystemPath::Separator
      && pos > slashPos)
  {
    return fsPath.substr(pos + 1);
  }
  return std::string();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::filename(const std::string &fsPath)
{

  std::string::size_type slashPos = fsPath.find_last_of(MXAFileSystemPath::Separator);
  if (slashPos == fsPath.size() - 1)
  {
    return MXAFileSystemPath::filename(fsPath.substr(0, fsPath.size() - 1) );
  }

  std::string fn = fsPath.substr(slashPos + 1, fsPath.size() - slashPos);
  return fn;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::mkdir(const std::string &name, bool createParentDirectories)
{
#if defined (WIN32)
  std::string dirName = name;
    if (createParentDirectories) {
        dirName = MXAFileSystemPath::toNativeSeparators(MXAFileSystemPath::cleanPath(dirName));
        // We spefically search for / so \ would break it..
        int oldslash = -1;
        if (dirName[0] == '\\' && dirName[1] == '\\') {
            // Don't try to create the root fsPath of a UNC fsPath;
            // CreateDirectory() will just return ERROR_INVALID_NAME.
            for (unsigned int i = 0; i < dirName.size(); ++i) {
                if (dirName[i] != MXAFileSystemPath::Separator) {
                    oldslash = i;
                    break;
                }
            }
            if (oldslash != -1)
                oldslash = dirName.find(MXAFileSystemPath::Separator, oldslash);
        }
        for (int slash=0; slash != -1; oldslash = slash) {
            slash = dirName.find(MXAFileSystemPath::Separator, oldslash+1);
            if (slash == -1) {
                if(oldslash == (int)(dirName.length()))
                    break;
                slash = dirName.length();
            }
            if (slash) {
              std::string chunk = dirName.substr(0, slash);
                bool existed = false;
                if (!isDirPath(chunk, &existed) && !existed) {
                   // if (!_mkdir(chunk.c_str() ))
                      if (!::CreateDirectoryA(chunk.c_str(), 0) )
                        return false;
                }
            }
        }
        return true;
    }
    return (!::CreateDirectoryA(dirName.c_str(), 0)  == 0);
#else

  std::string dirName = name;
    if (createParentDirectories) {
        dirName = MXAFileSystemPath::cleanPath(dirName);
        for(int oldslash = -1, slash=0; slash != -1; oldslash = slash) {
            slash = dirName.find(MXAFileSystemPath::Separator, oldslash+1);
            if (slash == -1) {
                if (oldslash == (int)dirName.length())
                    break;
                slash = dirName.length();
            }
            if (slash) {
                //QByteArray chunk = QFile::encodeName(dirName.left(slash));
                std::string chunk = dirName.substr(0, slash);
                MXA_STATBUF st;
                if (MXA_STAT(chunk.c_str(), &st) != -1)
                {
                  if ((st.st_mode & S_IFMT) != S_IFDIR)
                  {
                        return false;
                  }
                }
                else if (::mkdir(chunk.c_str(), 0777) != 0)
                {
                        return false;
                }
            }
        }
        return true;
    }
#if defined(__APPLE__)  // Mac X doesn't support trailing /'s
    if (dirName[dirName.length() - 1] == '/')
        dirName = dirName.substr(0, dirName.length() - 1);
#endif
    return (::mkdir(dirName.c_str(), 0777) == 0);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::remove(const std::string &fsPath)
{
  return UNLINK(MXAFileSystemPath::toNativeSeparators(fsPath).c_str()) == 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::rmdir(const std::string &name, bool recurseParentDirectories)
{
#if defined (WIN32)
  std::string dirName = name;
    if (recurseParentDirectories) {
        dirName = MXAFileSystemPath::toNativeSeparators(MXAFileSystemPath::cleanPath(dirName));
        for (int oldslash = 0, slash=dirName.length(); slash > 0; oldslash = slash) {
            std::string chunk = dirName.substr(0, slash);
            if (chunk.length() == 2 && isalpha(chunk[0]) && chunk[1] == ':')
                break;
            if (!isDirPath(chunk, 0))
                return false;
            if (!_rmdir(chunk.c_str()))
                return oldslash != 0;
            slash = dirName.find_last_of(MXAFileSystemPath::Separator, oldslash-1);
        }
        return true;
    }
    return (bool)(_rmdir(name.c_str()) == 0 );
#else
  std::string dirName = name;
    if (recurseParentDirectories) {
        dirName = MXAFileSystemPath::cleanPath(dirName);
        for(int oldslash = 0, slash=dirName.length(); slash > 0; oldslash = slash) {
            std::string chunk = dirName.substr(0, slash);
            MXA_STATBUF st;
            if (MXA_STAT(chunk.c_str(), &st) != -1) {
                if ((st.st_mode & S_IFMT) != S_IFDIR)
                    return false;
                if (::rmdir(chunk.c_str()) != 0)
                    return oldslash != 0;
            } else {
                return false;
            }
            slash = dirName.find_last_of(MXAFileSystemPath::Separator, oldslash-1);
        }
        return true;
    }
    return ::rmdir(dirName.c_str()) == 0;
#endif
}

#if defined (WIN32)
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::isDirPath(const std::string &folderPath, bool *existed)
{
    std::string fsPath = folderPath;
    if (fsPath.length() == 2 &&fsPath.at(1) == ':')
        fsPath += MXAFileSystemPath::Separator;

    DWORD fileAttrib = INVALID_FILE_ATTRIBUTES;
    fileAttrib = ::GetFileAttributesA(fsPath.c_str() );

    if (existed)
        *existed = fileAttrib != INVALID_FILE_ATTRIBUTES;

    if (fileAttrib == INVALID_FILE_ATTRIBUTES)
        return false;

    return (bool)(fileAttrib & FILE_ATTRIBUTE_DIRECTORY);
}
#endif


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::fromNativeSeparators(const std::string  &fsPath)
{
  std::string path(fsPath);
#if defined (WIN32)
  for (int i=0; i<(int)path.length(); i++) {
      if (path[i] ==  MXAFileSystemPath::Separator )
        path[i] =  MXAFileSystemPath::UnixSeparator;
  }
#endif
  return path;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::toNativeSeparators(const std::string &fsPath)
{
    std::string path(fsPath);
#if defined (WIN32)
    for (int i=0; i<(int)path.length(); i++) {
        if (path[i] ==  MXAFileSystemPath::UnixSeparator )
          path[i] =  MXAFileSystemPath::Separator;
    }
#endif
    return path;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::cleanPath(const std::string &fsPath)
{
    if (fsPath.length() == 0)
        return fsPath;
      std::string path(fsPath);
     char slash = '/';
     char dot = '.';
     if (MXAFileSystemPath::Separator != MXAFileSystemPath::UnixSeparator)
     {
       path = fromNativeSeparators(path);
     }

     // Peel off any trailing slash
     if (path[path.length() -1 ] == slash)
     {
       path = path.substr(0, path.length() -1);
     }

     std::vector<std::string> stk;
     std::string::size_type pos = 0;
     std::string::size_type pos1 = 0;

     // Check for UNC style paths first

     pos = path.find_first_of(slash, pos);
     pos1 = path.find_first_of(slash, pos + 1);
   #if defined (WIN32)
     if (pos == 0 && pos1 == 1)
     {
       pos1 = path.find_first_of(slash, pos1 + 1);
     } else
   #endif
     if (pos != 0)
     {
       stk.push_back(path.substr(0, pos));
     }


     while (pos1 != std::string::npos)
     {
       if (pos1 - pos == 3 && path[pos+1] == dot && path[pos+2] == dot)
       {
       //  std::cout << "Popping back element" << std::endl;
         if (stk.size() > 0) {
           stk.pop_back();
         }
       }
       else if (pos1 - pos == 2 && path[pos+1] == dot )
       {

       }
       else if (pos + 1 == pos1) {

       }
       else {
         stk.push_back(path.substr(pos, pos1-pos));
       }
       pos = pos1;
       pos1 = path.find_first_of(slash, pos + 1);
       if (pos1 == std::string::npos)
       {
         stk.push_back(path.substr(pos, path.length() - pos));
       }
     }
     std::string ret;
   //  std::cout << "|--2nd Pass: ";
     for (std::vector<std::string>::iterator iter = stk.begin(); iter != stk.end(); ++iter ) {
     // std::cout << *iter;
       ret.append(*iter);
     }

     ret = toNativeSeparators(ret);
     //std::cout << ret << std::endl;
     return ret;
}
