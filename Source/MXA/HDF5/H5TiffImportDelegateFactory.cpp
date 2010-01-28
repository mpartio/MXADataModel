///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
///////////////////////////////////////////////////////////////////////////////
#include <MXA/HDF5/H5TiffImportDelegateFactory.h>
#include <MXA/HDF5/H5TiffImportDelegate.h>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5TiffImportDelegateFactory::H5TiffImportDelegateFactory() :
  _fileNotFoundIsError(false),
  _importAsGrayScale(false)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5TiffImportDelegateFactory::~H5TiffImportDelegateFactory()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IImportDelegate::Pointer H5TiffImportDelegateFactory::newDataImportDelegate (const std::string &className )
{
  IImportDelegate::Pointer delegate; // Creates a Null Shared Pointer
  if ( className.compare( H5TiffImport::Detail::ClassName ) == 0)
  {
    H5TiffImportDelegate* d = new H5TiffImportDelegate();
    delegate.reset ( d );
    d->setFileNotFoundIsError(this->_fileNotFoundIsError);
    d->setImportAsGrayScale(this->_importAsGrayScale);
  }
  return delegate;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string H5TiffImportDelegateFactory::delegateClassName()
{
  return H5TiffImport::Detail::ClassName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5TiffImportDelegateFactory::setFileNotFoundIsError(bool value)
{
  this->_fileNotFoundIsError = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5TiffImportDelegateFactory::setImportAsGrayScale(bool value)
{
  this->_importAsGrayScale = value;
}
