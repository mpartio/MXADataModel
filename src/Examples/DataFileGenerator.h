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


#include "Headers/MXATypes.h"
#include "Headers/MXATypeDefs.h"
#include "Headers/LogTime.h"
#include "Interfaces/IDataImportDelegate.h"
#include "Interfaces/IFileIODelegate.h"
#include "MXADataModel/MXADataModel.h"
#include "MXADataModel/MXADataDimension.h"
#include "MXADataModel/MXADataRecord.h"
#include "MXADataModel/MXADataImport.h"
#include "MXADataModel/MXADataSource.h"

#include "HDF5/H5Lite.h"
#include "HDF5/H5Utilities.h"

#include <string>


/**
 * @brief
 * @class
 * @author
 * @date
 * @version
 */
class DataFileGenerator
{
public:
	DataFileGenerator();
	virtual ~DataFileGenerator();
	
	void setFilePath(const std::string &filePath);
	
	herr_t generate();
	
	MXADataModelPtr createSimpleModel();
	herr_t ImportSimpleData(MXADataModelPtr model, std::string outputFilePath);
	
	
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
  int32 importDataSource(MXADataSourcePtr dataSource, MXADataModelPtr model)
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
