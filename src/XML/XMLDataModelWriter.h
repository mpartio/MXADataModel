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

// MXA Includes
#include "Common/DLLExport.h"
#include "Core/MXADataModel.h"
#include "Base/IDataModelWriter.h"
#include "Base/IDataDimensionWriter.h"
#include "Base/IDataDimension.h"
#include "Base/IDataRecordWriter.h"
#include "Base/IDataRecord.h"
#include "XML/XMLIODelegate.h"

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
 * @version 1.0
 *  
 */
class MXA_EXPORT XMLDataModelWriter : public IDataModelWriter, 
                                     public IDataDimensionWriter,
                                     public IDataRecordWriter
{

public:
  /**
   * @brief Constructor
   * @param ioDelegate The IFileIODelegate object to use 
   * @param dataModel The DataModel Object to write
   * @param fileName The name of the file to write the data model
   */
  XMLDataModelWriter(IFileIODelegate* ioDelegate, MXADataModel* dataModel, const std::string &fileName);
  virtual ~XMLDataModelWriter();

  /**
   * @brief Writes the data model to an xml file
   * @param NOT_USED This parameter is not used/needed for xml writing
   * @return Error Code: Negative is Error Condition
   */
  int32 writeModelToFile(int32 NOT_USED=0);

  /**
   * @brief Writes template section of data model
   * @param indentDepth The number of spaced to indent this section of XML
   * @return Error Code: Negative is Error Condition
   */
  int32 writeDataModelTemplate(int32 indentDepth);
  
  /**
   * @brief Writes the Data Dimensions
   * @param indentDepth The number of spaced to indent this section of XML
   * @return Error Code: Negative is Error Condition
   */
  int32 writeDataDimensions(int32 indentDepth);
  
  /**
   * @brief Writes the Data Records
   * @param indentDepth The number of spaced to indent this section of XML
   * @return Error Code: Negative is Error Condition
   */
  int32 writeDataRecords(int32 indentDepth);
  
  /**
   * @brief Writes the RequiredMeta Data
   * @param indentDepth The number of spaced to indent this section of XML
   * @return Error Code: Negative is Error Condition
   */
  int32 writeRequiredMetaData(int32 indentDepth);
  
  /**
   * @brief Writes the User Meta Data
   * @param indentDepth The number of spaced to indent this section of XML
   * @return Error Code: Negative is Error Condition
   */
  int32 writeUserMetaData(int32 indentDepth);
  
  /**
   * @brief Writes the File Type and File version tags
   * @param indentDepth The number of spaces to indent this section of XML
   * @return Error Code: negative is error condition
   */
  int32 _writeMXAInfo(int depth);
  
  /**
   * @brief Writes the data root to the XML file
   * @param indentDepth The number of spaced to indent this section of XML
   */
  void _writeDataRoot(int indentDepth);

  // --------IDataDimensionWriter Interface ----------------------
  /**
   * @brief Writes an IDataDimension to the underlying data storage
   * @param dim Non-NULL IDataDimension object
   * @return Error condition
   */
  int32 writeDataDimension(IDataDimension* dim);
  
  // --------IDataRecordWriter Interface -------------------------
  /**
  * @brief Writes a IDataRecord to the underlying storage mechanism
  * @param record The Data Record to Write
  * @return Error Code: Negate is error condition
  */
  int32 writeDataRecord(IDataRecord* record);

protected:
  static std::string indent(int indent);
  static std::string _toUpper(const std::string value);
  static void _replaceAll(std::string&, std::string, std::string);
  static std::string escapedText(const std::string &value);
  static std::string escapedAttribute(const std::string &value);
  void _openTag(std::string, int, bool, std::map<std::string, std::string>);
  void _openTag(std::string, int, bool group=true);
  void _closeGroupTag(std::string, int);

  
private:
  MXADataModel*       _dataModel;
  XMLIODelegate*      _ioDelegate;
  const std::string   _fileName;
  
  boost::shared_ptr<std::ofstream> _ofstreamPtr;
  
  XMLDataModelWriter(const XMLDataModelWriter&);   //Copy Constructor Not Implemented
  void operator=(const XMLDataModelWriter&); //Copy Assignment Not Implemented
};



#endif /* _XML_DATAMODEL_WRITER_H_ */

