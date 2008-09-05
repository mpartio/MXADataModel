///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef EXAMPLEIMPORTDELEGATE_H_
#define EXAMPLEIMPORTDELEGATE_H_

#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Base/IImportDelegate.h>


/**
* @class ExampleImportDelegate ExampleImportDelegate.h src/Examples/DataImport/ExampleImportDelegate.h
* @brief An example of how to write a basic IDataImportDelegate class
* @author Mike Jackson
* @date April 2007
* @version $Revision: 1.7 $
*/
class ExampleImportDelegate : public IImportDelegate
{
public:
	ExampleImportDelegate();
	virtual ~ExampleImportDelegate();

  /**
   * @brief Imports the datasource to the data file
   * @param dataSource The source of the data
   * @param dataFile The IDataFile object
   * @return
   */
	int32 importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile);


  private:
      ExampleImportDelegate(const ExampleImportDelegate&);  //Copy constructor NOT implemented
      void operator=(const ExampleImportDelegate); //Copy assignment NOT implemented
};

#endif /*EXAMPLEIMPORTDELEGATE_H_*/
