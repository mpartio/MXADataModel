
#include "Base/IDataSource.h"
#include "Base/IImportDelegate.h"
#include "Core/MXADataSource.h"
#include "Core/MXADataImport.h"
#include "Common/LogTime.h"
#include "DataImportXmlParser.h"
#include "DataImport/ImportDelegateManager.h"
#include "Utilities/StringUtils.h"
#include "XML/XMLIODelegate.h"
#include "HDF5/H5IODelegate.h"

//-- C++ stdlib includes
#include <iostream>
#include <sstream>

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataImportXmlParser::DataImportXmlParser()
{
_xmlParseError=0;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
DataImportXmlParser::~DataImportXmlParser()
{
}

// *****************************************************************************
// *************** IDataImport Implementation **********************************
// *****************************************************************************


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int32 DataImportXmlParser::import()
{
  MXATypes::MXAError err = 1;
  
  if (this->_xmlFilename.empty() )
  {
    std::cout << "Input XML File must be set." << std::endl;
    return -3;
  }
  
  // Create a new Data Model
  this->_dataModel =  MXADataModel::New();
  
  // Parse the XML Configuration file
  err = this->_parseXMLFile();
  if (err < 0)
  {
    return err;
  }
  // Now create the Output file and leave it open
  this->_dataModel->writeModel(this->_outputFilePath, false);
  
  
  std::cout << "Starting Import loop.... " << std::endl;
  // Finally try to run the import loop
  for (IDataSources::iterator iter = _dataSources.begin(); iter != _dataSources.end(); ++iter)
  {
    err = (*(iter))->getImportDelegate()->importDataSource( *(iter), this->_dataModel );
    if ( err < 0 )
    {
      break;
    }
  }
  
  // The H5 output file will be closed by the MXADataModel Destructor
  
  return err;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataImportXmlParser::addDataSource (IDataSourcePtr dataSource ) {
  this->_dataSources.push_back(dataSource);
}

 
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataImportXmlParser::setOutputFilePath ( std::string new_var ) {
  _outputFilePath = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string DataImportXmlParser::getOutputFilePath ( ) {
  return _outputFilePath;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataImportXmlParser::setDataModel ( boost::shared_ptr<IDataModel> new_var ) {
  _dataModel = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataModelPtr DataImportXmlParser::getDataModel ( ) {
  return _dataModel;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataImportXmlParser::setDataSources ( IDataSources &new_var ) {
  _dataSources = new_var;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataSources DataImportXmlParser::getDataSources ( ) {
  return _dataSources;
}



// *****************************************************************************
// *************** Private Method Implementations ******************************
// *****************************************************************************

// -----------------------------------------------------------------------------
int DataImportXmlParser::_loadDataModelFromTemplateFile(std::string &modelFile)
{
  //std::cout << logTime() << "---------------- IMPORTING DATA MODEL TEMPLATE ------------------" << std::endl;
  MXATypes::MXAError err = -1;
  if ( StringUtils::endsWith(modelFile, std::string(".xml") ) )
  {
    //XMLIODelegate reader;
    IODelegatePtr reader (new XMLIODelegate);
    err = this->_dataModel->readModel(modelFile, reader, true);
    //err = reader.readModelFromFile(modelFile, dynamic_cast<MXADataModel*>(this->_dataModel.get()), true);
    if (err < 0)
    {
      std::cout << logTime() << "Error Reading DataModel from File: " << modelFile << std::endl;
    }
    else 
    {
      //this->_dataModel->printModel(std::cout, 1);
    }
    return err;
  }
  else 
  {
    H5IODelegate reader;
    err = reader.readModelFromFile(modelFile, dynamic_cast<MXADataModel*>(this->_dataModel.get()), true);
    return err;
  }
  //std::cout << logTime() << "---------------- IMPORTING DATA MODEL TEMPLATE COMPLETE------------------" << std::endl;
  return err;
}

// -----------------------------------------------------------------------------
int DataImportXmlParser::_parseXMLFile()
{

  // Clear any error messages that have been hanging around from previous imports
  _errorMessage.clear();
  
  char buf[BUFFER_SIZE];
  // Create and initialise an instance of the parser.
  ExpatParser parser( static_cast<ExpatEvtHandler*>( this ) );
  //this->_parser = &parser;
  parser.Create(NULL, NULL);
  parser.EnableElementHandler();
  parser.EnableCharacterDataHandler();
  // Load the XML file.
  FILE*  fh    = fopen(_xmlFilename.c_str(), "r");
  if (NULL == fh)
  {
    std::cout << "Could Not Open XML File for reading:" << std::endl;
    return -1;
  }
  bool   atEnd = false;
  size_t nRead;
  while (!atEnd && this->_xmlParseError >= 0)
  {
    // Read a block from the XML file and pass it to the parser
    nRead = fread(buf, 1, BUFFER_SIZE, fh);
    atEnd = feof(fh);
    parser.Parse(buf, nRead, atEnd);
  }
  fclose(fh);

  return this->_xmlParseError;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataImportXmlParser::OnStartElement(const XML_Char* name, const XML_Char** attrs)
{
    std::string currentTag(name);
    if ( currentTag.compare(MXA_DataImport::Data_Dimensions) == 0 )
    {
          start_Data_Dimensions_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::Data_Import) == 0 )
    {
          start_Data_Import_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::Data_Model) == 0 )
    {
          start_Data_Model_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::Dimension) == 0 )
    {
          start_Dimension_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::Explicit_Data_Source) == 0 )
    {
          start_Explicit_Data_Source_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::FilePath) == 0 )
    {
          start_File_Path_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::Implicit_Data_Source) == 0 )
    {
          start_Implicit_Data_Source_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::Index_Part) == 0 )
    {
          start_Index_Part_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::Output_File) == 0 )
    {
          start_Output_File_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA_DataImport::Text_Part) == 0 )
    {
          start_Text_Part_Tag(name, attrs);
    }
} // End OnStartElement(...)


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataImportXmlParser::OnEndElement(const XML_Char* name)
{

    std::string currentTag(name);
    if ( currentTag.compare(MXA_DataImport::Data_Dimensions) == 0 )
    {
          end_Data_Dimensions_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::Data_Import) == 0 )
    {
          end_Data_Import_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::Data_Model) == 0 )
    {
          end_Data_Model_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::Dimension) == 0 )
    {
          end_Dimension_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::Explicit_Data_Source) == 0 )
    {
          end_Explicit_Data_Source_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::FilePath) == 0 )
    {
          end_File_Path_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::Implicit_Data_Source) == 0 )
    {
          end_Implicit_Data_Source_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::Index_Part) == 0 )
    {
          end_Index_Part_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::Output_File) == 0 )
    {
          end_Output_File_Tag(name);
    }
    else if ( currentTag.compare(MXA_DataImport::Text_Part) == 0 )
    {
          end_Text_Part_Tag(name);
    }
} // End OnEndElement(...)


