///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////

//--MXA Includes
#include <MXA/MXAConfiguration.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Common/MXATypes.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include "MXAUnitTestDataFileLocations.h"

//-- C++ includes
#include <list>
#include <string>


//-- Boost Test Headers
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

//-- Boost Filesystem Headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#define DIM 6
#define DIM0 4
#define DIM1 3
#define DIM2 2

#define RANK_1D 1
#define RANK_2D 2
#define RANK_3D 3


using namespace MXATypes;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
  boost::filesystem::remove(H5UTIL_TEST_FILE_NAME);
#endif
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
herr_t testWritePointer1DArrayAttribute(hid_t file_id, const std::string &dsetName)
{
  T value = 0x0;
  herr_t err = -1;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "1DArrayAttribute<" + attributeKey + ">";
  int32 rank = 1;
  T data[DIM0];
  for(int i = 0; i < DIM0; ++i)
  {
    data[i] = (T)(i);
  }
  uint64 dims[1];
  dims[0] = DIM0;

  err = H5Lite::writePointerAttribute<T>(file_id, dsetName, attributeKey, rank, dims, (T*)data);
  BOOST_REQUIRE(err >= 0);
  return err;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
herr_t testWritePointer2DArrayAttribute(hid_t file_id, const std::string &dsetName)
{
  //std::cout << DEBUG_OUT(logTime) << "testWritePointer2DArrayAttribute" << std::endl;
  T value = 0x0;
  herr_t err = -1;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);

  attributeKey = "2DArrayAttribute<" + attributeKey + ">";
  int32 rank = RANK_2D;
  T data[DIM0][DIM1];
  for(int i = 0; i < DIM0; ++i)
  {
    for (int j = 0; j < DIM1; ++j) {
      data[i][j] = (T)(i * j);
    }
  }
  uint64 dims[RANK_2D];
  dims[0] = DIM0;
  dims[1] = DIM1;
  err = H5Lite::writePointerAttribute<T>(file_id, dsetName, attributeKey, rank, dims, (T*)data);
  BOOST_REQUIRE(err >= 0);
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
herr_t testWritePointer3DArrayAttribute(hid_t file_id, const std::string &dsetName)
{
  T value = 0x0;
  herr_t err = -1;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "3DArrayAttribute<" + attributeKey + ">";
  int32 rank = RANK_3D;
  T data[DIM0][DIM1][DIM2];
  for(int i = 0; i < DIM0; ++i) {
    for (int j = 0; j < DIM1; ++j) {
      for (int k = 0; k < DIM2; ++k) {
        data[i][j][k] = (T)(i * j * k);
      }
    }
  }
  uint64 dims[RANK_3D];
  dims[0] = DIM0;
  dims[1] = DIM1;
  dims[2] = DIM2;
  err = H5Lite::writePointerAttribute<T>(file_id, dsetName, attributeKey, rank, dims, (T*)data);
  BOOST_REQUIRE(err >= 0);
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename T>
herr_t testWriteVectorAttribute(hid_t file_id, std::string dsetName )
{
  T value = 0x0;
  herr_t err = -1;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "VectorAttribute<" + attributeKey + ">";

  int32 numElements = DIM0;
  std::vector<uint64> dims (1, DIM0);

  /* Make dataset */
  std::vector<T> data (DIM0, 0);
  for (int i = 0; i < numElements; ++i) {
    data[i] = (T)(i);
  }
  //std::cout << "Attribute->Write: " << objName;
  err = H5Lite::writeVectorAttribute( file_id, dsetName, attributeKey, dims, data );
  BOOST_REQUIRE(err >= 0);
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
herr_t testWriteScalarAttribute(hid_t file_id, const std::string &dsetName)
{
  T value = 0x0F;
  herr_t err = -1;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "ScalarAttribute<" + attributeKey + ">";
  err = H5Lite::writeScalarAttribute(file_id, dsetName, attributeKey, value);
  BOOST_REQUIRE(err >= 0);
  return err;
}


// -----------------------------------------------------------------------------
//  Uses Raw Pointers to save data to the data file
// -----------------------------------------------------------------------------
template <typename T>
herr_t testWritePointer2DArrayDataset(hid_t file_id)
{

  T value = 0x0;
  herr_t err = 1;
  int32 rank = 2;
  // Create the Dimensions
  uint64 dims[2];
  dims[0] = DIM0; dims[1] = DIM1;

  /* Make dataset char */
  int32 tSize = dims[0] * dims[1];
 // T data[dimx*dimy];
  std::vector<T> data(tSize);
  for (int32 i = 0; i < tSize; ++i) {
    data[i] = static_cast<T>( i * 5);
  }

  std::string dsetName = H5Lite::HDFTypeForPrimitiveAsStr(value);
  dsetName = "Pointer2DArrayDataset<" + dsetName + ">";
  std::cout << "Running " << dsetName << " ... ";
  err = H5Lite::writePointerDataset( file_id, dsetName, rank, dims, &(data.front()) );
  BOOST_REQUIRE(err >= 0);
  err = testWritePointer1DArrayAttribute<int8>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<uint8>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<int16>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<uint16>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<int32>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<uint32>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<int64>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<uint64>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<float32>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<float64>(file_id, dsetName);

  err = testWritePointer2DArrayAttribute<int8>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<uint8>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<int16>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<uint16>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<int32>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<uint32>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<int64>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<uint64>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<float32>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<float64>(file_id, dsetName);

  err = testWritePointer3DArrayAttribute<int8>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<uint8>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<int16>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<uint16>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<int32>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<uint32>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<int64>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<uint64>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<float32>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<float64>(file_id, dsetName);

  err = testWriteVectorAttribute<int8>(file_id, dsetName);
  err = testWriteVectorAttribute<uint8>(file_id, dsetName);
  err = testWriteVectorAttribute<int16>(file_id, dsetName);
  err = testWriteVectorAttribute<uint16>(file_id, dsetName);
  err = testWriteVectorAttribute<int32>(file_id, dsetName);
  err = testWriteVectorAttribute<uint32>(file_id, dsetName);
  err = testWriteVectorAttribute<int64>(file_id, dsetName);
  err = testWriteVectorAttribute<uint64>(file_id, dsetName);
  err = testWriteVectorAttribute<float32>(file_id, dsetName);
  err = testWriteVectorAttribute<float64>(file_id, dsetName);

  err = testWriteScalarAttribute<int8>(file_id, dsetName);
  err = testWriteScalarAttribute<uint8>(file_id, dsetName);
  err = testWriteScalarAttribute<int16>(file_id, dsetName);
  err = testWriteScalarAttribute<uint16>(file_id, dsetName);
  err = testWriteScalarAttribute<int32>(file_id, dsetName);
  err = testWriteScalarAttribute<uint32>(file_id, dsetName);
  err = testWriteScalarAttribute<int64>(file_id, dsetName);
  err = testWriteScalarAttribute<uint64>(file_id, dsetName);
  err = testWriteScalarAttribute<float32>(file_id, dsetName);
  err = testWriteScalarAttribute<float64>(file_id, dsetName);

  std::cout << " Passed" << std::endl;
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5UtilitiesTest()
{
  // herr_t err = -1;
   hid_t   file_id;
   /* Create a new file using default properties. */
   file_id = H5Fcreate( H5UTIL_TEST_FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
   BOOST_REQUIRE(file_id > 0);

   // std::cout << logTime() << "----------- Testing Writing/Reading of Datasets using Raw Pointers -----------" << std::endl;
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<int32>(file_id) >= 0);

   hid_t dsetId = H5Utilities::openHDF5Object(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>");
   BOOST_REQUIRE(dsetId > 0);

   BOOST_REQUIRE( H5Utilities::isGroup(file_id, "/") == true);
   BOOST_REQUIRE( H5Utilities::isGroup(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>") == false);

   std::string objName;
   int32 err = H5Utilities::objectNameAtIndex(file_id, 0, objName);
   BOOST_REQUIRE(objName.compare("Pointer2DArrayDataset<H5T_NATIVE_INT32>") == 0);

   hid_t objType = -1;
   err = H5Utilities::getObjectType(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", &objType);
   BOOST_REQUIRE(objType == H5G_DATASET);
   BOOST_REQUIRE(err >= 0);

   err = H5Utilities::getObjectType(file_id, "/", &objType);
   BOOST_REQUIRE(objType == H5G_GROUP);
   BOOST_REQUIRE(err >= 0);

   std::string objPath = H5Utilities::getObjectPath(dsetId, false);
   BOOST_REQUIRE(objPath.compare("Pointer2DArrayDataset<H5T_NATIVE_INT32>") == 0);

   err = H5Utilities::closeHDF5Object(dsetId);
   BOOST_REQUIRE(err >= 0);

   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 1", file_id) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 2/", file_id) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("Test Path 3/", file_id) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/", file_id) < 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 4/Test Path 7", file_id) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 5/Test Path 8/", file_id) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("Test Path 6/Test Path 9/", file_id) >= 0);

   hid_t grpId = H5Utilities::openHDF5Object(file_id, "Test Path 1");
   BOOST_REQUIRE(grpId > 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 1", grpId) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 2/", grpId) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("Test Path 3/", grpId) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/", grpId) < 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 4/Test Path 7", grpId) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 5/Test Path 8/", grpId) >= 0);
   BOOST_REQUIRE ( H5Utilities::createGroupsFromPath("Test Path 6/Test Path 9/", grpId) >= 0);
   err = H5Gclose(grpId);
   BOOST_REQUIRE(err >= 0);


   hid_t gid = H5Utilities::createGroup(file_id, "test group");
   BOOST_REQUIRE(gid >= 0);
   err = H5Utilities::closeHDF5Object(gid);

   std::list<std::string> groups;
   err = H5Utilities::getGroupObjects(file_id, H5Utilities::MXA_ANY, groups);
   BOOST_REQUIRE(err >= 0);
   BOOST_REQUIRE( groups.size() == 8);


   err = H5Utilities::createGroupsForDataset("/group1/group2/group3/data", file_id);
   BOOST_REQUIRE(err >= 0);
   gid = H5Utilities::openHDF5Object(file_id, "/group1/group2");
   BOOST_REQUIRE(gid >= 0);
   err = H5Utilities::closeHDF5Object(gid);

   bool success = H5Utilities::probeForAttribute(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", "ScalarAttribute<H5T_NATIVE_INT32>" );
   BOOST_REQUIRE(success == true);

   success = H5Utilities::probeForAttribute(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", "ScalarAttribute<>" );
   BOOST_REQUIRE(success == false);

   std::list<std::string> attributes;
   err = H5Utilities::getAllAttributeNames(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", attributes);
   BOOST_REQUIRE(err >= 0);
   BOOST_REQUIRE(attributes.size() == 50);

   dsetId = H5Utilities::openHDF5Object(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>");
   BOOST_REQUIRE(dsetId > 0);
   attributes.clear();
   err = H5Utilities::getAllAttributeNames(dsetId, attributes);
   BOOST_REQUIRE(err >= 0);
   BOOST_REQUIRE(attributes.size() == 50);
   err = H5Utilities::closeHDF5Object(dsetId);
   BOOST_REQUIRE(err >= 0);
//
   MXAAbstractAttributes allAttributes;
   err = H5Utilities::readAllAttributes(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", allAttributes );
   BOOST_REQUIRE(err >= 0);
   BOOST_REQUIRE(allAttributes.size() == 50);
   BOOST_REQUIRE(H5Fclose(file_id) >= 0);
}

// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) {
  boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "H5Utilities Tests" );
  test->add( BOOST_TEST_CASE( &H5UtilitiesTest), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);
  return test;
}


