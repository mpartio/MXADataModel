/*
 * TiffMaker.cpp
 *
 *  Created on: Sep 22, 2008
 *      Author: mjackson
 */

#include "Testing/TiffMaker.h"
#include "tif_config.h"
#include "tiffio.h"
#include <iostream>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TiffMaker::TiffMaker()
{
  // TODO Auto-generated constructor stub

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TiffMaker::~TiffMaker()
{
  // TODO Auto-generated destructor stub
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TiffMaker::createTiffFile(const std::string &filename)
{

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
    exit(42);
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
  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);
  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  // Actually write the image
  if (TIFFWriteEncodedStrip(output, 0, raster, width * height * 3) == 0)
  {
    fprintf(stderr, "Could not write image\n");
  }
  _TIFFfree(raster);
  TIFFClose(output);
}
