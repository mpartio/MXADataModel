/*                                       vim:set ts=4 sw=4 noai sr sta et cin:
 * XMLParser.cpp
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

#include <MXA/XML/ExpatParser.h>
#include <expat.h>
#include <assert.h>
#include <string.h>

/********************************************** Internal Parser Callbacks **/

#ifdef __cplusplus
extern "C" {
#endif

static void StartElementHandler(void* pUserData, const XML_Char* name, const XML_Char** attrs)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnStartElement(name, attrs);
}

static void EndElementHandler(void* pUserData, const XML_Char* name)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnEndElement(name);
}

static void CharacterDataHandler(void* pUserData, const XML_Char* data, int32 len)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnCharacterData(data, len);
}

static void ProcessingInstructionHandler(void* pUserData, const XML_Char* target, const XML_Char* data)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnProcessingInstruction(target, data);
}

static void CommentHandler(void* pUserData, const XML_Char* data)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnComment(data);
}

static void StartCdataSectionHandler(void* pUserData)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnStartCdataSection();
}

static void EndCdataSectionHandler(void* pUserData)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnEndCdataSection();
}

static void DefaultHandler(void* pUserData, const XML_Char* data, int32 len)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnDefault(data, len);
}

static int32 UnknownEncodingHandler(void* pUserData, const XML_Char* name, XML_Encoding* pInfo)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	return ph->OnUnknownEncoding(name, pInfo) ? 1 : 0;
}

static void StartNamespaceDeclHandler(void* pUserData, const XML_Char* prefix, const XML_Char* uri)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnStartNamespaceDecl(prefix, uri);
}

static void EndNamespaceDeclHandler(void* pUserData, const XML_Char* prefix)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnEndNamespaceDecl(prefix);
}

static void XmlDeclHandler(void* pUserData, const XML_Char* version, const XML_Char* encoding, int32 isStandalone)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnXmlDecl(version, encoding, isStandalone != 0);
}

static void StartDoctypeDeclHandler(void* pUserData, const XML_Char* doctype, const XML_Char* sysId, const XML_Char* pubId, int32 hasInternalSubset)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnStartDoctypeDecl(doctype, sysId, pubId, hasInternalSubset != 0);
}

static void EndDoctypeDeclHandler(void* pUserData)
{
	ExpatEvtHandler* ph = static_cast<ExpatEvtHandler*>(pUserData);
	ph->OnEndDoctypeDecl();
}

#ifdef __cplusplus
};
#endif

/****************************************** Parser Wrapper Implementation **/

ExpatParser::ExpatParser(ExpatEvtHandler* pHandler)
{
	m_parser   = NULL;
	m_pHandler = pHandler;
}

ExpatParser::~ExpatParser()
{
	Destroy();
}

bool ExpatParser::Create(const XML_Char* encoding, const XML_Char* sep)
{
	// Destroy the old parser.
	Destroy();

	// If the encoding or seperator are empty, then NULL.
	if (encoding != NULL && *encoding == '\0')
		encoding = NULL;
	if (sep != NULL && *sep == '\0')
		sep = NULL;

	// Create the new one, if possible, and initialise.
	m_parser = XML_ParserCreate_MM(encoding, NULL, sep);
	if (m_parser == NULL)
		return false;
	m_pHandler->OnPostCreate();

	// Set the user data used in callbacks.
	XML_SetUserData(m_parser, (void*) m_pHandler);
	return true;
}

void ExpatParser::Destroy()
{
	if (m_parser != NULL)
		XML_ParserFree(m_parser);
	m_parser = NULL;
}

bool ExpatParser::Parse(const char* buf, size_t len, bool isFinal)
{
	assert(m_parser != NULL);
	return XML_Parse(m_parser, buf, len, isFinal) != 0;
}

bool ExpatParser::Parse(const char* buf, bool isFinal)
{
	return Parse(buf, strlen(buf), isFinal);
}

bool ExpatParser::Parse(size_t len, bool isFinal)
{
	assert(m_parser != NULL);
	return XML_ParseBuffer(m_parser, len, isFinal) != 0;
}

void* ExpatParser::GetBuffer(size_t len)
{
	assert(m_parser != NULL);
	return XML_GetBuffer(m_parser, len);
}

/******************************** Parser callback enable/disable methods **/

void ExpatParser::EnableStartElementHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetStartElementHandler(m_parser, enable ? StartElementHandler : NULL);
}

void ExpatParser::EnableEndElementHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetEndElementHandler(m_parser, enable ? EndElementHandler : NULL);
}

