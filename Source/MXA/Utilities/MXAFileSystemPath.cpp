///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
/* Note that much of this implementation was inspired by the Qt source code.
 * www.trolltech.com
 */


#include "MXAFileSystemPath.h"

#include <MXA/MXAConfiguration.h>

#include <iostream>
#include <vector>

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
bool MXAFileSystemPath::isDirectory(const std::string &path)
{
  int error;
  MXA_STATBUF st;
  error = MXA_STAT(path.c_str(), &st);
  if (!error && (st.st_mode & S_IFMT) == S_IFDIR)
  {
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::isFile(const std::string &path)
{
  int error;
  MXA_STATBUF st;
  error = MXA_STAT(path.c_str(), &st);
  if (!error && (st.st_mode & S_IFMT) == S_IFREG)
  {
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::exists(const std::string &path)
{
  int error;
    MXA_STATBUF st;
    error = MXA_STAT(path.c_str(), &st);
    return (error == 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::extension(const std::string &path)
{
  std::string::size_type pos = path.find_last_of('.');
  std::string::size_type slashPos = path.find_last_of(MXAFileSystemPath::Separator);
  if (pos > 0
      && pos != std::string::npos
      && path[pos-1] != '/'
      && path[pos-1] != '\\'
      && pos > slashPos)
  {
    return path.substr(pos + 1);
  }
  return std::string();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::filename(const std::string &path)
{

  std::string::size_type slashPos = path.find_last_of(MXAFileSystemPath::Separator);
  if (slashPos == path.size() - 1)
  {
    return MXAFileSystemPath::filename(path.substr(0, path.size() - 1) );
  }

  std::string fn = path.substr(slashPos + 1, path.size() - slashPos);
  return fn;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::mkdir(const std::string &name, bool createParentDirectories)
{
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
                if (MXA_STAT(chunk.c_str(), &st) != -1) {
                    if ((st.st_mode & S_IFMT) != S_IFDIR)
                        return false;
                } else if (::mkdir(chunk.c_str(), 0777) != 0) {
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
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::remove(const std::string &path)
{
  return unlink(MXAFileSystemPath::toNativeSeparators(path).c_str()) == 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool MXAFileSystemPath::rmdir(const std::string &name, bool recurseParentDirectories)
{
  std::string dirName = name;
    if (recurseParentDirectories) {
        dirName = MXAFileSystemPath::cleanPath(dirName);
        for(int oldslash = 0, slash=dirName.length(); slash > 0; oldslash = slash) {
           // QByteArray chunk = QFile::encodeName(dirName.left(slash));
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
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::fromNativeSeparators(const std::string  &pathName)
{
    std::string n(pathName);
#if __WIN32
    for (int i=0; i<(int)n.length(); i++) {
        if (n[i] == '\\') { n[i] = '/'; }
    }
#endif
    return n;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::toNativeSeparators(const std::string &pathName)
{
    std::string n(pathName);
#if defined(__WIN32)
    for (int i=0; i<(int)n.length(); i++) {
        if (n[i] ==  '/' )
            n[i] =  '\\';
    }
#endif
    return n;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXAFileSystemPath::cleanPath(const std::string &path)
{
    if (path.length() == 0)
        return path;
    std::string name = path;
    char dir_separator = MXAFileSystemPath::Separator;
    if(dir_separator != '/') {
      name = MXAFileSystemPath::fromNativeSeparators(name);
    }

    int used = 0, levels = 0;
    const int len = name.length();
    std::vector<char> out(len);
    const char *p = name.data();
    for(int i = 0, last = -1, iwrite = 0; i < len; i++) {
        if(p[i] == '/') {
            while(i < len-1 && p[i+1] == '/') {
#if defined(__WIN32) //allow unc paths
                if(!i)
                    break;
#endif
                i++;
            }
            bool eaten = false;
            if(i < len - 1 && p[i+1] == '.') {
                int dotcount = 1;
                if(i < len - 2 && p[i+2] == '.')
                    dotcount++;
                if(i == len - dotcount - 1) {
                    if(dotcount == 1) {
                        break;
                    } else if(levels) {
                        if(last == -1) {
                            for(int i2 = iwrite-1; i2 >= 0; i2--) {
                                if(out[i2] == '/') {
                                    last = i2;
                                    break;
                                }
                            }
                        }
                        used -= iwrite - last - 1;
                        break;
                    }
                } else if(p[i+dotcount+1] == '/') {
                    if(dotcount == 2 && levels) {
                        if(last == -1 || iwrite - last == 1) {
                            for(int i2 = (last == -1) ? (iwrite-1) : (last-1); i2 >= 0; i2--) {
                                if(out[i2] == '/') {
                                    eaten = true;
                                    last = i2;
                                    break;
                                }
                            }
                        } else {
                            eaten = true;
                        }
                        if(eaten) {
                            levels--;
                            used -= iwrite - last;
                            iwrite = last;
                            last = -1;
                        }
                    } else if (dotcount == 2 && i > 0 && p[i - 1] != '.' ) {
                        eaten = true;
                        used -= iwrite - std::max(0, last);
                        iwrite = std::max(0, last);
                        last = -1;
                        ++i;
                    } else if(dotcount == 1) {
                        eaten = true;
                    }
                    if(eaten)
                        i += dotcount;
                } else {
                    levels++;
                }
            } else if(last != -1 && iwrite - last == 1) {
#ifdef __WIN32
                eaten = (iwrite > 2);
#else
                eaten = true;
#endif
                last = -1;
            } else if(last != -1 && i == len-1) {
                eaten = true;
            } else {
                levels++;
            }
            if(!eaten)
                last = i - (i - iwrite);
            else
                continue;
        } else if(!i && p[i] == '.') {
            int dotcount = 1;
            if(len >= 1 && p[1] == '.')
                dotcount++;
            if(len >= dotcount && p[dotcount] == '/') {
                if(dotcount == 1) {
                    i++;
                    while(i+1 < len-1 && p[i+1] == '/')
                        i++;
                    continue;
                }
            }
        }
        out[iwrite++] = p[i];
        used++;
    }
    std::string ret;
    if(used == len)
        ret = name;
    else
  ret = std::string(&(out.front()), used);

    // Strip away last slash except for root directories
    if ( (*(ret.end()) == '/') && !(ret.size() == 1 || (ret.size() == 3 && ret.at(1) == ':')))
    {
      ret = ret.substr(0, ret.length() - 1);
    }

    return ret;
}
