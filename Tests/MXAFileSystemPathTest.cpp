///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include <MXA/Common/MXATypes.h>
#include <MXA/Utilities/MXAFileSystemPath.h>
#include <Tests/MXAUnitTestDataFileLocations.h>

#include <iostream>
#include <fstream>

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#define FileWriter MXAFILEWRITER_CLASS_NAME
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
  std::cout << "|--Removing Test files" << std::endl;
#if REMOVE_TEST_FILES
  //boost::filesystem::remove(MXATesting::MXAFileSystemPathTest::OuptutFile);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CheckFile(const std::string &filepath,
               const std::string &filename,
               const std::string &extension)
{
  bool exists;
  bool isDir;
  bool isFile;
  bool ok;
 // std::string filepath(MXAUnitTest::MXAFileSystemPathTest::OutputFile );
  //std::cout << "|--Creating File '" << filepath << "'" << std::endl;
    { // Write a file so we can try and delete it
      std::cout << "|--Creating file to delete: '" << filepath << "'" << std::endl;
      std::ofstream outStream(filepath.c_str(), std::ios::out | std::ios::binary);
      BOOST_REQUIRE_EQUAL(false, outStream.fail() );
      char* data = "MXAFileSystemPath_Test Contents";
      outStream.write(data, 31);
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
    std::cout << "|--Deleting File: '" << filepath << "'" << std::endl;
    ok = MXAFileSystemPath::remove(filepath);
    BOOST_REQUIRE_EQUAL(ok, true);
    exists = MXAFileSystemPath::exists(filepath);
    BOOST_REQUIRE_EQUAL(exists, false);

    std::cout << "|--Trying to delete the same file again." << std::endl;
    ok = MXAFileSystemPath::remove(filepath);
    BOOST_REQUIRE_EQUAL(ok, false);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int MXAFileSystemPathTest_EntryPoint()
{
	int err = 0;
  bool exists;
  bool isDir;
  bool isFile;
  bool ok;

  std::string testdir = MXAUnitTest::MXATempDir + MXAUnitTest::MXAFileSystemPathTest::TestDir;


  std::string dirPath(testdir
                      + MXAFileSystemPath::Separator
                      + MXAUnitTest::MXAFileSystemPathTest::TestDirName1
                      + MXAFileSystemPath::Separator);
  std::cout << "|--Creating Deeply Nested Directories: '" << dirPath << "'" << std::endl;

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

  CheckFile(MXAUnitTest::MXAFileSystemPathTest::OutputFile,
                 MXAUnitTest::MXAFileSystemPathTest::OutputFileName,
                 MXAUnitTest::MXAFileSystemPathTest::Extension);

  std::string testFileName = ".hidden_file";
  std::string testFile = testdir + MXAUnitTest::DirSeparator + testFileName;
  std::string ext; // No Extension
  CheckFile(testFile, testFileName, ext);

  testFileName = "Normal.txt";
  ok = MXAFileSystemPath::mkdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, true);
  BOOST_REQUIRE_EQUAL(ok, true);
  testFile = testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator + testFileName;
  ext = "txt";
  CheckFile(testFile, testFileName, ext);
  ok = MXAFileSystemPath::rmdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, false);
  BOOST_REQUIRE_EQUAL(ok, true);

  testFileName = "No_Extension";
  ok = MXAFileSystemPath::mkdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, true);
  BOOST_REQUIRE_EQUAL(ok, true);
  testFile = testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator + testFileName;
  ext = "";
  CheckFile(testFile, testFileName, ext);
  ok = MXAFileSystemPath::rmdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, false);
  BOOST_REQUIRE_EQUAL(ok, true);

  testFileName = "EndsWithDot.";
  ok = MXAFileSystemPath::mkdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, true);
  BOOST_REQUIRE_EQUAL(ok, true);
  testFile = testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator + testFileName;
  ext = "";
  CheckFile(testFile, testFileName, ext);
  ok = MXAFileSystemPath::rmdir(testdir + MXAUnitTest::DirSeparator + "Dot.Dir" + MXAUnitTest::DirSeparator, false);
  BOOST_REQUIRE_EQUAL(ok, true);


  std::cout << "|--Removing top level test dir: '" << testdir << "'" << std::endl;
  ok = MXAFileSystemPath::rmdir(testdir, false);
  BOOST_REQUIRE_EQUAL(ok, true);

  return err;
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite(int32 /*argc*/, char* /*argv*/[])
{
  boost::unit_test::test_suite* test= BOOST_TEST_SUITE ( "MXAFileSystemPathTest Test Running");
  test->add( BOOST_TEST_CASE( &MXAFileSystemPathTest_EntryPoint), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}


