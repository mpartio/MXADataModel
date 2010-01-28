///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#include "MXA/Base/IMXADataModelWriterDelegate.h"
#include "MXA/Core/MXADataModelWriter.hpp"

#include <MXA/HDF5/H5WriterDelegate.h>
#include <MXA/XML/XMLFileUtilities.hpp>
#include "MXA/XML/XMLMXAAttributeWriter.hpp"
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/DataWrappers/MXAAsciiStringData.h>
#include <MXAUnitTestDataFileLocations.h>
#include <MXA/Utilities/MXADir.h>
#include "UnitTestSupport.hpp"

#include <iostream>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RemoveTestFiles()
{
  std::cout << "   Removing Test files" << std::endl;
#if REMOVE_TEST_FILES
  MXADir::remove( MXAUnitTest::WriterDelegateTest::TestFile);
#endif
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int WriterDelegateTest_EntryPoint()
{
  int err = 0;
  {
    MXADataModel::Pointer model = createModel();
    MXASharedString xmlString = MXAXMLModelStringWriter::writeModel(model);
    MXA_REQUIRE(xmlString.get() != NULL);
    std::cout << "ref: <============>\n" << *(xmlString.get()) << std::endl;
  }

  {
    MXADataModel::Pointer model = createModel();
    err = MXAXMLModelFileWriter::writeModel(model, MXAUnitTest::WriterDelegateTest::TestFile);
    MXA_REQUIRE(err > 0)
  }
  return err;
}


// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  int err = EXIT_SUCCESS;
  MXA_REGISTER_TEST ( WriterDelegateTest_EntryPoint() );
  PRINT_TEST_SUMMARY();
  return err;
}


