///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Core/MXASupportFile.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <Examples/ExampleFileLocations.h>
#include <Tests/TiffMaker.h>

#include <iostream>

// -----------------------------------------------------------------------------
// Create our data model
// -----------------------------------------------------------------------------
MXADataModelPtr createDataModel()
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
    MXADataModelPtr nullPtr;
    return nullPtr;
  }
  return modelPtr;
}

// -----------------------------------------------------------------------------
//  Create an example File to use as an import file
// -----------------------------------------------------------------------------
void createTiffImage(const std::string &file)
{
  TiffMaker tiffMaker;
  tiffMaker.createTiffFile(file);
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {

  // Create a data model to use for this example
  MXADataModelPtr model = createDataModel();
  if (NULL == model.get())
  {
    return EXIT_FAILURE;
  }

  // Create a tiff image that we would like to import as a support file
  createTiffImage(Examples::SupportFileInput);

  // Now create a 'SupportFile' object and add it to the data model. We use 'false'
  // as the last argument because we don't want the file to be read immediately,
  // but rather the file imported into the datamodel output file 'on demand' during
  // the saving of the DataModel to the HDF5 file
  ISupportFilePtr supFile = MXASupportFile::NewFromFileSystem(Examples::SupportFileInput, SupportFile::FileType::Binary, false);

  // We use 'true' as the last argument because we want the data model to keep the
  // internal indexing for us. Most of the time that is what you want to do.
  model->addSupportFile(supFile, true);

  // Create the hdf5 datafile using the datamodel we created for its model. This will
  // attempt to write the model (and support files) to the data file immediately.
  IDataFilePtr dataFile = H5MXADataFile::CreateFileWithModel(Examples::SupportFileLocation, model);


  // Since we used boost::shared_ptrs to manage our memory for us we don't have to
  // call delete on anything. The output file should now be available in the binary
  // directory where this program was compiled.

  return EXIT_SUCCESS;
}





