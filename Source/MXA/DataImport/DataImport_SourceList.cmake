SET (MXA_DATA_IMPORT_SRCS
    ${MXA_SOURCE_DIR}/MXA/DataImport/ImportDelegateManager.cpp
)

SET (MXA_DATA_IMPORT_HEADERS
  ${MXA_SOURCE_DIR}/MXA/DataImport/AbstractImportDelegateFactory.h
  ${MXA_SOURCE_DIR}/MXA/DataImport/ImportDelegateManager.h
)

IF (MXA_USE_HDF5 AND MXA_USE_XML)
SET ( MXA_DATA_IMPORT_SRCS 
    ${MXA_DATA_IMPORT_SRCS}
    ${MXA_SOURCE_DIR}/MXA/DataImport/DataImportXmlParser.cpp
)

SET (MXA_DATA_IMPORT_HEADERS
    ${MXA_DATA_IMPORT_HEADERS}
    ${MXA_SOURCE_DIR}/MXA/DataImport/DataImportXmlParser.h
)

ENDIF (MXA_USE_HDF5 AND MXA_USE_XML)

MXA_SOURCE_PROPERTIES(DataImport "${MXA_DATA_IMPORT_HEADERS}" "${MXA_DATA_IMPORT_SRCS}")

