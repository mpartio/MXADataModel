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
#include <Common/MXATypeDefs.h>
#include <Common/MXATypes.h>
#include <Base/IDataModel.h>
#include <Base/IDataImport.h>
#include <Core/MXADataModel.h>
#include <XML/ExpatParser.h>
#include <XML/ExpatEvtHandler.h>
#include <XML/XMLConstants.h>
#include <Utilities/DataSourcePathBuilder.h>

//-- STL includes
#include <string>

#define BUFFER_SIZE 1024


namespace MXA_DataImport {
  const std::string Data_Dimensions ("Data_Dimensions");
  const std::string Data_Import ("Data_Import");
  const std::string Data_Model ("Data_Model");
  const std::string Dimension ("Dimension");
  const std::string Explicit_Data_Source ("Explicit_Data_Source");
  const std::string File_Path ("File_Path");
  const std::string Implicit_Data_Source ("Implicit_Data_Source");
  const std::string Index_Part ("Index_Part");
  const std::string Output_File ("Output_File");
  const std::string Text_Part ("Text_Part");
  const std::string Delete_Existing_File ("Delete_Existing_File");
  
  //This section NOT auto-generated
  const std::string Attr_Template_File ("Template_File");
  const std::string Attr_Absolute_Path ("Absolute_Path");
  const std::string Attr_DataDimension_Values ("DataDimension_Values");
  const std::string Attr_Data_Record ("Data_Record");
  const std::string Attr_Text("Text");
  const std::string Attr_Source_Type ("Source_Type");
  const std::string Attr_Padding_Char ("Padding_Char");
  const std::string Attr_Total_Char_Length("Total_Char_Length");
  const std::string Attr_Numeric_Type("Numeric_Type");
  const std::string Attr_Data_Dimension("Data_Dimension");
  
} // End Namespace

typedef  std::map<std::string, std::string>        XMLAttributeMap;


/**
* @brief This class will parse a Data Import XML configuration file using a SAX
* like parsing algorithm based on the expat parser. 
* @class DataImportXmlParser DataImportXmlParser.h DataImportXmlParser.h
* @author Mike Jackson
* @date Sept 2007
* @version $Revision: 1.13 $
*/
class MXA_EXPORT DataImportXmlParser : public ExpatEvtHandler, public IDataImport
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
   * @brief returns the XML File that this importer is using
   */
  std::string getXMLInputFile()
  {
    return this->_xmlFilename;
  }
  
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

  //------------------- IDataImport Methods ------------------------------------
  /**
   * Set the value of m_outputFilePath
   * @param outputFilePath the new value of m_outputFilePath
   */
  void setOutputFilePath ( std::string outputFilePath );

  /**
   * Get the value of m_outputFilePath
   * @return the value of m_outputFilePath
   */
  std::string getOutputFilePath ( );
  
  /**
   * @brief Sets the OverWriteDataFile property
   */
  void setDeleteExistingDataFile( std::string deleteExistingDataFile);
  
  /**
   * @brief returns the OverWriteDataFile property
   * @return The value of the OverWriteDataFile property
   */
  std::string getDeleteExistingDataFile();


  /**
   * Set the value of m_dataModel
   * @param dataModel the new value of m_dataModel
   */
  void setDataFile ( IDataFilePtr dataModel );

  /**
   * Get the value of m_dataModel
   * @return the value of m_dataModel
   */
  IDataFilePtr getDataFile ( );


  /**
   * Set the value of m_dataSources
   * @param datasources the new value of m_dataSources
   */
  void setDataSources ( IDataSources &datasources );

  /**
   * Get the value of m_dataSources
   * @return the value of m_dataSources
   */
  IDataSources getDataSources ( );

  /**
   * @param  dataSource The datasource to add to the list
   */
  void addDataSource (IDataSourcePtr dataSource );

  /** @brief Imports the data into the data file
  * @return Error Condition. Zero or Positive is Success
  */
  int32 import();


private:
  std::string        _xmlFilename;
  std::string        _outputFilePath;
  std::string        _deleteExistingDataFile;
  IDataFilePtr       _dataFile;
  IDataModelPtr      _dataModel;
  IDataSources       _dataSources;
  int32              _xmlParseError;
  bool               _parseData;
  std::string        _errorMessage;
  ExpatParser*       _parser;
  
  // - Implicit Data Source Variables
  std::vector<IDataDimensionPtr>                  _implDataDimensions;
  std::map<IDataDimensionPtr, IStringSectionPtr>  _implPathMap;
  IDataRecordPtr                                _implDataRecord;
  std::string                                   _implSourceType;
  std::string                                   _implPreTextSection;
  
  
  DataImportXmlParser(const DataImportXmlParser&); //Copy Constructor Not Implemented
  void operator=(const DataImportXmlParser&); //Copy Assignment Not Implemented

  void _createDataSource(std::string pathTemplate, std::vector<IDataDimension*>::size_type index, std::vector<int> &dimVals);
  void _createDataSource2();
  
  //---------- Methods that are called for each starting and ending tag --------
  /** @brief Method that will be called when the 'Data_Dimensions' tag is found.  */
    void start_Data_Dimensions_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Data_Dimensions' tag is Exited.  */
    void end_Data_Dimensions_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Data_Import' tag is found.  */
    void start_Data_Import_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Data_Import' tag is Exited.  */
    void end_Data_Import_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Data_Model' tag is found.  */
    void start_Data_Model_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Data_Model' tag is Exited.  */
    void end_Data_Model_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Dimension' tag is found.  */
    void start_Dimension_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Dimension' tag is Exited.  */
    void end_Dimension_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Explicit_Data_Source' tag is found.  */
    void start_Explicit_Data_Source_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Explicit_Data_Source' tag is Exited.  */
    void end_Explicit_Data_Source_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'File_Path' tag is found.  */
    void start_File_Path_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'File_Path' tag is Exited.  */
    void end_File_Path_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Implicit_Data_Source' tag is found.  */
    void start_Implicit_Data_Source_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Implicit_Data_Source' tag is Exited.  */
    void end_Implicit_Data_Source_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Index_Part' tag is found.  */
    void start_Index_Part_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Index_Part' tag is Exited.  */
    void end_Index_Part_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Output_File' tag is found.  */
    void start_Output_File_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Output_File' tag is Exited.  */
    void end_Output_File_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'Text_Part' tag is found.  */
    void start_Text_Part_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Text_Part' tag is Exited.  */
    void end_Text_Part_Tag(const XML_Char* name);

    
    
   //----------------------- Helper Methods ------------------------------------ 
    /**
     * @brief Loads the data model from an xml file or HDF5 file
     * @param modelFile The xml or HDF5 file
     * @return Error condition
     */
    int32 _loadDataModelFromTemplateFile(const std::string &modelFile);
    
    /**
     * @brief Does the actual work of parsing the xml file and creating all the
     * datasources for the import
     * @return Error condition
     */
    int32 _parseXMLFile();
    
    /**
     * @brief Merges the Model in memory to the model resident on disk.
     */
    int32 _mergeModelToDisk();
    
}; // End Class DataImportXmlParser



#endif /* _DataImportXmlParser_H_ */
