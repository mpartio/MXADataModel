SET (MXA_DATA_WRAPPERS_SRCS
  ${MXA_SOURCE_DIR}/src/DataWrappers/MXAAsciiStringData.cpp
  ${MXA_SOURCE_DIR}/src/DataWrappers/MXARGBImage.cpp
)

SET (MXA_DATA_WRAPPERS_HEADERS
  ${MXA_SOURCE_DIR}/src/DataWrappers/MXA2DArray.hpp
  ${MXA_SOURCE_DIR}/src/DataWrappers/MXAArrayTemplate.hpp
  ${MXA_SOURCE_DIR}/src/DataWrappers/MXAAsciiStringData.h
  ${MXA_SOURCE_DIR}/src/DataWrappers/MXARGBImage.h

  )
  

INSTALL (FILES ${MXA_HDF5_HEADERS}
            DESTINATION include/MXADataModel/DataWrappers
            COMPONENT Headers           
)

source_group(src\\DataWrappers FILES ${MXA_DATA_WRAPPERS_SRCS} ${MXA_DATA_WRAPPERS_HEADERS} )
