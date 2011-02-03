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
class MXA_EXPORT MXARGBImage : public MXAArrayTemplate<uint8_t>
{
  public:
    static IMXAArray::Pointer CreateRGBImageArray(int32_t width, int32_t height);

    static MXARGBImage* New( int32_t width, int32_t height);

    virtual ~MXARGBImage();


    /**
     * Returns the number of dimensions the data has.
     */
    virtual int32_t getNumberOfDimensions ();

    // -----------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------
    virtual void getDimensions(size_t* dims)
    {
      dims[1] = _width;
      dims[0] = _height;
    }

    uint8_t* getPixelPointer(int32_t x, int32_t y);
    virtual int32_t getWidth() { return _width/3; }
    virtual int32_t getHeight() { return _height; }


    virtual int32_t resize(size_t size);

    /**
     * @brief Resizes the data array to the specified width and height
     * @param width The new width of the array
     * @param height The new height of the array
     * @return 1 on success and Zero (0) on failure
     */
    int32_t resizeArray(int32_t width, int32_t height);

    /**
    * @brief Serializes the value(s) of the array to a string using the delimiter
    * @delimiter The character to use to separate the values - Default is a single space
    */
    virtual std::string valueToString(char delimiter = ' ');

  protected:
      MXARGBImage( int32_t width, int32_t height);

  private:
    int32_t _width;
    int32_t _height;

    MXARGBImage(const MXARGBImage&);    //Not Implemented
    void operator=(const MXARGBImage&); //Not Implemented

};

#endif //_MXARGBImage_h_




