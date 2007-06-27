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
#ifndef _XMLEvtHandler_H_
#define _XMLEvtHandler_H_

#include "Headers/MXATypes.h"

//-- MXA XML Includes
#include "XML/ExpatParser.h"
#include "XML/XMLConstants.h"


#include <string>



//TODO: Write better documentation
/**
 * @brief This class is responsible 
 * @author Mike Jackson
 * @date June 2007
 * @version 1.0
 *   XMLEvtHandler.h
 */
class MXA_EXPORT XMLEvtHandler : public ExpatEvtHandler
{

public:
  XMLEvtHandler();
  virtual ~XMLEvtHandler();

  int32 getError() { return _error; }
  // Over ride from Base class
  void OnStartElement(const XML_Char* name, const XML_Char** attrs);

  // Over ride from Base class
  void OnEndElement(const XML_Char* name);

// Methods for each Start Tag that was encountered.
/** @brief Method that will be called when the <Data_Dimensions> tag is found.  */
  void onData_DimensionsStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <Data_Model> tag is found.  */
  void onData_ModelStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <Data_Records> tag is found.  */
  void onData_RecordsStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <Data_Root> tag is found.  */
  void onData_RootStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <Dimension> tag is found.  */
  void onDimensionStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <File_Root> tag is found.  */
  void onFile_RootStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <Meta_Data> tag is found.  */
  void onMeta_DataStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <Required_MD> tag is found.  */
  void onRequired_MDStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <Signal> tag is found.  */
  void onSignalStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <Signal_Group> tag is found.  */
  void onSignal_GroupStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <UserDefined_MD> tag is found.  */
  void onUserDefined_MDStartTag(const XML_Char* name, const XML_Char** attrs);

/** @brief Method that will be called when the <UserMetaData> tag is found.  */
  void onUserMetaDataStartTag(const XML_Char* name, const XML_Char** attrs);

// Methods for each End Tag that was encountered.
/** @brief Method that will be called when the <Data_Dimensions> tag is Exited.  */
  void onData_DimensionsEndTag(const XML_Char* name);

/** @brief Method that will be called when the <Data_Model> tag is Exited.  */
  void onData_ModelEndTag(const XML_Char* name);

/** @brief Method that will be called when the <Data_Records> tag is Exited.  */
  void onData_RecordsEndTag(const XML_Char* name);

/** @brief Method that will be called when the <Data_Root> tag is Exited.  */
  void onData_RootEndTag(const XML_Char* name);

/** @brief Method that will be called when the <Dimension> tag is Exited.  */
  void onDimensionEndTag(const XML_Char* name);

/** @brief Method that will be called when the <File_Root> tag is Exited.  */
  void onFile_RootEndTag(const XML_Char* name);

/** @brief Method that will be called when the <Meta_Data> tag is Exited.  */
  void onMeta_DataEndTag(const XML_Char* name);

/** @brief Method that will be called when the <Required_MD> tag is Exited.  */
  void onRequired_MDEndTag(const XML_Char* name);

/** @brief Method that will be called when the <Signal> tag is Exited.  */
  void onSignalEndTag(const XML_Char* name);

/** @brief Method that will be called when the <Signal_Group> tag is Exited.  */
  void onSignal_GroupEndTag(const XML_Char* name);

/** @brief Method that will be called when the <UserDefined_MD> tag is Exited.  */
  void onUserDefined_MDEndTag(const XML_Char* name);

/** @brief Method that will be called when the <UserMetaData> tag is Exited.  */
  void onUserMetaDataEndTag(const XML_Char* name);

    
private:
  int32 _error;
  
  XMLEvtHandler(const XMLEvtHandler&); //Copy Constructor Not Implemented
  void operator=(const XMLEvtHandler&); //Copy Assignment Not Implemented
}; // End Class XMLEvtHandler
#endif /* _XMLEvtHandler_H_ */

