SET (MXA_BASE_SRCS

)


SET (MXA_BASE_HEADERS
  ${MXA_SOURCE_DIR}/src/Base/IAttribute.h
  ${MXA_SOURCE_DIR}/src/Base/IAttributeHelper.h
  ${MXA_SOURCE_DIR}/src/Base/IAttributeWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IDataDimension.h
  ${MXA_SOURCE_DIR}/src/Base/IDataDimensionWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IDataFile.h
  ${MXA_SOURCE_DIR}/src/Base/IDataImport.h
  ${MXA_SOURCE_DIR}/src/Base/IImportDelegate.h
  ${MXA_SOURCE_DIR}/src/Base/IImportProperty.h
  ${MXA_SOURCE_DIR}/src/Base/IDataModel.h
  ${MXA_SOURCE_DIR}/src/Base/IDataModelReader.h
  ${MXA_SOURCE_DIR}/src/Base/IDataModelWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IDataRecord.h
  ${MXA_SOURCE_DIR}/src/Base/IDataRecordWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IDataSource.h
  ${MXA_SOURCE_DIR}/src/Base/IFileIODelegate.h
  ${MXA_SOURCE_DIR}/src/Base/INode.h
  ${MXA_SOURCE_DIR}/src/Base/IFileReader.h
  ${MXA_SOURCE_DIR}/src/Base/IFileWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IMXAArray.h
  ${MXA_SOURCE_DIR}/src/Base/IDataFileIO.h
  ${MXA_SOURCE_DIR}/src/Base/IDataset.h
  ${MXA_SOURCE_DIR}/src/Base/IRequiredMetaData.h
  ${MXA_SOURCE_DIR}/src/Base/ISupportFile.h
)

MXA_SOURCE_PROPERTIES(Base "${MXA_BASE_HEADERS}" "${MXA_BASE_SRCS}")

