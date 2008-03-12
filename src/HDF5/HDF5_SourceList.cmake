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
    ${MXA_SOURCE_DIR}/src/HDF5/H5AsciiStringData.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5RGBImage.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5AsciiStringAttribute.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXADataset.cpp
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
    ${MXA_SOURCE_DIR}/src/HDF5/H5AttributeArrayTemplate.hpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataArrayTemplate.hpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5AsciiStringData.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5AsciiStringAttribute.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5RGBImage.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Data2DArray.hpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXADataset.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXARequiredMetaData.h
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
        
