
#ifndef _HDF5AttributeWriter_h_
#define _HDF5AttributeWriter_h_

#include "Interfaces/IAttributeWriter.h"
#include "HDF5/H5Lite.h"

#include <vector>

class MXA_EXPORT H5AttributeWriter : public IAttributeWriter
{
public:
  H5AttributeWriter(){};
  virtual ~H5AttributeWriter(){};
  
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    template<typename T>
    int32 _writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, T value)
    {
        hid_t type = H5Lite::HDFTypeForPrimitive( value );
        return H5Lite::writeAttribute(locationId, datasetPath, key, value, type);
    }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------  
    template<typename T>
    int32 _writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<hsize_t> &dims, std::vector<T> value)
    {
      T test = value.front();
      hid_t type = H5Lite::HDFTypeForPrimitive( test );
      return H5Lite::writeAttribute(locationId, datasetPath, key, dims, value, type );
    }
    

    // -----------------------------------------------------------------------------
    //  
    // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::string &value)
   {
     //std::cout << "Writing Attribute for String '" << key << "'" << std::endl;
     return H5Lite::writeAttribute(locationId,datasetPath,key,value);
   }
  
   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, int8 value)
   {
      //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
  int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, uint8 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;    
     return _writeAttribute(locationId, datasetPath, key, value);
   }
  
   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
  int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, int16 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, uint16 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, value);
   }
  
   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, int32 value)
   {
      //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;    
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, uint32 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, value);
   }
  
   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, int64 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, uint64 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, value);
   }
  
   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
  int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, float32 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
  int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, float64 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, value);
   }
  
   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
  int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<int8> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<uint8> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }
  
   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<int16> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key,  dims, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<uint16> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<int32> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<uint32> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }
  
   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
  int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<int64> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<uint64> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------   
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<float32> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //  
   // -----------------------------------------------------------------------------  
   int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<unsigned long long int> &dims, std::vector<float64> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }
  
		
};

#endif //_HDF5AttributeWriter _h_
