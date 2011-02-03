///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DATAMODELXMLEVTHANDLER_H_
#define DATAMODELXMLEVTHANDLER_H_

#include "MXA/MXATypes.h"
#include "MXA/Common/MXASetGetMacros.h"
#include "MXA/Base/IDataModel.h"

#if MXA_HDF5_SUPPORT
#include <MXA/HDF5/H5Lite.h>
#endif

#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/XML/ExpatParser.h>
#include <MXA/XML/ExpatEvtHandler.h>
#include <MXA/XML/XMLConstants.h>

#include <vector>
#include <sstream>

/**
* @class DataModelXMLEvtHandler DataModelXMLEvtHandler.h MXA/XML/DataModelXMLEvtHandler.h
* @brief
* @author Michael A. Jackson for BlueQuartz Software
* @date Dec 3, 2009
* @version 1.0
*/
class MXA_EXPORT DataModelXMLEvtHandler : public ExpatEvtHandler
{
  public:

    MXA_SHARED_POINTERS(DataModelXMLEvtHandler)
    MXA_TYPE_MACRO(DataModelXMLEvtHandler)

    /**
     *
     */
    typedef ExpatEvtHandler::Pointer ExpatEvtHandlerType;

    /**
     * @brief
     * @param model
     * @param parser
     */
    static ExpatEvtHandlerType NewExpatEvtHandler(IDataModel::Pointer model)
    {
      ExpatEvtHandlerType sharedPtr(new DataModelXMLEvtHandler(model));
      return sharedPtr;
    }

    /**
     * @brief
     * @param model
     * @param parser
     */
    static Pointer New(IDataModel::Pointer model)
    {
      Pointer sharedPtr (new DataModelXMLEvtHandler(model));
      return sharedPtr;
    }

    virtual ~DataModelXMLEvtHandler();

    MXA_INSTANCE_PROPERTY(IDataModel::Pointer, DataModel)
    MXA_INSTANCE_PROPERTY(ExpatParser*, Parser)
    /**
     * @brief Returns the current parser error.
     */
    MXA_INSTANCE_PROPERTY(int32_t, ParseError)

    // -----------------------------------------------------------------------------
    //  ExpatEvtHandler Implementation Begin

    // Over ride from ExpatEvtHandler class
    void OnStartElement(const XML_Char* name, const XML_Char** attrs);

    // Over ride from ExpatEvtHandler class
    void OnEndElement(const XML_Char* name);

    // Over ride from ExpatEvtHandler class
    void OnCharacterData(const XML_Char* data, int32_t len);

    //  ExpatEvtHandler Implementation End
    // -----------------------------------------------------------------------------



  protected:
    DataModelXMLEvtHandler(IDataModel::Pointer model);

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

    /**
      * @brief Parses a string from the XML file that is data encoded as space delimited values
      * @param dims The dimensions of the data set
      * @return Error Code. Zero or positive is success.
      */
      template<typename T>
      int32_t readPrimitiveAttribute( const std::vector<size_t> &dims)
      {
        //std::cout << logTime() << "readPrimitiveAttribute: " << this->_userMDKey << std::endl;
        int32_t err = 1;
        std::istringstream istream (this->_userAttributeData);
        size_t size = 1;
        for (std::vector<size_t>::const_iterator iter = dims.begin(); iter != dims.end(); ++iter )
        {
          size *= *iter;
        }

        if (dims.size() == 1 && dims.at(0) == 1) // One Dimensional Array with 1 element
        {
          //std::cout << logTime() << "  Scalar Value" << std::endl;
          T temp;
          int32_t tmp;
          if (sizeof(T) == 1) // If we try to read a 'char' then the stream will only read a single char from the file, not what we want
          {
            if ( (istream >> tmp).good() )
            {
              IMXAArray::Pointer attr = MXAArrayTemplate<T>::CreateSingleValueArray( static_cast<T>(tmp) );
              this->m_DataModel->addUserMetaData(this->_userMDKey, attr);
            }
          }
          else
          {
            if ( (istream >> temp).good() )
            {
              IMXAArray::Pointer attr = MXAArrayTemplate<T>::CreateSingleValueArray( static_cast<T>(temp) );
              this->m_DataModel->addUserMetaData(this->_userMDKey, attr);
            }
          }

        }
        else // Multi-Dimensional Data
        {
         // std::cout << logTime() << "  Vector Value" << std::endl;
          std::vector<T> data;
          T temp;
          int32_t tmp;
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
            IMXAArray::Pointer attr =
              MXAArrayTemplate<T>::CreateMultiDimensionalArray(  dims.size(), &(dims.front()) );
            T* dest = static_cast<T*>(attr->getVoidPointer(0));
            ::memcpy(dest, &(data.front()), data.size() * sizeof(T) );

            this->m_DataModel->addUserMetaData(this->_userMDKey, attr);
          } else {
            err = -1;
          }
        }
        return err;
      }

  private:
      IDataRecord::Pointer    _currentParentRecord;
      std::string             _userAttributeData;
      bool                    _parseData;
      std::string             _userMDKey;
      std::string             _userMDDims;
      std::string             _userMDType;
      int32_t                   _indent;
      //int32_t                   _xmlParseError;

    DataModelXMLEvtHandler(const DataModelXMLEvtHandler&);    // Copy Constructor Not Implemented
    void operator=(const DataModelXMLEvtHandler&);  // Operator '=' Not Implemented
};

#endif /* DATAMODELXMLEVTHANDLER_H_ */
