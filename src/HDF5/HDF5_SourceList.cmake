SET( MXA_HDF5_SRCS
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXADataFile.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Image.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Lite.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Utilities.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelReader.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelWriter.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpIO.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpImportDelegate.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpImportDelegateFactory.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Dataset.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXARequiredMetaData.cpp
)
  
SET(MXA_HDF5_HEADERS
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXADataFile.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5AttributeWriter.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelReader.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelWriter.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Lite.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Image.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Utilities.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpIO.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpImportDelegate.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpImportDelegateFactory.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Dataset.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXARequiredMetaData.h
)


INSTALL (FILES ${MXA_HDF5_HEADERS}
            DESTINATION include/MXADataModel/HDF5
            COMPONENT Headers           
)


#-------------------------------------------------------------------------------
#  These sources depend on HDF5 and libTiff
#-------------------------------------------------------------------------------
SET(MXA_HDF5_TIFF_SRCS
    ${MXA_SOURCE_DIR}/src/HDF5/H5TiffIO.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5TiffImportDelegate.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5TiffImportDelegateFactory.cpp
    )
    
SET(MXA_HDF5_TIFF_HEADERS
    ${MXA_SOURCE_DIR}/src/HDF5/H5TiffIO.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5TiffImportDelegate.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5TiffImportDelegateFactory.h
    )
        

INSTALL (FILES ${MXA_HDF5_TIFF_HEADERS}
            DESTINATION include/MXADataModel/HDF5
            COMPONENT Headers           
)

