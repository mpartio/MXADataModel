/*
 * XMLStreamWriterDelegate.h
 *
 *  Created on: Dec 2, 2009
 *      Author: mjackson
 */

#ifndef XMLSTREAMWRITERDELEGATE_H_
#define XMLSTREAMWRITERDELEGATE_H_

#include "MXA/Common/MXASetGetMacros.h"
#include <MXA/Base/IDataModel.h>
#include "MXA/Base/IMXADataModelWriterDelegate.h"

#include <iostream>
/**
* @class XMLStreamWriterDelegate XMLStreamWriterDelegate.h MXA/XML/XMLStreamWriterDelegate.h
* @brief
* @author Michael A. Jackson for BlueQuartz Software
* @date Dec 2, 2009
* @version 1.0
*/
template<typename T>
class XMLStreamWriterDelegate: public IMXADataModelWriterDelegate
{
  public:
    MXA_SHARED_POINTERS(XMLStreamWriterDelegate<T> )
    MXA_TYPE_MACRO(XMLStreamWriterDelegate<T> )


    typedef typename boost::shared_ptr<T>  StreamPointer;

    static Pointer New()
    {
     StreamPointer out = StreamPointer(new T);
     Pointer sharedPtr(new XMLStreamWriterDelegate<T>(out));
     return sharedPtr;
    }

    virtual ~XMLStreamWriterDelegate();

    virtual int32_t writeModel(IDataModel::Pointer model);

    MXA_INSTANCE_PROPERTY_m(StreamPointer, StreamPointer);

  protected:
    XMLStreamWriterDelegate(StreamPointer stream);


    /**
     * @brief Writes template section of data model
     * @param indentDepth The number of spaced to indent this section of XML
     * @return Error Code: Negative is Error Condition
     */
    int32_t writeDataModelTemplate(int32_t indentDepth);

    /**
     * @brief Writes the Data Dimensions
     * @param indentDepth The number of spaced to indent this section of XML
     * @return Error Code: Negative is Error Condition
     */
    int32_t writeDataDimensions(int32_t indentDepth);

    /**
     * @brief Writes the Data Records
     * @param indentDepth The number of spaced to indent this section of XML
     * @return Error Code: Negative is Error Condition
     */
    int32_t writeDataRecords(int32_t indentDepth);

    /**
     * @brief Writes the RequiredMeta Data
     * @param indentDepth The number of spaced to indent this section of XML
     * @return Error Code: Negative is Error Condition
     */
    int32_t writeRequiredMetaData(int32_t indentDepth);

    /**
     * @brief Writes the User Meta Data
     * @param indentDepth The number of spaced to indent this section of XML
     * @return Error Code: Negative is Error Condition
     */
    int32_t writeUserMetaData(int32_t indentDepth);

    /**
     * @brief Writes the File Type and File version tags
     * @param depth The number of spaces to indent this section of XML
     * @return Error Code: negative is error condition
     */
    int32_t _writeMXAInfo(int32_t depth);

    /**
     * @brief Writes the data root to the XML file
     * @param indentDepth The number of spaced to indent this section of XML
     */
    void _writeDataRoot(int32_t indentDepth);

    // --------IDataDimensionWriter Interface ----------------------
    /**
     * @brief Writes an IDataDimension to the underlying data storage
     * @param dim Non-NULL IDataDimension object
     * @return Error condition
     */
    int32_t writeDataDimension(IDataDimension* dim);

    // --------IDataRecordWriter Interface -------------------------
    /**
    * @brief Writes a IDataRecord to the underlying storage mechanism
    * @param record The Data Record to Write
    * @return Error Code: Negate is error condition
    */
    int32_t writeDataRecord(IDataRecord* record);

    /**
    * @brief Writes the support files to the hdf5 data file
    * @param indentDepth The number of spaces to indent the xml code
    * @return error code
    */
    int32_t writeSupportFiles(int32_t indentDepth);

    static std::string _toUpper(const std::string value);
    static void _replaceAll(std::string&, std::string, std::string);
    static std::string escapedText(const std::string &value);
    static std::string escapedAttribute(const std::string &value);

    void _openTag(const std::string &tagName,
                  int32_t depth,
                  bool group,
                  std::map<std::string, std::string> &attributes);

    void _openTag(const std::string &tagName,
                  int32_t depth,
                  bool group=true);

    void _closeGroupTag(const std::string &tagName, int32_t depth);


  private:
    MXA_INSTANCE_PROPERTY_m(IDataModel::Pointer, DataModel)
    int32_t                   _dataRecordIndentation;

    XMLStreamWriterDelegate(const XMLStreamWriterDelegate&); // Copy Constructor Not Implemented
    void operator=(const XMLStreamWriterDelegate&); // Operator '=' Not Implemented
};

#include "XMLStreamWriterDelegate.txx"

#endif /* XMLSTREAMWRITERDELEGATE_H_ */
