SET (MXA_Base_SRCS

)


SET (MXA_Base_HEADERS
  ${PROJECT_CODE_DIR}/MXA/Base/IAttribute.h
  ${PROJECT_CODE_DIR}/MXA/Base/IAttributeHelper.h
  ${PROJECT_CODE_DIR}/MXA/Base/IAttributeWriter.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataDimension.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataDimensionWriter.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataFile.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataImport.h
  ${PROJECT_CODE_DIR}/MXA/Base/IImportDelegate.h
  ${PROJECT_CODE_DIR}/MXA/Base/IImportProperty.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataModel.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataModelReader.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataModelWriter.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataRecord.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataRecordWriter.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataSource.h
  ${PROJECT_CODE_DIR}/MXA/Base/IFileIODelegate.h
  ${PROJECT_CODE_DIR}/MXA/Base/INode.h
  ${PROJECT_CODE_DIR}/MXA/Base/IFileReader.h
  ${PROJECT_CODE_DIR}/MXA/Base/IFileWriter.h
  ${PROJECT_CODE_DIR}/MXA/Base/IMXAArray.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataFileIO.h
  ${PROJECT_CODE_DIR}/MXA/Base/IDataset.h
  ${PROJECT_CODE_DIR}/MXA/Base/IRequiredMetaData.h
  ${PROJECT_CODE_DIR}/MXA/Base/ISupportFile.h
  ${PROJECT_CODE_DIR}/MXA/Base/ITiffTagExtractor.h
  ${PROJECT_CODE_DIR}/MXA/Base/IMXADataModelWriterDelegate.h
  ${PROJECT_CODE_DIR}/MXA/Base/IMXADataModelReaderDelegate.h
)

if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()
cmp_IDE_SOURCE_PROPERTIES( "MXA/Base" "${MXA_Base_HEADERS}" "${MXA_Base_SRCS}" ${install_files})
