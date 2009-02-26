SET (MXA_UTILITIES_SRCS
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathIndexSection.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathTextSection.cpp
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathBuilder.cpp
)

SET (MXA_UTILITIES_HEADERS
  ${MXA_SOURCE_DIR}/MXA/Utilities/StringUtils.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/IStringSection.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathIndexSection.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathTextSection.h
  ${MXA_SOURCE_DIR}/MXA/Utilities/DataSourcePathBuilder.h
)



INSTALL (FILES ${MXA_UTILITIES_HEADERS}
            DESTINATION include/MXA/Utilities
            COMPONENT Headers           
)

MXA_SOURCE_PROPERTIES(Utilities "${MXA_UTILITIES_HEADERS}" "${MXA_UTILITIES_SRCS}")

