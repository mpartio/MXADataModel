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

#include <MXA/Common/MXATypes.h>
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IImportDelegate.h>


/**
* @class ExampleImportDelegate ExampleImportDelegate.h src/Examples/DataImport/ExampleImportDelegate.h
* @brief An example of how to write a basic IDataImportDelegate class.
* @author Mike Jackson
* @date April 2007
* @version $Revision: 1.2 $
*/
class ExampleImportDelegate : public IImportDelegate
{
public:
	ExampleImportDelegate();
	virtual ~ExampleImportDelegate();

  /**
   * @brief Imports the datasource to the data file.
   * @param dataSource The source of the data
   * @param dataFile The IDataFile object
   * @return
   */
	int32 importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile);

  /**
  * @brief Sets a property of this class with the given value.
  * @param key The name of the property.
  * @param value  The value to set the property to.
  */
  virtual int32 setProperty(const std::string &key, const std::string &value)
  {
    std::cout << "ExampleImportDelegate::setProperty is NOT implemented" << std::endl;
    return -1;
  }

  virtual int32 getProperty(const std::string &key,  std::string &value)
  {
    std::cout << "ExampleImportDelegate::getProperty is NOT implemented" << std::endl;
    return -1;
  }

  private:
      ExampleImportDelegate(const ExampleImportDelegate&);  //Copy constructor NOT implemented
      void operator=(const ExampleImportDelegate); //Copy assignment NOT implemented
};

#endif /*EXAMPLEIMPORTDELEGATE_H_*/
