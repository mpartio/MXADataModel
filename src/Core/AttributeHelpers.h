///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _ATTRIBUTEHELPERS_H_
#define _ATTRIBUTEHELPERS_H_

// MXA inludes
#include "Common/DLLExport.h"
#include "Common/MXATypes.h"
#include "Base/IAttributeHelper.h"
#include "Base/IAttributeWriter.h"

// STL Includes
#include <sstream>

// -----------------------------------------------------------------------------
//  Used for Strings
// -----------------------------------------------------------------------------
class StringAttributeHelper : public IAttributeHelper
{
  public:
    StringAttributeHelper(std::string value) 
    {
      _value = (value);
    }
    
    virtual ~StringAttributeHelper() {};
    
    void convert(std::stringstream &sstream)
    {
        sstream << _value;
    }
    
    std::string getValue()
    {
      return _value;
    }
   
  // -----------------------------------------------------------------------------
  //  IAttributeWriter Implementation
  // -----------------------------------------------------------------------------
  int32 writeAttribute(int32 fileId, std::string &datasetPath, std::string &key, IAttributeWriter &writer)
  {
    return writer.writeAttribute(fileId, datasetPath, key, _value);
  }

  protected:
    std::string _value;

};


// -----------------------------------------------------------------------------
//  Used for Numbers
// -----------------------------------------------------------------------------
template<typename T>
class NumberAttributeHelper : public IAttributeHelper
{
  public:
    NumberAttributeHelper(T value) :
      _value(value)
    {
      //_value = value;
    }
    
    virtual ~NumberAttributeHelper() {};
    
    void convert(std::stringstream &sstream)
    {
      int32 size = sizeof(T);
      if (size != 1) {
        sstream << _value;
      } else {
        sstream << static_cast<int32>(_value);
      }
    }
    
    T getValue()
    {
      return _value;
    }
    
    void getValue(T &value)
    {
      value = _value;
    }


  // -----------------------------------------------------------------------------
  //  IAttributeWriter Implementation
  // -----------------------------------------------------------------------------
  int32 writeAttribute(int32 fileId, std::string &datasetPath, std::string &key, IAttributeWriter &writer)
  {
    return writer.writeAttribute(fileId, datasetPath, key, _value);
  }

    
  protected:
    T _value;

};


// -----------------------------------------------------------------------------
//  Used for Vectors
// -----------------------------------------------------------------------------
template<typename T>
class VectorAttributeHelper : public IAttributeHelper
{
  public:
    VectorAttributeHelper(std::vector<T> value, std::vector<uint64> dims)
    {
      _value = value;
      _dims = dims;
    }
    
    virtual ~VectorAttributeHelper() {};
    
    void convert(std::stringstream &sstream)
    {
      int32 size = _value.size();
      for(int i = 0; i < size; ++i) 
      {
        sstream << _value[i];
        if (i < size - 1)
        {
          sstream << " ";
        }
      }
    }

    std::vector<T> getValue()
    {
      return _value;
    }
    
  // -----------------------------------------------------------------------------
  //  IAttributeWriter Implementation
  // -----------------------------------------------------------------------------
  int32 writeAttribute(int32 fileId, std::string &datasetPath, std::string &key, IAttributeWriter &writer)
  {
    return writer.writeAttribute(fileId, datasetPath, key, _dims, _value);
  }
    
  protected:
    std::vector<T> _value;
    std::vector<uint64> _dims;
      
};


#endif /* _ATTRIBUTEHELPERS_H_ */
