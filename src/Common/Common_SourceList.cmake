SET (MXA_COMMON_SRCS
  ${MXA_SOURCE_DIR}/src/Common/IO/Reader64.cpp
  ${MXA_SOURCE_DIR}/src/Common/IO/Writer64.cpp
)
SET (MXA_COMMON_HEADERS
  ${MXA_SOURCE_DIR}/src/Common/LogTime.h
  ${MXA_SOURCE_DIR}/src/Common/DLLExport.h
  ${MXA_SOURCE_DIR}/src/Common/MXAEndian.h
  ${MXA_SOURCE_DIR}/src/Common/MXATypeDefs.h
  ${MXA_SOURCE_DIR}/src/Common/MXATypes.h
  ${MXA_SOURCE_DIR}/src/Common/MXAErrorDefinitions.h
  ${MXA_SOURCE_DIR}/src/Common/IO/Reader64.h
  ${MXA_SOURCE_DIR}/src/Common/IO/Writer64.h
  ${MXA_SOURCE_DIR}/src/Common/Cast/Cast.h
)

if (MSVC)
    INSTALL (FILES   ${MXA_SOURCE_DIR}/src/Common/Win32Defines.h
             DESTINATION include/MXADataModel/Common
             COMPONENT Headers           
    )
endif(MSVC)

MXA_SOURCE_PROPERTIES(Common "${MXA_COMMON_HEADERS}" "${MXA_COMMON_SRCS}")

INSTALL (FILES   ${MXA_SOURCE_DIR}/src/Common/IO/Reader64.h
                 ${MXA_SOURCE_DIR}/src/Common/IO/Writer64.h
         DESTINATION include/MXADataModel/Common/IO
         COMPONENT Headers           
)

INSTALL (FILES   ${MXA_SOURCE_DIR}/src/Common/Cast/Cast.h
         DESTINATION include/MXADataModel/Common/Cast
         COMPONENT Headers           
)

