#ifndef EXAMPLEIMPORTDELEGATE_H_
#define EXAMPLEIMPORTDELEGATE_H_


#include <Base/IDataImportDelegate.h>


/**
* @class ExampleImportDelegate ExampleImportDelegate.h src/Examples/DataImport/ExampleImportDelegate.h
* @brief An example of how to write a basic IDataImportDelegate class 
* @author Mike Jackson
* @date April 2007
* @version $Revision: 1.5 $
*/
class ExampleImportDelegate : public IDataImportDelegate
{
public:
	ExampleImportDelegate();
	virtual ~ExampleImportDelegate();
  
  int32 encodeSourceToHDF5( int32 fileId, std::string recordGroup,
                            std::string recordName, std::string filePath);
                            
  private:
      ExampleImportDelegate(const ExampleImportDelegate&);  //Copy constructor NOT implemented
      void operator=(const ExampleImportDelegate); //Copy assignment NOT implemented
};

#endif /*EXAMPLEIMPORTDELEGATE_H_*/
