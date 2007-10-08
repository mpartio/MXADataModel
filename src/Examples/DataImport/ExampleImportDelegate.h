#ifndef EXAMPLEIMPORTDELEGATE_H_
#define EXAMPLEIMPORTDELEGATE_H_


#include <Base/IDataImportDelegate.h>


/**
* @class ExampleImportDelegate ExampleImportDelegate.h src/Examples/DataImport/ExampleImportDelegate.h
* @brief An example of how to write a basic IDataImportDelegate class 
* @author Mike Jackson
* @date April 2007
* @version $Revision: 1.6 $
*/
class ExampleImportDelegate : public IDataImportDelegate
{
public:
	ExampleImportDelegate();
	virtual ~ExampleImportDelegate();
  
	/**
	 * @brief Imports the data pointed to by the filepath into the HDF5 file
	 * @param fileId The HDF5 fileid
	 * @param recordGroup The Data record group 
	 * @param recordName The data record name
	 * @param filePath The path to the data file
	 * @return Negative value on error
	 */
  int32 encodeSourceToHDF5( hid_t fileId, const std::string &recordGroup,
                              const std::string &recordName, 
                              const std::string &filePath);
  
  
  private:
      ExampleImportDelegate(const ExampleImportDelegate&);  //Copy constructor NOT implemented
      void operator=(const ExampleImportDelegate); //Copy assignment NOT implemented
};

#endif /*EXAMPLEIMPORTDELEGATE_H_*/
