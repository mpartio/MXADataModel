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

#if defined (_WIN32)
const std::string DataInputFile("C:\\WINDOWS\\Temp\\DataInput_");
#else
const std::string DataInputFile("/tmp/DataInput_");
#endif

/**
* @class SimpleImportExample SimpleImportExample.h Examples/DataImport/SimpleImportExample.h
* @brief
* @author Michael A. Jackson. BlueQuartz Software http://www.bluequartz.net
* @date Sept 2008
* @version $Revision: 1.2 $
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

  MXADataModelPtr createSimpleModel();

  void createTestFiles(MXADataModelPtr model);


  private:
      SimpleImportExample(const SimpleImportExample&);  //Copy constructor NOT implemented
      void operator=(const SimpleImportExample); //Copy assignment NOT implemented

};

#endif /*_SIMPLE_IMPORT_EXAMPLE_H_*/
