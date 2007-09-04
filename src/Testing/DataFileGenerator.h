///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef DATAFILEGENERATOR_H_
#define DATAFILEGENERATOR_H_


#include "Common/MXATypes.h"
#include "Common/MXATypeDefs.h"
#include "Common/LogTime.h"
#include "Base/IDataImportDelegate.h"
#include "Base/IFileIODelegate.h"
#include "Core/MXADataModel.h"
#include "Core/MXADataDimension.h"
#include "Core/MXADataRecord.h"
#include "Core/MXADataImport.h"
#include "Core/MXADataSource.h"

#include "HDF5/H5Lite.h"
#include "HDF5/H5Utilities.h"

#include "Testing/AnyDataDelegate.h"


#include <string>

/** @namespace DataGen
 * @brief Used for the test data generation classes
 */
namespace DataGen
{
  const std::string DataRoot("DataFileGenerator/Data");
  const std::string Dimension1 ("Dimension 1");
  const std::string Dimension2 ("Dimension 2");
  const std::string ScalarRec ("Scalar Data");
  const std::string TableRec ("Table Data");
  const std::string VolumeRec ("Volume Data");
  
  const std::string Uint8Rec ("UInt8 Data");
  const std::string Uint16Rec ("UInt16 Data");
  const std::string Uint32Rec ("UInt32 Data");
  const std::string Uint64Rec ("UInt64 Data");
  const std::string Int8Rec ("Int8 Data");
  const std::string Int16Rec ("Int16 Data");
  const std::string Int32Rec ("Int32 Data");
  const std::string Int64Rec ("Int64 Data");
  const std::string Float32Rec ("Float32 Data");
  const std::string Float64Rec ("Float64 Data");
  
}

/**
 * @brief This class generates an HDF5 Data file with various types of dataset. 
 * 
 * Among those datasets imported into the data file include scalar and vector
 * versions of all the signed/unsigned integer types and 32/64 bit floating data.
 * See the source code for more information regarding the file layout
 * @class DataFileGenerator DataFileGenerator.h src/Testing/DataFileGenerator.h
 * @author Mike Jackson
 * @date July 2007
 * @version 1.0
 */
class DataFileGenerator
{
public:
	DataFileGenerator();
	virtual ~DataFileGenerator();
	
	/**
	 * @brief Sets the FilePath iVar
	 * @param filePath The path to the file
	 */
	void setFilePath(const std::string &filePath);
	
	/**
	 * @brief Generates the test data
	 * @return Error Condition
	 */
	herr_t generate();
	
	/**
	 * @brief Makes the Data Records for the data model
	 * @param model The MXA Data Model
	 * @param dataImport The IDataImport object
	 * @param parentRec The parent Data record to use for the newly created Data Record
	 * @param dims Valus for the Data Dimensions
	 * @return Error condition
	 */
	herr_t DataFileGenerator::makeRecords(MXADataModelPtr model, MXADataImportPtr dataImport, MXADataRecordPtr parentRec, std::vector<hsize_t> dims); 
	
/**
 * @brief Creates Data and stores the data in the HDF5 file
 * @param model The MXA Data Model
 * @param rec The data record for this data set
 * @param dataImport The IDataImport object
 * @param type The type of data
 * @param tableDims The dimensions of the Table
 * @return
 */
	template<typename T>
	herr_t createAndStore(MXADataModelPtr model, MXADataRecordPtr rec, 
	    MXADataImportPtr dataImport, T type, std::vector<hsize_t> tableDims)
	{

	  // We have two dimensions for this model, create a loop to create data sets for each possible dimension value
	   MXADataDimension* dim0 = model->getDataDimension(0); // Get the first Dimension, since there is only one this works
	  if (dim0 == NULL) 
	  {
	    std::cout << "Error getting the data dimension from the model: dim0" << std::endl;
	    return -1;
	  }
	   
	   MXADataDimension* dim1 = model->getDataDimension(1);
	   if (dim1 == NULL) 
	   {
	     std::cout << "Error getting the data dimension from the model: dim1" << std::endl;
	     return -1;
	   }
	   
	   // Set the start/end/increment values for each Data Dimension
	     int32 dim0Start = dim0->getStartValue();
	     int32 dim0End = dim0->getEndValue();
	     int32 dim0Increment = dim0->getIncrement();
	     
	     int32 dim1Start = dim1->getStartValue();
	     int32 dim1End = dim1->getEndValue();
	     int32 dim1Increment = dim1->getIncrement();
	     
	     // These are the actual dimensions of the data
	     IDataImportDelegate* delegate = new AnyDataDelegate<T>(tableDims);
	     boost::shared_ptr<IDataImportDelegate> delegatePtr (delegate);
	     
	     // Create a nested loop to create the necessary DataSource objects that will
	     //  be used to import the data into the HDF5 file
	     //std::cout << "CREATING DATA SOURCES" << std::endl;
	     for( int32 i = dim0Start; i <= dim0End; i += dim0Increment )
	     {
	       for (int j = dim1Start; j <= dim1End; j = j+ dim1Increment) 
	       {
	         //Create a Scalar Data Source
	         MXADataSourcePtr ds( new MXADataSource() );
	         std::vector<int32> dimValues;
	         dimValues.push_back(i);
	         dimValues.push_back(j);
	         ds->setDimensionValues(dimValues);
	         ds->setDataRecord(rec);
	         ds->setSourcePath(""); //Since we are doing everything in software, this is unused
	         ds->setImportDelegate(delegatePtr);
	         ds->setDataModel(model);
	         dataImport->addDataSource(ds);
	        
	       }
	     }
	     return 1;
	}
	
private:
  
