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
#ifndef XMLDATAMODELREADER_H_
#define XMLDATAMODELREADER_H_

//MXA Includes
#include <Common/DLLExport.h>
#include <Core/MXADataModel.h>
#include <Base/IDataModelReader.h>
#if MXA_HDF5_SUPPORT
#include <HDF5/H5Lite.h>
#endif
#include <DataWrappers/MXAArrayTemplate.hpp>
#include <XML/XMLIODelegate.h>
#include <XML/ExpatParser.h>
#include <XML/XMLConstants.h>

typedef  std::map<std::string, std::string>        XMLAttributeMap;
/**
 * @brief This class is a concrete implementation of the IDataModelReader abstract
 * class. This class is responsible for reading a data model from an XML File
 * @author Mike Jackson
 * @date June 2007
 * @version $Revision: 1.20 $
 * @class XMLDataModelReader XMLDataModelReader.h
 */
class MXA_EXPORT XMLDataModelReader  : public IDataModelReader,
                                       public ExpatEvtHandler
{
public:
	XMLDataModelReader(IDataModelPtr dataModel, const std::string &fileName);
	virtual ~XMLDataModelReader();

// -----------------------------------------------------------------------------
//  IDataModelReader Implementation
// -----------------------------------------------------------------------------
  /**
  * @brief Reads the Data Model from an XML File/
  * @param NOT_USED
  * @return Error: Negative is error Condition
  */
	int32 readDataModel(int32 NOT_USED) ;

// -----------------------------------------------------------------------------
//  ExpatEvtHandler Implementation
// -----------------------------------------------------------------------------
  // Over ride from ExpatEvtHandler class
  void OnStartElement(const XML_Char* name, const XML_Char** attrs);

  // Over ride from ExpatEvtHandler class
  void OnEndElement(const XML_Char* name);

  void OnCharacterData(const XML_Char* data, int32 len);

  /**
   * @brief Returns the current parser error.
   */
  int32 getParseError();

  /**
  * @brief Parses a string from the XML file that is data encoded as space delimited values
  * @param dims The dimensions of the data set
  * @return Error Code. Zero or positive is success.
  */
  template<typename T>
  int32 readPrimitiveAttribute( const std::vector<uint64> &dims)
  {
    //std::cout << logTime() << "readPrimitiveAttribute: " << this->_userMDKey << std::endl;
    int32 err = 1;
    std::istringstream istream (this->_userAttributeData);
    uint64 size = 1;
    for (std::vector<uint64>::const_iterator iter = dims.begin(); iter != dims.end(); ++iter )
    {
      size *= *iter;
    }

    if (dims.size() == 1 && dims.at(0) == 1) // One Dimensional Array with 1 element
    {
      //std::cout << logTime() << "  Scalar Value" << std::endl;
      T temp;
      int32 tmp;
      if (sizeof(T) == 1) // If we try to read a 'char' then the stream will only read a single char from the file, not what we want
      {
        if ( (istream >> tmp).good() )
        {
          IMXAArrayPtr attr = MXAArrayTemplate<T>::CreateSingleValueArray( static_cast<T>(tmp) );
          this->_dataModel->addUserMetaData(this->_userMDKey, attr);
        }
      }
      else
      {
        if ( (istream >> temp).good() )
        {
          IMXAArrayPtr attr = MXAArrayTemplate<T>::CreateSingleValueArray( static_cast<T>(temp) );
          this->_dataModel->addUserMetaData(this->_userMDKey, attr);
        }
      }

    }
    else // Multi-Dimensional Data
    {
     // std::cout << logTime() << "  Vector Value" << std::endl;
      std::vector<T> data;
      T temp;
      int32 tmp;
      if (sizeof(T) == 1) // If we try to read a 'char' then the stream will only read a single char from the file, not what we want
      {
        while( (istream >> tmp).good() )
        {
          data.push_back( static_cast<T>(tmp) );
        }
      }
      else
      {
        while ( (istream >> temp).good() )
        {
          data.push_back(temp);
        }
      }

      if (data.size() == size)
      {
        IMXAArrayPtr attr =
          MXAArrayTemplate<T>::CreateMultiDimensionalArray(  dims.size(), &(dims.front()) );
        T* dest = static_cast<T*>(attr->getVoidPointer(0));
        ::memcpy(dest, &(data.front()), data.size() * sizeof(T) );

        this->_dataModel->addUserMetaData(this->_userMDKey, attr);
      } else {
        err = -1;
      }
    }
    return err;
  }


private:
  //XMLIODelegate*    _ioDelegate;
  IDataModelPtr     _dataModel;
  const std::string _fileName;
  int32             _xmlParseError;
  IDataRecordPtr    _currentParentRecord;
  std::string       _userAttributeData;
  bool              _parseData;
  std::string       _userMDKey;
  std::string       _userMDDims;
  std::string       _userMDType;
  ExpatParser*      _parser;


  int32               _indent;

  XMLDataModelReader(const XMLDataModelReader&);   //Copy Constructor Not Implemented
  void operator=(const XMLDataModelReader&); //Copy Assignment Not Implemented


// -----------------------------------------------------------------------------
//  Methods to help with the parsing events
// -----------------------------------------------------------------------------
  // Methods for each Start Tag that was encountered.
  /** @brief Method that will be called when the 'Data_Dimensions' tag is found.  */
    void onData_DimensionsStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Data_Model' tag is found.  */
    void onData_ModelStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Data_Records' tag is found.  */
    void onData_RecordsStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Data_Root' tag is found.  */
    void onData_RootStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Dimension' tag is found.  */
    void onDimensionStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'File_Root' tag is found.  */
    void onFile_RootStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Meta_Data' tag is found.  */
    void onMeta_DataStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Required_MD' tag is found.  */
    void onRequired_MDStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Signal' tag is found.  */
    void onSignalStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'Signal_Group' tag is found.  */
    void onSignal_GroupStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'UserDefined_MD' tag is found.  */
    void onUserDefined_MDStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'UserMetaData' tag is found.  */
    void onUserMetaDataStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'UserMetaData' tag is found.  */
    void onSupportFilesStartTag(const XML_Char* name, const XML_Char** attrs);

  /** @brief Method that will be called when the 'UserMetaData' tag is found.  */
    void onSupportFileStartTag(const XML_Char* name, const XML_Char** attrs);

  // Methods for each End Tag that was encountered.
  /** @brief Method that will be called when the 'Data_Dimensions' tag is Exited.  */
    void onData_DimensionsEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Data_Model' tag is Exited.  */
    void onData_ModelEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Data_Records' tag is Exited.  */
    void onData_RecordsEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Data_Root' tag is Exited.  */
    void onData_RootEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Dimension' tag is Exited.  */
    void onDimensionEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'File_Root' tag is Exited.  */
    void onFile_RootEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Meta_Data' tag is Exited.  */
    void onMeta_DataEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Required_MD' tag is Exited.  */
    void onRequired_MDEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Signal' tag is Exited.  */
    void onSignalEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Signal_Group' tag is Exited.  */
    void onSignal_GroupEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'UserDefined_MD' tag is Exited.  */
    void onUserDefined_MDEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'UserMetaData' tag is Exited.  */
    void onUserMetaDataEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Support_Files' tag is Exited.  */
    void onSupportFilesEndTag(const XML_Char* name);

  /** @brief Method that will be called when the 'Support_File' tag is Exited.  */
    void onSupportFileEndTag(const XML_Char* name);

};

#endif /*XMLDATAMODELREADER_H_*/
