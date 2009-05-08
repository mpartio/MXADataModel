///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include <MXA/Common/MXATypes.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Utilities/MXAFileSystemPath.h>
#include <Tests/MXAUnitTestDataFileLocations.h>

#include <iostream>
#include <fstream>

#include <boost/shared_ptr.hpp>

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#define PRINT_LINE_NUMBER() \
  std::cout << "|--MXAFileSystemPathTest.cpp(" << __LINE__ << ")" << std::endl;

#define FileWriter MXAFILEWRITER_CLASS_NAME
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
  std::cout << "|--Removing Test files" << std::endl;
#if REMOVE_TEST_FILES
  MXAFileSystemPath::remove(MXAUnitTest::MXAFileSystemPathTest::OutputFile);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CheckFile(const std::string &filepath,
               const std::string &filename,
               const std::string &extension)
{
  std::cout  << "|-- CheckFile " << filepath << std::endl;
  bool exists;
  bool isDir;
  bool isFile;
  bool ok;
 // std::string filepath(MXAUnitTest::MXAFileSystemPathTest::OutputFile );
  //std::cout << "|--Creating File '" << filepath << "'" << std::endl;
    {
      // Write a file so we can try and delete it
      std::cout << "|--  Create: '" << filepath << "'" << std::endl;
      std::ofstream outStream(filepath.c_str(), std::ios::out | std::ios::binary);
      BOOST_REQUIRE_EQUAL(false, outStream.fail() );
      std::string data ( "MXAFileSystemPath_Test Contents");
      outStream.write(data.c_str(), data.length());
      BOOST_REQUIRE_EQUAL (outStream.bad(), false);
      outStream.close();
    }
    isDir = MXAFileSystemPath::isDirectory(filepath);
    BOOST_REQUIRE_EQUAL(isDir, false);
    isFile = MXAFileSystemPath::isFile(filepath);
    BOOST_REQUIRE_EQUAL(isFile, true);
    exists = MXAFileSystemPath::exists(filepath);
    BOOST_REQUIRE_EQUAL(exists, true);
    std::string fn = MXAFileSystemPath::filename(filepath);
    BOOST_REQUIRE_EQUAL(fn, filename);
    std::string ext = MXAFileSystemPath::extension(filepath);
    BOOST_REQUIRE_EQUAL(ext, extension);

    // Now try to delete the file
    std::cout << "|--  Delete: '" << filepath << "'" << std::endl;
    ok = MXAFileSystemPath::remove(filepath);
    BOOST_REQUIRE_EQUAL(ok, true);
    exists = MXAFileSystemPath::exists(filepath);
    BOOST_REQUIRE_EQUAL(exists, false);

    std::cout << "|--  Delete Again:" << std::endl;
    ok = MXAFileSystemPath::remove(filepath);
    BOOST_REQUIRE_EQUAL(ok, false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int FileNameTest()
{
  int err = 0;

  std::string filename = "some.thing";
  std::string filepath = filename;
  std::string ext = "thing";
  CheckFile(filepath, filename, ext);

  filename = ".some.thing";
  filepath = filename;
  ext = "thing";
  CheckFile(filepath, filename, ext);

  filename = ".something";
  filepath = filename;
  ext = "";
  CheckFile(filepath, filename, ext);

  filename = "something";
  filepath = filename;
  ext = "";
  CheckFile(filepath, filename, ext);

  filename = "something.";
  filepath = filename;
  ext = "";
  CheckFile(filepath, filename, ext);

  filename = ".some.thing.";
  filepath = filename;
  ext = "";
  CheckFile(filepath, filename, ext);


//------------------------------------------------------
#if defined (WIN32)
    const std::string DirSeparator = "\\";
#else
    const std::string DirSeparator = "/";
#endif
  filename = "some.thing";
  filepath = MXAUnitTest::MXATempDir + filename;
  ext = "thing";
  CheckFile(filepath, filename, ext);

  filename = ".some.thing";
  filepath = MXAUnitTest::MXATempDir + filename;
  ext = "thing";
  CheckFile(filepath, filename, ext);

  filename = ".something";
  filepath = MXAUnitTest::MXATempDir +  filename;
  ext = "";
  CheckFile(filepath, filename, ext);

  filename = "something";
  filepath = MXAUnitTest::MXATempDir + filename;
  ext = "";
  CheckFile(filepath, filename, ext);

  filename ="something.";
  filepath =  MXAUnitTest::MXATempDir + filename;
  ext = "";
  CheckFile(filepath, filename, ext);

  filename = ".some.thing.";
  filepath = MXAUnitTest::MXATempDir + filename;
  ext = "";
  CheckFile(filepath, filename, ext);

  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int FilesTest()
{
  std::cout  << "|- FilesTest -----------------" << std::endl;
  int err = 0;
  bool ok;

  std::string testdir = MXAUnitTest::MXATempDir + MXAUnitTest::MXAFileSystemPathTest::TestDir;


  CheckFile(MXAUnitTest::MXAFileSystemPathTest::OutputFile,
                 MXAUnitTest::MXAFileSystemPathTest::OutputFileName,
                 MXAUnitTest::MXAFileSystemPathTest::Extension);

  std::string testFileName = ".hidden_file";
  std::string testFilePath = testdir + MXAUnitTest::DirSeparator + testFileName;
  std::string ext; // No Extension
  CheckFile(testFilePath, testFileName, ext);

  testFileName = "Normal.txt";
  ok = MXAFileSystemPath::mkdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, true);
  BOOST_REQUIRE_EQUAL(ok, true);
  testFilePath = testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator + testFileName;
  ext = "txt";
  CheckFile(testFilePath, testFileName, ext);
  ok = MXAFileSystemPath::rmdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, false);
  BOOST_REQUIRE_EQUAL(ok, true);

  testFileName = "No_Extension";
  ok = MXAFileSystemPath::mkdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, true);
  BOOST_REQUIRE_EQUAL(ok, true);
  testFilePath = testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator + testFileName;
  ext = "";
  CheckFile(testFilePath, testFileName, ext);
  ok = MXAFileSystemPath::rmdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, false);
  BOOST_REQUIRE_EQUAL(ok, true);

  testFileName = "EndsWithDot.";
  ok = MXAFileSystemPath::mkdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, true);
  BOOST_REQUIRE_EQUAL(ok, true);
  testFilePath = testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator + testFileName;
  ext = "";
  CheckFile(testFilePath, testFileName, ext);
  ok = MXAFileSystemPath::rmdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, false);
  BOOST_REQUIRE_EQUAL(ok, true);


  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int AbsolutePathTest()
{
  std::cout  << "|- AbsolutePathTest -----------------" << std::endl;
  int err = 0;
  int success = 0;

  std::string testdir = MXAUnitTest::MXATempDir + MXAUnitTest::MXAFileSystemPathTest::TestDir;

  std::string currentPath = MXAFileSystemPath::currentPath();
  //std::cout << "currentPath: " << currentPath << std::endl;
  //std::cout << "MXATestBinaryDirectory:" << MXAUnitTest::MXATestBinaryDirectory << std::endl;
  success = currentPath.compare(MXAUnitTest::MXATestBinaryDirectory);
  BOOST_REQUIRE_EQUAL(success, 0);

  std::string file = MXAUnitTest::MXAFileSystemPathTest::OutputFileName;
  PRINT_LINE_NUMBER();
  CheckFile(MXAUnitTest::MXAFileSystemPathTest::OutputFileName, file, "bin");
  file = MXAFileSystemPath::absolutePath(file);
  std::string refPath = MXAUnitTest::MXATestBinaryDirectory + MXAFileSystemPath::Separator + MXAUnitTest::MXAFileSystemPathTest::OutputFileName;
  std::cout << "|-- file: " << file << std::endl;
  success = file.compare(refPath);
  BOOST_REQUIRE_EQUAL(success, 0);
  PRINT_LINE_NUMBER();
  CheckFile(  file, MXAUnitTest::MXAFileSystemPathTest::OutputFileName, "bin");

  // Check with a ./ prefixed to the file path
  file = MXAUnitTest::MXAFileSystemPathTest::OutputFileName;
  PRINT_LINE_NUMBER();
  CheckFile("." + MXAFileSystemPath::getSeparator() + MXAUnitTest::MXAFileSystemPathTest::OutputFileName, file, "bin");
  file = MXAFileSystemPath::absolutePath(file);
  refPath = MXAUnitTest::MXATestBinaryDirectory + MXAFileSystemPath::Separator + MXAUnitTest::MXAFileSystemPathTest::OutputFileName;
  std::cout << "|-- file: " << file << std::endl;
  success = file.compare(refPath);
  BOOST_REQUIRE_EQUAL(success, 0);
  PRINT_LINE_NUMBER();
  CheckFile(file, MXAUnitTest::MXAFileSystemPathTest::OutputFileName, "bin");

  // Check with a ../ prefixed to the file anem
  file = MXAUnitTest::MXAFileSystemPathTest::OutputFileName;
  PRINT_LINE_NUMBER();
  CheckFile( ".." + MXAFileSystemPath::getSeparator() + MXAUnitTest::MXAFileSystemPathTest::OutputFileName, file, "bin");
  file = MXAFileSystemPath::absolutePath(".." + MXAFileSystemPath::getSeparator() + file);
  refPath = MXAUnitTest::MXABuildDir + MXAFileSystemPath::Separator + MXAUnitTest::MXAFileSystemPathTest::OutputFileName;
  std::cout << "|-- file: " << file << std::endl;
  success = file.compare(refPath);
  BOOST_REQUIRE_EQUAL(success, 0);
  PRINT_LINE_NUMBER();
  CheckFile(file, MXAUnitTest::MXAFileSystemPathTest::OutputFileName, "bin");


  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int MakeDirectoriesTest()
{

  std::cout  << "|- MakeDirectoriesTest -----------------" << std::endl;
	int err = 0;
  bool exists;
  bool isDir;
  bool isFile;
  bool ok;
  bool isRelative;
  bool isAbsolute;

  std::string testdir = MXAUnitTest::MXATempDir + MXAUnitTest::MXAFileSystemPathTest::TestDir;


  std::string dirPath(testdir
                      + MXAFileSystemPath::Separator
                      + MXAUnitTest::MXAFileSystemPathTest::TestDirName1
                      + MXAFileSystemPath::Separator);
  std::cout << "|-- Creating Deeply Nested Directories: '" << dirPath << "'" << std::endl;

  exists = MXAFileSystemPath::exists(dirPath);
  BOOST_REQUIRE_EQUAL(exists, false);
  err = MXAFileSystemPath::mkdir(dirPath, true);
  BOOST_REQUIRE_EQUAL(err, 1);
  isDir = MXAFileSystemPath::isDirectory(dirPath);
  BOOST_REQUIRE_EQUAL(isDir, true);
  isFile = MXAFileSystemPath::isFile(dirPath);
  BOOST_REQUIRE_EQUAL(isFile, false);
  exists = MXAFileSystemPath::exists(dirPath);
  BOOST_REQUIRE_EQUAL(exists, true);
  ok = MXAFileSystemPath::rmdir(dirPath, false);
  BOOST_REQUIRE_EQUAL(ok, true);
  exists = MXAFileSystemPath::exists(dirPath);
  BOOST_REQUIRE_EQUAL(exists, false);

  dirPath = testdir
              + MXAFileSystemPath::Separator
              + MXAUnitTest::MXAFileSystemPathTest::TestDirName1;

  exists = MXAFileSystemPath::exists(dirPath);
  BOOST_REQUIRE_EQUAL(exists, false);
  err = MXAFileSystemPath::mkdir(dirPath, true);
  BOOST_REQUIRE_EQUAL(err, 1);
  isDir = MXAFileSystemPath::isDirectory(dirPath);
  BOOST_REQUIRE_EQUAL(isDir, true);
  isFile = MXAFileSystemPath::isFile(dirPath);
  BOOST_REQUIRE_EQUAL(isFile, false);
  exists = MXAFileSystemPath::exists(dirPath);
  BOOST_REQUIRE_EQUAL(exists, true);
  ok = MXAFileSystemPath::rmdir(dirPath, false);
  BOOST_REQUIRE_EQUAL(ok, true);
  exists = MXAFileSystemPath::exists(dirPath);
  BOOST_REQUIRE_EQUAL(exists, false);

  isRelative = MXAFileSystemPath::isRelativePath(dirPath);
  BOOST_REQUIRE_EQUAL(isRelative, false);
  isAbsolute = MXAFileSystemPath::isAbsolutePath(dirPath);
  BOOST_REQUIRE_EQUAL(isAbsolute, true);

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int RemoveDirectoriesTest()
{
  std::cout  << "|- RemoveDirectoriesTest -----------------" << std::endl;
  int err = 0;
  bool ok;
  std::string testdir = MXAUnitTest::MXATempDir + MXAUnitTest::MXAFileSystemPathTest::TestDir;
  std::cout << "|-- Removing top level test dir: '" << testdir << "'" << std::endl;
  ok = MXAFileSystemPath::rmdir(testdir, false);
  BOOST_REQUIRE_EQUAL(ok, true);

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int DirListTest()
{
  std::cout  << "|- DirListTest -----------------" << std::endl;

  int err = 0;
  std::vector<std::string> list = MXAFileSystemPath::entryList(MXAUnitTest::MXABuildDir);

  for (std::vector<std::string>::iterator iter = list.begin(); iter != list.end(); ++iter ) {
    std::cout << "|--- DIR_LIST_ENTRY--> " << *iter << std::endl;
  }

  std::string ppath = MXAFileSystemPath::parentPath(MXAUnitTest::MXATestBinaryDirectory);
  //std::cout << "Child Path: " << MXAUnitTest::MXATestBinaryDirectory << std::endl;
  //std::cout << "ppath: " << ppath << std::endl;
  //std::cout << "Ref Path:   " << MXAUnitTest::MXABuildDir << std::endl;
  int equal  = ppath.compare(MXAUnitTest::MXABuildDir);
  BOOST_REQUIRE_EQUAL(equal, 0);
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int FileNameExtensionTest()
{
  std::cout  << "|- FileNameExtensionTest -----------------" << std::endl;
  int err = 0;
  std::string file("SomeFile.txt");
  std::string base = MXAFileSystemPath::fileNameWithOutExtension(file);
  BOOST_REQUIRE_EQUAL(base, "SomeFile");

  file ="SomeFile";
  base = MXAFileSystemPath::fileNameWithOutExtension(file);
  BOOST_REQUIRE_EQUAL(base, "SomeFile");


  file ="SomeFile.";
  base = MXAFileSystemPath::fileNameWithOutExtension(file);
  BOOST_REQUIRE_EQUAL(base, "SomeFile.");


  file ="SomeFile.txt.bin";
  base = MXAFileSystemPath::fileNameWithOutExtension(file);
  BOOST_REQUIRE_EQUAL(base, "SomeFile.txt");

  file ="/SomePath/To/SomeFile.txt.bin";
  base = MXAFileSystemPath::fileNameWithOutExtension(file);
  BOOST_REQUIRE_EQUAL(base, "SomeFile.txt");

  return err;
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "MXAFileSystemPathTest Test Running");
  test->add( BOOST_TEST_CASE( &MakeDirectoriesTest), 0);
  test->add( BOOST_TEST_CASE( &FilesTest), 0);
  test->add( BOOST_TEST_CASE( &FileNameTest), 0);
  test->add( BOOST_TEST_CASE( &AbsolutePathTest), 0);
  test->add( BOOST_TEST_CASE( &DirListTest), 0);
  test->add( BOOST_TEST_CASE( &RemoveDirectoriesTest), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  test->add( BOOST_TEST_CASE( &FileNameExtensionTest), 0);
  return test;
}


