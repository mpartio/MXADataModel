///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#include "TiffMaker.h"

//-- Tif headers
#include "tif_config.h"
#include "tiffio.h"

//-- std C++ headers
#include <iostream>

#define USE_LZW_COMPRESSION 0

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TiffMaker::TiffMaker()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TiffMaker::~TiffMaker()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TiffMaker::createTiffFile(const std::string &filename)
{
  //TODO: Return on Error code with this function.
  const char* outfilename = filename.c_str();

  TIFF *output;
  uint32 width, height;
  unsigned char* raster;

  // Open the output image
  if ((output = TIFFOpen(outfilename, "w")) == NULL)
  {
    std::cout << "Error creating Tiff file at '" << filename << "'" << std::endl;
    return;
  }

  // We need to know the width and the height before we can malloc
  width = 256;
  height = 256;

  raster = (unsigned char *)_TIFFmalloc(sizeof(char) * width * height * 3);
  if (raster == NULL)
  {
    fprintf(stderr, "Could not allocate enough memory\n");
    return;
  }

  // Write some color data into the raster
  unsigned char* curRaster = raster;
  for (uint32 i = 0; i < height; ++i)
  {
    for (uint32 j = 0; j < width; ++j)
    {
      *curRaster = i;
      curRaster++;
      *curRaster = j;
      curRaster++;
      *curRaster = i + j;
      curRaster++;
    }
  }

  // Write the tiff tags to the file
  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);
#if USE_LZW_COMPRESSION
  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  TIFFSetField(output, TIFFTAG_PREDICTOR, PREDICTOR_HORIZONTAL);
#else
  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
#endif
  // Actually write the image
  if (TIFFWriteEncodedStrip(output, 0, raster, width * height * 3) == 0)
  {
    fprintf(stderr, "Could not write image\n");
  }
  _TIFFfree(raster);
  TIFFClose(output);
}
