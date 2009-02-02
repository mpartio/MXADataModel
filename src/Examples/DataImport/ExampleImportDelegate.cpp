#include <Examples/DataImport/ExampleImportDelegate.h>

#include <Base/IDataFile.h>
#include <Base/IDataSource.h>
#include <HDF5/H5Utilities.h>
#include <HDF5/H5Lite.h>
#include <Examples/DataImport/SimpleImportExample.h>

//-- hdf5 includes
#include <hdf5.h>

//-- C++ includes
#include <iostream>
#include <fstream>
#include <string>

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
int32 ExampleImportDelegate::importDataSource(IDataSourcePtr dataSource, IDataFilePtr mxaFile)
{
  std::cout << "Importing data file '" << dataSource->getSourcePath() << "'" << std::endl;
  herr_t err = 0;
  // The MXA file should already be open at this point but we can check to make sure
  if (mxaFile->isFileOpen() == false)
  {
    err = mxaFile->openFile(false); // Open the file for write access
    if (err < 0)
    {
      std::cout << "Could NOT open the data file '" << mxaFile->getFilename() << "'" << std::endl;
      return err;
    }
  }

  // Get the MXADataModel
  IDataModelPtr model = mxaFile->getDataModel();
  // Create the path where we are to store the data from the input data file
  std::string path(dataSource->generateInternalPath());
  uint32 pos = path.find_last_of("/");
  std::string parentPath(path.substr(0, pos));

  // Get the hdf5 fileId from the data model object
  hid_t fileId = mxaFile->getFileId();

  // Ensure all the intermediary groups in the hdf5 file are created.
  H5Utilities::createGroupsFromPath(parentPath, fileId);


  // Create a 2D Dataset using the DataWrappers
  int32 nDims = 2;
  int32 NX  =  5; /* dataset dimensions */
  int32 NY   =  3;
  uint64 dims[2] = {NX, NY};
  IMXAArrayPtr array = MXAArrayTemplate<int32>::CreateMultiDimensionalArray(nDims, dims);
  int32* data = static_cast<int32*>(array->getVoidPointer(0) );

  //Open the input file as a binary file
  std::ifstream fin(dataSource->getSourcePath().c_str(), std::ios::binary);
  // Read the contents of the data file into an MXAArray
  fin.read((char *)(data), array->getNumberOfElements() * sizeof(int32) );

  //Write the Data to the HDF5 File
  err = H5Lite::writeMXAArray(fileId, path, array.get() );
  return err;
}

