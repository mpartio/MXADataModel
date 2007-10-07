/*                                       vim:set ts=4 sw=4 noai sr sta et cin:
 * ExpatParser.h
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

#ifndef XMLPARSER_H_INCLUDED
#define XMLPARSER_H_INCLUDED

#include <expat.h>
#include <XML/ExpatEvtHandler.h>

/**
* @class ExpatParser ExpatParser.h src/XML/ExpatParser.h
* @brief A C++ wrapper around the Expat parser library.
* @date 2004
* @version $Revision: 1.4 $
*/
class MXA_EXPORT ExpatParser
{
private:

	XML_Parser     m_parser;
	ExpatEvtHandler* m_pHandler;

public:

	ExpatParser(ExpatEvtHandler* pHandler);

	~ExpatParser();

	/**
	 * Create a new parser.
	 */
	bool Create(const XML_Char* encoding = NULL, const XML_Char* sep = NULL);

	/**
	 * Destroy the current parser, if any.
	 */
	void Destroy(void);

	/**
	 * Parse the data pointed to by buf.
	 */
	bool Parse(const char* buf, size_t len, bool isFinal = true);
	bool Parse(const char* buf, bool isFinal = true);

	/**
	 * Parse internal buffer.
	 */
	bool Parse(size_t len, bool isFinal = true);

	/**
	 * Get the internal buffer.
	 */
	void* GetBuffer(size_t len);

	/**************************** Parser callback enable/disable methods **/

	/**
	 * Enable/Disable the start element handler.
	 */
	void EnableStartElementHandler(bool enable = true);

	/**
	 * Enable/Disable the end element handler.
	 */
	void EnableEndElementHandler(bool enable = true);

	/**
	 * Enable/Disable the element handlers.
	 */
	void EnableElementHandler(bool enable = true);

	/**
	 * Enable/Disable the character data handler.
	 */
	void EnableCharacterDataHandler(bool enable = true);

	/**
	 * Enable/Disable the processing instruction handler.
	 */
	void EnableProcessingInstructionHandler(bool enable = true);

	/**
	 * Enable/Disable the comment handler.
	 */
	void EnableCommentHandler(bool enable = true);

	/**
	 * Enable/Disable the start CDATA section handler
	 */
	void EnableStartCdataSectionHandler(bool enable = true);

	/**
	 * Enable/Disable the end CDATA section handler.
	 */
	void EnableEndCdataSectionHandler(bool enable = true);

	/**
	 * Enable/Disable the CDATA section handlers.
	 */
	void EnableCdataSectionHandler(bool enable = true);

	/**
	 * Enable/Disable default handler.
	 */
	void EnableDefaultHandler(bool enable = true, bool expand = true);

	/**
	 * Enable/Disable unknown encoding handler.
	 */
	void EnableUnknownEncodingHandler(bool enable = true);

	/**
	 * Enable/Disable start namespace handler.
	 */
	void EnableStartNamespaceDeclHandler(bool enable = true);

	/**
	 * Enable/Disable end namespace handler.
	 */
	void EnableEndNamespaceDeclHandler(bool enable = true);

	/**
	 * Enable/Disable namespace handlers.
	 */
	void EnableNamespaceDeclHandler(bool enable = true);

	/**
	 * Enable/Disable the XML declaration handler.
	 */
	void EnableXmlDeclHandler(bool enable = true);

	/**
	 * Enable/Disable the start DOCTYPE declaration handler.
	 */
	void EnableStartDoctypeDeclHandler(bool enable = true);

	/**
	 * Enable/Disable the end DOCTYPE declaration handler.
	 */
	void EnableEndDoctypeDeclHandler(bool enable = true);

	/**
	 * Enable/Disable the DOCTYPE declaration handler.
	 */
	void EnableDoctypeDeclHandler(bool enable = true);

	/************************************* Parser error reporting methods **/

	enum XML_Error GetErrorCode(void);

	long GetCurrentByteIndex(void);
	int  GetCurrentLineNumber(void);
	int  GetCurrentColumnNumber(void);
	int  GetCurrentByteCount(void);

	const char* GetInputContext(int* pOffset, int* pSize);
	const XML_LChar* GetErrorString();

	/*************************************************** Parser Interface **/

	static const XML_LChar* GetExpatVersion(void);
	static const XML_LChar* GetErrorString(enum XML_Error err);
};

#endif /* XMLPARSER_H_INCLUDED */
