SET (MXA_Utilities_SRCS
  ${PROJECT_CODE_DIR}/MXA/Utilities/DataSourcePathIndexSection.cpp
  ${PROJECT_CODE_DIR}/MXA/Utilities/DataSourcePathTextSection.cpp
  ${PROJECT_CODE_DIR}/MXA/Utilities/DataSourcePathBuilder.cpp
 # ${PROJECT_CODE_DIR}/MXA/Utilities/MXAFileSystemPath.cpp
  ${PROJECT_CODE_DIR}/MXA/Utilities/MXADir.cpp
  ${PROJECT_CODE_DIR}/MXA/Utilities/MXAFileInfo.cpp
  ${PROJECT_CODE_DIR}/MXA/Utilities/MXALogger.cpp
)

SET (MXA_Utilities_HDRS
  ${PROJECT_CODE_DIR}/MXA/Utilities/StringUtils.h
  ${PROJECT_CODE_DIR}/MXA/Utilities/IStringSection.h
  ${PROJECT_CODE_DIR}/MXA/Utilities/DataSourcePathIndexSection.h
  ${PROJECT_CODE_DIR}/MXA/Utilities/DataSourcePathTextSection.h
  ${PROJECT_CODE_DIR}/MXA/Utilities/DataSourcePathBuilder.h
 # ${PROJECT_CODE_DIR}/MXA/Utilities/MXAFileSystemPath.h
  ${PROJECT_CODE_DIR}/MXA/Utilities/MXADir.h
  ${PROJECT_CODE_DIR}/MXA/Utilities/MXAFileInfo.h
  ${PROJECT_CODE_DIR}/MXA/Utilities/MXALogger.h
)

IF (MSVC)
    SET (MXA_Utilities_SRCS ${MXA_Utilities_SRCS} ${PROJECT_CODE_DIR}/MXA/Utilities/MXADirent.c )
    SET (MXA_Utilities_HDRS ${MXA_Utilities_HDRS} ${PROJECT_CODE_DIR}/MXA/Utilities/MXADirent.h )
endif()

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()


cmp_IDE_SOURCE_PROPERTIES( "MXA/Utilities" "${MXA_Utilities_HDRS}" "${MXA_Utilities_SRCS}" ${install_files})

