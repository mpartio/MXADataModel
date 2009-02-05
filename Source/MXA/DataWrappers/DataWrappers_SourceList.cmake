SET (MXA_DATA_WRAPPERS_SRCS
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXAAsciiStringData.cpp
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXARGBImage.cpp
)

SET (MXA_DATA_WRAPPERS_HEADERS
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXA2DArray.hpp
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXAArrayTemplate.hpp
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXAAsciiStringData.h
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXARGBImage.h
  ${MXA_SOURCE_DIR}/MXA/DataWrappers/DataExportProperties.h
  )
  
MXA_SOURCE_PROPERTIES(DataWrappers "${MXA_DATA_WRAPPERS_HEADERS}" "${MXA_DATA_WRAPPERS_SRCS}")


