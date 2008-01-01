#include "H5StringDataset.h"
#include <Base/IDataFile.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Utilities.h>
#include <Dataset/IDataset.h>

//-- C++ Includes
#include <iostream>


// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
int32 H5StringDataset::writeToFile(IDataFilePtr dataFile)
{
   std::cout << "H5StringDataset::writeToFile" << std::endl;
#warning IMPLEMENT H5StringDataset::writeToFile
  return -1;
}

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
int32 H5StringDataset::readFromFile(IDataFilePtr dataFile)
{
  std::cout << "H5StringDataset::readFromFile" << std::endl;
#warning IMPLEMENT H5StringDataset::readFromFile
  return 1;
}
