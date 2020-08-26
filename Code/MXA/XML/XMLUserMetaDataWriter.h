///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
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
#ifdef MXA_HDF5_SUPPORT
#include <MXA/HDF5/H5Lite.h>
#endif

//-- boost includes
#include <boost/shared_ptr.hpp>


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
    const int32_t indentSize = 2;
    return std::string(indentSize * depth, ' ');
  }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
  template<typename T>
  int32_t _writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, T value)
  {
    int32_t size = sizeof(T);
    std::string sType = H5Lite::HDFTypeForPrimitiveAsStr(value);
    std::ofstream &stream = *(_ofstreamPtr.get());
    stream << indent(5) << "<UserMetaData " << MXA_XML::UserMetaData::Key << "=\"" << key << "\" dims=\"" << 1 <<  "\" type=\"" << sType << "\">\n";
    if (size != 1) {
      stream << indent(5) << value;
    } else {
      stream << indent(5) << static_cast<int32_t>(value);
    }
    stream << "\n"<< indent(5) << "</UserMetaData>\n";
    return 1;
  }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
  template<typename T>
  int32_t _writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<T> value)
  {
    std::ofstream &stream = *(_ofstreamPtr.get());
    std::string sType = H5Lite::HDFTypeForPrimitiveAsStr(value.front());
    stream << indent(5) << "<UserMetaData key=\"" << key << "\" dims=\"" ;

	std::vector<uint64_t>::size_type size = dims.size();
    for (std::vector<uint64_t>::size_type i = 0; i < size; ++i)
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
    for(std::vector<uint64_t>::size_type i = 0; i < size; ++i)
    {
     if ( i%dims[0] == 0)
      {
        stream << "\n" << indent(5);
      }
      if (sizeof(T) != 1 )
        stream  << value[i];
      else
        stream  << static_cast<int32_t>(value[i]);

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
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::string &value)
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
int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, float value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, double value)
 {
   //std::cout << "Writing Attribute for Scalar '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<int8_t> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<uint8_t> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<int16_t> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key,  dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<uint16_t> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<int32_t> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<uint32_t> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<int64_t> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<uint64_t> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<float> &value)
 {
   //std::cout << "Writing Attribute for Vector '" << key << "'" << std::endl;
   return _writeAttribute(locationId, datasetPath, key, dims, value);
 }

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
 int32_t writeAttribute(int32_t locationId, const std::string &datasetPath, const std::string &key, std::vector<uint64_t> &dims, std::vector<double> &value)
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

