#ifndef _IDataArray_h_
#define _IDataArray_h_

//#include <Base/IFileWriter.h>
//#include <Base/IFileReader.h>
#include <HDF5/H5Lite.h>
//-- HDF5 Includes
#include <hdf5.h>


/**
* @class IDataArray IDataArray.h Dataset/IDataArray.h
* @brief 
* @author Mike Jackson
* @date Dec 31, 2007
* @version $Revision: 1.1 $
*/
class IDataArray
{
  public:
    IDataArray(){}
    virtual ~IDataArray(){}
    
    
    virtual void takeOwnership() = 0;
    virtual void releaseOwnership() = 0;
    virtual void* getVoidPointer(mxaIdType i) = 0;
    virtual mxaIdType getNumberOfElements() = 0;
    virtual int32 getNumberOfDimensions() = 0;
    
    virtual int32 writeDataToFile(IDataFilePtr dataFile, const std::string &path) = 0;
    virtual int32 writeAttributeToFile(IDataFilePtr dataFile, const std::string &path, const std::string &key) = 0;

    virtual int32 readDataFromFile(IDataFilePtr dataFile, const std::string &path) = 0;
    virtual int32 readAttributeFromFile(IDataFilePtr dataFile, const std::string &path, const std::string &key) = 0;
    
    virtual void debugPrint(std::ostream &out) = 0;
  protected:  
  
  private:
      IDataArray(const IDataArray&);    //Not Implemented
      void operator=(const IDataArray&); //Not Implemented
  
};

#endif //_IDataArray_h_


\



