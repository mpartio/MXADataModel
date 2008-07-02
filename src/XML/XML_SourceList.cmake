SET ( MXA_XML_SRCS
    ${MXA_SOURCE_DIR}/src/XML/ExpatParser.cpp
    ${MXA_SOURCE_DIR}/src/XML/ExpatEvtHandler.cpp
    ${MXA_SOURCE_DIR}/src/XML/XMLIODelegate.cpp
    ${MXA_SOURCE_DIR}/src/XML/XMLDataModelWriter.cpp
    ${MXA_SOURCE_DIR}/src/XML/XMLDataModelReader.cpp
    ${MXA_SOURCE_DIR}/src/DataImport/DataImportXmlParser.cpp
)

SET (MXA_XML_HEADERS
    ${MXA_SOURCE_DIR}/src/XML/ExpatParser.h
    ${MXA_SOURCE_DIR}/src/XML/ExpatEvtHandler.h
    ${MXA_SOURCE_DIR}/src/XML/XMLIODelegate.h
    ${MXA_SOURCE_DIR}/src/XML/XMLDataModelWriter.h
    ${MXA_SOURCE_DIR}/src/XML/XMLDataModelReader.h
    ${MXA_SOURCE_DIR}/src/XML/XMLUserMetaDataWriter.h
    ${MXA_SOURCE_DIR}/src/DataImport/DataImportXmlParser.h
)

INSTALL (FILES ${MXA_XML_HEADERS}
            DESTINATION include/MXADataModel/XML
            COMPONENT Headers           
)

