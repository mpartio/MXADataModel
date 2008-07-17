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
#include <MXAConfiguration.h>
#include <Common/LogTime.h>
#include <Common/MXATypes.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Utilities.h>
#include <HDF5/H5Data2DArray.hpp>
#include <Testing/TestDataFileLocations.h>

//-- Boost Test Headers
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

//-- Boost Filesystem Headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>



#define DSET0_NAME "2D int32 array"
#define DSET1_NAME "dataset char"
#define DSET2_NAME "dataset short"
#define DSET3_NAME "dataset int"
#define DSET4_NAME "dataset long"
#define DSET5_NAME "dataset float"
#define DSET6_NAME "dataset double"
#define DSET7_NAME "dataset string"

#define DIM 6
#define DIM0 4
#define DIM1 3
#define DIM2 2

#define RANK_1D 1
#define RANK_2D 2
#define RANK_3D 3


/* Methods to test
 *
 * HDFTypeFromString
 * HDFTypeForPrimitiveAsStr
 * HDFTypeForPrimitive
 * writeVectorDataset - DONE
 * writePointerDataset - DONE
 * writeScalarDataset - DONE
 * writeStringDataset - DONE
 * writeStringDataset - DONE
 * writePointerAttribute - DONE
 * writeVectorAttribute - DONE
 * writeStringAttribute - DONE
 * writeStringAttribute - DONE
 * writeStringAttributes - DONE
 * writeScalarAttribute - DONE
 * readPointerDataset - DONE
 * readVectorDataset - DONE
 * readScalarDataset - DONE
 * readStringDataset - DONE
 * readStringDataset - DONE
 * readVectorAttribute - DONE
 * readScalarAttribute - DONE
 * readPointerAttribute - DONE
 * readStringAttribute - DONE
 * readStringAttribute - DONE
 * getAttributeNDims - DONE
 * getDatasetNDims - DONE
 * getDatasetType
 * getDatasetInfo - DONE
 * getAttributeInfo - DONE
 */

using namespace MXATypes;


