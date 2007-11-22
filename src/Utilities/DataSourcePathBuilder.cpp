#include "DataSourcePathBuilder.h"

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataSourcePathBuilder::DataSourcePathBuilder()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataSourcePathBuilder::~DataSourcePathBuilder()
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataSourcePathBuilder::addSection(IStringSectionPtr part)
{
  _parts.push_back(part);
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataSourcePathBuilder::clearSections()
{
  _parts.clear();
}

#if 0
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string DataSourcePathBuilder::build(std::vector<uint32> &dimValues, int8 &ok)
{
  ok = false;
  std::string path;
  IStringSectionPtr f;
  for (IStringSections::iterator iter = _parts.begin(); iter != _parts.end(); ++iter ) {
    f = *iter;
    path.append( f->toString(dimValues, ok) );
    if (false == ok)
    {
      break;
    }
  }
  return path;
}

#endif
