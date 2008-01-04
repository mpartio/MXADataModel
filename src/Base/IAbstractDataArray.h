///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _IAbstractDataArray_h_
#define _IAbstractDataArray_h_

#include <Base/IDataFileIO.h>


/**
* @class IAbstractDataArray IAbstractDataArray.h PathToHeader/IAbstractDataArray.h
* @brief This class holds a raw pointer to some allocated data that can be stored
* into or read from an HDF5 data file.
* The class design was borrowed heavily from the vtkDataArray class from www.vtk.org.
* The work was all performed by those individuals. I have merely changed a few 
* methods to meet my specific needs.
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.1 $
*/
class MXA_EXPORT IAbstractDataArray : public IDataFileIO
{
  public:
    IAbstractDataArray() {}
    virtual ~IAbstractDataArray() {}
    
    virtual void takeOwnership () = 0;
    virtual void releaseOwnership () = 0;
    virtual void* getVoidPointer ( mxaIdType i) = 0;
    virtual mxaIdType getNumberOfElements () = 0;
    virtual int32 getNumberOfDimensions () = 0;
    virtual int32 getDataType () = 0;
      
  protected:  
  
  private:
      IAbstractDataArray (const IAbstractDataArray&);    //Not Implemented
      void operator=(const IAbstractDataArray&); //Not Implemented
  
};

#endif //_IAbstractDataArray_h_




