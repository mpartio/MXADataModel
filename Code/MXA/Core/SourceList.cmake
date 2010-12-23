SET (MXA_Core_SRCS
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataModel.cpp
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataDimension.cpp
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataRecord.cpp
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataSource.cpp
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataImport.cpp
  ${PROJECT_CODE_DIR}/MXA/Core/MXASupportFile.cpp
  ${PROJECT_CODE_DIR}/MXA/Core/RequiredMetaData.cpp

)

SET (MXA_Core_HEADERS
  ${PROJECT_CODE_DIR}/MXA/Core/AttributeHelpers.h
  ${PROJECT_CODE_DIR}/MXA/Core/MXAConstants.h
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataDimension.h
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataImport.h
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataModel.h
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataRecord.h
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataSource.h
  ${PROJECT_CODE_DIR}/MXA/Core/MXASupportFile.h
  ${PROJECT_CODE_DIR}/MXA/Core/RequiredMetaData.h
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataModelWriter.hpp
  ${PROJECT_CODE_DIR}/MXA/Core/MXADataModelReader.hpp
)

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()
cmp_IDE_SOURCE_PROPERTIES( "MXA/Core" "${MXA_Core_HEADERS}" "${MXA_Core_SRCS}" ${install_files})
