SET (MXA_Utilities_SRCS
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathIndexSection.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathTextSection.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathBuilder.cpp
 # ${MXA_SOURCE_DIR}/MXA/Utilities/MXAFileSystemPath.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXADir.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXAFileInfo.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/MXALogger.cpp
)

SET (MXA_Utilities_HDRS
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
    SET (MXA_Utilities_SRCS ${MXA_Utilities_SRCS} ${MXA_SOURCE_DIR}/MXA/Utilities/MXADirent.c )
    SET (MXA_Utilities_HDRS ${MXA_Utilities_HDRS} ${MXA_SOURCE_DIR}/MXA/Utilities/MXADirent.h )
endif()

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()


cmp_IDE_SOURCE_PROPERTIES( "MXA/Utilities" "${MXA_Utilities_HDRS}" "${MXA_Utilities_SRCS}" ${install_files})

