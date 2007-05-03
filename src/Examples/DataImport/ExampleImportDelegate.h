#ifndef EXAMPLEIMPORTDELEGATE_H_
#define EXAMPLEIMPORTDELEGATE_H_


#include "MXADataModel/AbstractDataParserDelegate.h"



class ExampleImportDelegate : public AbstractDataParserDelegate
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
