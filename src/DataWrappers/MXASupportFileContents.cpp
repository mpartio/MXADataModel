///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////


#include <DataWrappers/MXASupportFileContents.h>


#if 0
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArrayPtr MXASupportFileContents::CreateMXASupportFileContents( uint64 fileSize)
{
  MXASupportFileContents* d = new MXASupportFileContents( fileSize, true);
  if ( d->_allocate() < 0)
  {  // Could not allocate enough memory, reset the pointer to null and return
    d = NULL;
  }
  IMXAArrayPtr ptr ( static_cast<IMXAArray*>(d) );
  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXASupportFileContents* MXASupportFileContents::New(uint64 fileSize)
{
  int32 err = 1;
  MXASupportFileContents* d = new MXASupportFileContents( fileSize );
  err = d->_allocate();
  if (err < 0)
  {
    delete d;
    d = NULL;
  }
  return d;
}

#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXASupportFileContents::MXASupportFileContents( uint64 fileSize ) :
MXAArrayTemplate<uint8>( fileSize, true)
{
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MXASupportFileContents::~MXASupportFileContents()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint8* MXASupportFileContents::getFilePointer(uint64 offset)
{
  return static_cast<uint8*>(this->getVoidPointer(offset));
}



