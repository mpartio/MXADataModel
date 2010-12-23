SET (MXA_DataWrappers_SRCS
  ${PROJECT_CODE_DIR}/MXA/DataWrappers/MXAAsciiStringData.cpp
  ${PROJECT_CODE_DIR}/MXA/DataWrappers/MXARGBImage.cpp
)

SET (MXA_DataWrappers_HEADERS
  ${PROJECT_CODE_DIR}/MXA/DataWrappers/MXA2DArray.hpp
  ${PROJECT_CODE_DIR}/MXA/DataWrappers/MXAArrayTemplate.hpp
  ${PROJECT_CODE_DIR}/MXA/DataWrappers/MXAAsciiStringData.h
  ${PROJECT_CODE_DIR}/MXA/DataWrappers/MXARGBImage.h
  ${PROJECT_CODE_DIR}/MXA/DataWrappers/DataExportProperties.h
  )
  
if ( DEFINED MXA_INSTALL_FILES)
if( ${MXA_INSTALL_FILES} EQUAL 1 )
    set (install_files "1")
endif()
endif()
cmp_IDE_SOURCE_PROPERTIES( "MXA/DataWrappers" "${MXA_DataWrappers_HEADERS}" "${MXA_DataWrappers_SRCS}" ${install_files})
