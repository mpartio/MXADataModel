///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _HDF5AttributeWriter_h_
#define _HDF5AttributeWriter_h_

#include <MXA/Base/IAttributeWriter.h>
#include <MXA/HDF5/H5Lite.h>

#include <vector>

/**
* @brief Writes Attributes for a dataset to an HDF5 data file
* @author Mike Jackson
* @version $Revision: 1.2 $
* @date March 2007
*/
class MXA_EXPORT H5AttributeWriter : public IAttributeWriter
{
public:
  H5AttributeWriter(){};
  virtual ~H5AttributeWriter(){};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
    template<typename T>
    int32_t _writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, T value)
    {
        return H5Lite::writeScalarAttribute(locationId, datasetPath, key, value);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
    template<typename T>
    int32_t _writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<T> &value)
    {
      return H5Lite::writeVectorAttribute(locationId, datasetPath, key, dims, value );
    }

   /**
   * @brief Writes a Scalar Attribute value for a given dataset
   * @param locationId An HDF5 id the represents a file or group that contains the dataset
   * @param datasetPath The Path to the dataset that the attribute is going to be written to
   * @param key The Key for the Attribute
   * @param value The Value of the Attribute
   * @return Standard HDF5 Error condition.
   */
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::string &value)
   {
   //  std::cout << "Writing Attribute for String '" << key << "'" << std::endl;
     return H5Lite::writeStringAttribute(locationId,datasetPath,key,value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, int8_t value)
   {
      //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
  int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, uint8_t value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
  int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, int16_t value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, uint16_t value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, int32_t value)
   {
      //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, uint32_t value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, int64_t value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, uint64_t value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
  int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, float32 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
  int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, float64 value)
   {
     //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   /**
   * @brief Writes a Vector/Matrix Attribute value for a given dataset
   * @param locationId An HDF5 id the represents a file or group that contains the dataset
   * @param datasetPath The Path to the dataset that the attribute is going to be written to
   * @param key The Key for the Attribute
   * @param dims The dimensions of the data set
   * @param value The Value of the Attribute
   * @return Standard HDF5 Error condition.
   */
  int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<int8_t> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<uint8_t> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<int16_t> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key,  dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<uint16_t> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<int32_t> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<uint32_t> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
  int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<int64_t> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<uint64_t> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<float32> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }

   // -----------------------------------------------------------------------------
   //
   // -----------------------------------------------------------------------------
   int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<hsize_t> &dims, std::vector<float64> &value)
   {
     //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
     return _writeAttribute(locationId, datasetPath, key, dims, value);
   }


};

#endif //_HDF5AttributeWriter _h_
