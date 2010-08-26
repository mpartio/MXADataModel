SET (MXA_DataImport_SRCS
    ${MXA_CODE_DIR}/MXA/DataImport/ImportDelegateManager.cpp
)

SET (MXA_DataImport_HEADERS
  ${MXA_CODE_DIR}/MXA/DataImport/AbstractImportDelegateFactory.h
  ${MXA_CODE_DIR}/MXA/DataImport/ImportDelegateManager.h
)

IF (MXA_USE_HDF5 AND MXA_USE_XML)
SET ( MXA_DataImport_SRCS 
    ${MXA_DataImport_SRCS}
    ${MXA_CODE_DIR}/MXA/DataImport/DataImportXmlParser.cpp
)

SET (MXA_DataImport_HEADERS
    ${MXA_DataImport_HEADERS}
    ${MXA_CODE_DIR}/MXA/DataImport/DataImportXmlParser.h
)

ENDIF (MXA_USE_HDF5 AND MXA_USE_XML)

IDE_SOURCE_PROPERTIES( "MXA/DataImport" "${MXA_DataImport_HEADERS}" "${MXA_DataImport_SRCS}")

