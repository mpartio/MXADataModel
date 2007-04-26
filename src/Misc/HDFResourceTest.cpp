

#include <string>
#include <iostream>
#include <vector>

#include "hdf5.h"

#include "HDF5/H5Lite.h"
#include "Headers/LogTime.h"

/* If we define CLOSE_FILE = 1, then the H5 file is closed and reopened each time through the loop
* If CLOSE_FILE= 0 then the file is left open the entire run.
* Things to note are: 
*   When the file is left open the time between each output to std::cout takes longer. Not
*     sure if it is linear or exponential.
*   Also if the file is left open the memory foot print will grow during the entire process.
*   By closing and opening the file on each loop iteration both of these problems go away. 
* 
*   My testing env is OS X 10.4.9 running Xcode 2.4.1 (latest GCC Tools from Apple) running
*   on a MacBook Pro (Core Duo) with 2GB RAM.
*   HDF5 1.6.5 was compiled with the following configure: ./configure --enable-cxx --prefix=/usr/local
*   I used "Big Top" ( part of the Xcode tools) to monitor the memory usage. "top" would also work.
*/
#define CLOSE_FILE 1



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
  int rank = 1;
  std::vector<hsize_t> dims(1, 1024);
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
  
  
  int num_open = H5Fget_obj_count(fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |  H5F_OBJ_DATATYPE | H5F_OBJ_ATTR);
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
