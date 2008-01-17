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
#include <iostream>

/**
* @class IAbstractDataArray IAbstractDataArray.h PathToHeader/IAbstractDataArray.h
* @brief This class holds a raw pointer to some allocated data that can be stored
* into or read from an HDF5 data file.
* The class design was borrowed heavily from the vtkDataArray class from www.vtk.org.
* The work was all performed by those individuals. I have merely changed a few 
* methods to meet my specific needs.
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.4 $
*/
class MXA_EXPORT IAbstractDataArray : public IDataFileIO
{
  public:
    IAbstractDataArray() {}
    virtual ~IAbstractDataArray() {}
    
    /**
     * @brief Makes this class responsible for freeing the memory
     */
    virtual void takeOwnership () = 0;
    
    /**
     * @brief This class will NOT free the memory associated with the internal pointer. 
     * This can be useful if the user wishes to keep the data around after this
     * class goes out of scope.
     */
    virtual void releaseOwnership () = 0;
    
    /**
     * @brief Returns a void pointer pointing to the index of the array. NULL 
     * pointers are entirely possible. No checks are performed to make sure 
     * the index is with in the range of the internal data array.
     * @param i The index to have the returned pointer pointing to.
     * @return Void Pointer. Possibly NULL.
     */
    virtual void* getVoidPointer ( uint64 i) = 0;
    
    /**
     * @brief Returns the number of elements in the internal array.
     */
    virtual uint64 getNumberOfElements () = 0;
    
    /**
     * Returns the number of dimensions the data has.
     */
    virtual int32 getNumberOfDimensions () = 0;
    
    /**
     * @brief Copies the values of the dimensions into the supplied pointer
     * @param dims
     */
    virtual void getDimensions(uint64* dims) = 0;
    
    /**
     * @brief Returns an enumerated type that can be used to find out the type
     * of primitive stored in the internal array.
     */
    virtual int32 getDataType () = 0;
    
    /**
     * @brief Prints information about the class to ostream
     * @param os
     * @param indent
     */
    virtual void printSelf(std::ostream &os, int32 indent) = 0;
    
    
    
  protected:  
  
  private:
      IAbstractDataArray (const IAbstractDataArray&);    //Not Implemented
      void operator=(const IAbstractDataArray&); //Not Implemented
  
};

#endif //_IAbstractDataArray_h_




