SET( MXA_HDF5_SRCS
    ${MXA_CODE_DIR}/MXA/HDF5/H5MXADataFile.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5Image.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5Lite.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5Utilities.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5ReaderDelegate.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5WriterDelegate.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5BmpIO.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5BmpImportDelegate.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5BmpImportDelegateFactory.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5Dataset.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5MXARequiredMetaData.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5MXAUtilities.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5Attribute.cpp
)
  
SET(MXA_HDF5_HEADERS
    ${MXA_CODE_DIR}/MXA/HDF5/H5MXADataFile.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5AttributeWriter.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5ReaderDelegate.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5WriterDelegate.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5Lite.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5Image.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5Utilities.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5BmpIO.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5BmpImportDelegate.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5BmpImportDelegateFactory.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5Dataset.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5MXARequiredMetaData.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5MXAUtilities.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5Attribute.h
)

IDE_SOURCE_PROPERTIES( "MXA/HDF5" "${MXA_HDF5_HEADERS}" "${MXA_HDF5_SRCS}")


#-------------------------------------------------------------------------------
#  These sources depend on HDF5 and libTiff
#-------------------------------------------------------------------------------
SET(MXA_HDF5_TIFF_SRCS
    ${MXA_CODE_DIR}/MXA/HDF5/H5TiffIO.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5TiffImportDelegate.cpp
    ${MXA_CODE_DIR}/MXA/HDF5/H5TiffImportDelegateFactory.cpp
    )
    
SET(MXA_HDF5_TIFF_HEADERS
    ${MXA_CODE_DIR}/MXA/HDF5/H5TiffIO.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5TiffImportDelegate.h
    ${MXA_CODE_DIR}/MXA/HDF5/H5TiffImportDelegateFactory.h
    )
        

IDE_SOURCE_PROPERTIES( "MXA/HDF5" "${MXA_HDF5_TIFF_HEADERS}" "${MXA_HDF5_TIFF_SRCS}")

