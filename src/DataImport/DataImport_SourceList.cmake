SET (MXA_DATA_IMPORT_SRCS
    ${MXA_SOURCE_DIR}/src/DataImport/ImportDelegateManager.cpp
)

SET (MXA_DATA_IMPORT_HEADERS
  ${MXA_SOURCE_DIR}/src/DataImport/AbstractImportDelegateFactory.h
  ${MXA_SOURCE_DIR}/src/DataImport/ImportDelegateManager.h
)


INSTALL (FILES ${MXA_DATA_IMPORT_HEADERS}
            DESTINATION include/MXADataModel/DataImport
            COMPONENT Headers           
)



