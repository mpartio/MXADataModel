SET (MXA_UTILITIES_SRCS
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathIndexSection.cpp
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathTextSection.cpp
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathBuilder.cpp
)

SET (MXA_UTILITIES_HEADERS
  ${MXA_SOURCE_DIR}/src/Utilities/StringUtils.h
  ${MXA_SOURCE_DIR}/src/Utilities/IStringSection.h
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathIndexSection.h
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathTextSection.h
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathBuilder.h
)



INSTALL (FILES ${MXA_UTILITIES_HEADERS}
            DESTINATION include/MXADataModel/Utilities
            COMPONENT Headers           
)

source_group(src\\Utilities FILES ${MXA_UTILITIES_SRCS} ${MXA_UTILITIES_HEADERS} )
