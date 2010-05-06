///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////

//--MXA Includes
#include <Tests/MXAUnitTestDataFileLocations.h>
#include <MXA/MXAConfiguration.h>
#include <MXA/Common/LogTime.h>
#include <MXA/MXATypes.h>
#include <MXA/Common/Win32Defines.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/Utilities/MXADir.h>
#include "UnitTestSupport.hpp"

//-- C++ includes
#include <list>
#include <string>



#define DIM 6
#define DIM0 4
#define DIM1 3
#define DIM2 2

#define RANK_1D 1
#define RANK_2D 2
#define RANK_3D 3

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
  MXADir::remove(MXAUnitTest::H5UtilTest::FileName);
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
  MXA_REQUIRE(attributeKey.empty() == false);
  attributeKey = "1DArrayAttribute<" + attributeKey + ">";
  int32_t rank = 1;
  T data[DIM0];
  for(int i = 0; i < DIM0; ++i)
  {
    data[i] = (T)(i);
  }
  uint64_t dims[1];
  dims[0] = DIM0;

  err = H5Lite::writePointerAttribute<T>(file_id, dsetName, attributeKey, rank, dims, (T*)data);
  MXA_REQUIRE(err >= 0);
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
  MXA_REQUIRE(attributeKey.empty() == false);

  attributeKey = "2DArrayAttribute<" + attributeKey + ">";
  int32_t rank = RANK_2D;
  T data[DIM0][DIM1];
  for(int i = 0; i < DIM0; ++i)
  {
    for (int j = 0; j < DIM1; ++j) {
      data[i][j] = (T)(i * j);
    }
  }
  uint64_t dims[RANK_2D];
  dims[0] = DIM0;
  dims[1] = DIM1;
  err = H5Lite::writePointerAttribute<T>(file_id, dsetName, attributeKey, rank, dims, (T*)data);
  MXA_REQUIRE(err >= 0);
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
  MXA_REQUIRE(attributeKey.empty() == false);
  attributeKey = "3DArrayAttribute<" + attributeKey + ">";
  int32_t rank = RANK_3D;
  T data[DIM0][DIM1][DIM2];
  for(int i = 0; i < DIM0; ++i) {
    for (int j = 0; j < DIM1; ++j) {
      for (int k = 0; k < DIM2; ++k) {
        data[i][j][k] = (T)(i * j * k);
      }
    }
  }
  uint64_t dims[RANK_3D];
  dims[0] = DIM0;
  dims[1] = DIM1;
  dims[2] = DIM2;
  err = H5Lite::writePointerAttribute<T>(file_id, dsetName, attributeKey, rank, dims, (T*)data);
  MXA_REQUIRE(err >= 0);
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
  MXA_REQUIRE(attributeKey.empty() == false);
  attributeKey = "VectorAttribute<" + attributeKey + ">";

  int32_t numElements = DIM0;
  std::vector<uint64_t> dims (1, DIM0);

  /* Make dataset */
  std::vector<T> data (DIM0, 0);
  for (int i = 0; i < numElements; ++i) {
    data[i] = (T)(i);
  }
  //std::cout << "Attribute->Write: " << objName;
  err = H5Lite::writeVectorAttribute( file_id, dsetName, attributeKey, dims, data );
  MXA_REQUIRE(err >= 0);
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
  MXA_REQUIRE(attributeKey.empty() == false);
  attributeKey = "ScalarAttribute<" + attributeKey + ">";
  err = H5Lite::writeScalarAttribute(file_id, dsetName, attributeKey, value);
  MXA_REQUIRE(err >= 0);
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
  int32_t rank = 2;
  // Create the Dimensions
  uint64_t dims[2];
  dims[0] = DIM0; dims[1] = DIM1;

  /* Make dataset char */
  int32_t tSize = dims[0] * dims[1];
 // T data[dimx*dimy];
  std::vector<T> data(tSize);
  for (int32_t i = 0; i < tSize; ++i) {
    data[i] = static_cast<T>( i * 5);
  }

  std::string dsetName = H5Lite::HDFTypeForPrimitiveAsStr(value);
  dsetName = "Pointer2DArrayDataset<" + dsetName + ">";
  std::cout << "Running " << dsetName << " ... ";
  err = H5Lite::writePointerDataset( file_id, dsetName, rank, dims, &(data.front()) );
  MXA_REQUIRE(err >= 0);
  err = testWritePointer1DArrayAttribute<int8_t>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<uint8_t>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<int16_t>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<uint16_t>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<int32_t>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<uint32_t>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<int64_t>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<uint64_t>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<float32>(file_id, dsetName);
  err = testWritePointer1DArrayAttribute<float64>(file_id, dsetName);

  err = testWritePointer2DArrayAttribute<int8_t>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<uint8_t>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<int16_t>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<uint16_t>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<int32_t>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<uint32_t>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<int64_t>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<uint64_t>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<float32>(file_id, dsetName);
  err = testWritePointer2DArrayAttribute<float64>(file_id, dsetName);

  err = testWritePointer3DArrayAttribute<int8_t>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<uint8_t>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<int16_t>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<uint16_t>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<int32_t>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<uint32_t>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<int64_t>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<uint64_t>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<float32>(file_id, dsetName);
  err = testWritePointer3DArrayAttribute<float64>(file_id, dsetName);

  err = testWriteVectorAttribute<int8_t>(file_id, dsetName);
  err = testWriteVectorAttribute<uint8_t>(file_id, dsetName);
  err = testWriteVectorAttribute<int16_t>(file_id, dsetName);
  err = testWriteVectorAttribute<uint16_t>(file_id, dsetName);
  err = testWriteVectorAttribute<int32_t>(file_id, dsetName);
  err = testWriteVectorAttribute<uint32_t>(file_id, dsetName);
  err = testWriteVectorAttribute<int64_t>(file_id, dsetName);
  err = testWriteVectorAttribute<uint64_t>(file_id, dsetName);
  err = testWriteVectorAttribute<float32>(file_id, dsetName);
  err = testWriteVectorAttribute<float64>(file_id, dsetName);

  err = testWriteScalarAttribute<int8_t>(file_id, dsetName);
  err = testWriteScalarAttribute<uint8_t>(file_id, dsetName);
  err = testWriteScalarAttribute<int16_t>(file_id, dsetName);
  err = testWriteScalarAttribute<uint16_t>(file_id, dsetName);
  err = testWriteScalarAttribute<int32_t>(file_id, dsetName);
  err = testWriteScalarAttribute<uint32_t>(file_id, dsetName);
  err = testWriteScalarAttribute<int64_t>(file_id, dsetName);
  err = testWriteScalarAttribute<uint64_t>(file_id, dsetName);
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
   file_id = H5Fcreate( MXAUnitTest::H5UtilTest::FileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
   MXA_REQUIRE(file_id > 0);

   // std::cout << logTime() << "----------- Testing Writing/Reading of Datasets using Raw Pointers -----------" << std::endl;
   MXA_REQUIRE ( testWritePointer2DArrayDataset<int32_t>(file_id) >= 0);

   hid_t dsetId = H5Utilities::openHDF5Object(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>");
   MXA_REQUIRE(dsetId > 0);

   MXA_REQUIRE( H5Utilities::isGroup(file_id, "/") == true);
   MXA_REQUIRE( H5Utilities::isGroup(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>") == false);

   std::string objName;
   int32_t err = H5Utilities::objectNameAtIndex(file_id, 0, objName);
   MXA_REQUIRE(objName.compare("Pointer2DArrayDataset<H5T_NATIVE_INT32>") == 0);

   hid_t objType = -1;
   err = H5Utilities::getObjectType(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", &objType);
   MXA_REQUIRE(objType == H5G_DATASET);
   MXA_REQUIRE(err >= 0);

   err = H5Utilities::getObjectType(file_id, "/", &objType);
   MXA_REQUIRE(objType == H5G_GROUP);
   MXA_REQUIRE(err >= 0);

   std::string objPath = H5Utilities::getObjectPath(dsetId, false);
   MXA_REQUIRE(objPath.compare("Pointer2DArrayDataset<H5T_NATIVE_INT32>") == 0);

   err = H5Utilities::closeHDF5Object(dsetId);
   MXA_REQUIRE(err >= 0);

   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 1", file_id) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 2/", file_id) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("Test Path 3/", file_id) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/", file_id) < 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 4/Test Path 7", file_id) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 5/Test Path 8/", file_id) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("Test Path 6/Test Path 9/", file_id) >= 0);

   hid_t grpId = H5Utilities::openHDF5Object(file_id, "Test Path 1");
   MXA_REQUIRE(grpId > 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 1", grpId) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 2/", grpId) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("Test Path 3/", grpId) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/", grpId) < 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 4/Test Path 7", grpId) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("/Test Path 5/Test Path 8/", grpId) >= 0);
   MXA_REQUIRE ( H5Utilities::createGroupsFromPath("Test Path 6/Test Path 9/", grpId) >= 0);
   err = H5Gclose(grpId);
   MXA_REQUIRE(err >= 0);

   hid_t gid = H5Utilities::createGroup(file_id, "test group");
   MXA_REQUIRE(gid >= 0);
   err = H5Utilities::closeHDF5Object(gid);

   std::list<std::string> groups;
   err = H5Utilities::getGroupObjects(file_id, H5Utilities::MXA_ANY, groups);
   MXA_REQUIRE(err >= 0);
   MXA_REQUIRE( groups.size() == 8);


   err = H5Utilities::createGroupsForDataset("/group1/group2/group3/data", file_id);
   MXA_REQUIRE(err >= 0);
   gid = H5Utilities::openHDF5Object(file_id, "/group1/group2");
   MXA_REQUIRE(gid >= 0);
   err = H5Utilities::closeHDF5Object(gid);

   bool success = H5Utilities::probeForAttribute(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", "ScalarAttribute<H5T_NATIVE_INT32>" );
   MXA_REQUIRE(success == true);

   success = H5Utilities::probeForAttribute(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", "ScalarAttribute<>" );
   MXA_REQUIRE(success == false);

   std::list<std::string> attributes;
   err = H5Utilities::getAllAttributeNames(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", attributes);
   MXA_REQUIRE(err >= 0);
   MXA_REQUIRE(attributes.size() == 50);

   dsetId = H5Utilities::openHDF5Object(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>");
   MXA_REQUIRE(dsetId > 0);
   attributes.clear();
   err = H5Utilities::getAllAttributeNames(dsetId, attributes);
   MXA_REQUIRE(err >= 0);
   MXA_REQUIRE(attributes.size() == 50);
   err = H5Utilities::closeHDF5Object(dsetId);
   MXA_REQUIRE(err >= 0);
//
   MXAAbstractAttributes allAttributes;
   err = H5Utilities::readAllAttributes(file_id, "Pointer2DArrayDataset<H5T_NATIVE_INT32>", allAttributes );
   MXA_REQUIRE(err >= 0);
   MXA_REQUIRE(allAttributes.size() == 50);
   MXA_REQUIRE(H5Fclose(file_id) >= 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void StressTestCreateGroups()
{
  std::cout << logTime() << " Starting StressTestCreateGroups()" << std::endl;
  char path[64];
  ::memset(path, 0, 64);
  int err = 0;
  hid_t   file_id;
  hid_t   grpId;

  /* Create a new file using default properties. */
  file_id = H5Fcreate( MXAUnitTest::H5UtilTest::GroupTest.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
  MXA_REQUIRE(file_id > 0);


  for (int i = 0; i < 100; ++i) {
    std::cout << logTime() << "Outer Loop: " << i << std::endl;
//    err = H5Fclose(file_id);
//    MXA_REQUIRE(err >= 0);
//    file_id = H5Fopen(MXAUnitTest::H5UtilTest::GroupTest.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    ::memset(path, 0, 64);
    snprintf(path, 64, "/%03d", i);
    grpId = H5Utilities::createGroup(file_id, path);
    MXA_REQUIRE(grpId > 0);
    err = H5Gclose(grpId);
    MXA_REQUIRE(err >= 0);


    for (int j = 0; j < 100; ++j) {

      snprintf(path, 64, "/%03d/%03d", i, j);
      grpId = H5Utilities::createGroup(file_id, path);
      MXA_REQUIRE(grpId > 0);
      MXA_REQUIRE(grpId > 0);
      err = H5Gclose(grpId);
      MXA_REQUIRE(err >= 0);

      for (int k = 0; k < 100; ++k) {

        snprintf(path, 64, "/%03d/%03d/%03d", i, j, k);
        grpId = H5Utilities::createGroup(file_id, path);
        MXA_REQUIRE(grpId >= 0);
        MXA_REQUIRE(grpId > 0);
        err = H5Gclose(grpId);
        MXA_REQUIRE(err >= 0);
      }
    }
  }
  err = H5Fclose(file_id);
  MXA_REQUIRE(err >= 0);
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
  int err = EXIT_SUCCESS;

  MXA_REGISTER_TEST( H5UtilitiesTest() );
  MXA_REGISTER_TEST( RemoveTestFiles() );
  PRINT_TEST_SUMMARY();
  return err;
}


