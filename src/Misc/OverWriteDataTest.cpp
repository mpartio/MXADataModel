
#include <hdf5.h>
#include <iostream>
#include <string>

#define H5FILE_NAME "/tmp/OverWriteDataTest.h5"

#define DATASETNAME "IntArray"
#define NX     5                      /* dataset dimensions */
#define NY     6
#define RANK   2

#define HDF_ERROR_HANDLER_OFF\
  herr_t (*_oldHDF_error_func)(void *);\
  void *_oldHDF_error_client_data;\
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);\
  H5Eset_auto(NULL, NULL);

#define HDF_ERROR_HANDLER_ON  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);
#define CloseH5A(aid, err, retError)\
   err = H5Aclose( attr_id );\
   if (err<0) {std::cout << "Error Closing Attribute." << std::endl;retErr = err;}

#define CloseH5D(did, err, retError)\
  err = H5Dclose(did);\
  if (err < 0) { std::cout << "Error Closing Dataset." << std::endl; retError = err;}

#define CloseH5S(sid, err, retError)\
  err = H5Sclose(sid); \
  if ( err < 0) {std::cout << "Error closing Dataspace." << std::endl;retErr = err;}

#define CloseH5T(tid, err, retError)\
  err = H5Tclose(tid);\
  if (err < 0 ) {std::cout << "Error closing DataType" << std::endl; retErr = err;}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  Writes a string to a HDF5 dataset