void ExpatParser::EnableElementHandler(bool enable)
{
	assert(m_parser != NULL);
	EnableStartElementHandler(enable);
	EnableEndElementHandler(enable);
}

void ExpatParser::EnableCharacterDataHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetCharacterDataHandler(m_parser, enable ? CharacterDataHandler : NULL);
}

void ExpatParser::EnableProcessingInstructionHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetProcessingInstructionHandler(m_parser,
	enable ? ProcessingInstructionHandler : NULL);
}

void ExpatParser::EnableCommentHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetCommentHandler(m_parser, enable ? CommentHandler : NULL);
}

void ExpatParser::EnableStartCdataSectionHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetStartCdataSectionHandler(m_parser, enable ? StartCdataSectionHandler : NULL);
}

void ExpatParser::EnableEndCdataSectionHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetEndCdataSectionHandler(m_parser, enable ? EndCdataSectionHandler : NULL);
}

void ExpatParser::EnableCdataSectionHandler(bool enable)
{
	assert(m_parser != NULL);
	EnableStartCdataSectionHandler(enable);
	EnableEndCdataSectionHandler(enable);
}

void ExpatParser::EnableDefaultHandler(bool enable, bool expand)
{
	assert(m_parser != NULL);
	if (expand)
		XML_SetDefaultHandlerExpand(m_parser, enable ? DefaultHandler : NULL);
	else
		XML_SetDefaultHandler(m_parser, enable ? DefaultHandler : NULL);
}

void ExpatParser::EnableUnknownEncodingHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetUnknownEncodingHandler(m_parser, enable ? UnknownEncodingHandler : NULL, NULL);
}

void ExpatParser::EnableStartNamespaceDeclHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetStartNamespaceDeclHandler(m_parser, enable ? StartNamespaceDeclHandler : NULL);
}

void ExpatParser::EnableEndNamespaceDeclHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetEndNamespaceDeclHandler(m_parser, enable ? EndNamespaceDeclHandler : NULL);
}

void ExpatParser::EnableNamespaceDeclHandler(bool enable)
{
	EnableStartNamespaceDeclHandler(enable);
	EnableEndNamespaceDeclHandler(enable);
}

void ExpatParser::EnableXmlDeclHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetXmlDeclHandler(m_parser, enable ? XmlDeclHandler : NULL);
}

void ExpatParser::EnableStartDoctypeDeclHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetStartDoctypeDeclHandler(m_parser, enable ? StartDoctypeDeclHandler : NULL);
}

void ExpatParser::EnableEndDoctypeDeclHandler(bool enable)
{
	assert(m_parser != NULL);
	XML_SetEndDoctypeDeclHandler(m_parser, enable ? EndDoctypeDeclHandler : NULL);
}

void ExpatParser::EnableDoctypeDeclHandler(bool enable)
{
	assert(m_parser != NULL);
	EnableStartDoctypeDeclHandler(enable);
	EnableEndDoctypeDeclHandler(enable);
}

/************************************* Parser error reporting methods **/

enum XML_Error ExpatParser::GetErrorCode(void) 
{
	assert(m_parser != NULL);
	return XML_GetErrorCode(m_parser);
}

long ExpatParser::GetCurrentByteIndex(void) 
{
	assert(m_parser != NULL);
	return XML_GetCurrentByteIndex(m_parser);
}

int ExpatParser::GetCurrentLineNumber(void) 
{
	assert(m_parser != NULL);
	return XML_GetCurrentLineNumber(m_parser);
}

int ExpatParser::GetCurrentColumnNumber(void) 
{
	assert(m_parser != NULL);
	return XML_GetCurrentColumnNumber(m_parser);
}

int ExpatParser::GetCurrentByteCount(void) 
{
	assert(m_parser != NULL);
	return XML_GetCurrentByteCount(m_parser);
}

const char* ExpatParser::GetInputContext(int32* pOffset, int32* pSize)
{
	assert(m_parser != NULL);
	return XML_GetInputContext(m_parser, pOffset, pSize);
}

const XML_LChar* ExpatParser::GetErrorString() 
{
	return XML_ErrorString(GetErrorCode());
}

/******************************************************* Parser Interface **/

const XML_LChar* ExpatParser::GetExpatVersion(void)
{
	return XML_ExpatVersion();
}

const XML_LChar* ExpatParser::GetErrorString(enum XML_Error err) 
{
	return XML_ErrorString(err);
}

