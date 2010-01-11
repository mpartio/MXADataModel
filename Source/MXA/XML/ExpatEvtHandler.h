/*                                       vim:set ts=4 sw=4 noai sr sta et cin:
 * ExpatEvtHandler.h
 * by Keith Gaughan <kmgaughan@eircom.net>
 *
 * A C++ wrapper around the Expat parser library.
 *
 * Copyright (c) Keith Gaughan, 2004.
 * All Rights Reserved.
 *
 * Permission is granted to anyone to use this software for any purpose on any
 * computer system, and to alter it and redistribute it, subject to the
 * following restrictions:
 *
 *  1. The author is not responsible for the consequences of use of this
 *     software, no matter how awful, even if they arise from flaws in it.
 *
 *  2. The origin of this software must not be misrepresented, either by
 *     explicit claim or by omission. Since few users ever read sources,
 *     credits must appear in the documentation.
 *
 *  3. Altered versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software. Since few users ever
 *     read sources, credits must appear in the documentation.
 *
 *  4. The author reserves the right to change the licencing details on any
 *     future releases of this package.
 *
 *  5. This notice may not be removed or altered.
 */

#ifndef XMLEVTHANDLER_H_
#define XMLEVTHANDLER_H_


//MXA Includes
#include <MXA/MXATypes.h>
#include "MXA/Common/MXASetGetMacros.h"
#include <expat.h>


/**
* @class ExpatEvtHandler ExpatEvtHandler.h src/XML/ExpatEvtHandler.h
* @brief Base class to handle Expat Events
* @author Keith Gaughan
* @date 2004
* @version $Revision: 1.2 $
*/
class MXA_EXPORT ExpatEvtHandler
{
public:

    MXA_SHARED_POINTERS(ExpatEvtHandler)
    MXA_TYPE_MACRO(ExpatEvtHandler)

  ExpatEvtHandler();
  virtual ~ExpatEvtHandler();

  virtual void OnPostCreate(void);
  virtual void OnStartElement(const XML_Char* name, const XML_Char** attrs);
  virtual void OnEndElement(const XML_Char* name);
  virtual void OnCharacterData(const XML_Char* data, int32_t len);
  virtual void OnProcessingInstruction(const XML_Char* target, const XML_Char* data);
  virtual void OnComment(const XML_Char* data);
  virtual void OnStartCdataSection(void);
  virtual void OnEndCdataSection(void);
  virtual void OnDefault(const XML_Char* data, int32_t len);
  virtual bool OnUnknownEncoding(const XML_Char* name, XML_Encoding* pInfo);
  virtual void OnStartNamespaceDecl(const XML_Char* prefix, const XML_Char* uri);
  virtual void OnEndNamespaceDecl(const XML_Char* prefix);
  virtual void OnXmlDecl(const XML_Char* version, const XML_Char* encoding, bool isStandalone);
  virtual void OnStartDoctypeDecl(const XML_Char* doctype, const XML_Char* sysId, const XML_Char* pubId, bool hasInternalSubset);
  virtual void OnEndDoctypeDecl(void);

  virtual int32_t getParseError();
private:
  ExpatEvtHandler(const ExpatEvtHandler&); //Copy Constructor Not Implemented
  void operator=(const ExpatEvtHandler&); //Copy Assignment Not Implemented
};


#endif /*XMLEVTHANDLER_H_*/
