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

#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <iostream>

/**
* @class IMXAArray IMXAArray.h PathToHeader/IMXAArray.h
* @brief This class holds a raw pointer to some allocated data that can be stored
* into or read from an HDF5 data file.
* The class design was borrowed heavily from the vtkDataArray class from www.vtk.org.
* The work was all performed by those individuals. I have merely changed a few
* methods to meet my specific needs.
* @author mjackson
* @date Jan 3, 2008
* @version $Revision: 1.4 $
*/
class MXA_EXPORT IMXAArray
{
  public:
    IMXAArray() {}
    virtual ~IMXAArray() {}

    /**
     * @brief Makes this class responsible for freeing the memory.
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
     * @brief Copies the values of the dimensions into the supplied pointer.
     * @param dims
     */
    virtual void getDimensions(uint64* dims) = 0;

    /**
     * @brief Returns an enumerated type that can be used to find out the type
     * of primitive stored in the internal array.
     */
    virtual int32 getDataType () = 0;

    /**
     * @brief Returns the number of bytes that make up the data type.
     * 1 = char
     * 2 = 16 bit integer
     * 4 = 32 bit integer/Float
     * 8 = 64 bit integer/Double
     */
    virtual size_t getTypeSize() = 0;

    /**
     * @brief Byte swaps the Elements in the array. Needed if you want to convert
     * the endianess of the data.
     */
    virtual void byteSwapElements() = 0;

    /**
     * @brief Sets all the values to zero.
     */
    virtual void initializeWithZeros() = 0;

    /**
     * @brief Prints information about the class to ostream.
     * @param os
     * @param indent
     */
    virtual void printSelf(std::ostream &os, int32 indent) = 0;

    /**
     * @brief Converts the data array into a string delimited by the supplied
     * delimiter.
     * @param delimiter The delimiter to use between each value
     * @return The generated string
     */
    virtual std::string valueToString(char delimiter) = 0;


    /**
     * @brief Saves the array to an external file on the system
     * @param expProps The properties to use when saving the array
     * @return Error code.
     */
    virtual int32 exportToFile(DataExportPropertiesPtr expProps) = 0;

  protected:

  private:
      IMXAArray (const IMXAArray&);    //Not Implemented
      void operator=(const IMXAArray&); //Not Implemented

};

#endif //_IAbstractDataArray_h_




