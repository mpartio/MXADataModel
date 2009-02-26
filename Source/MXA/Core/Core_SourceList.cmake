SET (MXA_CORE_SRCS
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataDimension.cpp
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataRecord.cpp
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataModel.cpp
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataSource.cpp
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataImport.cpp
  ${MXA_SOURCE_DIR}/MXA/Core/MXASupportFile.cpp
  ${MXA_SOURCE_DIR}/MXA/Core/RequiredMetaData.cpp
)

SET (MXA_CORE_HEADERS
  ${MXA_SOURCE_DIR}/MXA/Core/AttributeHelpers.h
  ${MXA_SOURCE_DIR}/MXA/Core/MXAConstants.h
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataDimension.h
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataImport.h
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataModel.h
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataRecord.h
  ${MXA_SOURCE_DIR}/MXA/Core/MXADataSource.h
  ${MXA_SOURCE_DIR}/MXA/Core/MXASupportFile.h
  ${MXA_SOURCE_DIR}/MXA/Core/RequiredMetaData.h
)

MXA_SOURCE_PROPERTIES(Core "${MXA_CORE_HEADERS}" "${MXA_CORE_SRCS}")
