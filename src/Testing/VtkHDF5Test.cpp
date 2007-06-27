

#include "Headers/MXATypeDefs.h"
#include "Headers/LogTime.h"
#include "MXADataModel/MXADataModel.h"
#include "MXADataModel/MXADataRecord.h"
#include "HDF5/H5Lite.h"
#include "HDF5/vtkHDF5.h"
#include "Testing/DataFileGenerator.h"

//-- HDF5 Includes
#include <hdf5.h>

//-- Vtk includes
#include <vtkType.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkTIFFWriter.h>
#include <vtkXMLImageDataWriter.h>


// -----------------------------------------------------------------------------
//  Define where to put our temporary files
// -----------------------------------------------------------------------------
#if defined (_WIN32)
#define FILE_NAME "C:\\WINDOWS\\Temp\\vtkhdf5test.h5"
#else 
#define FILE_NAME "/tmp/vtkhdf5test.h5"
#endif

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
herr_t test()
{
  herr_t err = -1;
  //First load the Data file
  MXADataModelPtr modelPtr = MXADataModel::New();
  modelPtr->readModel(FILE_NAME, false); // We pass 'false' so the file will stay open
  hid_t fileId = modelPtr->getIODelegate()->getOpenFileId();
  if (fileId < 0)
  {
    std::cout << logTime() << "Error: FileId was not valid."<< std::endl;
    return -1;
  }

  // We know that the data dimensions have ranges of 1->2 and 1->3, so lets get the data for the first index of each one.
  std::vector<int32> indices;
  indices.push_back(1); // Time step 1
  indices.push_back(1); // Slice number
  // We also know the exact path to the Data Record, so lets use it to retrieve the Data Record from the Model
  MXADataRecordPtr record = modelPtr->getDataRecordByPath(DataGen::TableRec + "/" + DataGen::Uint8Rec);
  if (NULL == record.get())
  {
    std::cout << logTime() << "Error getting '2D Array' Data Record"
        << std::endl;
    return -1;
  }

  // Have the DataModel generate the proper internal path relative to the root level and extending to the dataset
  std::string datasetPath = modelPtr->generatePathToDataset(indices, record.get() );

  vtkSmartPointer<vtkImageData> imgData = vtkSmartPointer<vtkImageData>::New();
  err = vtkHDF5::getDataAsVTKImage(fileId, datasetPath, imgData);
  if (err < 0)
  {
    std::cout << logTime() << "Dataset was NULL " << std::endl;
    return -1;
  }


  vtkSmartPointer<vtkXMLImageDataWriter> bmpWriter = vtkSmartPointer<vtkXMLImageDataWriter>::New();
  bmpWriter->SetFileName("/tmp/out.vti");
  bmpWriter->SetInput(imgData);
  bmpWriter->Update();
  return 1;

}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  std::cout << logTime() << "----- Running vtkHDF5 Bridge Test ------------- "
      << std::endl;

  herr_t err = 1;

  // Generate a Data file to use
  std::string outputFile(FILE_NAME);
  DataFileGenerator dfg;
  dfg.setFilePath(outputFile);
  err = dfg.generate();

  if (err < 0)
  {
    return EXIT_FAILURE;
  }

  test();
  
  std::cout << logTime() << " vtkHDF5 Bridge Test Complete --------------" << std::endl;
  return EXIT_SUCCESS;
}
