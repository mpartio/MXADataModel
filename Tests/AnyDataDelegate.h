///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ANYDATADELEGATE_H_
#define ANYDATADELEGATE_H_


#include "MXA/MXA.h"
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Base/IImportDelegate.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Core/MXADataModel.h>



/**
* @brief  Template Class to allow any type of data to be created and stored in an HDF5 File
* @author Mike Jackson
* @date June 2007
* @version $Revision: 1.2 $
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
    int32_t importDataSource(IDataSource::Pointer dataSource, IDataFile::Pointer dataFile)
    {
      std::string path ( dataSource->generateInternalPath() );
    //  std::cout << logTime() << "VectorDataDelegate::importDataSource() " << path << std::endl;
      uint32_t pos = path.find_last_of("/");
      std::string parentPath ( path.substr(0, pos)  );
      hid_t fileId = dataFile->getFileId();
      H5Utilities::createGroupsFromPath(parentPath, fileId);
      //Create data
      
      typename std::vector<T>::size_type numElements = 1;
      for (std::vector<hsize_t>::iterator iter = _dims.begin(); iter != _dims.end(); ++iter )
      {
        numElements += numElements * static_cast<typename std::vector<T>::size_type>(*iter);
      }
      std::vector<int32_t> dataDimValues = dataSource->getDimensionValues();
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
