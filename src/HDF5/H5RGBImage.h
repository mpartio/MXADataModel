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
#ifndef _H5RGBImage_h_
#define _H5RGBImage_h_


#include <HDF5/H5DataArrayTemplate.hpp>


/**
* @class H5RGBImage H5RGBImage.h HDF5/H5RGBImage.h
* @brief This class represents a 2D array of rgb values. The data is written to 
* the data file using the H5Image protocol. This means that some extra attributes
* are written to the dataset in addition to the data. If this is NOT what you want 
* then use the generic H5Data2DArray class instead.
* @author mjackson
* @date Jan 9, 2008
* @version $Revision: 1.2 $
*/
class MXA_EXPORT H5RGBImage : public H5DataArrayTemplate<uint8> 
{
  public:
    static MXAAbstractDataPtr CreateAbstractDataArray(const std::string &datasetPath, 
                                  int32 width, int32 height);
    
    static H5RGBImage* New(const std::string &datasetPath, 
                          int32 width, int32 height);
    
    virtual ~H5RGBImage();
     
  
    /**
     * Returns the number of dimensions the data has.
     */
    virtual int32 getNumberOfDimensions ();
    
    uint8* getPixelPointer(int32 x, int32 y);
    virtual int32 getWidth() { return _width; }
    virtual int32 getHeight() { return _height; }
    

    virtual int32 resize(uint64 size);
        
    /**
     * @brief
     * @param width
     * @param height
     * @return
     */
    int32 resizeArray(mxaIdType width, mxaIdType height);
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileWriter)
// -----------------------------------------------------------------------------
    virtual int32 writeToFile (IDataFilePtr dataFile);
    
// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------
    virtual int32 readFromFile(IDataFilePtr dataFile);
    
    
  protected:  
      H5RGBImage(const std::string &datasetPath, 
                 int32 width, int32 height);
  private:
    int32 _width;
    int32 _height;
    
      H5RGBImage(const H5RGBImage&);    //Not Implemented
      void operator=(const H5RGBImage&); //Not Implemented
  
};

#endif //_H5RGBImage_h_




