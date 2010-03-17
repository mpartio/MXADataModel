SET (MXA_UTILITIES_SRCS
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathIndexSection.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathTextSection.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathBuilder.cpp
 # ${MXA_SOURCE_DIR}/MXA/Utilities/MXAFileSystemPath.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXADir.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXAFileInfo.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXALogger.cpp
)

SET (MXA_UTILITIES_HEADERS
  ${MXA_SOURCE_DIR}/MXA/Utilities/StringUtils.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/IStringSection.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathIndexSection.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathTextSection.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathBuilder.h
 # ${MXA_SOURCE_DIR}/MXA/Utilities/MXAFileSystemPath.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXADir.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXAFileInfo.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXALogger.h
)

IF (MSVC)
    SET (MXA_UTILITIES_SRCS ${MXA_UTILITIES_SRCS} ${MXA_SOURCE_DIR}/MXA/Utilities/MXADirent.c )
    SET (MXA_UTILITIES_HEADERS ${MXA_UTILITIES_HEADERS} ${MXA_SOURCE_DIR}/MXA/Utilities/MXADirent.h )
endif()

if ( NOT DEFINED MXA_IGNORE_INSTALL_RULES )
INSTALL (FILES ${MXA_UTILITIES_HEADERS}
            DESTINATION include/MXA/Utilities
            COMPONENT Headers           
)
endif()
IDE_SOURCE_PROPERTIES( "MXA/Utilities" "${MXA_UTILITIES_HEADERS}" "${MXA_UTILITIES_SRCS}")

