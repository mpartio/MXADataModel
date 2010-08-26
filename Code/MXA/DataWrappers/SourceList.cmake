SET (MXA_DataWrappers_SRCS
  ${MXA_CODE_DIR}/MXA/DataWrappers/MXAAsciiStringData.cpp
  ${MXA_CODE_DIR}/MXA/DataWrappers/MXARGBImage.cpp
)

SET (MXA_DataWrappers_HEADERS
  ${MXA_CODE_DIR}/MXA/DataWrappers/MXA2DArray.hpp
  ${MXA_CODE_DIR}/MXA/DataWrappers/MXAArrayTemplate.hpp
  ${MXA_CODE_DIR}/MXA/DataWrappers/MXAAsciiStringData.h
  ${MXA_CODE_DIR}/MXA/DataWrappers/MXARGBImage.h
  ${MXA_CODE_DIR}/MXA/DataWrappers/DataExportProperties.h
  )
  
IDE_SOURCE_PROPERTIES( "MXA/DataWrappers" "${MXA_DataWrappers_HEADERS}" "${MXA_DataWrappers_SRCS}")


