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
* @brief The class allows for some interrogation of the file system regarding the
* existance of files or directories and also allows for the creation and deletion
* of files or directories from the filesystem.
*
* @author mjackson
* @date Mar 10, 2009
* @version $Revision$
*/
class MXAFileSystemPath
{
  public:
#if defined (WIN32)
    static MXA_EXPORT const char Separator = '\\';
#else
    static MXA_EXPORT const char Separator = '/';
#endif
    static MXA_EXPORT const char UnixSeparator = '/';

    /**
     * @brief Is the path specified a directory on the filesystem
     * @param path Path to examine
     */
    static MXA_EXPORT bool isDirectory(const std::string &path);

    /**
     * @brief Does the path designate a file on the file system
     * @param path Path to examine
     */
    static MXA_EXPORT bool isFile(const std::string &path);

    /**
     * @brief Does the path actually exist on the file system
     * @param path Path to examine
     */
    static MXA_EXPORT bool exists(const std::string &path);

    /**
     * @brief Get any file extension on the filepath
     * @param path Path to examine
     */
    static MXA_EXPORT std::string extension(const std::string &path);

    /**
     * @brief Return the filename WITH the extension
     * @param path Path to examine
     */
    static MXA_EXPORT std::string filename(const std::string &path);

    /**
     * @brief Create a directory or structure of directories
     * @param path The path to create
     * @param createParentDirectories If true then any directories missing from
     * the path will also be created.
     * @return True if all directories were created successfully.
     */
    static MXA_EXPORT bool mkdir(const std::string &path, bool createParentDirectories);

    /**
     * @brief Removes a directory from the file system. Note that the directory
     * must be empty, including hidden files
     * @param path to delete from the filesystem
     * @param recurseParentDirectories
     */
    static MXA_EXPORT bool rmdir(const std::string &path, bool recurseParentDirectories);

    /**
     * @brief Remove a file from the filesystem
     * @param The path to the file to remove
     * @return True on successful removal
     */
    static MXA_EXPORT bool remove(const std::string &path);

    /**
     * @brief Cleans a file system path of extra './', '//' and '/../' elements
     * @param path Path to clean
     * @return A new string containing the cleaned path.
     */
    static MXA_EXPORT std::string cleanPath(const std::string &path);

    /**
     * @brief Converts from native directory separators to unix separators
     * @param The path to conver
     * @return Newly converted path
     */
    static MXA_EXPORT std::string fromNativeSeparators(const std::string  &path);

    /**
     * @brief Converts a path to use native directory separators
     * @param The path to convert
     * @return The newly converted path
     */
    static MXA_EXPORT std::string toNativeSeparators(const std::string &path);

#if defined (WIN32)
    static MXA_EXPORT bool isDirPath(const std::string &path, bool *existed);
#endif

  protected:
    MXAFileSystemPath();
    ~MXAFileSystemPath();

  private:
      MXAFileSystemPath(const MXAFileSystemPath&);    //Not Implemented
      void operator=(const MXAFileSystemPath&); //Not Implemented

};

#endif //_MXAFileSystemPath_h_



