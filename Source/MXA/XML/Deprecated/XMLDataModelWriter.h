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
#ifndef _XML_DATAMODEL_WRITER_H_
#define _XML_DATAMODEL_WRITER_H_

#error THIS NEEDS TO GO AWAY. Use the XMLStreamWriterDelegate instead

// MXA Includes
#include <MXA/Common/DLLExport.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Base/IDataModelWriter.h>
#include <MXA/Base/IDataDimensionWriter.h>
#include <MXA/Base/IDataDimension.h>
#include <MXA/Base/IDataRecordWriter.h>
#include <MXA/Base/IDataRecord.h>

//-- STL Headers
#include <iostream>
#include <string>
#include <fstream>

//Forward Declarations
class MXANode;

/**
 * @brief Concrete implementation of IDataModelWriter that is used to write
 * the data model to an XML file
 * @author  Mike Jackson
 * @date June 2007
 * @version $Revision: 1.2 $
 *
 */
class MXA_EXPORT XMLDataModelWriter
{

public:
  /**
   * @brief Constructor
   * @param dataModel The DataModel Object to write
   * @param fileName The name of the file to write the data model
   */
  XMLDataModelWriter( IDataModel::Pointer dataModel, const std::string &fileName);
  virtual ~XMLDataModelWriter();

  /**
   * @brief Writes the data model to an xml file
   * @param NOT_USED This parameter is not used/needed for xml writing
   * @return Error Code: Negative is Error Condition
   */
  int32_t writeModel(int32_t NOT_USED=0);

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

protected:
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
  IDataModel::Pointer     _dataModel;
  const std::string       _fileName;
  int32_t                   _dataRecordIndentation;

  boost::shared_ptr<std::ofstream> _ofstreamPtr;

  XMLDataModelWriter(const XMLDataModelWriter&);   //Copy Constructor Not Implemented
  void operator=(const XMLDataModelWriter&); //Copy Assignment Not Implemented
};



#endif /* _XML_DATAMODEL_WRITER_H_ */

