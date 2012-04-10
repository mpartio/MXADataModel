SET (MXA_DataWrappers_SRCS
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXAAsciiStringData.cpp
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXARGBImage.cpp
)

SET (MXA_DataWrappers_HDRS
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXA2DArray.hpp
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXAArrayTemplate.hpp
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXAAsciiStringData.h
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXARGBImage.h
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/DataExportProperties.h
  )
  
if ( DEFINED MXA_INSTALL_FILES)
if( ${MXA_INSTALL_FILES} EQUAL 1 )
    set (install_files "1")
endif()
endif()
cmp_IDE_SOURCE_PROPERTIES( "MXA/DataWrappers" "${MXA_DataWrappers_HDRS}" "${MXA_DataWrappers_SRCS}" ${install_files})
