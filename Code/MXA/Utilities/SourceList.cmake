SET (MXA_Utilities_SRCS
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathIndexSection.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathTextSection.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/DataSourcePathBuilder.cpp
 # ${MXA_CODE_DIR}/MXA/Utilities/MXAFileSystemPath.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/MXADir.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/MXAFileInfo.cpp
  ${MXA_CODE_DIR}/MXA/Utilities/MXALogger.cpp
)

SET (MXA_Utilities_HEADERS
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
    SET (MXA_Utilities_SRCS ${MXA_Utilities_SRCS} ${MXA_CODE_DIR}/MXA/Utilities/MXADirent.c )
    SET (MXA_Utilities_HEADERS ${MXA_Utilities_HEADERS} ${MXA_CODE_DIR}/MXA/Utilities/MXADirent.h )
endif()

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()


cmp_IDE_SOURCE_PROPERTIES( "MXA/Utilities" "${MXA_Utilities_HEADERS}" "${MXA_Utilities_SRCS}" ${install_files})
