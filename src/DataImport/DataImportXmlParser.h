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
#ifndef _DataImportXmlParser_H_
#define _DataImportXmlParser_H_

//-- MXA Includes
#include "Common/MXATypeDefs.h"
#include "Common/MXATypes.h"
#include "Base/IDataModel.h"
#include "Base/IDataImport.h"
#include "Core/MXADataModel.h"
#include "XML/ExpatParser.h"
#include "XML/ExpatEvtHandler.h"
#include "XML/XMLConstants.h"


//-- STL includes
#include <string>

#define BUFFER_SIZE 1024


namespace MXA_DataImport {
  const std::string Data_Dimensions ("Data_Dimensions");
  const std::string Data_Import ("Data_Import");
  const std::string Data_Model ("Data_Model");
  const std::string Dimension ("Dimension");
  const std::string Explicit_Data_Source ("Explicit_Data_Source");
  const std::string FilePath ("FilePath");
  const std::string Implicit_Data_Source ("Implicit_Data_Source");
  const std::string Index_Part ("Index_Part");
  const std::string Output_File ("Output_File");
  const std::string Text_Part ("Text_Part");
  
  //This section NOT auto-generated
  const std::string Attr_Template_File ("Template_File");
  const std::string Attr_Absolute_Path ("Absolute_Path");
  const std::string Attr_DataDimension_Values ("DataDimension_Values");
  const std::string Attr_Data_Record ("Data_Record");
  const std::string Attr_File_Path ("File_Path");
} // End Namespace

typedef  std::map<std::string, std::string>        XMLAttributeMap;


/**
* @class DataImportXmlParser DataImportXmlParser.h DataImportXmlParser.h
* @brief 
* @author Mike Jackson
* @date Sept 2007
* @version $Revision: 1.1 $
*/
class DataImportXmlParser : public ExpatEvtHandler
{

public:
  DataImportXmlParser();
  virtual ~DataImportXmlParser();
  
  /**
   * @brief Sets the input file to parse
   * @param inputFile The path to an xml file
   */
  void setXMLInputFile(const std::string &inputFile)
  {
    this->_xmlFilename = inputFile;
  }
  
  /**
   * @brief Parses the XML file producing an IDataModel object and a vector of
   * IDataSource objects
   * @return Error Condition. Negative is an error.
   */
  int parseXMLFile();
  
  
  /**
   * @brief Sets the data model to use during the parsing
   * @return Boost shared pointer to the data model produced during the parsing
   */
  IDataModelPtr getDataModel()
  {
    return this->_dataModel;
  }

  
  /**
   * @brief Sets the IDataImport Object to store our IDataSources into
   * @param dataImportPtr IDataImport Boost Shared Pointer
   */
  void setIDataImport(IDataImportPtr dataImportPtr)
  {
    this->_dataImport = dataImportPtr;
  }
  
  
  /**
   * @brief will copy the data sources into the provided array
   * @param dataSources Vector of IDataSourcePtr objects
   */
  void getDataSources(IDataSources dataSources);
 
  
  //------------- ExpatEvtHandler Methods --------------------------------------
  /**
   * @brief Called when the beginning of an element is found
   * @param name The name of the element
   * @param attrs The attributes of the element
   */
  void OnStartElement(const XML_Char* name, const XML_Char** attrs);

  /**
   * @brief Called the the end of an element is encountered
   * @param name The name of the element that is ending
   */
  void OnEndElement(const XML_Char* name);

  

private:
  std::string        _xmlFilename;

  IDataModelPtr      _dataModel;
  IDataImportPtr     _dataImport;
  int32              _xmlParseError;
  bool               _parseData;
  std::string        _errorMessage;
  ExpatParser*       _parser;
  
  
  DataImportXmlParser(const DataImportXmlParser&); //Copy Constructor Not Implemented
  void operator=(const DataImportXmlParser&); //Copy Assignment Not Implemented


  // Methods for each Start Tag that was encountered.
  /** @brief Method that will be called when the 'Data_Dimensions' tag is found.  */
    void start_Data_Dimensions_Tag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Data_Dimension' tag is found.  */
    void start_Dimension_Tag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Data_Import' tag is found.  */
    void start_Data_Import_Tag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Data_Model' tag is found.  */
    void start_Data_Model_Tag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Data_Source' tag is found.  */
    void start_Implicit_Data_Source_Tag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Explicit_Data_Source' tag is found.  */
    void start_Explicit_Data_Source_Tag(const XML_Char* name, const XML_Char** attrs);
    
  /** @brief Method that will be called when the 'FilePath' tag is found.  */
    void start_FilePath_Tag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Index_Part' tag is found.  */
    void start_Index_Part_Tag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Text_Part' tag is found.  */
    void start_Text_Part_Tag(const XML_Char* name, const XML_Char** attrs);
    
  /** @brief Method that will be called when the 'Output_File' tag is found.  */
    void start_Output_File_Tag(const XML_Char* name, const XML_Char** attrs);

  // Methods for each End Tag that was encountered.
  /** @brief Method that will be called when the 'Data_Dimensions' tag is Exited.  */
    void end_Data_Dimensions_Tag(const XML_Char* name);
      
  /** @brief Method that will be called when the 'Data_Dimension' tag is Exited.  */
    void end_Dimension_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Data_Import' tag is Exited.  */
    void end_Data_Import_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Data_Model' tag is Exited.  */
    void end_Data_Model_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Implicit_Data_Source' tag is Exited.  */
    void end_Implicit_Data_Source_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Explicit_Data_Source' tag is Exited.  */
    void end_Explicit_Data_Source_Tag(const XML_Char* name);
      
  /** @brief Method that will be called when the 'FilePath' tag is Exited.  */
    void end_FilePath_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Index_Part' tag is Exited.  */
    void end_Index_Part_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Text_Part' tag is Exited.  */
    void end_Text_Part_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Output_File' tag is Exited.  */
    void end_Output_File_Tag(const XML_Char* name);
    
    
   //----------------------- Helper Methods ------------------------------------ 
    
    int _loadDataModelFromTemplateFile(std::string &modelFile);
    
    
}; // End Class DataImportXmlParser
#endif /* _DataImportXmlParser_H_ */
