#include <MXA/HDF5/H5BmpImportDelegateFactory.h>
#include <MXA/HDF5/H5BmpImportDelegate.h>



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
IImportDelegate::Pointer H5BmpImportDelegateFactory::newDataImportDelegate (const std::string &className )
{
  IImportDelegate::Pointer delegate; // Creates a Null Shared Pointer
  if ( className.compare( H5BmpImport::Detail::ClassName ) == 0)
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
int32_t H5BmpImportDelegateFactory::setProperty(const std::string &key, const std::string &value)
{
  SET_PROPERTY_BODY(H5BmpImport, bool, FileNotFoundIsError, key, value);
  SET_PROPERTY_BODY(H5BmpImport, bool, ImportAsGrayScale, key, value);
  return 0;
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
