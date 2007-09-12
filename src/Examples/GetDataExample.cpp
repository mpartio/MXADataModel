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

#include "Common/LogTime.h"
#include "Testing/DataFileGenerator.h"
#include <hdf5.h>

// -----------------------------------------------------------------------------
//  Define where to put our temporary files
// -----------------------------------------------------------------------------
#if defined (_WIN32)
  #define FILE_NAME "C:\\WINDOWS\\Temp\\GetDataExample.h5"
#else 
  #define FILE_NAME "/tmp/GetDataExample.h5"
#endif


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  std::cout << logTime() << "----- Running GetData Example ------------- " << std::endl;
  
  herr_t err = 1;
  // Generate a Data file to use
  std::string outputFile(FILE_NAME);
  DataFileGenerator dfg;
  dfg.setFilePath(outputFile);
  err = dfg.generate();
  if (err < 0)
    return EXIT_FAILURE;
  
#if 1
  //First load the Data file
  MXADataModelPtr modelPtr = MXADataModel::New();
  modelPtr->readModel(FILE_NAME, false); // We pass 'false' so the file will stay open
  hid_t fileId = modelPtr->getIODelegate()->getOpenFileId();
  if (fileId < 0)
  {
    std::cout << logTime() << "Error: FileId was not valid." << std::endl;
    return -1;
  }
  // Now we can interrogate the model to get some information
  IDataDimensions dims = modelPtr->getDataDimensions();
  MXADataDimension* dim = NULL;
  //Loop over the Data Dimensions and print each one to std::cout
  for (IDataDimensions::iterator iter = dims.begin(); iter != dims.end(); ++iter )
  {
    dim = static_cast<MXADataDimension*>((*(iter)).get() );
    if (NULL == dim)
    {
      std::cout << logTime() << "Error: Dimension was NULL. " << std::endl;
      break;
    }
  }
  
  // We know that the data dimensions have ranges of 1->2 and 1->3, so lets get the data for the first index of each one.
  std::vector<int32> indices;
  indices.push_back(1); indices.push_back(1);
  // We also know the exact path to the Data Record, so lets use it to retrieve the Data Record from the Model
  IDataRecordPtr record = modelPtr->getDataRecordByNamedPath(DataGen::TableRec + "/" + DataGen::Float32Rec);
  if (NULL == record.get() )
  {
    std::cout << logTime() << "Error getting '2D Array' Data Record" << std::endl;
    return -1;
  }
  
  // Have the DataModel generate the proper internal path relative to the root level and extending to the dataset
  std::string dsetPath = modelPtr->generatePathToDataset(indices, record.get() );
  
  std::vector<float32> data; // This will hold our data. The next call will call 'clear' and 'resize' the vector as needed
  err = H5Lite::readVectorDataset(fileId, dsetPath, data);
  if (err < 0)
  {
    std::cout << logTime() << "Error Retrieving Dataset at " << dsetPath << std::endl;
    return -1;
  }
  //Print the data:
  for (std::vector<float32>::iterator iter = data.begin(); iter != data.end(); ++iter )
  {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;
  
  //If you know nothing of the datatype then you will have to interrogate the dataset
  // to find out... 
  //uint32 type_size = 0;
 // H5T_class_t type_class = -1;
 // err = H5Lite::getDatasetInfo(fileId, dsetPath, dims, type_class, type_size);
  if (err < 0)
  {
    std::cout << logTime() << "Error Retrieving Dataset info for " << dsetPath << std::endl;
    return -1;
  }
#endif
  std::cout << logTime() << "------------- GetData Example Complete ------------ " << std::endl;
  
  return 0;
}
