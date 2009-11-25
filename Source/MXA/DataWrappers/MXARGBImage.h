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
#ifndef _MXARGBImage_h_
#define _MXARGBImage_h_


#include <MXA/DataWrappers/MXAArrayTemplate.hpp>


/**
* @class MXARGBImage MXARGBImage.h HDF5/MXARGBImage.h
* @brief This class represents a 2D array of rgb values. The data is written to
* the data file using the H5Image protocol. This means that some extra attributes
* are written to the dataset in addition to the data. If this is NOT what you want
* then use the generic MXA2DArray class instead.
* @author mjackson
* @date Jan 9, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT MXARGBImage : public MXAArrayTemplate<uint8>
{
  public:
    static IMXAArray::Pointer CreateRGBImageArray(int32 width, int32 height);

    static MXARGBImage* New( int32 width, int32 height);

    virtual ~MXARGBImage();


    /**
     * Returns the number of dimensions the data has.
     */
    virtual int32 getNumberOfDimensions ();

    // -----------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------
    virtual void getDimensions(uint64* dims)
    {
      dims[1] = _width;
      dims[0] = _height;
    }

    uint8* getPixelPointer(int32 x, int32 y);
    virtual int32 getWidth() { return _width/3; }
    virtual int32 getHeight() { return _height; }


    virtual int32 resize(uint64 size);

    /**
     * @brief Resizes the data array to the specified width and height
     * @param width The new width of the array
     * @param height The new height of the array
     * @return 1 on success and Zero (0) on failure
     */
    int32 resizeArray(int32 width, int32 height);

    /**
    * @brief Serializes the value(s) of the array to a string using the delimiter
    * @delimiter The character to use to separate the values - Default is a single space
    */
    virtual std::string valueToString(char delimiter = ' ');

  protected:
      MXARGBImage( int32 width, int32 height);

  private:
    int32 _width;
    int32 _height;

    MXARGBImage(const MXARGBImage&);    //Not Implemented
    void operator=(const MXARGBImage&); //Not Implemented

};

#endif //_MXARGBImage_h_




