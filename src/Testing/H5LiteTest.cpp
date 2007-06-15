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
#include "Headers/LogTime.h"
#include "Headers/MXATypes.h"
#include "HDF5/H5Lite.h"

//-- Boost includes
#include <boost/test/unit_test.hpp>

//using boost::unit_test::test_suite;
using namespace boost::unit_test;


#if defined (_WIN32)
  #define FILE_NAME "C:\\WINDOWS\\Temp\\test_lite1.h5"
  #define FILE_NAME2 "C:\\WINDOWS\\Temp\\test_lite2.h5"
#else 
  #define FILE_NAME "/tmp/test_lite1.h5"
  #define FILE_NAME2 "/tmp/test_lite2.h5"
#endif


#define DSET0_NAME "2D int32 array"
#define DSET1_NAME "dataset char"
#define DSET2_NAME "dataset short"
#define DSET3_NAME "dataset int"
#define DSET4_NAME "dataset long"
#define DSET5_NAME "dataset float"
#define DSET6_NAME "dataset double"
#define DSET7_NAME "dataset string"

#define DIM 6


using namespace MXATypes;

test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] );
herr_t testMakeStringDataset(hid_t file_id);
herr_t testMakeStringAttribute(hid_t file_id);
void H5LiteTest();

// -----------------------------------------------------------------------------
//  Uses Raw Pointers to save data to the data file
// -----------------------------------------------------------------------------
template <typename T>
herr_t testMakeDatasetFromPointer(hid_t &file_id, std::string name, T type ) {
  herr_t err = 1;
  int32 dimx = 2;
  int32 dimy = 3;
  int32 rank = 2;
  // Create the Dimensions
  uint64 dims[2];
  dims[0] = dimx; dims[1] = dimy;
  
  /* Make dataset char */
  int32 tSize = dimx * dimy;
 // T data[dimx*dimy];
  std::vector<T> data(tSize);
  for (int32 i = 0; i < tSize; ++i) {
    data[i] = static_cast<T>(type * i * 5);
  }
  //std::cout << "Write/Read->Vector: " << name;
  err = H5Lite::writePointerDataset( file_id, name, rank, dims, &(data.front()) );
  if (err<0) {
     std::cout << " - Failed - Could not write data to file." << std::endl;
     return err;; 
  }

  /* Now read the dataset into another vector and compare */

  //T rData[dimx*dimy];
  std::vector<T> rData(tSize);
  err = H5Lite::readPointerDataset( file_id, name, &(rData.front() ) );
  if (err<0) {
    std::cout << " - Failed - Error Reading Data from File" << std::endl;
    return err;; 
  }
  
  if ( (std::memcmp(&(data.front()), &(rData.front() ), tSize)) == 0) {
    err = 0;
   // std::cout << " - Passed" << std::endl;
  } else {
    std::cout << " - Failed - Data Read from the file does not match the stored data." << std::endl;
    err = -1;
  }

  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template <typename T>
herr_t testMakeDataset(hid_t &file_id, std::string name, T type ) {
  herr_t err = -1;
  hsize_t dimx = 2;
  hsize_t dimy = 3;
  // Create the Dimensions
  std::vector<uint64> dims;
  dims.push_back(dimx);
  dims.push_back(dimy);
  
  /* Make dataset char */
  std::vector<T> data;
  for (int i = 0; i < DIM; ++i) {
    data.push_back(type);
  }
  //std::cout << "Write/Read->Vector: " << name;
  err = H5Lite::writeVectorDataset( file_id, name, dims, data );
  if (err<0) {
     std::cout << " - Failed - Could not write data to file." << std::endl;
     return err;; 
  }

  /* Now read the dataset into another vector and compare */
  std::vector<T> rData; //Create a vector to hold the data.
  err = H5Lite::readVectorDataset( file_id, name, rData);
  if (err<0) {
    std::cout << " - Failed - Error Reading Data from File" << std::endl;
    return err;
  }
  if ( data == rData) {
    err = 0;
    //std::cout << " - Passed" << std::endl;
  } else {
  #if 0
  	std::cout << "\nRead Data: " << std::endl;
  	for (uint32 i = 0; i < rData.size(); ++i)
  	{
  		std::cout << rData[i] << " ";
  	}
  	
  	std::cout << "\nWritten Data: " << std::endl;
  	for (uint32 i = 0; i < data.size(); ++i)
  	{
  		std::cout << data[i] << " ";
  	}
  	#endif
    std::cout << " - Failed - Data Read from the file does not match the stored data." << std::endl;
    err = -1;
  }
 
 // std::cout << "Write/Read->Scalar: " << name;
  name += "_Single";
  T sData = type;
  err = H5Lite::writeScalarDataset(file_id, name, sData);
  if (err<0) {
     std::cout << " - Failed - Could not write single value data to file." << std::endl;
     return err;; 
  }
  /* Now read the dataset into another vector and compare */
  T srData; //Create something to hold the data.
  err = H5Lite::readScalarDataset( file_id, name, srData);
  if (err<0) {
    std::cout << " - Failed - Error Reading Single Value Data from File" << std::endl;
    return err;; 
  }
  if ( sData == srData) {
    err = 0;
   // std::cout << " - Passed" << std::endl;
  } else {
    std::cout << " - Failed - Data Read from the file does not match the stored data." << std::endl;
    err = -1;
  }
  return err;
}
 
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t testMakeStringDataset(hid_t file_id) {
  herr_t err = 0;
  std::string dsetName ("H5T_STR_NULLTERM");
  std::string strData ("Some data for the File");
  //std::cout << "Write/Read->H5T_STR_NULLTERM: ";
  err = H5Lite::writeStringDataset(file_id, dsetName, strData);
  if (err<0) {
     std::cout << " - Failed - Could not write data to file." << std::endl;
     return err;; 
  }
  std::string rData;
  err = H5Lite::readStringDataset(file_id, dsetName, rData);
  if ( strData == rData) {
    err = 0;
   // std::cout << " - Passed" << std::endl;
  } else {
    std::cout << " - Failed - Data Read from the file does not match the stored data." << std::endl;
    err = -1;
  }
  return err;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template<typename T>
void printData(std::vector<T> &data, std::vector<T> &rData) {
 // std::cout << "Wrote/Read" << std::endl;
  for (typename std::vector<T>::iterator iter = data.begin(); iter < data.end(); ++iter ) {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;
   
  for (typename std::vector<T>::iterator iter = rData.begin(); iter < rData.end(); ++iter ) {
    std::cout << *iter << " ";
  }
   
  std::cout << std::endl;
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template <typename T>
herr_t testMakeAttribute(hid_t &file_id, std::string objName, T type ) {
  std::string testAttribute("Test_Attribute");
  herr_t err = -1;
  int32 numElements = 4;
  // Create the Dimensions
  std::vector<uint64> dims;
  dims.push_back(numElements);
  
  /* Make dataset */
  std::vector<T> data;
  T _dat = 0x0f;
  for (int i = 0; i < numElements; ++i) {
    data.push_back( _dat );
  }
  //std::cout << "Attribute->Write: " << objName;
  err = H5Lite::writeVectorAttribute( file_id, objName, testAttribute, dims, data );
  if (err < 0 ) {
    std::cout << " - Failed - Could not Write the Attribute: " << objName << std::endl;
    return err;
  }
  
  std::vector<T> rData(numElements, 0); //allocate and zero out the memory
  err = H5Lite::readVectorAttribute(file_id, objName, testAttribute, rData);
  if ( data == rData ) {
 //   std::cout << " - Passed" << std::endl;
    err = 0;
  } else { 
    std::cout << " - Failed - Written and Read data are not the same." << std::endl;
    printData(data, rData);
    err = -1;
  }
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t testMakeStringAttribute(hid_t file_id) {
  herr_t err = 0;
  std::string dsetName ("H5T_STR_NULLTERM");
  std::string testAttribute("Test_Attribute");
  std::string strData ("Some data for the Attribute");

 // std::cout << "WriteAttribute->H5T_STR_NULLTERM: ";
  err = H5Lite::writeStringAttribute(file_id, dsetName, testAttribute, strData);
  
  std::string rData;
  err = H5Lite::readStringAttribute(file_id, dsetName, testAttribute, rData);
  //int32 size = rData.size();
   if ( strData == rData) {
   // std::cout << " - Passed" << std::endl;
     err = 0;
  } else { 
    std::cout << " - Failed - Written and Read data are not the same" << std::endl;
    err = -1;
  }
   return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5LiteTest() {
 // herr_t err = -1;
  hid_t   file_id;
  /* Create a new file using default properties. */
  file_id = H5Fcreate( FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
  //Create the Extra Groups
  hid_t sintGid = H5Gcreate(file_id, "Signed Int", 0);
  hid_t uintGid = H5Gcreate(file_id, "Unsigned Int", 0);
  hid_t pointer = H5Gcreate(file_id, "Pointer", 0);
  herr_t err = H5Gclose(sintGid);
  if (err < 0)
  {
    std::cout << "Error closing Group sintGid" << std::endl;
    return;
  }
  err = H5Gclose(uintGid);
  if (err < 0)
  {
    std::cout << "Error closing Group uintGid" << std::endl;
    return;
  }
  err = H5Gclose(pointer);
  if (err < 0)
  {
    std::cout << "Error closing Group c99Gid" << std::endl;
    return;
  }
  
  // ******************* Test Writing Data *************************************
//  std::cout << logTime() << "----------- Testing Writing/Reading of Datasets -----------" << std::endl;
  BOOST_REQUIRE ( testMakeDataset(file_id, "Signed Int/H5T_NATIVE_INT64",   Int64Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Unsigned Int/H5T_NATIVE_UINT64",  Uint64Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Signed Int/H5T_NATIVE_INT8",   Int8Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Unsigned Int/H5T_NATIVE_UINT8",  Uint8Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Signed Int/H5T_NATIVE_INT16",    Int16Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Unsigned Int/H5T_NATIVE_UINT16",  Uint16Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Signed Int/H5T_NATIVE_INT32",  Int32Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Unsigned Int/H5T_NATIVE_UINT32",  Uint32Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "H5T_NATIVE_FLOAT",  Float32Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "H5T_NATIVE_DOUBLE",  Float64Type) >= 0);
  BOOST_REQUIRE ( testMakeStringDataset(file_id)  >= 0);
 

 // std::cout << logTime() << "----------- Testing Writing/Reading of Datasets using Raw Pointers -----------" << std::endl; 
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/Signed Int-H5T_NATIVE_INT8 Ptr", Int8Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/Unsigned Int-H5T_NATIVE_UINT8 Ptr", Uint8Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/Signed Int-H5T_NATIVE_INT16 Ptr",   Int16Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/Unsigned Int-H5T_NATIVE_UINT16 Ptr", Uint16Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/Signed Int-H5T_NATIVE_INT32 Ptr", Int32Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/Unsigned Int-H5T_NATIVE_UINT32 Ptr", Uint32Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/Signed Int-H5T_NATIVE_INT64 Ptr",  Int64Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/Unsigned Int-H5T_NATIVE_UINT64 Ptr", Uint64Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/H5T_NATIVE_FLOAT Ptr", Float32Type) >= 0);
  BOOST_REQUIRE ( testMakeDatasetFromPointer(file_id, "Pointer/H5T_NATIVE_DOUBLE Ptr", Float64Type) >= 0);

  // ******************* Test Writing Attributes *******************************
 // std::cout << logTime() << "----------- Testing Writing/Reading of Attributes -----------" << std::endl;
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Signed Int/H5T_NATIVE_INT8",   Int8Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Unsigned Int/H5T_NATIVE_UINT8",  Uint8Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Signed Int/H5T_NATIVE_INT16",    Int16Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Unsigned Int/H5T_NATIVE_UINT16",  Uint16Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Signed Int/H5T_NATIVE_INT32",  Int32Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Unsigned Int/H5T_NATIVE_UINT32",  Uint32Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Signed Int/H5T_NATIVE_INT64",   Int64Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Unsigned Int/H5T_NATIVE_UINT64",  Uint64Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "H5T_NATIVE_FLOAT",  Float32Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "H5T_NATIVE_DOUBLE",  Float64Type) >= 0);
  BOOST_REQUIRE ( testMakeStringAttribute(file_id)  >= 0);

  
  /* Close the file. */
  H5Fclose( file_id );
 // std::cout << logTime() << "Testing Complete" << std::endl;
}


// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) {
    //test_suite* test= BOOST_TEST_SUITE( "H5Lite Tests" );
    framework::master_test_suite().p_name.value = "H5Lite Tests";
    // register the test case in test tree and specify number of expected failures so
    // this example will pass at runtime. We expect 2 errors: one from failed check and 
    // one from memory acces violation
    framework::master_test_suite().add( BOOST_TEST_CASE( &H5LiteTest), 0);
    return 0; 
}

