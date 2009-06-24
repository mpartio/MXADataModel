#include <MXA/DataImport/DataImportXmlParser.h>

#include <MXA/Common/LogTime.h>
#include <MXA/Base/IDataSource.h>
#include <MXA/Base/IImportDelegate.h>
#include <MXA/Base/IDataFile.h>
#include <MXA/Core/MXADataSource.h>
#include <MXA/Core/MXADataImport.h>
#include <MXA/Core/MXASupportFile.h>
#include <MXA/DataImport/ImportDelegateManager.h>
#include <MXA/Utilities/StringUtils.h>
#include <MXA/Utilities/DataSourcePathIndexSection.h>
#include <MXA/Utilities/DataSourcePathTextSection.h>
#include <MXA/XML/XMLDataModelReader.h>

#if MXA_HDF5_SUPPORT
#include <MXA/HDF5/H5MXADataFile.h>
#endif

//-- C++ stdlib includes
#include <iostream>
#include <sstream>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataImportXmlParser::DataImportXmlParser()
{
_xmlParseError=0;
_verbose = 0;
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
  err = this->parseXMLFile();
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
 // int i = 1;
  for (IDataSource::Collection::iterator iter = _dataSources.begin(); iter != _dataSources.end(); ++iter)
  {
    if (this->_verbose) { std::cout << "Importing data source: " << (*(iter))->getSourcePath()   << std::endl; }
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

  IDataModel::Pointer fModelPtr = _dataFile->getDataModel();
  IDataModel* fModel = fModelPtr.get();
  IDataModel* mModel = this->_dataModel.get();
  //Sanity check the number of dims first
  if (mModel->getNumberOfDataDimensions() != fModel->getNumberOfDataDimensions())
  {
    this->_xmlParseError = -1;
    return -1;
  }

  int32 dimSize = mModel->getNumberOfDataDimensions();
  IDataDimension::Pointer mDim;
  IDataDimension::Pointer fDim;
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
void DataImportXmlParser::addDataSource (IDataSource::Pointer dataSource ) {
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
void DataImportXmlParser::setDataFile ( IDataFile::Pointer dataFile )
{
  _dataFile = dataFile;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataFile::Pointer DataImportXmlParser::getDataFile ( )
{
  return _dataFile;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::setDataSources ( IDataSource::Collection &new_var ) {
  _dataSources = new_var;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IDataSource::Collection DataImportXmlParser::getDataSources ( ) {
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
       IDataFile::Pointer file = H5MXADataFile::OpenFile(modelFile, true);
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
int DataImportXmlParser::parseXMLFile()
{

  if (this->_dataModel.get() == NULL)
  {
    // Create a new Data Model
    this->_dataModel =  MXADataModel::New();
  }
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
    else if (currentTag.compare(MXA_DataImport::Import_Property) == 0)
    {
          start_Import_Property_Tag(name, attrs);
    }
    else if ( currentTag.compare(MXA::SupportFiles) == 0 )
    {
          onSupportFilesStartTag(name, attrs);
    }
    else if ( currentTag.compare(MXA_XML::Support_File) == 0 )
    {
          onSupportFileStartTag(name, attrs);
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
    else if (currentTag.compare(MXA_DataImport::Import_Property) == 0)
    {
          end_Import_Property_Tag(name);
    }
    else if ( currentTag.compare(MXA::SupportFiles) == 0 )
    {
          onSupportFilesEndTag(name);
    }
    else if ( currentTag.compare(MXA_XML::Support_File) == 0 )
    {
          onSupportFileEndTag(name);
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
  bool readXMLTemplate = false;
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
        else { readXMLTemplate = true; }
      }
      else
      {
        std::cout << "Ignoring Unknown Attribute in <"<< MXA_DataImport::Data_Model << "> " << attrs[i] << "=" << attrs[i+1] << std::endl;
      }
    }
    if (false == readXMLTemplate)
    {
      this->_xmlParseError = -1;
      std::cout << "You must supply the " << MXA_DataImport::Attr_Template_File << " attribute for the <Data_Model> tag."  << std::endl;
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
    IDataDimension::Pointer dim = this->_dataModel->getDataDimension(attrMap[MXA::MXA_NAME_TAG]);
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
  IDataRecord::Pointer recordPtr = model->getDataRecordByNamedPath(dataRecordPath, NULL);
  if ( NULL == recordPtr.get() )
  {
    std::cout << "Error Retrieving Data Record from model. Path given from XML file was '"
    << dataRecordPath << "' and that path was not found in the data model."<< std::endl;
    this->_xmlParseError = -1;
    return;
  }

  IDataSource::Pointer ds( new MXADataSource() ); //Create a new MXADataSource
  ds->setDimensionValues(dimValues);
  ds->setDataRecord(recordPtr);
  ds->setSourcePath(sourceFilePath);


  IImportDelegate::Pointer importDelegatePtr = ImportDelegateManager::createNewImportDelegate(sourceType);
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
   //  std::cout << "DataImportXmlParser::start_Implicit_Data_Source_Tag " << std::string(name) << std::endl;
     // Read the attributes into a map for easier look up
     _implDataDimensions.clear(); // Clear for new values
     _implPathMap.clear(); // Clear the PathMap to prepare for new values
     _implDataRecord.reset(); //Clear the Pointer
     _implSourceType = "";
     XMLAttributeMap* propMap = new XMLAttributeMap;
     _curImportPropertyMap.reset(propMap);

     // Read all the XML attributes into the map
     XMLAttributeMap attrMap;
     for (int i = 0; attrs[i]; i += 2) {
       attrMap[ std::string(attrs[i]) ] = std::string( attrs[i + 1] );
     }

     // Get the Data Record and the Source Type from the attributes
     std::string dataRecordPath (attrMap[MXA_DataImport::Attr_Data_Record] );
     _implSourceType = attrMap[MXA_DataImport::Attr_Source_Type];

     IDataModel* model = static_cast<MXADataModel*>(this->_dataModel.get() );
     IDataRecord::Pointer recordPtr = model->getDataRecordByNamedPath(dataRecordPath, NULL);
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
 // std::cout << "  DataImportXmlParser::_createDataSource" << std::endl;
  this->_createDataSource(pathTemplate, index, dimVals);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::_createDataSource(std::string currentTemplate,
                                            std::vector<IDataDimension*>::size_type index,
                                            std::vector<int> &dimValues)
{
  // std::cout << "  DataImportXmlParser::_createDataSource" << std::endl;
  IDataDimension::Pointer dim = _implDataDimensions[index];
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
  //  std::cout << dim->getDimensionName() << " i:" << i << std::endl;
    // Create a new Path
    IStringSection::Pointer strSection = _implPathMap[dim];
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
      IDataSource::Pointer ds( new MXADataSource() ); //Create a new MXADataSource
      ds->setDimensionValues(dimValues);
      ds->setDataRecord(_implDataRecord);
      ds->setSourcePath(completePath);

      IImportDelegate::Pointer importDelegatePtr = ImportDelegateManager::createNewImportDelegate(_implSourceType);
      if (importDelegatePtr.get() != NULL )
      {
        getCurrentImportDelegateProperties(importDelegatePtr);
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::getCurrentImportDelegateProperties(IImportDelegate::Pointer importDelegatePtr)
{
  XMLAttributeMap* map = this->_curImportPropertyMap.get();
  for (XMLAttributeMap::iterator iter = map->begin(); iter != map->end(); ++iter)
  {
//    std::cout << "getting property '" << (*iter).first << "' with value '" << (*iter).second << "'" << std::endl;
    importDelegatePtr.get()->setProperty( (*iter).first, (*iter).second);
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

  XMLAttributeMap::iterator mapIter = attrMap.end();
  uint32 index = _implDataDimensions.size();
  std::string paddingChar;
  mapIter = attrMap.find(MXA_DataImport::Attr_Padding_Char);
  // Check for the Padding_Char attribute
  if (mapIter != attrMap.end() )
  {
    paddingChar = (*mapIter).second;
  }

  // Check for the Max size of the character string to generate
  int32 width = -1; // Default to -1 so the string grows as needed
  std::string charLength("");
  mapIter = attrMap.find(MXA_DataImport::Attr_Max_Char_Length);
  if (mapIter != attrMap.end() )
  {
    charLength = (*mapIter).second;
    if (charLength.empty() == false )
    {
      StringUtils::stringToNum(width, charLength);
    }
  }

  char check = 0;
  if ( !paddingChar.empty() ) { check = check | 0x01; }
  if ( !charLength.empty()  ) { check = check | 0x02; }
  if (check == 1 || check == 2)
  {
    std::cout << "Dataimport xml parsing error: The tag 'Index_Part' is missing an attriubte." << std::endl;
    std::cout << "  Either '" << MXA_DataImport::Attr_Max_Char_Length << "' and '" << MXA_DataImport::Attr_Padding_Char
      << "' must BOTH appear or NEITHER must appear." << std::endl;
    this->_xmlParseError = -12;
    return;
  }

  // Get the Numeric type (int or float)
  std::string numericType ( attrMap[MXA_DataImport::Attr_Numeric_Type] );

  IStringSection::Pointer section (new DataSourcePathIndexSection(index, paddingChar, width, numericType) );
  section->setPreText(_implPreTextSection);

  std::string dimName = attrMap[MXA_DataImport::Attr_Data_Dimension];
  IDataModel* model = static_cast<MXADataModel*>(this->_dataModel.get() );
  IDataDimension::Pointer dim = model->getDataDimension(dimName);
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Index_Part_Tag(const XML_Char* name)
{
  //std::cout << "Ending " << std::string(name) << std::endl;
  _implPreTextSection.clear(); //Clear this variable at the close of the tag
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::start_Import_Property_Tag(const XML_Char* name, const XML_Char** attrs)
{
  XMLAttributeMap* attrMap = _curImportPropertyMap.get();
  for (int i = 0; attrs[i]; i += 4)
  {
   // std::cout << "Adding Import_Property: " << attrs[i+1] << "=" << attrs[i+3] << std::endl;
    attrMap->insert(XMLAttributeMapElement(std::string(attrs[i+1]), std::string( attrs[i + 3] )) );
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::end_Import_Property_Tag(const XML_Char* name)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::onSupportFilesStartTag(const XML_Char* name, const XML_Char** attrs)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::onSupportFilesEndTag(const XML_Char* name)
{
  // Increment our counter

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::onSupportFileStartTag(const XML_Char* name, const XML_Char** attrs)
{

  ISupportFile::Pointer sfile = MXASupportFile::New();
  for (int i = 0; attrs[i]; i += 2)
  {
    if (MXA::MXA_FILESYSTEM_PATH_TAG.compare(attrs[i]) == 0)
    {
      sfile->setFileSystemPath(attrs[i + 1] );
    }
    else if (MXA::MXA_FILETYPE_TAG.compare(attrs[i]) == 0)
    {
      sfile->setFileType(attrs[i + 1] );
    }
  }
  // Adding the support file will set it's index correctly
  this->_dataModel->addSupportFile(sfile, true);
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DataImportXmlParser::onSupportFileEndTag(const XML_Char* name)
{

}


