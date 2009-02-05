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
#ifndef ANYDATADELEGATE_H_
#define ANYDATADELEGATE_H_


#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Base/IImportDelegate.h>
#include <Base/IDataFile.h>
#include <Core/MXADataModel.h>



/**
* @brief  Template Class to allow any type of data to be created and stored in an HDF5 File
* @author Mike Jackson
* @date June 2007
* @version $Revision: 1.1 $
* @class AnyDataDelegate AnyDataDelegate.h src/Test/AnyDataDelegate.h
*/
template <typename T>
class AnyDataDelegate: public IImportDelegate
{
  
  public: 
    AnyDataDelegate(std::vector<hsize_t> dims) 
    {
      this->_dims = dims;
    }
    
    virtual ~AnyDataDelegate() {};
    
    /**
     * @brief  Imports the data from the datasource object into the HDF5 data file
     * @param dataSource Valid IDataSource Object
     * @param dataFile The IDataFile object
     * @return error condition. Negative is error.
     */
    int32 importDataSource(IDataSourcePtr dataSource, IDataFilePtr dataFile)
    {
      std::string path ( dataSource->generateInternalPath() );
    //  std::cout << logTime() << "VectorDataDelegate::importDataSource() " << path << std::endl;
      uint32 pos = path.find_last_of("/");
      std::string parentPath ( path.substr(0, pos)  );
      hid_t fileId = dataFile->getFileId();
      H5Utilities::createGroupsFromPath(parentPath, fileId);
      //Create data
      
      typename std::vector<T>::size_type numElements = 1;
      for (std::vector<hsize_t>::iterator iter = _dims.begin(); iter != _dims.end(); ++iter )
      {
        numElements += numElements * static_cast<typename std::vector<T>::size_type>(*iter);
      }
      std::vector<int32> dataDimValues = dataSource->getDimensionValues();
      std::vector<T> data;
      data.resize(numElements);
      for (typename std::vector<T>::size_type i = 0; i < numElements; ++i)
      {
        data[i] = static_cast<T>(i * dataDimValues[0]);
      }
      return H5Lite::writeVectorDataset(fileId, path, _dims, data);
    }
  
  private:
    std::vector<hsize_t> _dims;
};


#endif /*ANYDATADELEGATE_H_*/
