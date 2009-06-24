///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

/**
 * This example covers a more complex situation where you would be importing
 * a custom data file that needs special parsing code. This code would be put into
 * an IImportDelegate class. That class is instantiated through a factory method
 * that is registered with the static ImportDelegateManager class. After that is
 * all complete this example creates some temporary data files in order to have
 * something for the delegate to read. The MXADataImport class is then used to
 * run the import procedure. The Import delegate will read the binary files that
 * were created earlier and write the data into the HDF5 file.
 */


//-- MXA Includes
#include <MXA/DataImport/DataImportXmlParser.h>
#include <MXA/DataImport/ImportDelegateManager.h>
#include <Examples/DataImport/SimpleImportExample.h>
#include <Examples/DataImport/ExampleImportDelegateFactory.h>
#include <Examples/ExampleFileLocations.h>

//-- C++ Includes
#include <iostream>
#include <string>


// -----------------------------------------------------------------------------
//  This program takes NO arguments
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
  std::cout << "Starting SimpleImportExample" << std::endl;

  // Register the known ImportDelegates, which will also instantiate the ImportDelegateManager instance.
  // The ImportDelegateManager instance is a singleton so only one can ever be active per program.
  ImportDelegateManager::registerKnownImportDeletegateFactories();

  // Register our Import Delegate by using the static methods from ImportDelegateManager
  AbstractImportDelegateFactory::Pointer exampleImportDelegateFactory ( new ExampleImportDelegateFactory() );
  ImportDelegateManager::registerImportDelegateFactory(exampleImportDelegateFactory);

  // Instantiate the SimpleImportExample class
  SimpleImportExample simple;
  // Execute the import code
  simple.runImport( Examples::SimpleImportExampleFile );


  return EXIT_SUCCESS;
}


