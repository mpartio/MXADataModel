IF (MXA_USE_XML)

SET ( MXA_XML_SRCS
    ${MXA_SOURCE_DIR}/MXA/XML/ExpatParser.cpp
    ${MXA_SOURCE_DIR}/MXA/XML/ExpatEvtHandler.cpp
    ${MXA_SOURCE_DIR}/MXA/XML/DataModelXMLEvtHandler.cpp
)

SET (MXA_XML_HDRS
    ${MXA_SOURCE_DIR}/MXA/XML/ExpatParser.h
    ${MXA_SOURCE_DIR}/MXA/XML/ExpatEvtHandler.h
	${MXA_SOURCE_DIR}/MXA/XML/XMLFileUtilities.hpp
    ${MXA_SOURCE_DIR}/MXA/XML/DataModelXMLEvtHandler.h
    ${MXA_SOURCE_DIR}/MXA/XML/XMLMXAAttributeWriter.hpp
    ${MXA_SOURCE_DIR}/MXA/XML/XMLUserMetaDataWriter.h
    ${MXA_SOURCE_DIR}/MXA/XML/XMLConstants.h
    ${MXA_SOURCE_DIR}/MXA/XML/XMLStreamWriterDelegate.hpp
    ${MXA_SOURCE_DIR}/MXA/XML/XMLStreamWriterDelegate.txx
    ${MXA_SOURCE_DIR}/MXA/XML/XMLStreamReaderDelegate.hpp
)

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()

cmp_IDE_SOURCE_PROPERTIES( "MXA/XML" "${MXA_XML_HDRS}" "${MXA_XML_SRCS}" ${install_files})

SET (MXA_XML_SUPPORT "1" CACHE INTERNAL "")

endif()