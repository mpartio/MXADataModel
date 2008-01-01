#ifndef _IDataArrayTemplate_h_
#define _IDataArrayTemplate_h_

#include <Dataset/IDataArray.h>


/**
* @class IDataArrayTemplate IDataArrayTemplate.h Dataset/IDataArrayTemplate.h
* @brief 
* @author mjackson
* @date Dec 31, 2007
* @version $Revision: 1.1 $
*/
template<typename T>
class IDataArrayTemplate : public IDataArray
{
  public:
    
    static IDataArrayPtr New(int32 numElements, T type)
    {
      IDataArrayPtr ptr ( new IDataArrayTemplate<T>(numElements) );
      return ptr;
    }
        
    
    virtual ~IDataArrayTemplate() {}
     
    
// -----------------------------------------------------------------------------
//  IFileWriter Implementation is pure virtual. Subclasses need to implement.
// -----------------------------------------------------------------------------
    virtual int32 writeToFile(IDataFilePtr dataFile)
    {
      return -1;
    }

// -----------------------------------------------------------------------------
//  IFileReader Implementation is pure virtual. Subclasses need to implement
// -----------------------------------------------------------------------------
    virtual int32 readFromFile(IDataFilePtr dataFile)
    {
      return -1;
    }
        
        
  protected:
    IDataArrayTemplate(int32 numElements)
    {
      this->_size = numElements;
    }
    
    T* _data;
    int32 _size;
    
  private:
      IDataArrayTemplate(const IDataArrayTemplate&);    //Not Implemented
      void operator=(const IDataArrayTemplate&); //Not Implemented
  
};

#endif //_IDataArrayTemplate_h_