herr_t testMakeStringDataset(hid_t file_id);
herr_t testMakeStringAttribute(hid_t file_id);
void H5LiteTest();
MXADataModelPtr createModelTemplate();
template<typename T>
herr_t testWritePointer1DArrayAttribute(hid_t file_id, const std::string &dsetName);



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
#if REMOVE_TEST_FILES
  BOOST_REQUIRE ( boost::filesystem::remove(H5LITE_TEST_FILE_NAME) == true );
 // BOOST_REQUIRE ( boost::filesystem::remove(H5LITE_TEST_LARGE_FILE) == true );
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
herr_t testReadPointer1DArrayAttribute(hid_t file_id, const std::string &dsetName)
{
  T value = 0x0;
  herr_t err = -1;
  herr_t retErr = err;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "1DArrayAttribute<" + attributeKey + ">";
  std::vector<T> referenceData(DIM0, 0);
  for(int i = 0; i < DIM0; ++i)
  {
    referenceData[i] = (T)(i);
  }

  H5T_class_t attr_type;
  size_t attr_size;
  hid_t typeId = -1;
  std::vector<uint64> dims;  //Reusable for the loop
  err = H5Lite::getAttributeInfo(file_id, dsetName, attributeKey, dims, attr_type, attr_size, typeId);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(dims.size() == 1);
  BOOST_REQUIRE(attr_size == sizeof(T));
  hid_t rank = 0;
  err = H5Lite::getAttributeNDims(file_id, dsetName, attributeKey, rank);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(rank == 1);
  CloseH5T(typeId, err, retErr); //Close the H5A type Id that was retrieved during the loop
  mxaIdType numElements = 1;
  for (std::vector<uint64>::size_type i = 0; i < dims.size(); ++i)
  {
    numElements = numElements * dims[i];
  }
  std::vector<T> data(numElements, 0);
  err = H5Lite::readPointerAttribute<T>(file_id, dsetName, attributeKey, &(data.front() ) );
  BOOST_REQUIRE(err >= 0);

  BOOST_REQUIRE (data == referenceData);
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
herr_t testReadPointer2DArrayAttribute(hid_t file_id, const std::string &dsetName)
{
  T value = 0x0;
  herr_t err = -1;
  herr_t retErr = err;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "2DArrayAttribute<" + attributeKey + ">";
  T referenceData[DIM0][DIM1];
  for(int i = 0; i < DIM0; ++i)
  {
    for (int j = 0; j < DIM1; ++j) {
      referenceData[i][j] = (T)(i * j);
    }
  }
  H5T_class_t attr_type;
  size_t attr_size;
  hid_t typeId = -1;
  std::vector<uint64> dims;  //Reusable for the loop
  err = H5Lite::getAttributeInfo(file_id, dsetName, attributeKey, dims, attr_type, attr_size, typeId);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(dims.size() == 2);
  BOOST_REQUIRE(attr_size == sizeof(T));
  hid_t rank = 0;
  err = H5Lite::getAttributeNDims(file_id, dsetName, attributeKey, rank);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(rank == 2);
  CloseH5T(typeId, err, retErr); //Close the H5A type Id that was retrieved during the loop
  mxaIdType numElements = 1;
  for (std::vector<uint64>::size_type i = 0; i < dims.size(); ++i)
  {
    numElements = numElements * dims[i];
  }
  std::vector<T> data(numElements, 0);
  err = H5Lite::readPointerAttribute<T>(file_id, dsetName, attributeKey, &(data.front() ) );
  BOOST_REQUIRE(err >= 0);

  BOOST_REQUIRE (::memcmp( &(data.front() ), referenceData, sizeof(T)*numElements) == 0);
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
template<typename T>
herr_t testReadPointer3DArrayAttribute(hid_t file_id, const std::string &dsetName)
{
  T value = 0x0;
  herr_t err = -1;
  herr_t retErr = err;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "3DArrayAttribute<" + attributeKey + ">";
  T referenceData[DIM0][DIM1][DIM2];
  for(int i = 0; i < DIM0; ++i) {
    for (int j = 0; j < DIM1; ++j) {
      for (int k = 0; k < DIM2; ++k) {
        referenceData[i][j][k] = (T)(i * j * k);
      }
    }
  }
  H5T_class_t attr_type;
  size_t attr_size;
  hid_t typeId = -1;
  std::vector<uint64> dims;  //Reusable for the loop
  err = H5Lite::getAttributeInfo(file_id, dsetName, attributeKey, dims, attr_type, attr_size, typeId);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(dims.size() == 3);
  BOOST_REQUIRE(attr_size == sizeof(T));
  hid_t rank = 0;
  err = H5Lite::getAttributeNDims(file_id, dsetName, attributeKey, rank);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(rank == 3);
  CloseH5T(typeId, err, retErr); //Close the H5A type Id that was retrieved during the loop
  mxaIdType numElements = 1;
  for (std::vector<uint64>::size_type i = 0; i < dims.size(); ++i)
  {
    numElements = numElements * dims[i];
  }
  std::vector<T> data(numElements, 0);
  err = H5Lite::readPointerAttribute<T>(file_id, dsetName, attributeKey, &(data.front() ) );
  BOOST_REQUIRE(err >= 0);

  BOOST_REQUIRE (::memcmp( &(data.front() ), referenceData, sizeof(T)*numElements) == 0);
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
template <typename T>
herr_t testReadVectorAttribute(hid_t file_id, std::string dsetName )
{
  T value = 0x0;
  herr_t err = -1;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "VectorAttribute<" + attributeKey + ">";

  int32 numElements = DIM0;
  std::vector<uint64> dims (1, DIM0);

  std::vector<T> data (DIM0, 0);
  for (int i = 0; i < numElements; ++i) {
    data[i] = (T)(i);
  }
  std::vector<T> rData(numElements, 0); //allocate and zero out the memory
  err = H5Lite::readVectorAttribute(file_id, dsetName, attributeKey, rData);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE( data == rData );
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
//
// -----------------------------------------------------------------------------
template<typename T>
herr_t testReadScalarAttribute(hid_t file_id, const std::string &dsetName)
{
  T value = 0x0F;
  T refValue = value;
  herr_t err = -1;
  std::string attributeKey = H5Lite::HDFTypeForPrimitiveAsStr(value);
  BOOST_REQUIRE(attributeKey.empty() == false);
  attributeKey = "ScalarAttribute<" + attributeKey + ">";

  err = H5Lite::readScalarAttribute(file_id, dsetName, attributeKey, value);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(refValue == value);
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
template<typename T>
herr_t testWriteVectorDataset(hid_t file_id)
{
  T value = 0x0;
  herr_t err = 1;

  std::vector<hsize_t> dims(RANK_1D, DIM0);
  /* Make dataset char */
  std::vector<T> data(DIM0, 0);
  for (int32 i = 0; i < DIM0; ++i) {
    data[i] = static_cast<T>( i * 5);
  }

  std::string dsetName = H5Lite::HDFTypeForPrimitiveAsStr(value);
  dsetName = "VectorDataset<" + dsetName + ">";
  std::cout << "Running " << dsetName << " ... ";
  err = H5Lite::writeVectorDataset( file_id, dsetName, dims, data );
  BOOST_REQUIRE(err >= 0);

  std::cout << " Passed" << std::endl;
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
herr_t testReadVectorDataset(hid_t file_id)
{
  T value = 0x0;
  herr_t err = 1;

  std::vector<hsize_t> dims(RANK_1D, DIM0);
  std::vector<T> referenceData(DIM0, 0);
  for (int32 i = 0; i < DIM0; ++i) {
    referenceData[i] = static_cast<T>( i * 5);
  }

  std::string dsetName = H5Lite::HDFTypeForPrimitiveAsStr(value);
  std::cout << "Running testReadVectorDataset <" + dsetName + "> ... ";
  dsetName = "VectorDataset<" + dsetName + ">";

  std::vector<T> data;
  err = H5Lite::readVectorDataset( file_id, dsetName, data );
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(data == referenceData);

  std::cout << " Passed" << std::endl;
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
herr_t testWriteScalarDataset(hid_t file_id)
{
  T value = 0x0F;
  herr_t err = 1;

  std::string dsetName = H5Lite::HDFTypeForPrimitiveAsStr(value);
  dsetName = "ScalarDataset<" + dsetName + ">";
  std::cout << "Running " << dsetName << " ... ";
  err = H5Lite::writeScalarDataset(file_id, dsetName, value );
  BOOST_REQUIRE(err >= 0);

  std::cout << " Passed" << std::endl;
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
herr_t testReadScalarDataset(hid_t file_id)
{
  T value = 0x0F;
  T refValue = value;
  herr_t err = 1;

  std::string dsetName = H5Lite::HDFTypeForPrimitiveAsStr(value);
  std::cout << "Running testReadScalarDataset <" + dsetName + "> ... ";
  dsetName = "ScalarDataset<" + dsetName + ">";

  err = H5Lite::readScalarDataset(file_id, dsetName, value );
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(refValue == value );

  std::cout << " Passed" << std::endl;
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t testWriteStringDatasetAndAttributes(hid_t file_id)
{
  std::cout << "Running testWriteStringDatasetAndAttributes ......";
  herr_t err = 0;
  std::string dsetName ("std::string");
  std::string strData ("THIS IS THE DATA");
  err = H5Lite::writeStringDataset(file_id, dsetName, strData);
  BOOST_REQUIRE(err >= 0);
  // Write the Attributes
  std::string attributeKey ("std::string");
  std::string attrData ("THIS IS THE ATTRIBUTE DATA");
  err = H5Lite::writeStringAttribute(file_id, dsetName, attributeKey, attrData);
  BOOST_REQUIRE(err >= 0);

  const char* attrDataPtr = attrData.c_str();
  attributeKey = "c_string";
  err = H5Lite::writeStringAttribute(file_id, dsetName, attributeKey, attrData.size() + 1, attrDataPtr);

  std::map<std::string, std::string> attrMap;
  attrMap[attributeKey] = attrData;
  attributeKey = "std::string";
  attrMap[attributeKey] = attrData;

  // Dataset via pointer (c_str)
  const char* strPtr = strData.c_str();
  dsetName = "c_string";
  err = H5Lite::writeStringDataset(file_id, dsetName, strData.size() + 1, strPtr);
  BOOST_REQUIRE(err >= 0);

  err = H5Lite::writeStringAttributes(file_id, dsetName, attrMap);
  BOOST_REQUIRE(err >= 0);

  std::cout << " Passed" << std::endl;
  return err;
}


// -----------------------------------------------------------------------------
//  Uses Raw Pointers to read data from the data file
// -----------------------------------------------------------------------------
template <typename T>
herr_t testReadPointer2DArrayDataset(hid_t file_id)
{
  // Create the reference data
  T value = 0x0;
  herr_t err = 1;
  std::vector<hsize_t> dims(2, 0);
  dims[0] = DIM0; dims[1] = DIM1;

  int32 tSize = dims[0] * dims[1];
  std::vector<T> referenceData(tSize);
  for (int32 i = 0; i < tSize; ++i) {
    referenceData[i] = static_cast<T>( i * 5);
  }
  // Generate the data set name
  std::string dsetName = H5Lite::HDFTypeForPrimitiveAsStr(value);
  std::cout << "Running testReadPointer2DArrayDataset<" << dsetName << "> ... ";
  dsetName = "Pointer2DArrayDataset<" + dsetName + ">";

  H5T_class_t attr_type;
  size_t attr_size;

  err = H5Lite::getDatasetInfo(file_id, dsetName, dims, attr_type, attr_size);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(dims.size() == 2);
  BOOST_REQUIRE(attr_size == sizeof(T));
  hid_t rank = 0;
  err = H5Lite::getDatasetNDims(file_id, dsetName, rank);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE(rank == 2);
  mxaIdType numElements = 1;
  for (std::vector<hsize_t>::size_type i = 0; i < dims.size(); ++i)
  {
    numElements = numElements * dims[i];
  }

  std::vector<T> data(numElements, 0);
  err = H5Lite::readPointerDataset(file_id, dsetName, &(data.front() ) );
  BOOST_REQUIRE(err >= 0);
  // Compare the data...
  BOOST_REQUIRE(data == referenceData);

  //Read all the attributes
  err = testReadPointer1DArrayAttribute<int8>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<uint8>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<int16>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<uint16>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<int32>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<uint32>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<int64>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<uint64>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<float32>(file_id, dsetName);
  err = testReadPointer1DArrayAttribute<float64>(file_id, dsetName);

  err = testReadPointer2DArrayAttribute<int8>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<uint8>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<int16>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<uint16>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<int32>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<uint32>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<int64>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<uint64>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<float32>(file_id, dsetName);
  err = testReadPointer2DArrayAttribute<float64>(file_id, dsetName);

  err = testReadPointer3DArrayAttribute<int8>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<uint8>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<int16>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<uint16>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<int32>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<uint32>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<int64>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<uint64>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<float32>(file_id, dsetName);
  err = testReadPointer3DArrayAttribute<float64>(file_id, dsetName);
//
  err = testReadVectorAttribute<int8>(file_id, dsetName);
  err = testReadVectorAttribute<uint8>(file_id, dsetName);
  err = testReadVectorAttribute<int16>(file_id, dsetName);
  err = testReadVectorAttribute<uint16>(file_id, dsetName);
  err = testReadVectorAttribute<int32>(file_id, dsetName);
  err = testReadVectorAttribute<uint32>(file_id, dsetName);
  err = testReadVectorAttribute<int64>(file_id, dsetName);
  err = testReadVectorAttribute<uint64>(file_id, dsetName);
  err = testReadVectorAttribute<float32>(file_id, dsetName);
  err = testReadVectorAttribute<float64>(file_id, dsetName);

  err = testReadScalarAttribute<int8>(file_id, dsetName);
  err = testReadScalarAttribute<uint8>(file_id, dsetName);
  err = testReadScalarAttribute<int16>(file_id, dsetName);
  err = testReadScalarAttribute<uint16>(file_id, dsetName);
  err = testReadScalarAttribute<int32>(file_id, dsetName);
  err = testReadScalarAttribute<uint32>(file_id, dsetName);
  err = testReadScalarAttribute<int64>(file_id, dsetName);
  err = testReadScalarAttribute<uint64>(file_id, dsetName);
  err = testReadScalarAttribute<float32>(file_id, dsetName);
  err = testReadScalarAttribute<float64>(file_id, dsetName);

  std::cout << " Passed" << std::endl;
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
herr_t testReadStringDatasetAndAttributes(hid_t file_id)
{
  std::cout << "Running testReadStringDatasetAndAttributes ......";
  herr_t err = 0;

  std::string refData ("THIS IS THE DATA");
  std::string refAttrData ("THIS IS THE ATTRIBUTE DATA");


  std::string dsetName ("std::string");
  std::string strData ("");
  err = H5Lite::readStringDataset(file_id, dsetName, strData);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE( refData.compare(strData) == 0);

  // Write the Attributes
  std::string attributeKey ("std::string");
  std::string attrData ("");
  err = H5Lite::readStringAttribute(file_id, dsetName, attributeKey, attrData);
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE( refAttrData.compare(attrData) == 0);

  std::vector<uint8> attrDataPtr (refAttrData.size() + 1, 0);
  attributeKey = "c_string";
  err = H5Lite::readStringAttribute(file_id, dsetName, attributeKey, &(attrDataPtr.front() ) );
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE( refAttrData.compare(attrData) == 0);

  // Dataset via pointer (c_str)
  std::vector<uint8> strDataPtr (refData.size() + 1, 0);
  dsetName = "c_string";
  err = H5Lite::readStringDataset(file_id, dsetName, &(strDataPtr.front() ) );
  BOOST_REQUIRE(err >= 0);
  BOOST_REQUIRE( ::memcmp(&(strDataPtr.front()), refData.c_str(), refData.size() + 1) == 0 );

  std::cout << " Passed" << std::endl;
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5LiteTest()
{
 // herr_t err = -1;
  hid_t   file_id;
  /* Create a new file using default properties. */
  file_id = H5Fcreate( H5LITE_TEST_FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
  BOOST_REQUIRE(file_id > 0);
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

  // std::cout << logTime() << "----------- Testing Writing/Reading of Datasets using Raw Pointers -----------" << std::endl;
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<int8>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<uint8>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<int16>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<uint16>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<int32>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<uint32>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<int64>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<uint64>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<float32>(file_id) >= 0);
   BOOST_REQUIRE ( testWritePointer2DArrayDataset<float64>(file_id) >= 0);

   BOOST_REQUIRE ( testWriteVectorDataset<int8>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<uint8>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<int16>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<uint16>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<int32>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<uint32>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<int64>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<uint64>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<float32>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteVectorDataset<float64>(file_id) >= 0);

   BOOST_REQUIRE ( testWriteScalarDataset<int8>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<uint8>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<int16>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<uint16>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<int32>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<uint32>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<int64>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<uint64>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<float32>(file_id) >= 0);
   BOOST_REQUIRE ( testWriteScalarDataset<float64>(file_id) >= 0);

   BOOST_REQUIRE ( testWriteStringDatasetAndAttributes(file_id) >= 0);

//  // ******************* Test Reading Data *************************************
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<int8>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<uint8>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<int16>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<uint16>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<int32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<uint32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<int64>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<uint64>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<float32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadPointer2DArrayDataset<float64>(file_id) >= 0);


   BOOST_REQUIRE ( testReadVectorDataset<int8>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<uint8>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<int16>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<uint16>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<int32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<uint32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<int64>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<uint64>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<float32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadVectorDataset<float64>(file_id) >= 0);
//
   BOOST_REQUIRE ( testReadScalarDataset<int8>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<uint8>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<int16>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<uint16>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<int32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<uint32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<int64>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<uint64>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<float32>(file_id) >= 0);
   BOOST_REQUIRE ( testReadScalarDataset<float64>(file_id) >= 0);

   BOOST_REQUIRE ( testReadStringDatasetAndAttributes(file_id) >= 0);

  /* Close the file. */
  H5Fclose( file_id );
 // std::cout << logTime() << "Testing Complete" << std::endl;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TestLargeFileSupport()
{
 // herr_t err = -1;
  hid_t   file_id;
  /* Create a new file using default properties. */
  file_id = H5Fcreate( H5LITE_TEST_LARGE_FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
  BOOST_REQUIRE(file_id > 0);
  std::vector<int> buffer(1000000); // Create a 4 MegaByte Buffer
	int32 rank = 1;
	std::vector<uint64> dims(1,1000000);
	std::string dsetName;
  for (int i = 0; i < 1000; ++i)
  {
	  dsetName = "/" + StringUtils::numToString(i);
	  H5Lite::writePointerDataset<int>(file_id, dsetName, rank, &(dims.front()), &(buffer.front()) );
	  std::cout << "Large File " << i << "/1000" << std::endl;

  }

	herr_t err = H5Fclose(file_id);
BOOST_REQUIRE(err >= 0);

}


// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
boost::unit_test::test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] )
{
  boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "H5Lite Tests" );
  test->add( BOOST_TEST_CASE( &H5LiteTest), 0);
 // test->add( BOOST_TEST_CASE( &TestLargeFileSupport), 0);
  test->add( BOOST_TEST_CASE( &RemoveTestFiles), 0);

  return test;
}

