#include "MXARGBImage.h"
#include <MXA/Common/LogTime.h>
#include <iostream>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArray::Pointer MXARGBImage::CreateRGBImageArray( int32_t width, int32_t height)
{
  IMXAArray::Pointer ptr;
  int32_t err = 1;
  MXARGBImage* d = new MXARGBImage(width, height);
  err = d->_allocate();
  if (err >= 0)
  { // No errors, swap in the pointer
    ptr.reset( static_cast<IMXAArray*>(d) );
  }
  else
  {
    delete d; // Clean up the memory
  }
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXARGBImage* MXARGBImage::New( int32_t width, int32_t height)
{
  int32_t err = 1;
  MXARGBImage* d = new MXARGBImage( width, height);
  err = d->_allocate();
  if (err < 0)
  {
    delete d;
    d = NULL;
  }
  return d;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXARGBImage::MXARGBImage( int32_t width, int32_t height) :
MXAArrayTemplate<uint8_t>( (size_t)((size_t)width * (size_t)height * 3), true),
_width(width * 3),
_height(height)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXARGBImage::~MXARGBImage()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXARGBImage::getNumberOfDimensions ()
{
  return 2;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint8_t* MXARGBImage::getPixelPointer(int32_t x, int32_t y)
{
  size_t index = (_width * y ) + x * 3;
  return static_cast<uint8_t*>(this->getVoidPointer(index));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXARGBImage::resize(size_t size)
{
  if(this->_resizeAndExtend(size) || size <= 0)
    {
    this->_width = static_cast<int32_t>(size);
    this->_height = 1;
    return 1;
    }
  else
    {
    return 0;
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MXARGBImage::resizeArray(int32_t width, int32_t height)
{
  int32_t err =  this->resize((size_t)width * (size_t)height * 3);
  this->_width = width * 3;
  this->_height = height;
  if (err == 0)
  {
    this->_width = 0;
    this->_height = 0;
  }
  return err;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string MXARGBImage::valueToString(char delimiter)
{
  std::stringstream sstream;

  size_t nElements = this->getNumberOfElements();
  size_t limit = nElements - 1;
  int32_t wLimit = _width * 3;
  int32_t width = 0;
  uint8_t* data = this->getPointer(0);
  for(size_t i = 0; i < nElements; i=i+3)
  {

    sstream << "[" << data[i] << "," << data[i+1] << "," << data[i+2] << "]";
    if (i < limit && width != wLimit)
    {
      sstream << delimiter;
    }
    if (width == wLimit) // Format in multiple rows
    {
      sstream << "\n";
      width = 0;
    }
    width += 3;
  }
  return sstream.str();
}
