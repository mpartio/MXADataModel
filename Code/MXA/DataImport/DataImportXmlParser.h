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
#ifndef _DataImportXmlParser_H_
#define _DataImportXmlParser_H_

//-- MXA Includes
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/MXATypes.h>
#include <MXA/Base/IDataModel.h>
#include <MXA/Base/IDataImport.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/XML/ExpatParser.h>
#include <MXA/XML/ExpatEvtHandler.h>
#include <MXA/XML/XMLConstants.h>
#include <MXA/Utilities/DataSourcePathBuilder.h>

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
  const std::string Import_Property ("Import_Property");

  //This section NOT auto-generated
  const std::string Attr_Template_File ("Template_File");
  const std::string Attr_Absolute_Path ("Absolute_Path");
  const std::string Attr_DataDimension_Values ("DataDimension_Values");
  const std::string Attr_Data_Record ("Data_Record");
  const std::string Attr_Text("Text");
  const std::string Attr_Source_Type ("Source_Type");
  const std::string Attr_Padding_Char ("Padding_Char");
  const std::string Attr_Max_Char_Length("Max_Char_Length");
  const std::string Attr_Numeric_Type("Numeric_Type");
  const std::string Attr_Data_Dimension("Data_Dimension");
  const std::string Attr_Key ("Key");
  const std::string Attr_Value ("Value");

} // End Namespace

typedef  std::map<std::string, std::string>        XMLAttributeMap;
typedef  boost::shared_ptr<XMLAttributeMap>        XMLAttributeMapPtr;
typedef std::pair<std::string, std::string>        XMLAttributeMapElement;

/**
* @brief This class will parse a Data Import XML configuration file using a SAX
* like parsing algorithm based on the expat parser.
* @class DataImportXmlParser DataImportXmlParser.h DataImportXmlParser.h
* @author Mike Jackson
* @date Sept 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT DataImportXmlParser : public ExpatEvtHandler, public IDataImport
{

public:

    MXA_SHARED_POINTERS(DataImportXmlParser)

  DataImportXmlParser();
  virtual ~DataImportXmlParser();

  /**
   * @brief Sets the input file to parse
   * @param inputFile The path to an xml file
   */
  void setXMLInputFile(const std::string &inputFile)
  {
    this->m_XmlFilename = inputFile;
  }

  /**
   * @brief returns the XML File that this importer is using
   */
  std::string getXMLInputFile()
  {
    return this->m_XmlFilename;
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
  void setDataFile ( IDataFile::Pointer dataModel );

  /**
   * Get the value of m_dataModel
   * @return the value of m_dataModel
   */
  IDataFile::Pointer getDataFile ( );


  /**
   * Set the value of m_dataSources
   * @param datasources the new value of m_dataSources
   */
  void setDataSources ( IDataSource::Collection &datasources );

  /**
   * Get the value of m_dataSources
   * @return the value of m_dataSources
   */
  IDataSource::Collection getDataSources ( );

  /**
   * @param  dataSource The datasource to add to the list
   */
  void addDataSource (IDataSource::Pointer dataSource );

  /**
   * @brief Does the actual work of parsing the xml file and creating all the
   * datasources for the import
   * @return Error condition
   */
  int32_t parseXMLFile();


  /** @brief Imports the data into the data file
  * @return Error Condition. Zero or Positive is Success
  */
  int32_t import();

  MXA_INSTANCE_PROPERTY(bool, Verbose);

private:
  std::string        m_XmlFilename;
  std::string        m_OutputFilePath;
  std::string        m_DeleteExistingDataFile;
  IDataFile::Pointer       m_DataFile;
  IDataModel::Pointer      m_DataModel;
  IDataSource::Collection       m_DataSources;
  int32_t              m_XmlParseError;
  bool               m_ParseData;
  std::string        m_ErrorMessage;
  ExpatParser*       m_Parser;

  // - Implicit Data Source Variables
  std::vector<IDataDimension::Pointer>                  m_ImplDataDimensions;
  std::map<IDataDimension::Pointer, IStringSection::Pointer>  m_ImplPathMap;
  IDataRecord::Pointer                                m_ImplDataRecord;
  std::string                                   m_ImplSourceType;
  std::string                                   m_ImplPreTextSection;
  XMLAttributeMapPtr                            m_CurImportPropertyMap;

  DataImportXmlParser(const DataImportXmlParser&); //Copy Constructor Not Implemented
  void operator=(const DataImportXmlParser&); //Copy Assignment Not Implemented

  void _createDataSource(std::string pathTemplate, std::vector<IDataDimension*>::size_type index, std::vector<int> &dimVals);
  void _createDataSource2();
  void getCurrentImportDelegateProperties(IImportDelegate::Pointer importDelegatePtr);

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

  /** @brief Method that will be called when the 'Text_Part' tag is found.  */
    void start_Import_Property_Tag(const XML_Char* name, const XML_Char** attrs);
  /** @brief Method that will be called when the 'Text_Part' tag is Exited.  */
    void end_Import_Property_Tag(const XML_Char* name);

  /** @brief Method that will be called when the 'UserMetaData' tag is found.  */
    void onSupportFilesStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'UserMetaData' tag is found.  */
    void onSupportFileStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Support_Files' tag is Exited.  */
    void onSupportFilesEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Support_File' tag is Exited.  */
    void onSupportFileEndTag(const XML_Char* name);

   //----------------------- Helper Methods ------------------------------------
    /**
     * @brief Loads the data model from an xml file or HDF5 file
     * @param modelFile The xml or HDF5 file
     * @return Error condition
     */
    int32_t _loadDataModelFromTemplateFile(const std::string &modelFile);

    /**
     * @brief Merges the Model in memory to the model resident on disk.
     */
    int32_t _mergeModelToDisk();

}; // End Class DataImportXmlParser



#endif /* _DataImportXmlParser_H_ */