  std::string _filePath;
  
  DataFileGenerator(const DataFileGenerator&);  //Copy constructor NOT implemented
    void operator=(const DataFileGenerator); //Copy assignment NOT implemented
};


/**
*  @brief This class is used to show simply how to write some data into an HDF5 file.
*  Some of the features of the IDataImportDelegate are not shown. This class
*  will simply write a single value to the HDF5 file.
* @author Mike Jackson
* @date April 2007
*/
class ScalarDataDelegate: public IDataImportDelegate
{
public:
  ScalarDataDelegate(){};
  virtual ~ScalarDataDelegate(){};

// -----------------------------------------------------------------------------
//  Implemented Method from the IDataImportDelegate interface 
// -----------------------------------------------------------------------------
  int32 importDataSource(IDataSourcePtr dataSource, IDataModelPtr model)
  {
    
    std::string path ( dataSource->generateInternalPath() );
  //  std::cout << logTime() << "ScalarDataDelegate::importDataSource() " << path << std::endl;
    uint32 pos = path.find_last_of("/");
    std::string parentPath ( path.substr(0, pos)  );
    int32 value = 55;
    hid_t fileId = model->getIODelegate()->getOpenFileId();
    H5Utilities::createGroupsFromPath(parentPath,  fileId);
    //Write the Data to the HDF5 File
    return H5Lite::writeScalarDataset(fileId, path, value);
  }
  
private:
  ScalarDataDelegate(const ScalarDataDelegate&);   //Copy Constructor Not Implemented
  void operator=(const ScalarDataDelegate&); //Copy Assignment Not Implemented
  
};

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class VectorDataDelegate: public IDataImportDelegate
{
public:
  VectorDataDelegate(){};
  virtual ~VectorDataDelegate(){};

// -----------------------------------------------------------------------------
//  Implemented Method from the IDataImportDelegate interface 
// -----------------------------------------------------------------------------
  int32 importDataSource(MXADataSourcePtr dataSource, MXADataModelPtr model)
  {
    std::string path ( dataSource->generateInternalPath() );
  //  std::cout << logTime() << "VectorDataDelegate::importDataSource() " << path << std::endl;
    uint32 pos = path.find_last_of("/");
    std::string parentPath ( path.substr(0, pos)  );
    hid_t fileId = model->getIODelegate()->getOpenFileId();
    H5Utilities::createGroupsFromPath(parentPath, fileId);
    //Create data in a 2x5 table
    std::vector<hsize_t> dims;
    dims.push_back(128);
    dims.push_back(128);
    dims.push_back(128);
    std::vector<uint8> data;
    for (int i = 0; i < 128; ++i)
    {
      for (int j = 0; j < 128; ++j)
      {
        for (int k = 0; k < 128; ++k)
        data.push_back( (i*j *k) * 255);
      }
    }
    
    return H5Lite::writeVectorDataset(fileId, path, dims, data);
  }
  
private:
  VectorDataDelegate(const VectorDataDelegate&);   //Copy Constructor Not Implemented
  void operator=(const VectorDataDelegate&); //Copy Assignment Not Implemented
  
};


#endif /*DATAFILEGENERATOR_H_*/
