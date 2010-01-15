SET ( MXA_XML_SRCS
    ${MXA_SOURCE_DIR}/MXA/XML/ExpatParser.cpp
    ${MXA_SOURCE_DIR}/MXA/XML/ExpatEvtHandler.cpp
    ${MXA_SOURCE_DIR}/MXA/XML/DataModelXMLEvtHandler.cpp
#    ${MXA_SOURCE_DIR}/MXA/XML/XMLIODelegate.cpp
#    ${MXA_SOURCE_DIR}/MXA/XML/XMLDataModelWriter.cpp
#    ${MXA_SOURCE_DIR}/MXA/XML/XMLDataModelReader.cpp
)

SET (MXA_XML_HEADERS
    ${MXA_SOURCE_DIR}/MXA/XML/ExpatParser.h
    ${MXA_SOURCE_DIR}/MXA/XML/ExpatEvtHandler.h
#    ${MXA_SOURCE_DIR}/MXA/XML/XMLIODelegate.h
#    ${MXA_SOURCE_DIR}/MXA/XML/XMLDataModelWriter.h
#    ${MXA_SOURCE_DIR}/MXA/XML/XMLDataModelReader.h
	${MXA_SOURCE_DIR}/MXA/XML/XMLFileUtilities.hpp
    ${MXA_SOURCE_DIR}/MXA/XML/DataModelXMLEvtHandler.h
    ${MXA_SOURCE_DIR}/MXA/XML/XMLMXAAttributeWriter.hpp
    ${MXA_SOURCE_DIR}/MXA/XML/XMLUserMetaDataWriter.h
    ${MXA_SOURCE_DIR}/MXA/XML/XMLConstants.h
    ${MXA_SOURCE_DIR}/MXA/XML/XMLStreamWriterDelegate.hpp
    ${MXA_SOURCE_DIR}/MXA/XML/XMLStreamWriterDelegate.txx
    ${MXA_SOURCE_DIR}/MXA/XML/XMLStreamReaderDelegate.hpp
)



IDE_SOURCE_PROPERTIES( "MXA/XML" "${MXA_XML_HEADERS}" "${MXA_XML_SRCS}")
