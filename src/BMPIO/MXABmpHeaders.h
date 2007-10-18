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
#ifndef MXA_BMP_HEADERS_H
#define MXA_BMP_HEADERS_H

#include <Common/MXATypes.h>

enum CompressionType {
  BMP_BI_RGB=0, 
  BMP_BI_RLE8, 
  BMP_BI_RLE4 
  };

/**
 * @brief
 * @structure
 */
struct MXABMPFileHeader
{
	int16 signature;		/* Must be 4D42 hex */
	int32 fileSize;			/* size of file in bytes */
	int16 reserved1;
	int16 reserved2;
	int32 dataOffset;		/* starting address of bitmap data */
};

/* Only supported header is the V3 header */
/**
 * @brief
 * @structure
 */
struct MXABMPDIBHeader
{
	uint32 headerSize;		/* Must be 40 */
	int32 width;
	int32 height;
	int16 numColorPlanes;	/* Everything I see says this must be 1 */
	int16 bitsPerPixel;		/* Typical values are 1, 4, 8, 16, 24 and 32. */
	CompressionType compressionMethod;
	uint32 imageSize;
	int32 horizontalResolution;
	int32 verticalResolution;
	uint32 numPaletteColors;
	uint32 numImportantColors;
};



#endif