// -----------------------------------------------------------------------------
herr_t writeStringDataset (hid_t loc_id, const std::string& dsetName, const std::string &data, bool overwrite = false)
{
  hid_t   did=-1;
  hid_t   sid=-1;
  hid_t   tid = -1;
  size_t  size = 0;
  herr_t err = -1;
  herr_t retErr = 0;


  /* create a string data type */
  if ( (tid = H5Tcopy( H5T_C_S1 )) >= 0 )
  {
    size = data.size() + 1;
    if ( H5Tset_size(tid, size) >= 0 )
    {
      if ( H5Tset_strpad(tid, H5T_STR_NULLTERM ) >= 0 )
      {
      /* Create the data space for the dataset. */
        if ( (sid = H5Screate( H5S_SCALAR )) >= 0 )
        {
          /* Create the dataset. */
          if (overwrite == true)
          {
            HDF_ERROR_HANDLER_OFF
            did = H5Dopen(loc_id, dsetName.c_str() );
            if (did > 0)
            {
              hsize_t storageSize = H5Dget_storage_size(did);
              if (did > 0 && storageSize != data.size() + 1)
              { // Data sizes are NOT the same. Delete current data set
                err = H5Gunlink(loc_id, dsetName.c_str() );
                if (err < 0 )
                {
                  retErr = err;
                }
                err = H5Dclose(did);
                did = -1;
              }
            }
            HDF_ERROR_HANDLER_ON
          }
          if (did < 0 && retErr >= 0) // dataset does not exist
          {
            did = H5Dcreate(loc_id, dsetName.c_str(), tid, sid, H5P_DEFAULT);
          }
          if ( did >= 0 && retErr >= 0)
          {
             if ( !data.empty() )
              {
                  err = H5Dwrite(did, tid, H5S_ALL, H5S_ALL,H5P_DEFAULT, data.c_str() );
                  if (err<0 ) {
                    std::cout << "Error Writing String Data" << std::endl;
                    retErr = err;
                  }
              }
          } else {
            retErr = did;
          }
          CloseH5D(did, err, retErr);
//          err = H5Dclose(did);
//          if (err < 0) {
//            std::cout << "Error Closing Dataset." << std::endl;
//            retErr = err;
//          }
        }
        CloseH5S(sid, err, retErr);
//        err = H5Sclose(sid);
//        if ( err < 0) {
//          std::cout << "Error closing Dataspace." << std::endl;
//          retErr = err;
//        }
      }
    }
    CloseH5T(tid, err, retErr);
//    err = H5Tclose(tid);
//    if (err < 0 ) {
//     std::cout << "Error closing DataType" << std::endl;
//     retErr = err;
//    }
  }
  return retErr;
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  std::cout << "Starting OverWriteDataTest ... " << std::endl;
  hid_t file, dataset; /* file and dataset handles */
  hid_t datatype, dataspace; /* handles */
  hsize_t dimsf[2]; /* dataset dimensions */
  herr_t status;
  int data[NX][NY]; /* data to write */
  int i, j;
  
//  HDF_ERROR_HANDLER_OFF;
//  status = H5Fopen("/tmp/junk.h5", H5F_ACC_RDWR, H5P_DEFAULT);
//  if (status < 0)
//  {
//    std::cout << "Can not open file" << std::endl;
//    return -1;
//  }
//  HDF_ERROR_HANDLER_ON;
  /*
   * Data  and output buffer initialization.
   */
  for (j = 0; j < NX; j++)
  {
    for (i = 0; i < NY; i++)
      data[j][i] = i + j;
  }
  /*
   * 0 1 2 3 4 5
   * 1 2 3 4 5 6
   * 2 3 4 5 6 7
   * 3 4 5 6 7 8
   * 4 5 6 7 8 9
   */

  /*
   * Create a new file using H5F_ACC_TRUNC access,
   * default file creation properties, and default file
   * access properties.
   */
  file = H5Fcreate(H5FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  /*
   * Describe the size of the array and create the data space for fixed
   * size dataset.
   */
  dimsf[0] = NX;
  dimsf[1] = NY;
  dataspace = H5Screate_simple(RANK, dimsf, NULL);

  /*
   * Define datatype for the data in the file.
   * We will store little endian INT numbers.
   */
  datatype = H5Tcopy(H5T_NATIVE_INT);
  status = H5Tset_order(datatype, H5T_ORDER_LE);

  /*
   * Create a new dataset within the file using defined dataspace and
   * datatype and default dataset creation properties.
   */
  dataset = H5Dcreate(file, DATASETNAME, datatype, dataspace, H5P_DEFAULT);

  /*
   * Write the data to the dataset using default transfer properties.
   */
  status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
  
  /*
   * Close/release resources.
   */
  H5Sclose(dataspace);
  H5Tclose(datatype);
  H5Dclose(dataset);
  H5Fclose(file);
  
  //Reopen the file for Read/Write Access
  file = H5Fopen(H5FILE_NAME, H5F_ACC_RDWR, H5P_DEFAULT);
  dataset = H5Dopen(file, DATASETNAME);
  if (dataset < 0)
  {
    std::cout << "Could not open dataset. dataset=" << dataset << std::endl;
    return -1;
  }
  //Update the data
  for (j = 0; j < NX; j++)
  {
    for (i = 0; i < NY; i++)
      data[j][i] = i + j * 1000;
  }
  
  // Update the data
#if 1
 // for (int i = 0; i < 1000; ++i) {
    status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    if (status < 0)
    {
      std::cout << "Could NOT over write data. Status=" << status << std::endl;
    }
  //}
#endif
  
  
  /*
    * Create scalar attribute.
    */
   hid_t aid2  = H5Screate(H5S_SCALAR);
   hid_t attr2 = H5Acreate(dataset, "Integer attribute", H5T_NATIVE_INT, aid2, H5P_DEFAULT);

   /*
   * Write scalar attribute.
   */
  int point = 999;
  status = H5Awrite(attr2, H5T_NATIVE_INT, &point);
  if (status < 0)
  {
    std::cout << "Error Writing Attribute. Status=" << status << std::endl;
  }
  
  point = 111;
  status = H5Awrite(attr2, H5T_NATIVE_INT, &point);
  if (status < 0)
  {
    std::cout << "Error Writing Attribute. Status=" << status << std::endl;
  }

  
  // Test strings
  std::string testString ("111111111|222222222|333333333|444444444|");
  writeStringDataset(file, "String Test", testString);
  testString  = "123456789|123456789|123456789|123456789|123456789|";
  writeStringDataset(file, "String Test", testString, true);
  //H5Gunlink(file, "String Test");
  testString = "More Testing for the String data set";
  hid_t gid = H5Gcreate(file, "Group", 0);
  writeStringDataset(gid, "String Test", testString, true);
  H5Gclose(gid);
  
  H5Sclose(aid2);
  H5Aclose(attr2);
  H5Dclose(dataset);
  H5Fclose(file);
  
  std::cout << "Ending OverWriteDataTest ... " << std::endl;

  return 1;
}
