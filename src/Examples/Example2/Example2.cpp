//-- MXA Includes
#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Common/LogTime.h>
#include <Core/MXADataModel.h>
#include <XML/XMLDataModelWriter.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5Utilities.h>
#include <HDF5/H5MXADataFile.h>
#include <HDF5/H5DataArrayTemplate.hpp>
#include <HDF5/H5RGBImage.h>
#include <HDF5/H5Data2DArray.hpp>
// HDF5 Include
#include <hdf5.h>

// -----------------------------------------------------------------------------
//  Define where to put our temporary files
// -----------------------------------------------------------------------------
#if defined (_WIN32)
  #define MXA_FILE "C:\\WINDOWS\\Temp\\MXA_Example2.h5"
#define XML_FILE "C:\\WINDOWS\\Temp\\MXA_Example2.xml"
#else 
  #define MXA_FILE "/tmp/MXA_Example2.h5"
#define XML_FILE "/tmp/MXA_Example2.xml"
#endif

// Declare methods
void listDataDimensions(MXADataModel* model);
void listDataRecords(MXADataModel* model);
MXAAbstractDataPtr captureSampleImage(const std::string &datasetPath);
int32 generateData();

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv) 
{
 std::cout << "Starting Example 2." << std::endl;
 if ( generateData() == EXIT_FAILURE)
 {
   return EXIT_FAILURE;
 }
 
 
 
 return EXIT_SUCCESS; 
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 generateData()
{


  //Instatiate a new model using the predefined boost shared pointer type
  MXADataModelPtr modelPtr = MXADataModel::New();
  MXADataModel* model = modelPtr.get();
  
  //Define at what path in the HDF5 file the data will be stored
  model->setDataRoot("/Experimental Data");
  
  //Instantiate 2 Data Dimensions
  // The first dimension has 10 elements from 0 to 9 and increments by 1. Since this
  // is the first dimension we give it an index of 0
  int32 index = 0;
  int32 count = 10;
  int32 start = 0;
  int32 end = 9;
  int32 increment = 1;
  int32 uniform = 1;
  MXADataDimensionPtr dim1 = MXADataDimension::New("Time", "Time (minutes)", index, count, start, end, increment, uniform);
  
  // The second dimension will have 4 elements ranging from 2 to 8 with an increment of 2;
  index = 1;
  count = 4;
  start = 200;
  end = 800;
  increment = 200;
  uniform = 1;
  MXADataDimensionPtr dim2 = MXADataDimension::New("Pressure", "Press (kPa)", index, count, start, end, increment, uniform);
  
  //Next we need to add these dimensions to the model. Since we are using Boost shared pointers
  // the dimension objects are refcounted thus relieving us from having to worry about cleaning up
  // the memory allocations
  model->addDataDimension(dim1);
  model->addDataDimension(dim2);
  
  
  // Next we need to create a data record to hold one of the dependent variables for our experiment.
  // In our sample experiment we are going to measure the temperature and record an image of the sample.
  // The important argument is the 'luid' argument. These need to be unique within each group of Data Records.
  MXADataRecordPtr temp = MXADataRecord::New(0, "Temperature" , "Temp (K)");
  MXADataRecordPtr cameraImage = MXADataRecord::New(1, "Camera", "Camera Image");
  
  // Next, add these Records to the Data Model
  model->addDataRecord(temp);
  model->addDataRecord(cameraImage);
  
  //Lastly a certain number of meta data fields are required to be set to non-empty values
  std::map<std::string, std::string> md;
  md[MXA::MXA_CREATOR_TAG] = "Mike Jackson"; // Who is performing the experiment
  md[MXA::MXA_DATE_TAG] = "2006:12:24 15:34.51"; // What date is the experiment being performed
  md[MXA::MXA_DSET_NAME_TAG] = "Testing Data Import"; // Give this specific experiment a name or other identifying tag
  md[MXA::MXA_DESCRIPTION_TAG] = "Application to test importing data to the data file"; // Give a short description of the experiment
  md[MXA::MXA_PEDIGREE_TAG] = MXA::MXA_PEDIGREE_ORIGINAL_VALUE; // Is the data being stored original data or was it imported from another source?
  md[MXA::MXA_DERIVED_SRC_TAG] = MXA::MXA_NOT_APPLICABLE_VALUE; // The data is original from the instrument so this tag is not needed
  md[MXA::MXA_RIGHTS_TAG] = MXA::MXA_RIGHTS_UNLIMITED_VALUE; // There are no limitations on the distribution of the data
  md[MXA::MXA_RELEASE_NUMBER_TAG] = "90312901291239012390"; // The Data has been through a local public affairs office which assigned the data this unique ID
  model->setRequiredMetaData(md);
  
  // So now our model should be valid. We can check the validity of the model with the following:
  std::string message;
  bool valid = model->isValid(message);
  if ( !valid )
  {
    std::cout << "Model was NOT valid. Exiting with Failure. Error message given was: \n" << message << std::endl;
    return EXIT_FAILURE;
  }
  
  //Write the model to a new HDF5 file, deleting any existing file and
  // allowing the Hdf5 file to remain open for further processing
  IDataFilePtr dataFile = H5MXADataFile::CreateFileWithModel(MXA_FILE, modelPtr);
  
  if (NULL == dataFile.get() )
  {
    std::cout << "Error Writing Model to HDF5 File" << std::endl;
    return EXIT_FAILURE;
  }
  
  //Lets store some data into the HDF5 File. In our experiment we are recording the time
  // in 1 minute intervals for 10 minutes and also incrementing the pressure by
  // 200 KPa starting at 200 and ending at 800 KPa. At each combination of those
  // values we are taking the temperature and capturing an image of our sample
  std::vector<int32> indices(2, 0); // we keep this for re-use during the loop
  std::string temperaturePath;
  std::string cameraImagePath;
  float temperature = 1200.0f;
  MXAAbstractDataPtr imagePtr;
  
  H5DataArrayTemplate<float32>* temperatureArray = H5DataArrayTemplate<float32>::New("", 1);
  MXAAbstractDataPtr temperaturePtr(temperatureArray); // Let boost clean up the memory when we are finished with it
  int32 err = 0;
  // Define the height/width of our camera "image"
  std::vector<hsize_t> dims (2,0);
  dims[0] = 10;
  dims[1] = 10;
  for (int t = 0; t <= 9; ++t)
  {
    indices[0] = t;
    for (int p = 200; p <= 800; p+=200)
    {
      temperature += (float)p;
      indices[1] = p;
      temperaturePath = H5Utilities::generateH5PathToDataset(modelPtr, indices, temp );
      cameraImagePath = H5Utilities::generateH5PathToDataset(modelPtr, indices, cameraImage );
      
      temperatureArray->setDatasetPath(temperaturePath);
      temperatureArray->setValue(0, temperature);
       // Write the temperature value to the HDF5 File
      err = temperatureArray->writeToFile(dataFile);
      if (err < 0)
      {
        std::cout << "Error writing temperature dataset for (t,p):" << t << "," << p << std::endl;
        break;
      }
      imagePtr = captureSampleImage(cameraImagePath);
      err = imagePtr->writeToFile(dataFile);
      if (err < 0)
      {
        std::cout << "Error writing image dataset for (t,p):" << t << "," << p << std::endl;
        break;
      }
    }
  }
 
  std::cout << "... Ending Example 2" << std::endl;
  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAbstractDataPtr captureSampleImage(const std::string &datasetPath)
{
  MXAAbstractDataPtr imagePtr;
  H5RGBImage* image = H5RGBImage::New(datasetPath, 256, 256);
  if (NULL == image)
  {
    return imagePtr;
  }
  imagePtr.reset(image);
  uint8* data = image->getPixelPointer(0,0);
  int32 xDim = 256;
  int32 yDim = 256;
  for (int i = 0; i < xDim; ++i) {
    for (int j = 0; j < yDim; ++j) {
      data[0] = i * j;
      data[1] = i;
      data[2] = j;
      data +=3; // move the pointer by 3 bytes
    }
  }
  return imagePtr;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXAAbstractDataPtr captureSampleSignal(const std::string &datasetPath)
{
  MXAAbstractDataPtr imagePtr;
  H5Data2DArray<uint8>* image = H5Data2DArray<uint8>::New(datasetPath, 256, 256);
  if (NULL == image)
  {
    return imagePtr;
  }
  imagePtr.reset(image);
  uint8* data = image->getPointer(0,0);
  int32 xDim = 256;
  int32 yDim = 256;
  for (int i = 0; i < xDim; ++i) {
    for (int j = 0; j < yDim; ++j) {
      data[0] = i * j;
      data++; // move the pointer by 3 bytes
    }
  }
  return imagePtr;
}


