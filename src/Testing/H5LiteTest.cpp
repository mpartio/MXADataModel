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


#include "HDF5/H5Lite.h"
#include "Headers/MXATypes.h"

//-- Boost includes
#include <boost/test/unit_test.hpp>

using boost::unit_test::test_suite;


#if defined (_WIN32)
  #define FILE_NAME "C:\\WINDOWS\\Temp\\test_lite1.h5"
  #define FILE_NAME2 "C:\\WINDOWS\\Temp\\test_lite2.h5"
#else 
  #define FILE_NAME "/tmp/test_lite1.h5"
  #define FILE_NAME2 "/tmp/test_lite2.h5"
#endif


#define DSET0_NAME "2D int array"
#define DSET1_NAME "dataset char"
#define DSET2_NAME "dataset short"
#define DSET3_NAME "dataset int"
#define DSET4_NAME "dataset long"
#define DSET5_NAME "dataset float"
#define DSET6_NAME "dataset double"
#define DSET7_NAME "dataset string"

#define DIM 6

//TODO: Add tests for Vector Attributes
//TODO: Add test for raw pointer writeDataset template methods

using namespace MXATypes;

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
template <typename T>
herr_t testMakeDataset(hid_t &file_id, std::string name, hid_t &dataType, T type ) {
  herr_t err = -1;
  hsize_t dimx = 2;
  hsize_t dimy = 3;
  // Create the Dimensions
  std::vector<hsize_t> dims;
  dims.push_back(dimx);
  dims.push_back(dimy);
  
  /* Make dataset char */
  std::vector<T> data;
  for (int i = 0; i < DIM; ++i) {
    data.push_back(type);
  }
  std::cout << "Write/Read->Vector: " << name;
  err = H5Lite::writeDataset( file_id, name, dims, data, dataType );
  if (err<0) {
     std::cout << " - Failed - Could not write data to file." << std::endl;
     return err;; 
  }

  /* Now read the dataset into another vector and compare */
  std::vector<T> rData; //Create a vector to hold the data.
  err = H5Lite::readDataset( file_id, name, rData, dataType);
  if (err<0) {
    std::cout << " - Failed - Error Reading Data from File" << std::endl;
    return err;; 
  }
  if ( data == rData) {
    err = 0;
    std::cout << " - Passed" << std::endl;
  } else {
    std::cout << " - Failed - Data Read from the file does not match the stored data." << std::endl;
    err = -1;
  }
 
  std::cout << "Write/Read->Scalar: " << name;
  name += "_Single";
  T sData = type;
  err = H5Lite::writeDataset(file_id, name, sData, dataType);
  if (err<0) {
     std::cout << " - Failed - Could not write single value data to file." << std::endl;
     return err;; 
  }
  /* Now read the dataset into another vector and compare */
  T srData; //Create a vector to hold the data.
  err = H5Lite::readDataset( file_id, name, srData, dataType);
  if (err<0) {
    std::cout << " - Failed - Error Reading Single Value Data from File" << std::endl;
    return err;; 
  }
  if ( sData == srData) {
    err = 0;
    std::cout << " - Passed" << std::endl;
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
  std::cout << "Write/Read->H5T_STR_NULLTERM: ";
  err = H5Lite::writeDataset(file_id, dsetName, strData);
  if (err<0) {
     std::cout << " - Failed - Could not write data to file." << std::endl;
     return err;; 
  }
  std::string rData;
  err = H5Lite::readDataset(file_id, dsetName, rData);
  if ( strData == rData) {
    err = 0;
    std::cout << " - Passed" << std::endl;
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
  std::cout << "Wrote/Read" << std::endl;
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
herr_t testMakeAttribute(hid_t &file_id, std::string objName, hid_t &dataType, T type ) {
  std::string testAttribute("Test_Attribute");
  herr_t err = -1;
  int numElements = 4;
  // Create the Dimensions
  std::vector<hsize_t> dims;
  dims.push_back(numElements);
  
  /* Make dataset */
  std::vector<T> data;
  for (int i = 0; i < numElements; ++i) {
    data.push_back( 0x0F );
  }
  std::cout << "Attribute->Write: " << objName;
  err = H5Lite::writeAttribute( file_id, objName, testAttribute, dims, data, dataType );
  if (err < 0 ) {
    std::cout << " - Failed - Could not Write the Attribute" << std::endl;
    return err;
  }
  
  std::vector<T> rData(numElements, 0); //allocate and zero out the memory
  err = H5Lite::readAttribute(file_id, objName, testAttribute, rData, dataType);
  if ( data == rData ) {
    std::cout << " - Passed" << std::endl;
  } else { 
    std::cout << " - Failed - Written and Read data are not the same." << std::endl;
    printData(data, rData);
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
  int size = strData.size();
  std::cout << "WriteAttribute->H5T_STR_NULLTERM: ";
  err = H5Lite::writeAttribute(file_id, dsetName, testAttribute, strData);
  
  
  std::string rData;
  err = H5Lite::readAttribute(file_id, dsetName, testAttribute, rData);
   size = rData.size();
   if ( strData == rData) {
    std::cout << " - Passed" << std::endl;
  } else { 
    std::cout << " - Failed - Written and Read data are not the same" << std::endl;
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
  herr_t err = H5Gclose(sintGid);
  err = H5Gclose(uintGid);
  // ******************* Test Writing Data *************************************
  std::cout << "\n----------- Testing Writing/Reading of Datasets -----------" << std::endl;
  BOOST_REQUIRE ( testMakeDataset(file_id, "Signed Int/H5T_NATIVE_INT8",  H5T_NATIVE_INT8, Int8Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Unsigned Int/H5T_NATIVE_UINT8", H5T_NATIVE_UINT8, Uint8Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Signed Int/H5T_NATIVE_INT16",   H5T_NATIVE_INT16, Int16Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Unsigned Int/H5T_NATIVE_UINT16", H5T_NATIVE_UINT16, Uint16Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Signed Int/H5T_NATIVE_INT32", H5T_NATIVE_INT32, Int32Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Unsigned Int/H5T_NATIVE_UINT32", H5T_NATIVE_UINT32, Uint32Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Signed Int/H5T_NATIVE_INT64",  H5T_NATIVE_INT64, Int64Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "Unsigned Int/H5T_NATIVE_UINT64", H5T_NATIVE_UINT64, Uint64Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "H5T_NATIVE_FLOAT", H5T_NATIVE_FLOAT, Float32Type) >= 0);
  BOOST_REQUIRE ( testMakeDataset(file_id, "H5T_NATIVE_DOUBLE", H5T_NATIVE_DOUBLE, Float64Type) >= 0);
  BOOST_REQUIRE ( testMakeStringDataset(file_id)  >= 0);
  
  // ******************* Test Writing Attributes *******************************
  std::cout << "\n----------- Testing Writing/Reading of Attributes -----------" << std::endl;
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Signed Int/H5T_NATIVE_INT8",  H5T_NATIVE_INT8, Int8Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Unsigned Int/H5T_NATIVE_UINT8", H5T_NATIVE_UINT8, Uint8Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Signed Int/H5T_NATIVE_INT16",   H5T_NATIVE_INT16, Int16Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Unsigned Int/H5T_NATIVE_UINT16", H5T_NATIVE_UINT16, Uint16Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Signed Int/H5T_NATIVE_INT32", H5T_NATIVE_INT32, Int32Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Unsigned Int/H5T_NATIVE_UINT32", H5T_NATIVE_UINT32, Uint32Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Signed Int/H5T_NATIVE_INT64",  H5T_NATIVE_INT64, Int64Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "Unsigned Int/H5T_NATIVE_UINT64", H5T_NATIVE_UINT64, Uint64Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "H5T_NATIVE_FLOAT", H5T_NATIVE_FLOAT, Float32Type) >= 0);
  BOOST_REQUIRE ( testMakeAttribute(file_id, "H5T_NATIVE_DOUBLE", H5T_NATIVE_DOUBLE, Float64Type) >= 0);
  BOOST_REQUIRE ( testMakeStringAttribute(file_id)  >= 0);

  /* Close the file. */
  H5Fclose( file_id );
  std::cout << "Testing Complete" << std::endl;
}


// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
    test_suite* test= BOOST_TEST_SUITE( "H5Lite Tests" );
    //test->add( new DataModelTest () );
    
    test->add( BOOST_TEST_CASE( &H5LiteTest), 0);

    return test; 
}

