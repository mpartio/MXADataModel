
#include <Base/IDataSource.h>
#include <Base/IImportDelegate.h>
#include <Base/IDataFile.h>
#include <Core/MXADataSource.h>
#include <Core/MXADataImport.h>
#include <Common/LogTime.h>
#include <DataImport/DataImportXmlParser.h>
#include <DataImport/ImportDelegateManager.h>
#include <Utilities/StringUtils.h>
#include <Utilities/DataSourcePathIndexSection.h>
#include <Utilities/DataSourcePathTextSection.h>
#include <XML/XMLDataModelReader.h>
#include <HDF5/H5MXADataFile.h>

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
  
  if (this->_dataModel->getDataDimension(0).get() == NULL)
  {
    std::cout << logTime() << "Data Model has Zero Dimensions." << "\n Error Location->" << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
    return -1010;
  }
  // Now create the Output file and leave it open
//  bool deleteExisting = false;
  if ( this->_deleteExistingDataFile.compare("true") == 0 ) 
  {    
    _dataFile = H5MXADataFile::CreateFileWithModel(this->_outputFilePath, this->_dataModel);
    if (NULL == _dataFile.get())
    {
      std::cout << "DataImportXmlParser::import - Error Creating Output file: " << this->_outputFilePath << std::endl;
      return -100;
    }
  }
  else
  {
    //Merge Models from file and the model currently in memory
    // Only the Dimensions should change
    err = this->_mergeModelToDisk();    
    // Check for error after merging the models
    if (err < 0)
    {
      return err;
    }
  }

  
  std::cout << logTime() << "Input XML: " << this->_xmlFilename << std::endl;
  std::cout << logTime() << "Output File: " << this->_outputFilePath << std::endl;
  //std::cout << "Starting Import loop.... " << std::endl;
  
  // Finally try to run the import loop
  for (IDataSources::iterator iter = _dataSources.begin(); iter != _dataSources.end(); ++iter)
  {
    err = (*(iter))->getImportDelegate()->importDataSource( *(iter), this->_dataFile );
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
int32 DataImportXmlParser::_mergeModelToDisk()
{
  
  int32 err = 1;
  //Read the model from the file, leaving it open and in Read/Write mode
  _dataFile = H5MXADataFile::OpenFile(this->_outputFilePath, false);
  
  if ( NULL == _dataFile.get() ) // Ouput file does NOT exist on file system, write the model in memory
  {
    _dataFile = H5MXADataFile::CreateFileWithModel(this->_outputFilePath, this->_dataModel);
    if(_dataFile.get() == NULL) { err = -1; }
    else { err = 1; }
    return err;
  }
  
  IDataModelPtr fModelPtr = _dataFile->getDataModel();
  IDataModel* fModel = fModelPtr.get();
  IDataModel* mModel = this->_dataModel.get();
  //Sanity check the number of dims first
  if (mModel->getNumberOfDataDimensions() != fModel->getNumberOfDataDimensions())
  {
    this->_xmlParseError = -1;
    return -1;
  }
 
  int32 dimSize = mModel->getNumberOfDataDimensions();
  IDataDimensionPtr mDim;
  IDataDimensionPtr fDim;
  // Iterate over the Dimensions
  for (int32 i = 0; i < dimSize; ++i) {
    mDim = mModel->getDataDimension(i);
    fDim = fModel->getDataDimension(i);
    if (mDim->getStartValue() < fDim->getStartValue())
    {
      fDim->setStartValue( mDim->getStartValue());
    }
    if (mDim->getEndValue() > fDim->getEndValue())
    {
      fDim->setEndValue( mDim->getEndValue() );
    }
    // We are only going to sync the start and end values. Anything else would 
    // throw the model into chaos
  }
  //write the model to disk and close the file
  err = _dataFile->saveDataModel();
  this->_dataModel = fModelPtr;
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
void DataImportXmlParser::setDeleteExistingDataFile( std::string deleteExistingDataFile)
{
  this->_deleteExistingDataFile = deleteExistingDataFile;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string DataImportXmlParser::getDeleteExistingDataFile()
{
  return this->_deleteExistingDataFile;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataImportXmlParser::setDataFile ( IDataFilePtr dataFile ) 
{
  _dataFile = dataFile;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IDataFilePtr DataImportXmlParser::getDataFile ( ) 
{
  return _dataFile;
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
int DataImportXmlParser::_loadDataModelFromTemplateFile(const std::string &modelFile)
{
  //std::cout << logTime() << "---------------- IMPORTING DATA MODEL TEMPLATE ------------------" << std::endl;
  MXATypes::MXAError err = -1;
  if ( StringUtils::endsWith(modelFile, std::string(".xml") ) )
  {
    XMLDataModelReader reader(this->_dataModel, modelFile);
    err = reader.readDataModel(-1);
    if (err < 0)
    {
      std::cout << DEBUG_OUT(logTime) << "\n\t\t Error Reading DataModel from File: " << modelFile << std::endl;
    }
    return err;
  }

  if ( StringUtils::endsWith(modelFile, std::string(".mxa") ) 
      || StringUtils::endsWith(modelFile, std::string(".h5") )
      || StringUtils::endsWith(modelFile, std::string(".hdf5") ) )
    {
       err = 1;
       IDataFilePtr file = H5MXADataFile::OpenFile(modelFile, true);
       if (NULL == file.get())
       {
         std::cout << logTime() << "DataImportXmlParser::_loadDataModelFromTemplateFile - Error Reading DataModel from File: " << modelFile << std::endl;
         err = -1;
         return err;
       }
       this->_dataModel = file->getDataModel();
     }
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
    std::cout << "Could Not Open XML File for reading: " << _xmlFilename << std::endl;
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
    else if ( currentTag.compare(MXA_DataImport::File_Path) == 0 )
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
    else if ( currentTag.compare(MXA_DataImport::File_Path) == 0 )
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
    IDataDimensionPtr dim = this->_dataModel->getDataDimension(attrMap[MXA::MXA_NAME_TAG]);
    if ( NULL != dim.get() )
    {
      dim->setIndex(index);
      dim->setCount(count);
      dim->setUniform(uniform);
      dim->setStartValue(start);
      dim->setEndValue(end);
      dim->setIncrement(increment);
    }
    else  // If the Dimension is NULL, then that dimension is not in the model 
    {
      std::cout << DEBUG_OUT(logTime) << "\n\t\tThe Data Dimension '" <<  attrMap[MXA::MXA_NAME_TAG] << "' was not found in the Data Model Template File. Check the spelling in each xml file as possible causes for the mismatch." << std::endl;
      this->_xmlParseError = -1;
    }
}

void DataImportXmlParser::end_Dimension_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}


// ******************** Starting Output_File **************************************
void DataImportXmlParser::start_Output_File_Tag(const XML_Char* name, const XML_Char** attrs)
{
  std::string absoluteFilePath;
  std::string deleteExistingDataFile;
  for (int i = 0; attrs[i]; i += 2)
  {
    if (MXA_DataImport::Attr_Absolute_Path.compare(attrs[i]) == 0)
    { 
      absoluteFilePath.append(attrs[i+1]); // add the file path to the empty string
    }
    else if (MXA_DataImport::Delete_Existing_File.compare(attrs[i]) == 0)
    {
      deleteExistingDataFile.append(attrs[i+1]); // add the overwrite value to the empty string
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
  
  // Check to make sure the Overwrite_Existing_File was found
  if (deleteExistingDataFile.empty() == false )
  {
    this->setDeleteExistingDataFile(deleteExistingDataFile);
  } 
  else 
  {
    _errorMessage.append("Did not find the 'Delete_Existing_File' attribute in the 'Output_File' tag");
    this->_xmlParseError = -1; // Set the error flag to stop the import
  }
}

void DataImportXmlParser::end_Output_File_Tag(const XML_Char* name)
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
  std::string sourceFilePath (attrMap[MXA_DataImport::File_Path]);
  std::string sourceType (attrMap[MXA_DataImport::Attr_Source_Type]);
  //Read the Data Dimension Values
  std::istringstream istream ( attrMap[MXA_DataImport::Attr_DataDimension_Values] );
  std::vector<int> dimValues;
  int32 temp;
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

// ******************** Starting Implicit_Data_Source **************************************
void DataImportXmlParser::start_Implicit_Data_Source_Tag(const XML_Char* name, const XML_Char** attrs)
{
  //   std::cout << "Starting " << std::string(name) << std::endl;
     // Read the attributes into a map for easier look up
     _implDataDimensions.clear(); // Clear for new values
     _implPathMap.clear(); // Clear the PathMap to prepare for new values
     _implDataRecord.reset(); //Clear the Pointer
     _implSourceType = "";
     
     // Read all the XML attributes into the map
     XMLAttributeMap attrMap;
     for (int i = 0; attrs[i]; i += 2) {
       attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
     }
     
     // Get the Data Record and the Source Type from the attributes
     std::string dataRecordPath (attrMap[MXA_DataImport::Attr_Data_Record] );
     _implSourceType = attrMap[MXA_DataImport::Attr_Source_Type];

     IDataModel* model = static_cast<MXADataModel*>(this->_dataModel.get() );
     IDataRecordPtr recordPtr = model->getDataRecordByNamedPath(dataRecordPath, NULL);
     if ( NULL == recordPtr.get() )
     {
       std::cout << "Error Retrieving Data Record from model. Path given from XML file was '"
       << dataRecordPath << "' and that path was not found in the data model."<< std::endl;
       this->_xmlParseError = -1;
       return;
     } 
     else
     {
       _implDataRecord = recordPtr;
     }
     
}

void DataImportXmlParser::end_Implicit_Data_Source_Tag(const XML_Char* name)
{
  //std::cout << "Ending " << std::string(name) << std::endl;
  std::vector<IDataDimension*>::size_type index = 0;
  std::string pathTemplate;
  std::vector<int> dimVals;
  this->_createDataSource(pathTemplate, index, dimVals);
}
  
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void DataImportXmlParser::_createDataSource(std::string currentTemplate, std::vector<IDataDimension*>::size_type index, std::vector<int> &dimValues)
{
  // std::cout << "_createDataSource" << std::endl;
  IDataDimensionPtr dim = _implDataDimensions[index];
  int32 start = dim->getStartValue();
  int32 end = dim->getEndValue();
  int32 incr = dim->getIncrement();
  if (index == dimValues.size())
  {
    dimValues.push_back(start);
  }
  int8 ok = true;
  for (int i = start; i <= end; i+=incr)
  {
    // Create a new Path 
    IStringSectionPtr strSection = _implPathMap[dim];
    std::string newPath = strSection->toString(i, ok);
    std::string completePath = currentTemplate + newPath;
    dimValues[index] = i; // Set the correct dimension Value for this increment
    if ( index+1 < _implDataDimensions.size()) // <============ RECURSIVE ALGORITHM HERE !!!!!
    {
      this->_createDataSource(completePath, index+1, dimValues);
    }
    else
    {
      // Create the data source
      completePath.append( _implPreTextSection ); // This should be dangling since we never had another index part 
      IDataSourcePtr ds( new MXADataSource() ); //Create a new MXADataSource
      ds->setDimensionValues(dimValues);
      ds->setDataRecord(_implDataRecord);
      ds->setSourcePath(completePath);

      
      ImportDelegateManagerPtr idManagerPtr = ImportDelegateManager::instance();
      IImportDelegatePtr importDelegatePtr = idManagerPtr->newDataImportDelegate(_implSourceType);
      if (importDelegatePtr.get() != NULL )
      {
        ds->setImportDelegate(importDelegatePtr);
        ds->setDataModel(this->_dataModel);
        this->addDataSource(ds);
      } else {
        std::cout << "Could not locate an ImportDelegate for Source_Type '" << _implSourceType << "'" << std::endl;
        this->_xmlParseError = -1;
      }
    }
  }  
}



// ******************** Starting File_Path **************************************
void DataImportXmlParser::start_File_Path_Tag(const XML_Char* name, const XML_Char** attrs)
{
  //   std::cout << "Starting " << std::string(name) << std::endl;
  _implPreTextSection.clear();
}

void DataImportXmlParser::end_File_Path_Tag(const XML_Char* name)
{
    // std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Text_Part **************************************
void DataImportXmlParser::start_Text_Part_Tag(const XML_Char* name, const XML_Char** attrs)
{
  //std::cout << "Starting " << std::string(name) << std::endl;
  XMLAttributeMap attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
  std::string text ( attrMap[MXA_DataImport::Attr_Text] );
  _implPreTextSection.append(text);
}

void DataImportXmlParser::end_Text_Part_Tag(const XML_Char* name)
{
  //  std::cout << "Ending " << std::string(name) << std::endl;
}

// ******************** Starting Index_Part **************************************
void DataImportXmlParser::start_Index_Part_Tag(const XML_Char* name, const XML_Char** attrs)
{
 // std::cout << "Starting " << std::string(name) << std::endl;
  XMLAttributeMap attrMap;
  for (int i = 0; attrs[i]; i += 2) {
    attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
  }
  
  
  uint32 index = _implDataDimensions.size();
  std::string paddingChar ( attrMap[ MXA_DataImport::Attr_Padding_Char] );
  int8 fillChar = paddingChar.at(0);
  std::string charLength ( attrMap[MXA_DataImport::Attr_Total_Char_Length] );
  int32 width = 1;
  StringUtils::stringToNum(width, charLength);
  std::string numericType ( attrMap[MXA_DataImport::Attr_Numeric_Type] );
  
  IStringSectionPtr section (new DataSourcePathIndexSection(index, fillChar, width, numericType) );
  section->setPreText(_implPreTextSection);
  
  std::string dimName = attrMap[MXA_DataImport::Attr_Data_Dimension];
  IDataModel* model = static_cast<MXADataModel*>(this->_dataModel.get() );
  IDataDimensionPtr dim = model->getDataDimension(dimName);
  //std::cout << "Dim Pointer: " << dim << std::endl;
  
  
  if ( dim.get() == NULL)
  {
    std::cout << logTime() << "DataImportXmlParser::start_Index_Part_Tag: Could not retrieve Data Dimension from Model" << std::endl;
    this->_xmlParseError = -11;
    return;
  }
  // Add the Data Dimension to the vector
  _implDataDimensions.push_back(dim);
  //Add an entry to the map
  _implPathMap[dim] = section;
}

void DataImportXmlParser::end_Index_Part_Tag(const XML_Char* name)
{
  //std::cout << "Ending " << std::string(name) << std::endl;
  _implPreTextSection.clear(); //Clear this variable at the close of the tag
}





