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



#include <DataImport/DataImportXmlParser.h>
#include <DataImport/ImportDelegateManager.h>
#include <Examples/DataImport/SimpleImportExample.h>
#include <Examples/DataImport/ExampleImportDelegateFactory.h>

#include <iostream>
#include <string>
// -----------------------------------------------------------------------------
//  Define where to put our temporary files
// -----------------------------------------------------------------------------
#if defined (_WIN32)
const std::string SimpleImportFile("C:\\WINDOWS\\Temp\\SimpleImportExample.h5");
#else
const std::string SimpleImportFile("/tmp/SimpleImportExample.h5");
#endif

// -----------------------------------------------------------------------------
//  This program takes NO arguments
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
  std::cout << "Starting SimpleImportExample" << std::endl;

  // Instantiate the Instance Manager for import delegates
  ImportDelegateManagerPtr idManager = ImportDelegateManager::instance();

  // Register our Import Delegate
  AbstractImportDelegateFactoryPtr exampleImportDelegateFactory ( new ExampleImportDelegateFactory() );
  idManager->registerDataImportFactory( exampleImportDelegateFactory );

  SimpleImportExample simple;
  simple.runImport( SimpleImportFile );
  return EXIT_SUCCESS;
}


