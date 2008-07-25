SET (MXA_CORE_SRCS
  ${MXA_SOURCE_DIR}/src/Core/MXADataDimension.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataRecord.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataModel.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataSource.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataImport.cpp

)

SET (MXA_CORE_HEADERS
  ${MXA_SOURCE_DIR}/src/Core/AttributeHelpers.h
  ${MXA_SOURCE_DIR}/src/Core/MXAConstants.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataDimension.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataImport.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataModel.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataRecord.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataSource.h

)


INSTALL (FILES ${MXA_CORE_HEADERS}
            DESTINATION include/MXADataModel/Core
            COMPONENT Headers           
)

source_group(src\\Core FILES ${MXA_CORE_HEADERS} ${MXA_CORE_SRCS} )
