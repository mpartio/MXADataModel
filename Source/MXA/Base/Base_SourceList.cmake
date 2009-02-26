SET (MXA_BASE_SRCS

)


SET (MXA_BASE_HEADERS
  ${MXA_SOURCE_DIR}/MXA/Base/IAttribute.h
  ${MXA_SOURCE_DIR}/MXA/Base/IAttributeHelper.h
  ${MXA_SOURCE_DIR}/MXA/Base/IAttributeWriter.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataDimension.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataDimensionWriter.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataFile.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataImport.h
  ${MXA_SOURCE_DIR}/MXA/Base/IImportDelegate.h
  ${MXA_SOURCE_DIR}/MXA/Base/IImportProperty.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataModel.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataModelReader.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataModelWriter.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataRecord.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataRecordWriter.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataSource.h
  ${MXA_SOURCE_DIR}/MXA/Base/IFileIODelegate.h
  ${MXA_SOURCE_DIR}/MXA/Base/INode.h
  ${MXA_SOURCE_DIR}/MXA/Base/IFileReader.h
  ${MXA_SOURCE_DIR}/MXA/Base/IFileWriter.h
  ${MXA_SOURCE_DIR}/MXA/Base/IMXAArray.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataFileIO.h
  ${MXA_SOURCE_DIR}/MXA/Base/IDataset.h
  ${MXA_SOURCE_DIR}/MXA/Base/IRequiredMetaData.h
  ${MXA_SOURCE_DIR}/MXA/Base/ISupportFile.h
  ${MXA_SOURCE_DIR}/MXA/Base/ITiffTagExtractor.h
)

MXA_SOURCE_PROPERTIES(Base "${MXA_BASE_HEADERS}" "${MXA_BASE_SRCS}")

