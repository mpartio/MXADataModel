///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _SIMPLE_IMPORT_EXAMPLE_H_
#define _SIMPLE_IMPORT_EXAMPLE_H_

#include <Common/MXATypeDefs.h>

#include <string>


/**
* @class SimpleImportExample SimpleImportExample.h Examples/DataImport/SimpleImportExample.h
* @brief This class shows how to write code that will read data from an external
* file specified in an IDataSource object and write that data to the correct location
* within the HDF5 file as specified in the MXA Data Model.
* @author Michael A. Jackson. BlueQuartz Software http://www.bluequartz.net
* @date Sept 2008
* @version $Revision: 1.4 $
*/
class SimpleImportExample
{
public:
	SimpleImportExample();
	virtual ~SimpleImportExample();

  /**
   * @brief Runs the example
   * @param outputFilePath The path to the HDF5 file that will be created
   */
  void runImport(const std::string &outputFilePath);

protected:

  /**
   * @brief Creates a simple MXA model to use for this example.
   * @return A valid MXA model object
   */
  MXADataModelPtr createSimpleModel();

  /**
   * @brief Creates some files with binary data that are used as input for this example program.
   * @param model The mxa data model
   */
  void createTestFiles(MXADataModelPtr model);


  private:
      SimpleImportExample(const SimpleImportExample&);  //Copy constructor NOT implemented
      void operator=(const SimpleImportExample); //Copy assignment NOT implemented

};

#endif /*_SIMPLE_IMPORT_EXAMPLE_H_*/
