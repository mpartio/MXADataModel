
#include "Base/IDataSource.h"
#include "Core/MXADataSource.h"
#include "Common/LogTime.h"
#include "DataImportXmlParser.h"
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

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int DataImportXmlParser::parseXMLFile()
{
  //Sanity Checks

  if (this->_xmlFilename.empty() ) { return -1; }
  
  if (NULL != this->_dataModel.get() ) 
  {
    IDataModel* nullModel = NULL;
    this->_dataModel.reset(nullModel); // Effectively frees the previous model if there was one
  }
#if 0
  Not sure that I want/need this section - Depends what I do with the IDataImport API
  if (NULL != this->_dataImport.get() )
  {
    IDataImport* nullDataImport = NULL;
    this->_dataImport.reset(nullDataImport);
  }
#endif
  // Clear any error messages that have been hanging around from previous imports
  _errorMessage.clear();
  
  //Create a new IDataModel object
  this->_dataModel.reset( new MXADataModel);
  //Set the model into the IDataImport object
  this->_dataImport->setDataModel(this->_dataModel);
  
  
  char buf[BUFFER_SIZE];
  // Create and initialise an instance of the parser.
  ExpatParser parser( dynamic_cast<ExpatEvtHandler*>( this ) );
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
          start_FilePath_Tag(name, attrs);
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
          end_FilePath_Tag(name);
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



// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Data_Model' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::start_Data_Model_Tag(const XML_Char* name, const XML_Char** attrs)
{
    //std::cout << "Start <" << name << "> Tag" << std::endl;
    int err = 0;
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

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Dimension' 
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Data_Dimensions' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::start_Data_Dimensions_Tag(const XML_Char* name, const XML_Char** attrs)
{

    printf("Starting %s\n", name); 
}


// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Data_Import' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::start_Data_Import_Tag(const XML_Char* name, const XML_Char** attrs)
{

    //std::cout << "Start <" << name << "> Tag" << std::endl; 
}



// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Implicit_Data_Source' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::start_Implicit_Data_Source_Tag(const XML_Char* name, const XML_Char** attrs)
{

    //std::cout << "Start <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Explicit_Data_Source' 
// -----------------------------------------------------------------------------
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
  
  //Read the Data Dimension Values
  std::istringstream istream ( attrMap[MXA_DataImport::Attr_DataDimension_Values] );
  std::vector<int> dimValues;
  int temp;
  while ( (istream >> temp).good() )
  {
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
#warning FIX THIS
  //  ds->setImportDelegate(tiffImportDelegatePtr); <=== This needs to be figured out
  ds->setDataModel(this->_dataModel);
  this->_dataImport->addDataSource(ds);
        
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'FilePath' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::start_FilePath_Tag(const XML_Char* name, const XML_Char** attrs)
{

    //std::cout << "Start <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Index_Part' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::start_Index_Part_Tag(const XML_Char* name, const XML_Char** attrs)
{

    //std::cout << "Start <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Text_Part' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::start_Text_Part_Tag(const XML_Char* name, const XML_Char** attrs)
{

    //std::cout << "Start <" << name << "> Tag" << std::endl; 
}


// -----------------------------------------------------------------------------
// Method fired when encountering starting of Tag 'Output_File' 
// -----------------------------------------------------------------------------
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
    this->_dataImport->setOutputFilePath(absoluteFilePath);
  } 
  else 
  {
    _errorMessage.append("Did not find the 'Absolute_Path' attribute in the 'Output_File' tag");
    this->_xmlParseError = -1; // Set the error flag to stop the import
  }
}

// *****************************************************************************
// *****************************************************************************

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Data_Dimensions' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Data_Dimensions_Tag(const XML_Char* name)
{
  printf("Ending %s\n", name); 
}


// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Dimension' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Dimension_Tag(const XML_Char* name)
{

    //std::cout << "End <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Data_Import' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Data_Import_Tag(const XML_Char* name)
{

    //std::cout << "End <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Data_Model' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Data_Model_Tag(const XML_Char* name)
{

    //std::cout << "End <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Implicit_Data_Source' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Implicit_Data_Source_Tag(const XML_Char* name)
{

    //std::cout << "End <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Explicit_Data_Source' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Explicit_Data_Source_Tag(const XML_Char* name)
{

    //std::cout << "End <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'FilePath' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_FilePath_Tag(const XML_Char* name)
{

    //std::cout << "End <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Index_Part' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Index_Part_Tag(const XML_Char* name)
{

    //std::cout << "End <" << name << "> Tag" << std::endl; 
}

// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Text_Part' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Text_Part_Tag(const XML_Char* name)
{

    //std::cout << "End <" << name << "> Tag" << std::endl; 
}


// -----------------------------------------------------------------------------
// Method fired when encountering the End of Tag 'Output_File' 
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Output_File_Tag(const XML_Char* name)
{
//    printf("Ending %s\n", name); 
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
int DataImportXmlParser::_loadDataModelFromTemplateFile(std::string &modelFile)
{
  std::cout << logTime() << "---------------- IMPORTING DATA MODEL TEMPLATE ------------------" << std::endl;
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
      this->_dataModel->printModel(std::cout, 1);
    }
    return err;
  }
  else 
  {
    H5IODelegate reader;
    err = reader.readModelFromFile(modelFile, dynamic_cast<MXADataModel*>(this->_dataModel.get()), true);
    return err;
  }
  std::cout << logTime() << "---------------- IMPORTING DATA MODEL TEMPLATE COMPLETE------------------" << std::endl;
  return err;
}



