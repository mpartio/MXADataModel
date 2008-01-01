
#include <Common/MXATypeDefs.h>
#include <Base/IFileWriter.h>
#include <Base/IDataFile.h>
#include <string>
#include <iostream>

#include <boost/shared_ptr.hpp>

This layout makes things easier for ME but not easier for the end programmer. Implement 
each class in its own and take care of all the heavy lifting in the write* methods

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class IDataset : public IFileWriter
{
  public:
    IDataset() {}
    virtual ~IDataset() {}
    
    void setDatasetPath(const std::string &path)
    {
      _path = path;
    }
    std::string getDatasetPath()
    {
      return _path;
    }
   
    int getRank()
    {
     return _rank; 
    }
   
    void setDataDimensions(int rank, int32* dims)
    {
      _rank = rank;
      if (_dims != NULL)
      {
        ::free(dims);
      }
      _dims = static_cast<int32*>(::calloc(rank, sizeof(int32) ) );
      ::memcpy(_dims, dims, sizeof(int32)*rank);
    }
  
    virtual void setData(void* ptr, bool copyData = false)
    {
      if (copyData == true)
      {
        
      }
      else 
      {
        _data = ptr;
      }
    }
    
   
  private:
    std::string _path;
    int32 _rank;
    int32* _dims;
    void*  _data;
    
    IDataset(const IDataset&);    // Copy Constructor Not Implemented
    void operator=(const IDataset&);  // Operator '=' Not Implemented
  
};

typedef boost::shared_ptr<IDataset>   IDatasetPtr;

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class H5MXADataset : public IDataset
{
  public:
    H5MXADataset() {};
    ~H5MXADataset() {};
  

     void addAttribute(IDatasetPtr ds)
     {
       
     }
    
     int32 writeToFile(IDataFilePtr dataFile)
     {
       
     }
     
     
  private:

    
    H5MXADataset(const H5MXADataset&);    // Copy Constructor Not Implemented
    void operator=(const H5MXADataset&);  // Operator '=' Not Implemented
};


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
class H5MXAAttribute : public IDataset
{
  public:
    H5MXAAttribute() {};
    ~H5MXAAttribute() {};
  

     int32 writeToFile(IDataFilePtr dataFile)
     {
       std::cout << getRank() << std::endl;
       
     }
     
     
  private:
    
    H5MXAAttribute(const H5MXAAttribute&);    // Copy Constructor Not Implemented
    void operator=(const H5MXAAttribute&);  // Operator '=' Not Implemented
};

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int main(int argc, char **argv) 
{
  H5MXADataset* h5ds = new H5MXADataset;
  IDatasetPtr ds (h5ds);
  IDatasetPtr attr (new H5MXAAttribute);
  
  h5ds->addAttribute(attr);
  
  return EXIT_SUCCESS;
}
