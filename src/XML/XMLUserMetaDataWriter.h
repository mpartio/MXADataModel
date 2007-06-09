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
#ifndef _XMLATTRIBUTEWRITER_H_
#define _XMLATTRIBUTEWRITER_H_

//-- MXA Includes
#include "Interfaces/IAttributeWriter.h"

//-- boost includes
#include <boost/shared_ptr.hpp>


/**
 * @brief
 * @author
 * @date
 * @version
 *  
 */
class XMLUserMetaDataWriter : public IAttributeWriter
{

public:
  XMLUserMetaDataWriter(boost::shared_ptr<std::ofstream> ofstreamPtr)
  {
    _ofstreamPtr = ofstreamPtr; 
  }
  
  virtual ~XMLUserMetaDataWriter(){};
  
  
  // -----------------------------------------------------------------------------
  //  
  // -----------------------------------------------------------------------------
  std::string indent(int depth)
  {
    const int32 indentSize = 2;
    return std::string(indentSize * depth, ' ');
  }
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    template<typename T>
    int32 _writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, T value)
    {
      int32 size = sizeof(T);
      std::string sType = StringUtils::typeForPrimitive(value);
      std::ofstream &stream = *(_ofstreamPtr.get());
      stream << indent(5) << "<UserMetaData key=\"" << key << "\" dims=\"" << 1 <<  "\" type=\"" << sType << "\">\n";
      if (size != 1) {
        stream << indent(5) << value;
      } else {
        stream << indent(5) << static_cast<int32>(value);
      }
      stream << "\n"<< indent(5) << "</UserMetaData>\n";
      return 1;
    }

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------  
    template<typename T>
    int32 _writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<T> value)
    {
      std::ofstream &stream = *(_ofstreamPtr.get());
      std::string sType = StringUtils::typeForPrimitive(value.front());
      stream << indent(5) << "<UserMetaData key=\"" << key << "\" dims=\"" ;
     // T typesize = value.front();
      
      int32 size = dims.size();
      for (int i = 0; i < size; ++i)
      {
        stream << dims[i];
        if (i < size - 1)
        {
          stream << " ";
        }
      }
      stream << "\" type=\"" << sType << "\">";
      // Now Write the data
      size = value.size();
      for(int i = 0; i < size; ++i) 
      {
       if ( i%dims[0] == 0)
        {
          stream << "\n" << indent(5);
        }
        if (sizeof(T) != 1 )
          stream  << value[i];
        else
          stream  << static_cast<int32>(value[i]);
          
        if (i < size - 1)
        {
          stream << " ";
        }
       
      }
      stream << "\n"<< indent(5) << "</UserMetaData>\n";
      return 1;
    }

      // -----------------------------------------------------------------------------
      //  
      // -----------------------------------------------------------------------------  
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::string &value)
     {
       locationId = 0; // Keeps Compiler Quiet
       std::ofstream &stream = *(_ofstreamPtr.get());
//       int32 size = value.size();
       stream << indent(5) << "<UserMetaData key=\"" << key << "\" dims=\"" << 1 <<  "\" type=\"string\">\n";
       stream << indent(5) << value << "\n";
       stream << indent(5) << "</UserMetaData>\n";
       return 1;
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
    int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<int8> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------  
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<uint8> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------  
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<int16> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key,  dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------  
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<uint16> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------  
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<int32> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------  
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<uint32> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------  
    int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<int64> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------  
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<uint64> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------   
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<float32> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

     // -----------------------------------------------------------------------------
     //  
     // -----------------------------------------------------------------------------  
     int32 writeAttribute(int32 locationId, std::string &datasetPath, std::string &key, std::vector<uint64> &dims, std::vector<float64> &value)
     {
       //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;     
       return _writeAttribute(locationId, datasetPath, key, dims, value);
     }

private:
  boost::shared_ptr<std::ofstream> _ofstreamPtr;
  
    XMLUserMetaDataWriter(const XMLUserMetaDataWriter&);   //Copy Constructor Not Implemented
    void operator=(const XMLUserMetaDataWriter&); //Copy Assignment Not Implemented
};



#endif /* _XMLATTRIBUTEWRITER_H_ */

