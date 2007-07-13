///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _MXA_H5IMAGE_H
#define _MXA_H5IMAGE_H

//TODO: Reimplement Indexed image read/write
//TODO: Check other methods to conform to latest H5IM Spec
//TODO: Add ImageSpec Version attribute

//MXA Includes
#include "Headers/DLLExport.h"
#include "Headers/MXATypes.h"

//-- STL headers
#include <string>

//-- HDF5 Header
#include "hdf5.h"

namespace H5ImageConst
{
  // -----------------------------------------------------------------------------
  //  
  // -----------------------------------------------------------------------------
    const std::string ImageClass ("CLASS");
    const std::string Image("IMAGE");
    const std::string ImageVersion("IMAGE_VERSION");
    const std::string ImageSubclass("IMAGE_SUBCLASS");
    const std::string ImageIndexed("IMAGE_INDEXED");
    const std::string InterlacePixel("INTERLACE_PIXEL");
    const std::string InterlacePlane("INTERLACE_PLANE");
    const std::string ImageTrueColor("IMAGE_TRUECOLOR");
    const std::string InterlaceMode ("INTERLACE_MODE");
    const std::string Palette("PALETTE");
    const std::string PalVersion("PAL_VERSION");
    const std::string PalColorMap("PAL_COLORMAP");
    const std::string PalType("PAL_TYPE");
    const std::string Standard8("STANDARD8");
    const std::string RGB_COLOR("RGB");
    const std::string PalVersionValue("1.2");
    const std::string ImageGrayScale("IMAGE_GRAYSCALE");
    const std::string ImageWhiteIsZero("IMAGE_WHITE_IS_ZERO");
    const std::string ImageColorModel("IMAGE_COLORMODEL");
    const std::string DisplayOrigin("DISPLAY_ORIGIN");
    const std::string UpperLeft("UL");
    const std::string LowerLeft("LL");
    const std::string UpperRight("UR");
    const std::string LowerRight("LR");
}

/**
* @brief Reimplementation of the H5IM API from the HDF5 High Level API set.
* @date April 2007
*/
class H5Image
{

public:

/**
* @brief 
* @param loc_id
* @return 
*/
static MXA_EXPORT herr_t H5IM_find_palette( hid_t loc_id);

/**
* @brief 
* @param loc_id
* @param datasetName
* @param width
* @param height
* @param buffer
* @return 
*/
static MXA_EXPORT herr_t makeGrayScaleImage( hid_t loc_id,
                            std::string datasetName,
                            hsize_t width,
                            hsize_t height,
                            const unsigned char *buffer );

/**
* @brief 
* @param loc_id
* @param datasetName
* @param width
* @param height
* @param interlace
* @param buffer
* @return 
*/
static MXA_EXPORT herr_t H5IMmake_image_24bit( hid_t loc_id,
                             std::string datasetName,
                             hsize_t width,
                             hsize_t height,
                             const std::string &interlace,
                             const unsigned char *buffer );

/**
* @brief 
* @param loc_id
* @param datasetName
* @param width
* @param height
* @param planes
* @param interlace
* @param npals
* @return 
*/
static MXA_EXPORT herr_t H5IMget_image_info( hid_t loc_id,
                     std::string datasetName,
                     hsize_t *width,
                     hsize_t *height,
                     hsize_t *planes,
                     std::string interlace,
                     hssize_t *npals );

/**
* @brief 
* @param loc_id
* @param datasetName
* @param buffer
* @return 
*/
static MXA_EXPORT herr_t H5IMread_image( hid_t loc_id,
                       std::string datasetName,
                       unsigned char *buffer );


/**
* @brief 
* @param loc_id
* @param pal_name
* @param pal_dims
* @param pal_data
* @return 
*/
static MXA_EXPORT herr_t H5IMmake_palette( hid_t loc_id,
                         std::string pal_name,
                         const hsize_t *pal_dims,
                         const unsigned char *pal_data );

/**
* @brief 
* @param loc_id
* @param imageName
* @param pal_name
* @return 
*/
static MXA_EXPORT herr_t H5IMlink_palette( hid_t loc_id,
                        std::string imageName,
                        std::string pal_name );

/**
* @brief 
* @param loc_id
* @param imageName
* @param pal_name
* @return 
*/
static MXA_EXPORT herr_t H5IMunlink_palette( hid_t loc_id,
                           std::string imageName,
                           std::string pal_name );

/**
* @brief 
* @param loc_id
* @param imageName
* @param npals
* @return 
*/
static MXA_EXPORT herr_t H5IMget_npalettes( hid_t loc_id,
                          std::string imageName,
                          hssize_t *npals );

/**
* @brief 
* @param loc_id
* @param imageName
* @param pal_number
* @param pal_dims
* @return 
*/
static MXA_EXPORT herr_t H5IMget_palette_info( hid_t loc_id,
                        std::string imageName,
                        int32 pal_number,
                        hsize_t *pal_dims );


/**
* @brief 
* @param loc_id
* @param imageName
* @param pal_number
* @param pal_data
* @return 
*/
static MXA_EXPORT herr_t H5IMget_palette( hid_t loc_id,
                        std::string imageName,
                        int32 pal_number,
                        unsigned char *pal_data );

/**
* @brief 
* @param loc_id
* @param datasetName
* @return 
*/
static MXA_EXPORT herr_t H5IMis_image( hid_t loc_id,
                     std::string datasetName );

/**
* @brief 
* @param loc_id
* @param datasetName
* @return 
*/
static MXA_EXPORT herr_t H5IMis_palette( hid_t loc_id,
                     std::string datasetName );

};





#endif
