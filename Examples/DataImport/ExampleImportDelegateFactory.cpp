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
IImportDelegatePtr ExampleImportDelegateFactory::newDataImportDelegate(const std::string &className )
{
  IImportDelegatePtr delegate; // Creates a Null Shared Pointer
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
