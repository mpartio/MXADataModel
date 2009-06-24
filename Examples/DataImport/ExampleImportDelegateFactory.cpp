#include <Examples/DataImport/ExampleImportDelegateFactory.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ExampleImportDelegateFactory::ExampleImportDelegateFactory()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ExampleImportDelegateFactory::~ExampleImportDelegateFactory()
{

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IImportDelegate::Pointer ExampleImportDelegateFactory::newDataImportDelegate(const std::string &className )
{
  IImportDelegate::Pointer delegate; // Creates a Null Shared Pointer
  if (className.compare(ExampleImport::Detail::ClassName) == 0)
  {
    delegate.reset(new ExampleImportDelegate());
  }
  return delegate;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string ExampleImportDelegateFactory::delegateClassName()
{
  return ExampleImport::Detail::ClassName;
}
