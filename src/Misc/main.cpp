//Include MXA Headers
#include "Headers/MXATypes.h"
#include "Headers/LogTime.h"

// C++ includes
#include <string>
#include <sstream>
#include <iostream>
#include "HDF5/H5Lite.h"
#include "hdf5.h"
#include "H5LT.h"

#define CLOSE_FILE 1

#if 0
template<typename T>
hid_t HDFTypeForPrimitive(T value)
{
  if (typeid(value) == typeid(char)) return H5T_NATIVE_INT8;
  if (typeid(value) == typeid(signed char)) return H5T_NATIVE_INT8;  
  if (typeid(value) == typeid(unsigned char)) return H5T_NATIVE_UINT8;
  
  if (typeid(value) == typeid(short int)) return H5T_NATIVE_INT16;
  if (typeid(value) == typeid(signed short int)) return H5T_NATIVE_INT16;
  if (typeid(value) == typeid(unsigned short int)) return H5T_NATIVE_UINT16;
  
  if (typeid(value) == typeid(int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(signed int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(long int)) return H5T_NATIVE_INT32;
  if (typeid(value) == typeid(signed long int)) return H5T_NATIVE_INT32;
  
  if (typeid(value) == typeid(unsigned int)) return H5T_NATIVE_UINT32;
  if (typeid(value) == typeid(unsigned long int)) return H5T_NATIVE_UINT32;
  
  if (typeid(value) == typeid(long long int)) return H5T_NATIVE_INT64;
  if (typeid(value) == typeid(signed long long int)) return H5T_NATIVE_INT64;
  if (typeid(value) == typeid(unsigned long long int)) return H5T_NATIVE_UINT64;
  
  
  if (typeid(value) == typeid(float)) return H5T_NATIVE_FLOAT;
  if (typeid(value) == typeid(double)) return H5T_NATIVE_DOUBLE;
  return -1;
}
#endif


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
hid_t _createGroup(hid_t loc_id, std::string group) 
{
  hid_t grp_id;
  herr_t err = -1;

  // Suspend the HDF Error Handlers
  herr_t (*_oldHDF_error_func)(void *);
  void *_oldHDF_error_client_data;

  // Store the current HDF error handlers
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);

  // Turn off error handling
  H5Eset_auto(NULL, NULL);

  err = H5Gget_objinfo(loc_id, group.c_str(), 0, NULL);
//  std::cout << "H5Gget_objinfo = " << err << " for " << group << std::endl;  
  if (err == 0)
  {
    grp_id = H5Gopen(loc_id, group.c_str());
  } else 
  {
    grp_id = H5Gcreate(loc_id, group.c_str(), 0);
  }
  // Turn the HDF Error handlers back on
  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);
  return grp_id;
}
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 createGroupsFromPath(std::string path, hid_t parent)
{
  if (parent <= 0) {
    std::cout << "Bad parent Id. Returning from createGroupsFromPath" << std::endl;
    return -1;
  }
  //std::cout << "Creating Group: " << path << std::endl;
  // remove any front slash
  uint32 pos = path.find_first_of("/", 0);
  if ( 0 == pos ) 
  {
    path = path.substr(1, path.size());
  }
  
  //Remove any trailing slash
  pos = path.find_last_of("/");
  if ( pos == (path.size() - 1) ) // slash was in the last position
  {
    path = path.substr(0, pos);
  }
  
  if (path.size() == 0) 
  {
  //  std::cout << "  Removal of front and trailing slashes caused string length to be 0" << std::endl;
    return -1; // The path that was passed in was only a slash.. 
  }
  
  std::string first;
  std::string second;

  pos = path.find_first_of("/", 0);
  if (pos == std::string::npos) // No slash found
  {
    first = path;
    second = "";
  } 
  else 
  {
    first = path.substr(0, pos);
    second = path.substr(pos, path.size());
  }
  hid_t gid = 1;
  herr_t err = -1;
#if 1
  gid = _createGroup(parent, first);
  if (!second.empty() && gid > 0)
  {
    err = createGroupsFromPath(second, gid);
  }
  err = H5Gclose(gid); // Close the group
#endif
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t writeFloatData( hid_t loc_id,  const char *dset_name, int32 rank,
                                 const hsize_t *dims, const float *data )
{
  //std::cout << ">>>>> Attempting Write of Float Data........." << std::endl;
  hid_t   did = -1; //Dataset ID
  hid_t   sid = -1; //DataSpace ID
  herr_t   err = 0;  // Error condition
      
  /* Create the data space for the dataset. */
  sid = H5Screate_simple( rank, dims, NULL );
  if ( sid < 0 ) {
   return sid;
  }
    
  /* Create the dataset. */
  did = H5Dcreate( loc_id, dset_name, H5T_NATIVE_FLOAT, sid, H5P_DEFAULT );
  if ( did >= 0 ) {
    /* Write the dataset only if there is data to write */
    if ( data ) //Check for a valid pointer
    {
      err = H5Dwrite( did, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data );
      if (err<0) {
        std::cout << "writeFloatData() Error Writing data to the HDF File. Error:" << err << std::endl; 
      }
    }
       
    /* End access to the dataset and release resources used by it. */
    err = H5Dclose( did );
    if ( err < 0 ) {
     return err;
    }
        /* Terminate access to the data space. */
    err = H5Sclose( sid );
    if ( err < 0 ) {
     return err;
    }
  
  } 
  return err;
}

void more(hid_t fileId, int32 &global) 
{
  char name[20]; memset(name, 0, sizeof(name) );
  int32 rank = 1;
//  float data[65536];
  std::vector<float> data(10000000, 0);
  hsize_t dims[1] ={ 65536 };
 // memset(data, 0xF0F0F0F0, sizeof(data)); //put some data into the array

  for (int i = global; i < global + 1000; ++i)
  {
    if (i%100 == 0) {
      std::cout << "HEre" << std::endl;
    }
    std::cout << "i=" << i << std::endl;
    snprintf(name, sizeof(name), "%d", i);
    writeFloatData(fileId, name, rank, dims, &(data.front() ) );
  }
  global += 1000;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
  std::cout << "Testing Pure Group Creation...." << std::endl;
  std::string filename  ("/tmp/PureGroupTest.h5");
  herr_t err = 1;
  hid_t fileId = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  
  std::vector<char> gName(20, 0);
  char* gNamePtr = &(gName.front());
  int32 rank = 1;
  std::vector<uint64> dims(1, 1024);
  hsize_t* dimsPtr = &(dims.front());
  std::vector<int> buff(1024,0xFFAAFFAA);
  int* buffer = &(buff.front());
  
  for (int i = 0; i < 10000; ++i)
  {
    
    snprintf(gNamePtr, gName.size(), "%d", i);
    hid_t iGroup = H5Gcreate(fileId, gNamePtr, 100);
    for (int j = 0; j < 10000; ++j) {
      snprintf(gNamePtr, gName.size(), "%d/%d", i, j);
      hid_t jGroup = H5Gcreate(fileId, gNamePtr, 100);
      
//      err = H5LTmake_dataset_int( jGroup, "data", rank, dims, buffer );
      std::string dsetName("dsetName");
      err = H5Lite::writeDataset(jGroup, dsetName, dims, buff, H5Lite::HDFTypeForPrimitive(buff.front()));
      if (err < 0)
      {
        std::cout << "Error creating data for " << i << "/" << j << std::endl;
      }
      err = H5Gclose(jGroup);
      if (err<0)
      {
        std::cout << "Error Closing jGroup: " << j << std::endl;
      }
    }
    err = H5Gclose(iGroup); //Close the group
    if (err<0)
    {
      std::cout << "Error Closing iGroup: " << i << std::endl;
    }
#if CLOSE_FILE
    std::cout << logTime() << i << " : Cycling File" << std::endl;
    err = H5Fclose(fileId);
    if (err < 0) { std::cout << "Error closing File:" << std::endl;}
    fileId = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
#else 
    std::cout << logTime() << i << " : NOT Cycling File" << std::endl;
#endif
  }
  
  
  int32 num_open = H5Fget_obj_count(fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |  H5F_OBJ_DATATYPE | H5F_OBJ_ATTR);
  if (num_open > 0) {
    std::cout << "WARNING: " << num_open << " IDs weren't closed. Closing them."  << std::endl;
  }
  err = H5Fclose(fileId);
  if (err < 0) 
  {
    std::cout << "Error Closing file: " << err << std::endl;
  }
  std::cout << "Done Testing" << std::endl;
  return 0;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main3(int argc, char **argv) {
  std::cout << "Writing a bunch of Data" << std::endl;
  // Suspend the HDF Error Handlers
  herr_t (*_oldHDF_error_func)(void *);
  void *_oldHDF_error_client_data;

  // Store the current HDF error handlers
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);

  // Turn off error handling
  H5Eset_auto(NULL, NULL);
  
  
  std::string filename  ("/tmp/H5GroupTest.h5");
  herr_t err = -1;
  hid_t fileId = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  int32 GLOBAL = 0;
  for (int i = 0; i < 1; ++i)
  {
    more(fileId, GLOBAL);
    int32 num_open = H5Fget_obj_count(fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |  H5F_OBJ_DATATYPE | H5F_OBJ_ATTR);
    if (num_open > 0) {
      std::cout << "WARNING: Some IDs weren't closed. Closing them."  << std::endl;
    }
    H5Fclose(fileId);
    fileId = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
  }
  
  
  
  err = H5Fclose(fileId);
  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);
  std::cout << "Done writing data" << std::endl;
  return 0;
}
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main2 (int argc, char const* argv[])
{
  
  char c8 = 0;
  signed char sc8 = 0;
  unsigned char uc8 = 0;
  int8  i8  = 0;
  uint8 ui8 = 1;
  int16 i16 = 0;
  uint16 ui16 = 0;
   signed long int32 i32 = 0;
  uint32 ui32 = 0;
  signed long long int32 i64 = 0;
  
  unsigned long long int32 ui64 = 0;
  float32 f32 = 0.0;
  float64 f64 = 0.0;
  
  unsigned short s = 0;
  
std::cout << "char          : " << HDFTypeForPrimitive(c8) << std::endl;
std::cout << "signed char   : " << HDFTypeForPrimitive(sc8) << std::endl;
std::cout << "unsigned char : " << HDFTypeForPrimitive(uc8) << std::endl;
  std::cout << "int8     : " << HDFTypeForPrimitive(i8) << std::endl;
  std::cout << "uint8    : " << HDFTypeForPrimitive(ui8) << std::endl;
  std::cout << "int16    : " << HDFTypeForPrimitive(i16) << std::endl;
  std::cout << "uint16   : " << HDFTypeForPrimitive(ui16) << std::endl;
  std::cout << "int32    : " << HDFTypeForPrimitive(i32) << std::endl;
  std::cout << "uint32   : " << HDFTypeForPrimitive(ui32) << std::endl;
  std::cout << "int64    : " << HDFTypeForPrimitive(i64) << std::endl;
  std::cout << "uint64   : " << HDFTypeForPrimitive(ui64) << std::endl;
  std::cout << "float32  : " << HDFTypeForPrimitive(f32) << std::endl;
  std::cout << "float64  : " << HDFTypeForPrimitive(f64) << std::endl;

  std::cout << "short    : " << HDFTypeForPrimitive(s) << std::endl;
  
  char ff = 0xFF;
 // std::cout << "Char (FF) : " << (int)ff << std::endl;
  return 0;
}




