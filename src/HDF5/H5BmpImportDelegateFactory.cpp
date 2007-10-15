#include <HDF5/H5BmpImportDelegateFactory.h>
#include <HDF5/H5BmpImportDelegate.h>



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5BmpImportDelegateFactory::H5BmpImportDelegateFactory() :
  _fileNotFoundIsError(false),
  _importAsGrayScale(false)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5BmpImportDelegateFactory::~H5BmpImportDelegateFactory()
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IImportDelegatePtr H5BmpImportDelegateFactory::newDataImportDelegate (const std::string &className )
{
  IImportDelegatePtr delegate; // Creates a Null Shared Pointer
  if ( className.compare( H5BmpImportDelegateFactory_Detail::ClassName ) == 0)
  {
    H5BmpImportDelegate* d = new H5BmpImportDelegate();
    delegate.reset ( d );
    d->setFileNotFoundIsError(this->_fileNotFoundIsError);
    d->setImportAsGrayScale(this->_importAsGrayScale);
  }
  return delegate;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string H5BmpImportDelegateFactory::delegateClassName()
{
  return H5BmpImport::Detail::ClassName;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5BmpImportDelegateFactory::setFileNotFoundIsError(bool value)
{
  this->_fileNotFoundIsError = value;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5BmpImportDelegateFactory::setImportAsGrayScale(bool value)
{
  this->_importAsGrayScale = value;
}
