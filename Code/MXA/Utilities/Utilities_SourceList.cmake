SET (MXA_UTILITIES_SRCS
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathIndexSection.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathTextSection.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathBuilder.cpp
 # ${MXA_CODE_DIR}/MXA/Utilities/MXAFileSystemPath.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/MXADir.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/MXAFileInfo.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/MXALogger.cpp
)

SET (MXA_UTILITIES_HEADERS
  ${MXA_CODE_DIR}/MXA/Utilities/StringUtils.h
  ${MXA_CODE_DIR}/MXA/Utilities/IStringSection.h
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathIndexSection.h
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathTextSection.h
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathBuilder.h
 # ${MXA_CODE_DIR}/MXA/Utilities/MXAFileSystemPath.h
  ${MXA_CODE_DIR}/MXA/Utilities/MXADir.h
  ${MXA_CODE_DIR}/MXA/Utilities/MXAFileInfo.h
  ${MXA_CODE_DIR}/MXA/Utilities/MXALogger.h
)

IF (MSVC)
    SET (MXA_UTILITIES_SRCS ${MXA_UTILITIES_SRCS} ${MXA_CODE_DIR}/MXA/Utilities/MXADirent.c )
    SET (MXA_UTILITIES_HEADERS ${MXA_UTILITIES_HEADERS} ${MXA_CODE_DIR}/MXA/Utilities/MXADirent.h )
endif()

if ( ${MXA_INSTALL_FILES} EQUAL 1 )
    INSTALL (FILES ${MXA_UTILITIES_HEADERS}
            DESTINATION include/MXA/Utilities
            COMPONENT Headers   )
endif()
IDE_SOURCE_PROPERTIES( "MXA/Utilities" "${MXA_UTILITIES_HEADERS}" "${MXA_UTILITIES_SRCS}")

