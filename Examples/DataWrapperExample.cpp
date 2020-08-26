///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////


//-- MXA Includes
#include "MXA/MXA.h"
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>

#ifdef MXA_HDF5_SUPPORT
//-- HDF5 includes
#include <hdf5.h>

#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#endif

#include <Examples/ExampleFileLocations.h>

//-- C++ Includes
#include <iostream>




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InitializeFile()
{
  herr_t err = 0;
  hid_t fileId = H5Utilities::createFile(Examples::DataWrapperExampleFile);
  if (fileId < 0)
  {
    std::cout << "COULD NOT CREATE OUTPUT FILE '" << Examples::DataWrapperExampleFile << "'" << std::endl;
    return;
  }

  err = H5Utilities::closeFile(fileId);
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void WriteHDF5File(IMXAArray* data, const std::string &datasetname)
{
  herr_t err = 0;
  hid_t fileId = H5Utilities::openFile(Examples::DataWrapperExampleFile, false);
  if (fileId < 0)
  {
    std::cout << "COULD NOT CREATE OUTPUT FILE '" << Examples::DataWrapperExampleFile << "'" << std::endl;
    return;
  }

  err = H5Lite::writeMXAArray(fileId, datasetname, data);
  if (err < 0)
  {
    std::cout << "ERROR WRITING DATA ARRAY '" << datasetname << "'" << std::endl;
  }

  err = H5Utilities::closeFile(fileId);

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
/**
 * @brief This example program simply shows how to use the DataWrapper classes to
 * wrap some raw arrays with C++ style objects. Also included are examples of using
 * the classes in combination with the boost::shared_ptr classes. At the end of running
 * the example you can use h5dump to show the contents of the file.
 *
 * @param argc
 * @param argv
 * @return exit code.
 */
int main(int argc, char **argv) {
  std::cout << "Running DataWrapper Example.." << std::endl;

  // Create our output file, overwriting any existing file
  InitializeFile();

  size_t numValues = 10;
  // Create an Array to hold a 1 Dimensional Array of 10 values of 32 bit integers
  MXAArrayTemplate<int32_t>* int32_tarray = MXAArrayTemplate<int32_t>::New(numValues);

  // Since we used the static "New" method without any wrapper we are responsible for deleting the pointer.
  // The array is full of junk values as we have not placed any default values into the array. You should
  // not count on the compiler to set the values for you.

  // We can Initialize the values 2 different ways. We can set each value in a loop using the
  // setValue() method or we can get a pointer to the front of the array and then
  // use pointer manipulations to set the values.
  for (int i = 0; i < numValues; ++i)
  {
    int32_tarray->setValue(i, 33);  // Set each value in the array to 33
  }

  int32_t* ptr32 = int32_tarray->getPointer(0); // Get a pointer to the '0' index which is the first element in the array
  for (int i = 0; i < numValues; ++i)
  {
    *ptr32 = 33;  // Set each value in the array to 33
    ptr32++; // Increment the pointer
  }

  //Other operations on the data include resizing the array, which could potentially
  // destroy any existing data, retrieving the number of dimensions and the sizes
  // of the dimensions

  int32_t numDimensions = int32_tarray->getNumberOfDimensions();
  std::cout << "There are " << numDimensions << " dimensions in the array." << std::endl;
  std::vector<size_t> dimensions(numDimensions);
  int32_tarray->getDimensions( &(dimensions.front() ) );

  for (int i = 0; i < numDimensions; ++i) {
    std::cout << "Dimension[" << i << "] = " << dimensions[i] << std::endl;
  }
  //Write the data
  WriteHDF5File(int32_tarray, "Int32PointerExampleData");

  delete int32_tarray; // Clean up the memory

  std::cout << "-------------------------------------------------" << std::endl;

  // We can create a Multi-Dimensional Array easily enough
  numDimensions = 3;
  // Add some more dimension sizes into the dimensions vector.
  dimensions.push_back(3);
  dimensions.push_back(5);

  // Create a 3D array of size 10 x 3 x 5 of 8bit signed char values
  MXAArrayTemplate<int8_t>* int8_tArray = MXAArrayTemplate<int8_t>::New(numDimensions, &(dimensions.front() ) );
  numValues = int8_tArray->getNumberOfElements();

  int8_t* ptr8 = int8_tArray->getPointer(0); // Get a pointer to the '0' index which is the first element in the array
  for (int i = 0; i < numValues; ++i)
  {
    *ptr8 = 1;  // Set each value in the array to 33
    ptr8++; // Increment the pointer
  }

  numDimensions = int8_tArray->getNumberOfDimensions();
  std::cout << "There are " << numDimensions << " dimensions in the array." << std::endl;
  dimensions.resize(numDimensions);
  int8_tArray->getDimensions( &(dimensions.front() ) );

  for (int i = 0; i < numDimensions; ++i) {
    std::cout << "Dimension[" << i << "] = " << dimensions[i] << std::endl;
  }

  WriteHDF5File(int8_tArray, "RawPointerExampleData");

  delete int8_tArray; // Clean up the memory

  std::cout << "-------------------------------------------------" << std::endl;

  /* The next section is basically the same as the above but uses the boost shared
   * pointer wrapped versions. This is advantageous because it turns the pointer
   * into a reference counted object and allows for easy memory management
   */
  {
    numValues = 10;
    IMXAArray::Pointer bPtr32 = MXAArrayTemplate<int32_t>::CreateArray(numValues);
    MXAArrayTemplate<int32_t>* wrappedPointer = dynamic_cast< MXAArrayTemplate<int32_t>* > (bPtr32.get() );

    // bPtr32 is a boost::shared_ptr object that wraps the actual pointer to the MXAArrayTemplate
    // bPtr32 will ALWAYS be valid. The contained pointer is what you want to check to make
    // sure the allocation was successful. If the allocation was NOT successful then
    // the contained pointer will be NULL.
    if ( NULL == bPtr32.get() )
    {
      std::cout << "MXAArrayTemplate<int32_t> was NOT allocated properly." << std::endl;
    }
    // We can initialize the array in the same way since the operators for boost::shared_ptr are overloaded.
    for (int i = 0; i < numValues; ++i)
    {
      wrappedPointer->setValue(i,33);
    }
    // Write the data to an HDF5 file
    WriteHDF5File(wrappedPointer, "WrappedBoostSharedPointer");


    // When this code section exits, the bPtr32 will go out of scope and the destructor
    // for the MXAArrayTemplate class will get called and the array cleaned up. The
    // pointer 'wrappedPointer' will also become invalid.
    wrappedPointer = NULL;
  }

  std::cout << "Data Wrapper Example Complete." << std::endl;
  return EXIT_SUCCESS;
}


