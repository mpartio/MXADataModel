SET (MXA_Core_SRCS
  ${MXA_CODE_DIR}/MXA/Core/MXADataModel.cpp
  ${MXA_CODE_DIR}/MXA/Core/MXADataDimension.cpp
  ${MXA_CODE_DIR}/MXA/Core/MXADataRecord.cpp
  ${MXA_CODE_DIR}/MXA/Core/MXADataSource.cpp
  ${MXA_CODE_DIR}/MXA/Core/MXADataImport.cpp
  ${MXA_CODE_DIR}/MXA/Core/MXASupportFile.cpp
  ${MXA_CODE_DIR}/MXA/Core/RequiredMetaData.cpp

)

SET (MXA_Core_HEADERS
  ${MXA_CODE_DIR}/MXA/Core/AttributeHelpers.h
  ${MXA_CODE_DIR}/MXA/Core/MXAConstants.h
  ${MXA_CODE_DIR}/MXA/Core/MXADataDimension.h
  ${MXA_CODE_DIR}/MXA/Core/MXADataImport.h
  ${MXA_CODE_DIR}/MXA/Core/MXADataModel.h
  ${MXA_CODE_DIR}/MXA/Core/MXADataRecord.h
  ${MXA_CODE_DIR}/MXA/Core/MXADataSource.h
  ${MXA_CODE_DIR}/MXA/Core/MXASupportFile.h
  ${MXA_CODE_DIR}/MXA/Core/RequiredMetaData.h
  ${MXA_CODE_DIR}/MXA/Core/MXADataModelWriter.hpp
  ${MXA_CODE_DIR}/MXA/Core/MXADataModelReader.hpp
)

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()
cmp_IDE_SOURCE_PROPERTIES( "MXA/Core" "${MXA_Core_HEADERS}" "${MXA_Core_SRCS}" ${install_files})
