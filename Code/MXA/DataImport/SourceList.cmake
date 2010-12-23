SET (MXA_DataImport_SRCS
    ${PROJECT_CODE_DIR}/MXA/DataImport/ImportDelegateManager.cpp
)

SET (MXA_DataImport_HEADERS
  ${PROJECT_CODE_DIR}/MXA/DataImport/AbstractImportDelegateFactory.h
  ${PROJECT_CODE_DIR}/MXA/DataImport/ImportDelegateManager.h
)

IF (MXA_USE_HDF5 AND MXA_USE_XML)
SET ( MXA_DataImport_SRCS 
    ${MXA_DataImport_SRCS}
    ${PROJECT_CODE_DIR}/MXA/DataImport/DataImportXmlParser.cpp
)

SET (MXA_DataImport_HEADERS
    ${MXA_DataImport_HEADERS}
    ${PROJECT_CODE_DIR}/MXA/DataImport/DataImportXmlParser.h
)

ENDIF (MXA_USE_HDF5 AND MXA_USE_XML)

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()
cmp_IDE_SOURCE_PROPERTIES( "MXA/DataImport" "${MXA_DataImport_HEADERS}" "${MXA_DataImport_SRCS}" ${install_files})

