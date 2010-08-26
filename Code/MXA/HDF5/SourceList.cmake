IF ( MXA_USE_HDF5 )
    if ( DEFINED MXA_INSTALL_FILES)
        if( ${MXA_INSTALL_FILES} EQUAL 1 )
            set (install_files "1")
        endif()
    endif()
    
    SET(MXA_HDF5_SUPPORT "1" CACHE INTERNAL "")
    
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
    cmp_IDE_SOURCE_PROPERTIES( "MXA/HDF5" "${MXA_HDF5_HEADERS}" "${MXA_HDF5_SRCS}" ${install_files})
    
    IF(MXA_USE_TIFF)
        #-------------------------------------------------------------------------------
        #  These sources depend on HDF5 and libTiff
        #-------------------------------------------------------------------------------
        SET(MXA_HDF5_SRCS
            ${MXA_HDF5_SRCS}
            ${MXA_CODE_DIR}/MXA/HDF5/H5TiffIO.cpp
            ${MXA_CODE_DIR}/MXA/HDF5/H5TiffImportDelegate.cpp
            ${MXA_CODE_DIR}/MXA/HDF5/H5TiffImportDelegateFactory.cpp
        )
            
        SET(MXA_HDF5_HEADERS
            ${MXA_HDF5_HEADERS}
            ${MXA_CODE_DIR}/MXA/HDF5/H5TiffIO.h
            ${MXA_CODE_DIR}/MXA/HDF5/H5TiffImportDelegate.h
            ${MXA_CODE_DIR}/MXA/HDF5/H5TiffImportDelegateFactory.h
        )
        cmp_IDE_SOURCE_PROPERTIES( "MXA/HDF5" "${MXA_HDF5_TIFF_HEADERS}" "${MXA_HDF5_TIFF_SRCS}" ${install_files})
        SET (MXA_TIFF_SUPPORT "1" CACHE INTERNAL "")
    endif()

    OPTION(MXA_BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)
    IF(MXA_BUILD_VTK_HDF5)
    #-- Find VTK packages -----------------------------------------
        FIND_PACKAGE(VTK REQUIRED)
        INCLUDE(${VTK_USE_FILE})
          
        # List the kits from VTK that are needed by this project.
        SET(vtkLocal_LIBS vtkCommon vtksys vtkFiltering vtkIO vtkGraphics )
        
        SET (MXA_HDF5_SRCS ${MXA_HDF5_SRCS} ${MXA_CODE_DIR}/MXA/HDF5/vtkHDF5.cpp)
        SET (MXA_HDF5_HEADERS ${MXA_HDF5_HEADERS} ${MXA_CODE_DIR}/MXA/HDF5/vtkHDF5.h)
        SET (DEP_LIBS ${DEP_LIBS} ${vtkLocal_LIBS})
        SET(HDF5_VTK_BRIDGE "1")
    ENDIF (MXA_BUILD_VTK_HDF5)
  

endif()
