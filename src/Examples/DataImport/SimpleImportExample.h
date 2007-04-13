#ifndef SIMPLEEXAMPLE_H_
#define SIMPLEEXAMPLE_H_

#include <string>

const std::string DATAMODEL_FILE("ni-al-cr-sample-datamodel.xml");
const std::string MHD_FILE("ni-al-cr-sample-datafile.h5");
const std::string MHD_FILE_W_SOURCES ("ni-al-cr-sample-datafile-w-sources.h5");

/**
* @class SimpleImportExample SimpleImportExample.h UnitTests/ModelConstructionEx/SimpleImportExample.h
* @brief 
* @author
* @date
* @version
*/
class SimpleImportExample
{
public:
	SimpleImportExample();
	virtual ~SimpleImportExample();
  
  /**
   * @brief Runs the example
   */
  void run();
  
  
  private:
      SimpleImportExample(const SimpleImportExample&);  //Copy constructor NOT implemented
      void operator=(const SimpleImportExample); //Copy assignment NOT implemented
    
};

#endif /*SIMPLEEXAMPLE_H_*/