// ******************** Starting Data_Dimensions **************************************
void DataImportXmlParser::start_Data_Dimensions_Tag(const XML_Char* name, const XML_Char** attrs)
{
    // std::cout << "Starting " << std::string(name) << std::endl;
}

void DataImportXmlParser::end_Data_Dimensions_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Data_Import **************************************
void DataImportXmlParser::start_Data_Import_Tag(const XML_Char* name, const XML_Char** attrs)
{
    // std::cout << "Starting " << std::string(name) << std::endl;
}

void DataImportXmlParser::end_Data_Import_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Data_Model **************************************
void DataImportXmlParser::start_Data_Model_Tag(const XML_Char* name, const XML_Char** attrs)
{
    //std::cout << "Start <" << name << "> Tag" << std::endl;
    for (int i = 0; attrs[i]; i += 2)
    {
      if (MXA_DataImport::Attr_Template_File.compare( attrs[i] ) == 0)
      {  // Found the template attribute - now load the model from that file
        std::string filename (attrs[i+1]);
        this->_xmlParseError = this->_loadDataModelFromTemplateFile( filename );
        if (_xmlParseError < 0)
         {
           std::cout << "Could not load template model from file: " << std::string(attrs[i+1]) << std::endl;
         }
      }
      else
      {
        std::cout << "Ignoring Unknown Attribute in <"<< MXA_DataImport::Data_Model << "> " << attrs[i] << "=" << attrs[i+1] << std::endl;
      }
    }
}

void DataImportXmlParser::end_Data_Model_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Dimension **************************************
void DataImportXmlParser::start_Dimension_Tag(const XML_Char* name, const XML_Char** attrs)
{
    //std::cout << "Start <" << name << "> Tag" << std::endl; 
    XMLAttributeMap attrMap;
    for (int i = 0; attrs[i]; i += 2) {
      attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
    }
    int32  start, end, increment = std::numeric_limits<int32>::max();
    int32  index, count, uniform = std::numeric_limits<int32>::min();
    
    //Check for each Attribute. If the attribute was in the list then parse its value
    if ( attrMap.find(MXA::MXA_INDEX_TAG) != attrMap.end() ) { StringUtils::stringToNum(index, attrMap[MXA::MXA_INDEX_TAG], std::dec); }
    if ( attrMap.find(MXA::MXA_COUNT_TAG) != attrMap.end() ) { StringUtils::stringToNum(count, attrMap[MXA::MXA_COUNT_TAG], std::dec);  }
    if ( attrMap.find(MXA::MXA_START_VALUE_TAG) != attrMap.end() ) { StringUtils::stringToNum(start, attrMap[MXA::MXA_START_VALUE_TAG], std::dec);  }
    if ( attrMap.find(MXA::MXA_END_VALUE_TAG) != attrMap.end() ) { StringUtils::stringToNum(end, attrMap[MXA::MXA_END_VALUE_TAG], std::dec);  }
    if ( attrMap.find(MXA::MXA_INCREMENT_TAG) != attrMap.end() ) { StringUtils::stringToNum(increment, attrMap[MXA::MXA_INCREMENT_TAG], std::dec);  }
    if ( attrMap.find(MXA::MXA_UNIFORM_TAG) != attrMap.end() ) { StringUtils::stringToNum(uniform, attrMap[MXA::MXA_UNIFORM_TAG], std::dec); }
    
    // Over ride what is currently set in the data model with the values from the 'Dimension' tag
    IDataDimension* dimPtr = this->_dataModel->getDataDimension(attrMap[MXA::MXA_NAME_TAG]);
    if ( NULL != dimPtr )
    {
     dimPtr->setIndex(index);
     dimPtr->setCount(count);
     dimPtr->setUniform(uniform);
     dimPtr->setStartValue(start);
     dimPtr->setEndValue(end);
     dimPtr->setIncrement(increment);
    }
}

