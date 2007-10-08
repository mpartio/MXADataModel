/*                                       vim:set ts=4 sw=4 noai sr sta et cin:
 * ExpatEvtHandler.cpp
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

#include <XML/ExpatEvtHandler.h>

ExpatEvtHandler::ExpatEvtHandler()
{
}

ExpatEvtHandler::~ExpatEvtHandler()
{
}

/*********************************** Event Handler Default Implementation **/

void ExpatEvtHandler::OnPostCreate(void)
{
}

void ExpatEvtHandler::OnStartElement(const XML_Char* name, const XML_Char** attrs)
{
}

void ExpatEvtHandler::OnEndElement(const XML_Char* name)
{
}

void ExpatEvtHandler::OnCharacterData(const XML_Char* data, int32 len)
{
}

void ExpatEvtHandler::OnProcessingInstruction(const XML_Char* target, const XML_Char* data)
{
}

void ExpatEvtHandler::OnComment(const XML_Char* data)
{
}

void ExpatEvtHandler::OnStartCdataSection(void)
{
}

void ExpatEvtHandler::OnEndCdataSection(void)
{
}

void ExpatEvtHandler::OnDefault(const XML_Char* data, int32 len)
{
}

bool ExpatEvtHandler::OnUnknownEncoding(const XML_Char* name, XML_Encoding* pInfo)
{
  return false;
}

void ExpatEvtHandler::OnStartNamespaceDecl(const XML_Char* prefix, const XML_Char* uri)
{
}

void ExpatEvtHandler::OnEndNamespaceDecl(const XML_Char* prefix)
{
}

void ExpatEvtHandler::OnXmlDecl(const XML_Char* version, const XML_Char* encoding, bool isStandalone)
{
}

void ExpatEvtHandler::OnStartDoctypeDecl(const XML_Char* doctype, const XML_Char* sysId, const XML_Char* pubId, bool hasInternalSubset)
{
}

void ExpatEvtHandler::OnEndDoctypeDecl(void)
{
}

