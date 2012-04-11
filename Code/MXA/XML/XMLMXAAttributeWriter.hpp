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
#ifndef _XMLMXAAttributeWriter_h_
#define _XMLMXAAttributeWriter_h_

#include <MXA/Base/IMXAArray.h>
#if MXA_HDF5_SUPPORT
#include <hdf5.h>
#endif
/**
* @class XMLMXAAttributeWriter XMLMXAAttributeWriter.hpp XML/XMLMXAAttributeWriter.hpp
* @brief Delegate type class that writes attributes to an xml file
* @author mjackson
* @date Jan 15, 2008
* @version $Revision: 1.2 $
*/

class XMLMXAAttributeWriter
{
  public:
    XMLMXAAttributeWriter(boost::shared_ptr<std::ostream> ofstreamPtr) :
      m_OutStream(ofstreamPtr)
      {}

    virtual ~XMLMXAAttributeWriter() {}

// -----------------------------------------------------------------------------
//  Does indenting for pretty printing the XML
// -----------------------------------------------------------------------------
      std::string indent(int depth)
      {
        const int32_t indentSize = 2;
        return std::string(indentSize * depth, ' ');
      }

/**
 * @brief Writes the attribute to the xml file
 * @param attributeKey The name of the attribute to write
 * @param attribute The attribute to write
 * @return Error condition - Negative is error
 */
    int32_t writeAttribute(const std::string &attributeKey, IMXAArray::Pointer attribute)
    {
      int32_t typeId = attribute->getDataType();
      int32_t err = -1;
      if (typeId == H5T_NATIVE_FLOAT)
      {
        err = this->writeAttributeData<float>(attributeKey, attribute);
      }
      else if (typeId == H5T_NATIVE_DOUBLE)
      {
        err = this->writeAttributeData<double>(attributeKey, attribute);
      }

      else if (typeId == H5T_NATIVE_INT8)
      {
        err = this->writeAttributeData<int8_t>(attributeKey, attribute);
      }
      else if (typeId == H5T_NATIVE_UINT8)
      {
        err = this->writeAttributeData<uint8_t>(attributeKey, attribute);
      }

      else if (typeId == H5T_NATIVE_INT16)
      {
        err = this->writeAttributeData<int16_t>(attributeKey, attribute);
      }
      else if (typeId == H5T_NATIVE_UINT16)
      {
        err = this->writeAttributeData<uint16_t>(attributeKey, attribute);
      }

      else if (typeId == H5T_NATIVE_INT32)
      {
        err = this->writeAttributeData<int32_t>(attributeKey, attribute);
      }
      else if (typeId == H5T_NATIVE_UINT32)
      {
        err = this->writeAttributeData<uint32_t>(attributeKey, attribute);
      }

      else if (typeId == H5T_NATIVE_INT64)
      {
        err = this->writeAttributeData<int64_t>(attributeKey, attribute);
      }
      else if (typeId == H5T_NATIVE_UINT64)
      {
        err = this->writeAttributeData<uint64_t>(attributeKey, attribute);
      }

      else if (typeId == H5T_STRING)
      {
        err = this->writeStringAttributeData(attributeKey, attribute);
      }
      else
      {
        std::cout << logTime() << "typeId for Attribute could not be determined." << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
      }
      return err;
    }

/**
 * @brief Template based method to write an attribute to an xml file
 * @param attributeKey The name of the attribute to write
 * @param attribute The attribute to write to the xml file
 * @return Error Condition - Negative is Error.
 */
    template<typename T>
    int32_t writeAttributeData(const std::string &attributeKey, IMXAArray::Pointer attribute)
    {
      std::ostream &stream = *(m_OutStream.get());
      T* value = static_cast<T*> (attribute->getVoidPointer(0) );
      uint64_t nElements = attribute->getNumberOfElements();
      int32_t size = attribute->getNumberOfDimensions();

      std::string sType = H5Lite::HDFTypeForPrimitiveAsStr(value[0]);
      stream << indent(5) << "<UserMetaData key=\"" << attributeKey << "\" dims=\"" ;
      std::vector<size_t> dims(size, 0);
      attribute->getDimensions( &(dims.front() ) );

      // Write the values of each dimension to the file
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
      for(uint64_t i = 0; i < nElements; ++i)
      {
       if ( i%dims[0] == 0)
        {
          stream << "\n" << indent(5);
        }
        if (sizeof(T) != 1 )
        {
          stream  << value[i];
        }
        else
        {
          stream  << static_cast<int32_t>(value[i]);
        }
        stream << " ";

      }
      stream << "\n"<< indent(5) << "</UserMetaData>\n";
      return 1;
    }

/**
 * @brief Writes a string based attribute to an xml file
 * @param attributeKey The name of the attribute to write
 * @param attribute The attribute to write
 * @return Error Condition - Negative is error
 */
    int32_t writeStringAttributeData(const std::string &attributeKey, IMXAArray::Pointer attribute)
    {
      std::ostream &stream = *(m_OutStream.get());
      stream << indent(5) << "<UserMetaData key=\"" << attributeKey << "\" dims=\"1\" type=\"H5T_STRING\">";
      char* s = static_cast<char*>(attribute->getVoidPointer(0));
      uint64_t nElements = attribute->getNumberOfElements();
      if (s[nElements-1] == 0) // Null Terminated
      {
        nElements -= 1;
      }
      std::string str(s, nElements);

      stream << str << "</UserMetaData>" << std::endl;
      return 1;
    }

  protected:

  private:
    boost::shared_ptr<std::ostream> m_OutStream;

    XMLMXAAttributeWriter(const XMLMXAAttributeWriter&);    //Not Implemented
    void operator=(const XMLMXAAttributeWriter&); //Not Implemented

};

#endif //_XMLMXAAttributeWriter_h_