void DataImportXmlParser::end_Dimension_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Explicit_Data_Source **************************************
void DataImportXmlParser::start_Explicit_Data_Source_Tag(const XML_Char* name, const XML_Char** attrs)
{
  // Read the attributes into a map for easier look up
  XMLAttributeMap attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
  
  // Get the Data Record and the File Path from the attributes
  std::string dataRecordPath (attrMap[MXA_DataImport::Attr_Data_Record] );
  std::string sourceFilePath (attrMap[MXA_DataImport::Attr_File_Path]);
  std::string sourceType (attrMap[MXA_DataImport::Attr_Source_Type]);
  //Read the Data Dimension Values
  std::istringstream istream ( attrMap[MXA_DataImport::Attr_DataDimension_Values] );
  std::vector<int> dimValues;
  int temp;
  while ( istream.good() )
  {
    istream >> temp;
    dimValues.push_back(temp);
  }
  
  //Convenience Pointer
  IDataModel* model = static_cast<MXADataModel*>(this->_dataModel.get() );
  IDataRecordPtr recordPtr = model->getDataRecordByNamedPath(dataRecordPath, NULL);
  if ( NULL == recordPtr.get() )
  {
    std::cout << "Error Retrieving Data Record from model. Path given from XML file was '"
    << dataRecordPath << "' and that path was not found in the data model."<< std::endl;
    this->_xmlParseError = -1;
    return;
  }
  
  IDataSourcePtr ds( new MXADataSource() ); //Create a new MXADataSource
  ds->setDimensionValues(dimValues);
  ds->setDataRecord(recordPtr);
  ds->setSourcePath(sourceFilePath);

  
  ImportDelegateManagerPtr idManagerPtr = ImportDelegateManager::instance();
  IImportDelegatePtr importDelegatePtr = idManagerPtr->newDataImportDelegate(sourceType);
  if (importDelegatePtr.get() != NULL )
  {
    ds->setImportDelegate(importDelegatePtr);
    ds->setDataModel(this->_dataModel);
    this->addDataSource(ds);
  } else {
    std::cout << "Could not locate an ImportDelegate for Source_Type '" << sourceType << "'" << std::endl;
    this->_xmlParseError = -1;
  }
}


void DataImportXmlParser::end_Explicit_Data_Source_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting File_Path **************************************
void DataImportXmlParser::start_File_Path_Tag(const XML_Char* name, const XML_Char** attrs)
{
    // std::cout << "Starting " << std::string(name) << std::endl;
}

void DataImportXmlParser::end_File_Path_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Implicit_Data_Source **************************************
void DataImportXmlParser::start_Implicit_Data_Source_Tag(const XML_Char* name, const XML_Char** attrs)
{
     std::cout << "Starting " << std::string(name) << std::endl;
}

void DataImportXmlParser::end_Implicit_Data_Source_Tag(const XML_Char* name)
{
     std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Index_Part **************************************
void DataImportXmlParser::start_Index_Part_Tag(const XML_Char* name, const XML_Char** attrs)
{
    // std::cout << "Starting " << std::string(name) << std::endl;
}

void DataImportXmlParser::end_Index_Part_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Output_File **************************************
void DataImportXmlParser::start_Output_File_Tag(const XML_Char* name, const XML_Char** attrs)
{
  std::string absoluteFilePath;
  for (int i = 0; attrs[i]; i += 2)
  {
    if (MXA_DataImport::Attr_Absolute_Path.compare(attrs[i]) == 0)
    { 
      absoluteFilePath.append(attrs[i+1]); // add the file path to the empty string
    }
    else
    {
      std::cout << "Ignoring Unknown Attribute in <"<< MXA_DataImport::Data_Model << "> "<< attrs[i]<< "="<< attrs[i+1]<< std::endl;
    }
  }
  // Check to make sure the file path was found
  if (absoluteFilePath.empty() == false )
  {
    this->setOutputFilePath(absoluteFilePath);
  } 
  else 
  {
    _errorMessage.append("Did not find the 'Absolute_Path' attribute in the 'Output_File' tag");
    this->_xmlParseError = -1; // Set the error flag to stop the import
  }
}

void DataImportXmlParser::end_Output_File_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Text_Part **************************************
void DataImportXmlParser::start_Text_Part_Tag(const XML_Char* name, const XML_Char** attrs)
{
    // std::cout << "Starting " << std::string(name) << std::endl;
}

void DataImportXmlParser::end_Text_Part_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}




