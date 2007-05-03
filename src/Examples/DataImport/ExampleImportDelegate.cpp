


#include "ExampleImportDelegate.h"
#include "MXAHDFInterface.h"
#include "MXADataModel/HDFTools/ImageIO/MHDTiffIO.h"
//#include "StringUtilities.h"

#include "hdf5.h"

#include <iostream>

// -----------------------------------------------------------------------------
//  Constructor
// -----------------------------------------------------------------------------
ExampleImportDelegate::ExampleImportDelegate()
{
}

// -----------------------------------------------------------------------------
//  Destructor
// -----------------------------------------------------------------------------
ExampleImportDelegate::~ExampleImportDelegate()
{
}



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 ExampleImportDelegate::encodeSourceToHDF5( hid_t fileId, std::string recordGroup,
                            std::string recordName, std::string filePath)
{
  herr_t err = 0;
  hid_t gid = MXAHDFInterface::openHDF5Object(fileId, recordGroup);
  std::cout << " filePath: " << filePath << "\n";
  std::cout << " recordGroup: " << recordGroup << std::endl;
  std::cout << " recordName: " << recordName << std::endl;
 
  int32 NX  =  5; /* dataset dimensions */
  int32 NY   =  3;
  int32 RANK  = 2;

  hid_t       dataset;         /* file and dataset handles */
  hid_t       datatype, dataspace;   /* handles */
  hsize_t     dimsf[2];              /* dataset dimensions */
  herr_t      status;
  int32         data[NX][NY];          /* data to write */
  int32         i, j;

    /*
     * Data  and output buffer initialization.
     */
    for (j = 0; j < NX; j++) {
      for (i = 0; i < NY; i++) {
          data[j][i] = i + j;
      }
    }
       
    /*
     * 0 1 2 3 4 5
     * 1 2 3 4 5 6
     * 2 3 4 5 6 7
     * 3 4 5 6 7 8
     * 4 5 6 7 8 9
     */


    /*
     * Describe the size of the array and create the data space for fixed
     * size dataset.
     */
    dimsf[0] = NX;
    dimsf[1] = NY;
    dataspace = H5Screate_simple(RANK, dimsf, NULL);

    /*
     * Define datatype for the data in the file.
     * We will store little endian int32 numbers.
     */
    datatype = H5Tcopy(H5T_NATIVE_INT);
    status = H5Tset_order(datatype, H5T_ORDER_LE);

    /*
     * Create a new dataset within the file using defined dataspace and
     * datatype and default dataset creation properties.
     */
    dataset = H5Dcreate(gid, recordName.c_str(), datatype, dataspace, H5P_DEFAULT);

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

  
  err = MXAHDFInterface::closeHDF5Object(gid);   
  return err;  
}




