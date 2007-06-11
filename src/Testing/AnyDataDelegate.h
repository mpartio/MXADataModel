#ifndef ANYDATADELEGATE_H_
#define ANYDATADELEGATE_H_


#include "Headers/MXATypes.h"
#include "Headers/MXATypeDefs.h"
#include "Interfaces/IDataImportDelegate.h"
#include "MXADataModel/MXADataModel.h"



// -----------------------------------------------------------------------------
//  Class to allow any type of data to be created and stored in an HDF5 File
// -----------------------------------------------------------------------------
template <typename T>
class AnyDataDelegate: public IDataImportDelegate
{
  
  public: 
    AnyDataDelegate(std::vector<uint64> dims) 
    {
      this->_dims = dims;
    }
    
    virtual ~AnyDataDelegate() {};
    
    int32 importDataSource(MXADataSourcePtr dataSource, MXADataModelPtr model)
    {
      std::string path ( dataSource->generateInternalPath() );
    //  std::cout << logTime() << "VectorDataDelegate::importDataSource() " << path << std::endl;
      uint32 pos = path.find_last_of("/");
      std::string parentPath ( path.substr(0, pos)  );
      hid_t fileId = model->getIODelegate()->getOpenFileId();
      H5Utilities::createGroupsFromPath(parentPath, fileId);
      //Create data
      unsigned long int numElements = 1;
      for (std::vector<uint64>::iterator iter = _dims.begin(); iter != _dims.end(); ++iter )
      {
        numElements += numElements * (*iter);
      }
      std::vector<int32> dataDimValues = dataSource->getDimensionValues();
      std::vector<T> data;
      data.resize(numElements);
      for (unsigned long int i = 0; i < numElements; ++i)
      {
        data[i] = static_cast<T>(i * dataDimValues[0]);
      }
  
      
      return H5Lite::writeVectorDataset(fileId, path, _dims, data);
    }
  
  private:
    std::vector<uint64> _dims;
};


#endif /*ANYDATADELEGATE_H_*/
