#include "MXABmpIO.h"
#include <iostream>

//-- MXA Includes
#include <Common/MXAEndian.h>
#include <Common/MXATypes.h>
#include <Common/DLLExport.h>
#include <Common/IO/Reader64.h>
#include <BMPIO/MXABmpHeaders.h>

//-- C Includes
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXABmpIO::MXABmpIO() :
  width(0),
  height(0),
  numChannels(0),
  isGrayscale(false),
  bytesRead(0)
{
 
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXABmpIO::~MXABmpIO()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
LOAD_TEXTUREBMP_RESULT MXABmpIO::loadBMPData( const char* fName )
{
  // Open file for buffered read.
  //file = fopen(fName,"rb");
  LOAD_TEXTUREBMP_RESULT res = LOAD_TEXTUREBMP_SUCCESS;
  _reader64Ptr = Reader64Ptr(new Reader64(fName) );
  if ( false == _reader64Ptr->initReader() )
  {
    return LOAD_TEXTUREBMP_COULD_NOT_FIND_OR_READ_FILE;
  }

  bytesRead=0;

// Read File Header
  res=readFileHeader();

// Read Info Header
  res=readInfoHeader();

// Read Palette
  res=readPalette();

  // The bitmap data we are going to hand to OpenGL
  this->bitmapData.resize( width*height*3 );

  // Read Data
  res = readBitmapData( &(bitmapData.front() ) );

  // Only clean up bitmapData if there was an error.
  if (res == LOAD_TEXTUREBMP_SUCCESS)
  {
    numChannels = 3;
  }
  // Reset the Shared Pointer forces the wrapped pointer to be deleted 
  //  and the underlying file closed
  Reader64Ptr nullReader;
  _reader64Ptr.swap(nullReader);
  
  this->flipBitmap();

  return res;
}

// -----------------------------------------------------------------------------
// Reads and returns a 32-bit value from the file.
// -----------------------------------------------------------------------------
int32 MXABmpIO::read32BitValue()
{
  int32 value;
  this->_reader64Ptr->read<MXA::Endian::FromLittleToSystem>(value);
  bytesRead+=4;
  return value;
}

// -----------------------------------------------------------------------------
// Reads and returns a 16-bit value from the file
// -----------------------------------------------------------------------------
int16 MXABmpIO::read16BitValue()
{
  int16 value;
  this->_reader64Ptr->read<MXA::Endian::FromLittleToSystem>(value);
  bytesRead+=2;
  return value;
}

// -----------------------------------------------------------------------------
// Reads and returns a 8-bit value from the file
// -----------------------------------------------------------------------------
uint8 MXABmpIO::read8BitValue()
{
  uint8 value;
  this->_reader64Ptr->readPrimitive(value);
  bytesRead+=1;
  return value;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
LOAD_TEXTUREBMP_RESULT MXABmpIO::readFileHeader()
{
  // Read "BM" tag in ASCII.
  if (read8BitValue()!=66 || read8BitValue()!=77)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  // Read file size. We do not need this. Ignore.
  this->fileHeader.fileSize = read32BitValue();
  
  // Skip the two reserved areas
  this->fileHeader.reserved1 = read16BitValue();
  this->fileHeader.reserved2 = read16BitValue();
  
  // Read the byte offset
  this->fileHeader.dataOffset = read32BitValue();
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
LOAD_TEXTUREBMP_RESULT MXABmpIO::readInfoHeader()
{
  // We expect this to be at least 40. If it is larger we read
  // some more bytes in the end to be forward compatible.
  uint32 sizeOfInfoHeader = (uint32)read32BitValue();
  
  if (sizeOfInfoHeader<40)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  width = dibHeader.width = read32BitValue();
  
  height = dibHeader.height = read32BitValue();
  
  // Read number of planes. According to the specification this
  // must be 1.
  if (read16BitValue()!=1)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  dibHeader.bitsPerPixel = read16BitValue();
  
  if (this->dibHeader.bitsPerPixel != 1 && 
      this->dibHeader.bitsPerPixel != 4 && 
      this->dibHeader.bitsPerPixel != 8 && 
      this->dibHeader.bitsPerPixel != 24)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  dibHeader.compressionMethod = (CompressionType)read32BitValue();
  
  if (dibHeader.compressionMethod != BMP_BI_RGB && 
      dibHeader.compressionMethod != BMP_BI_RLE8 && 
      dibHeader.compressionMethod != BMP_BI_RLE4)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  // Read image size. We do not need this since we have the 
  // image size.
  dibHeader.imageSize = read32BitValue();
  
  // Pixel to device mapping. This is irrelevant since we simply
  // want the bitmap.
  dibHeader.horizontalResolution = read32BitValue();
  dibHeader.verticalResolution = read32BitValue();
  
  // Read colors used. We do not need this, so it is ignored.
  dibHeader.numPaletteColors = read32BitValue();
  
  // Read the number of important colors. This will not be needed
  // in OpenGL so we will ignore this.
  dibHeader.numImportantColors = read32BitValue();
  
  // Apply padding in end of header to be forward compatible.
  sizeOfInfoHeader -= 40;
  while (sizeOfInfoHeader>0)
  {
    read8BitValue();
    sizeOfInfoHeader--;
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// The palette follows directly after the
// info header
//
//typedef struct tagRGBQUAD {     /* rgbq */
//  BYTE    rgbBlue;
//  BYTE    rgbGreen;
//  BYTE    rgbRed;
//  BYTE    rgbReserved;
// } RGBQUAD;
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
LOAD_TEXTUREBMP_RESULT MXABmpIO::readPalette()
{ 
  // 24-bit images are not paletted.
  if (this->dibHeader.bitsPerPixel == 24)
    return LOAD_TEXTUREBMP_SUCCESS;
  
  int32 numColors = 1<<this->dibHeader.bitsPerPixel;
  for (int i=0;i<numColors;i++)
  {
    // Read RGB.
    for (int j=2;j>=0;j--)
      this->palette[j][i] = read8BitValue();
    
    // Skip reversed byte.
    read8BitValue();
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
LOAD_TEXTUREBMP_RESULT MXABmpIO::readBitmapData1Bit(uint8* bitmapData)
{
  // 1-bit format cannot be compressed
  if (this->dibHeader.compressionMethod != BMP_BI_RGB)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;

  uint8 byteRead;
  for (int y=0;y<height;y++)
  {
    int32 index = y*width*3;
    for (int x=0;x<width;x++)
    {
      if (x%8==0)
      {
        byteRead = read8BitValue();
      }
      
      uint8 color = (byteRead >> (7-(x%8))) & 1;
      
      bitmapData[index+x*3] = palette[0][color];
      bitmapData[index+x*3+1] = palette[1][color];
      bitmapData[index+x*3+2] = palette[2][color]; 
    }

    // Pad to 32-bit boundery.
    while(bytesRead%4 != 0)
      read8BitValue();
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// -----------------------------------------------------------------------------
// This is called after the first byte has been found to be 0
// and the second to be 0-2. This is only used in RLE-4 and RLE-8
// encoding.
bool MXABmpIO::handleEscapeCode(int secondByte, int* x, int* y, 
                             LOAD_TEXTUREBMP_RESULT* res)
{
  if (secondByte==0x00)
  {
    // End of line
    (*x)=0;
    if ((*y)>=height)
    {
      *res = LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
      return true;
    }
    (*y)++;
  }
  else if (secondByte==0x01)
  {
    // End of bitmap
    *res = LOAD_TEXTUREBMP_SUCCESS;
    return true;            
  }
  else // secondByte=0x02
  {
    // Delta. Move drawing cursor.
    *x += read8BitValue();
    *y += read8BitValue();
    if (*x>=width || *x<0 || *y>=height || *y<0)
    {
      *res = LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------
// Draw a 4-bit image. Can be uncompressed or RLE-4 compressed.
LOAD_TEXTUREBMP_RESULT MXABmpIO::readBitmapData4Bit(uint8* bitmapData)
{
  if (this->dibHeader.compressionMethod != BMP_BI_RGB && 
      this->dibHeader.compressionMethod != BMP_BI_RLE4)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  // Uncompressed 4 bit encoding
  if (this->dibHeader.compressionMethod == BMP_BI_RGB)
  {
    for (int j=0;j<height;j++)
    {
      int32 index = j*width*3;
      unsigned char byteValue;
      unsigned char color;
      for (int i=0;i<width;i++)
      {      
        if (i%2==0)
        {
          byteValue = read8BitValue();
          color = byteValue>>4;           
        }
        else
        {
          color = byteValue & 0x0F;
        }
        
        bitmapData[index+i*3] = palette[0][color];
        bitmapData[index+i*3+1] = palette[1][color];
        bitmapData[index+i*3+2] = palette[2][color];
      }
      
      // Pad to 32-bit boundery.
      for (int k=0; k<(((width+1)/2)%4);k++)
        read8BitValue();
    }
  }
  
  // RLE encoded 4-bit compression
  if (this->dibHeader.compressionMethod == BMP_BI_RLE4)
  {
    // Drawing cursor pos  
    int32 x=0;
    int32 y=0;
    
    // Clear bitmap data since it is legal not to 
    // fill it all out.
    memset(bitmapData,0,sizeof(unsigned char)*width*height*3);

    bytesRead=0;
    
    while(true)
    {
      unsigned char firstByte = read8BitValue();
      unsigned char secondByte = read8BitValue();
      
      // Is this an escape code or absolute encoding?
      if (firstByte==0)
      {
        // Is this an escape code
        if (secondByte<=0x02)
        {
          LOAD_TEXTUREBMP_RESULT res;
          if (handleEscapeCode(secondByte,&x,&y,&res))
            return res;
        }
        else
        {
          // Absolute encoding
          int32 index = y*width*3;
          int32 color;
          unsigned char databyte;
          for (int i=0; i<secondByte; i++)
          {
            if (x>=width)
              return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;

            if (i%2==0)
            {
              databyte = read8BitValue();
              color = databyte >> 4;
            }
            else
            {
              color = databyte & 0x0F;
            }

            bitmapData[index+x*3] = palette[0][color];
            bitmapData[index+x*3+1] = palette[1][color];
            bitmapData[index+x*3+2] = palette[2][color]; 
            x++;
          }
          
          // Pad to 16-bit word boundery
          while (bytesRead%2 != 0)
            read8BitValue();
        }
      }
      else
      {
        // If not absolute or escape code, perform data decode for next 
        // length of colors
        int32 color1 = secondByte >> 4;
        int32 color2 = secondByte & 0x0F;
        
        for (int i=0;i<firstByte;i++)
        {
          if (x>=width)
            return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
          
          int32 color;
          if (i%2==0)
            color = color1;
          else
            color = color2;
          
          int32 index = y*width*3+x*3;
          bitmapData[index] = palette[0][color];
          bitmapData[index+1] = palette[1][color];
          bitmapData[index+2] = palette[2][color];
          x++;
        }        
      }
    }
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// -----------------------------------------------------------------------------
// Read 8-bit image. Can be uncompressed or RLE-8 compressed.
LOAD_TEXTUREBMP_RESULT MXABmpIO::readBitmapData8Bit(uint8* bitmapData)
{
  if (this->dibHeader.compressionMethod != BMP_BI_RGB && 
      this->dibHeader.compressionMethod != BMP_BI_RLE8)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  if (this->dibHeader.compressionMethod == BMP_BI_RGB)
  {
    // For each scan line
    for (int i=0;i<height;i++)
    {
      int32 index = i*width*3;
      for (int j=0;j<width;j++)
      {
        int32 color = read8BitValue(); 
		    int32 temp = index + j * 3;
        bitmapData[temp++] = palette[0][color];
        bitmapData[temp++] = palette[1][color];
        bitmapData[temp] = palette[2][color];
      }                                             
      
      // go to next alignment of 4 bytes.
      for (int k=0; k<(width%4);k++)
        read8BitValue();
    }
  }
  
  if (this->dibHeader.compressionMethod == BMP_BI_RLE8)
  {
    // Drawing cursor pos
    int32 x=0;
    int32 y=0;

    bytesRead=0;
    
    // Clear bitmap data since it is legal not to 
    // fill it all out.
    memset(bitmapData,0,sizeof(unsigned char)*width*height*3);
    
    while(true)
    {
      unsigned char firstByte = read8BitValue();
      unsigned char secondByte = read8BitValue();
      
      // Is this an escape code or absolute encoding?
      if (firstByte==0)
      {
        // Is this an escape code
        if (secondByte<=0x02)
        {
          LOAD_TEXTUREBMP_RESULT res;
          if (handleEscapeCode(secondByte,&x,&y,&res))
            return res;
        }
        else
        {
          // Absolute encoding
          int32 index = y*width*3;
          for (int i=0; i<secondByte; i++)
          {
            if (x>=width)
              return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
            int32 color = read8BitValue(); 
            bitmapData[index+x*3] = palette[0][color];
            bitmapData[index+x*3+1] = palette[1][color];
            bitmapData[index+x*3+2] = palette[2][color]; 
            x++;
          } // end for (int i=0; i<secondByte; i++)
          
          // Pad to 16-bit word boundery
          if (secondByte%2 == 1)
            read8BitValue();
        }
      }
      else
      {
        // If not, perform data decode for next length of colors
        for (int i=0;i<firstByte;i++)
        {
          if (x>=width)
            return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
          int32 index = y*width*3+x*3;
          bitmapData[index] = palette[0][secondByte];
          bitmapData[index+1] = palette[1][secondByte];
          bitmapData[index+2] = palette[2][secondByte];
          x++;
        }        // end for (int i=0;i<firstByte;i++)
      }
    }
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// -----------------------------------------------------------------------------
// Load a 24-bit image. Cannot be encoded.
LOAD_TEXTUREBMP_RESULT MXABmpIO::readBitmapData24Bit(uint8* bitmapData)
{
  // 24-bit bitmaps cannot be encoded. Verify this.
  if (this->dibHeader.compressionMethod != BMP_BI_RGB)
    return LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT;
  
  for (int i=0;i<height;i++)
  {
    int32 index = i*width*3;
    for (int j=0;j<width;j++)
    {
      bitmapData[index+j*3+2] = read8BitValue();
      bitmapData[index+j*3+1] = read8BitValue();
      bitmapData[index+j*3] = read8BitValue();
    }                                             

    // go to next alignment of 4 bytes.
//    for (int k=0; k<((width*3)%4);k++), the fix:
  for (int k=(width*3)%4; k!=0 && k<4;k++)
      read8BitValue();
  }
  
  return LOAD_TEXTUREBMP_SUCCESS;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
LOAD_TEXTUREBMP_RESULT MXABmpIO::readBitmapData(uint8* bitmapData)
{
  // Pad until byteoffset. Most images will need no padding
  // since they already are made to use as little space as
  // possible.
  while(bytesRead<this->fileHeader.dataOffset)
    read8BitValue();
  
  // The data reading procedure depends on the bit depth.
  switch(this->dibHeader.bitsPerPixel)
  {
  case 1: 
    return readBitmapData1Bit(bitmapData);
  case 4:
    return readBitmapData4Bit(bitmapData);
  case 8:
    return readBitmapData8Bit(bitmapData);
  default: // 24 bit.
    return readBitmapData24Bit(bitmapData);
  }
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXABmpIO::getHeight()
{
  return this->height;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXABmpIO::getWidth()
{
  return this->width;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 MXABmpIO::getNumberOfChannels()
{
  return this->numChannels;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void MXABmpIO::flipBitmap()
{
  
  std::vector<uint8> flippedVec;
  uint8* flippedImage = 0x0;
  uint8* temp;
  int32 element1, element2, width3, el1, el2;
	
	width3 = width * 3;
	
  if ( !isGrayscale )
  {
  	flippedVec.resize(height * width3);
  	flippedImage = &(flippedVec.front());
  	for ( int32 row1 = 0, row2 = (height - 1); row1 < height || row2 > 0; row1++, row2-- )
  	{
		el1 = (row1 * width3); // Beginning of scan line in source
		el2 = (row2 * width3); // Beginning of scan line in result
  	  for ( int32 col = 0; col < width3; col+=3 )
  	  {
		  // Calculate beginning of this pixel's data for both images
  	    element1 = el1 + col;
  	    element2 = el2 + col;
  	    flippedImage[element2++] = bitmapData[element1++];
  	    flippedImage[element2++] = bitmapData[element1++];
  	    flippedImage[element2] = bitmapData[element1];
  	  }
  	}
  }
  else
  {
    flippedVec.resize(width * height);
    flippedImage = &(flippedVec.front());
  	for ( int32 row1 = 0, row2 = (height - 1); row1 < height || row2 > 0; row1++, row2-- )
  	{
  	  for ( int32 col = 0; col < width; col++ )
  	  {
  	    element1 = (row1 * width) + col;
  	    element2 = (row2 * width) + col;
  	    flippedImage[element2] = bitmapData[element1];
  	  }
  	}
  }
  this->bitmapData.swap(flippedVec);
}

void MXABmpIO::convertToGrayscale()
{
  //Uses the function Y = 0.3R + 0.59G + 0.11B
  if ( this->isGrayscale )
  {
    return;
  }
  std::vector<uint8> grayscaleVec;
  uint8* grayscaleImage;
  uint8* temp;
  int32 element1, element2;
//  grayscaleImage = new uint8[this->width * this->height];
  grayscaleVec.resize(this->width * this->height);
  grayscaleImage = &(grayscaleVec.front());
  for ( int32 row = 0; row < height; row++ )
  {
    for ( int32 col = 0; col < width; col++ )
    { 
      element1 = (row * width) + col;
      element2 = element1 * 3;
      grayscaleImage[element1] = bitmapData[element2] * 0.3 + 
        bitmapData[element2 + 1] * 0.59 + 
        bitmapData[element2 + 2] * 0.11;
    }
  }
  isGrayscale = true;
  this->bitmapData.swap(grayscaleVec);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool MXABmpIO::isGrayscaleImage()
{
  return isGrayscale;
}

// -----------------------------------------------------------------------------
//  This function copies the bitmap data array to the vector passed into it.
//  It is the responsibility of the calling code to make sure the clean up the
//	memory used by the array when they are done with it.
// -----------------------------------------------------------------------------
void MXABmpIO::copyDataArray(std::vector<uint8> &buffer)
{
  std::vector<uint8>::size_type numElements;
  if ( isGrayscale )
    numElements = width * height;
  else 
    numElements = width * height * 3;
  buffer.reserve(numElements);
  buffer.resize(numElements);
  if (buffer.size() == 0 ) 
  {
	  std::cout << "buffer size was 0" << std::endl;
  }
  ::memcpy( &(buffer.front()), &(this->bitmapData.front()), numElements);
}
