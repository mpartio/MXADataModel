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
int32 ExampleImportDelegate::importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile)
{
  std::cout << "Importing data file '" << dataSource->getSourcePath() << "'" << std::endl;
  herr_t err = 0;
  // The data file should already be open at this point but we can check to make sure
  if (dataFile->isFileOpen() == false)
  {
    err = dataFile->openFile(false); // Open the file for write access
    if (err < 0)
    {
      std::cout << "Could NOT open the data file '" << dataFile->getFilename() << "'" << std::endl;
      return err;
    }
  }
  IDataModelPtr model = dataFile->getDataModel();
  std::string path(dataSource->generateInternalPath());
  uint32 pos = path.find_last_of("/");
  std::string parentPath(path.substr(0, pos));

  hid_t fileId = dataFile->getFileId();
  H5Utilities::createGroupsFromPath(parentPath, fileId);


  // Create a 2D Dataset using the DataWrappers
  int32 nDims = 2;
  int32 NX  =  5; /* dataset dimensions */
  int32 NY   =  3;
  uint64 dims[2] = {NX, NY};
  IMXAArrayPtr array = MXAArrayTemplate<int32>::CreateMultiDimensionalArray(nDims, dims);
  int32* data = static_cast<int32*>(array->getVoidPointer(0) );


  std::string inputFile = dataSource->getSourcePath();
  std::ifstream fin(inputFile.c_str(), std::ios::binary);

  fin.read((char *)(data), array->getNumberOfElements() * sizeof(int32) );

  //Write the Data to the HDF5 File
  err = H5Lite::writeMXAArray(fileId, path, array.get() );
  return err;
}

