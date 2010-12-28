IF (MXA_USE_XML)

SET ( MXA_XML_SRCS
    ${PROJECT_CODE_DIR}/MXA/XML/ExpatParser.cpp
    ${PROJECT_CODE_DIR}/MXA/XML/ExpatEvtHandler.cpp
    ${PROJECT_CODE_DIR}/MXA/XML/DataModelXMLEvtHandler.cpp
)

SET (MXA_XML_HDRS
    ${PROJECT_CODE_DIR}/MXA/XML/ExpatParser.h
    ${PROJECT_CODE_DIR}/MXA/XML/ExpatEvtHandler.h
	${PROJECT_CODE_DIR}/MXA/XML/XMLFileUtilities.hpp
    ${PROJECT_CODE_DIR}/MXA/XML/DataModelXMLEvtHandler.h
    ${PROJECT_CODE_DIR}/MXA/XML/XMLMXAAttributeWriter.hpp
    ${PROJECT_CODE_DIR}/MXA/XML/XMLUserMetaDataWriter.h
    ${PROJECT_CODE_DIR}/MXA/XML/XMLConstants.h
    ${PROJECT_CODE_DIR}/MXA/XML/XMLStreamWriterDelegate.hpp
    ${PROJECT_CODE_DIR}/MXA/XML/XMLStreamWriterDelegate.txx
    ${PROJECT_CODE_DIR}/MXA/XML/XMLStreamReaderDelegate.hpp
)

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()

cmp_IDE_SOURCE_PROPERTIES( "MXA/XML" "${MXA_XML_HDRS}" "${MXA_XML_SRCS}" ${install_files})

SET (MXA_XML_SUPPORT "1" CACHE INTERNAL "")

endif()