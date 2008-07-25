SET (MXA_COMMON_SRCS
  ${MXA_SOURCE_DIR}/src/Common/IO/Reader64.cpp
  ${MXA_SOURCE_DIR}/src/Common/IO/Writer64.cpp
)
SET (MXA_COMMON_HEADERS
  ${MXA_SOURCE_DIR}/src/Common/LogTime.h
  ${MXA_SOURCE_DIR}/src/Common/DLLExport.h
  ${MXA_SOURCE_DIR}/src/Common/MXATypeDefs.h
  ${MXA_SOURCE_DIR}/src/Common/MXATypes.h
  ${MXA_SOURCE_DIR}/src/Common/MXAErrorDefinitions.h
  ${MXA_SOURCE_DIR}/src/Common/IO/Reader64.h
  ${MXA_SOURCE_DIR}/src/Common/IO/Writer64.h
  ${MXA_SOURCE_DIR}/src/Common/Cast/Cast.h
)

INSTALL (FILES ${MXA_COMMON_HEADERS}
            DESTINATION include/MXADataModel/Common
            COMPONENT Headers           
)

source_group(src\\Common FILES ${MXA_COMMON_HEADERS} ${MXA_COMMON_SRCS} )

