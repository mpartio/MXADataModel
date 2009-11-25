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
#ifndef _XML_USERATTRIBUTE_WRITER_H_
#define _XML_USERATTRIBUTE_WRITER_H_

//-- MXA Includes
#include <MXA/Base/IAttributeWriter.h>
#include <MXA/XML/XMLConstants.h>
#if MXA_HDF5_SUPPORT
#include <MXA/HDF5/H5Lite.h>
#endif

//-- boost includes
#include <boost/smart_ptr/shared_ptr.hpp>


/**
 * @brief This class writes User meta data from the data model to an xml file
 * @author Mike Jackson
 * @date June 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT XMLUserMetaDataWriter : public IAttributeWriter
{

public:
  XMLUserMetaDataWriter(boost::shared_ptr<std::ofstream> ofstreamPtr)
  {
    _ofstreamPtr = ofstreamPtr;
  }

  virtual ~XMLUserMetaDataWriter(){};


// -----------------------------------------------------------------------------
//  Does indenting for pretty printing the XML
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
  int32 _writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, T value)
  {
    int32 size = sizeof(T);
    std::string sType = H5Lite::HDFTypeForPrimitiveAsStr(value);
    std::ofstream &stream = *(_ofstreamPtr.get());
    stream << indent(5) << "<UserMetaData " << MXA_XML::UserMetaData::Key << "=\"" << key << "\" dims=\"" << 1 <<  "\" type=\"" << sType << "\">\n";
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
  int32 _writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<T> value)
  {
    std::ofstream &stream = *(_ofstreamPtr.get());
    std::string sType = H5Lite::HDFTypeForPrimitiveAsStr(value.front());
    stream << indent(5) << "<UserMetaData key=\"" << key << "\" dims=\"" ;

	std::vector<uint64>::size_type size = dims.size();
    for (std::vector<uint64>::size_type i = 0; i < size; ++i)
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
    for(std::vector<uint64>::size_type i = 0; i < size; ++i)
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
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::string &value)
 {
   locationId = 0; // Keeps Compiler Quiet
   std::ofstream &stream = *(_ofstreamPtr.get());
   stream << indent(5) << "<UserMetaData key=\"" << key << "\" dims=\"" << 1 <<  "\" type=\"H5T_STRING\">";
   stream << value;
   stream << indent(0) << "</UserMetaData>\n";
   return 1;
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, int8 value)
 {
    //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, uint8 value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, int16 value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, uint16 value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, int32 value)
 {
    //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, uint32 value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, int64 value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, uint64 value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, float32 value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, float64 value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<int8> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<uint8> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<int16> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key,  dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<uint16> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<int32> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<uint32> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<int64> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<uint64> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<float32> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32 writeAttribute(int32 locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64> &dims, std::vector<float64> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

private:
  boost::shared_ptr<std::ofstream> _ofstreamPtr;

    XMLUserMetaDataWriter(const XMLUserMetaDataWriter&);   //Copy Constructor Not Implemented
    void operator=(const XMLUserMetaDataWriter&); //Copy Assignment Not Implemented
};



#endif /* _XML_USERATTRIBUTE_WRITER_H_ */

