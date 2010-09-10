IF ( MXA_USE_HDF5 )
    if ( DEFINED MXA_INSTALL_FILES)
        if( ${MXA_INSTALL_FILES} EQUAL 1 )
            set (install_files "1")
        endif()
    endif()
    
    SET(MXA_HDF5_SUPPORT "1" CACHE INTERNAL "")
    
    SET( MXA_HDF5_SRCS
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5MXADataFile.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Image.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Lite.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Utilities.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5ReaderDelegate.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5WriterDelegate.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5BmpIO.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5BmpImportDelegate.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5BmpImportDelegateFactory.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Dataset.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5MXARequiredMetaData.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5MXAUtilities.cpp
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Attribute.cpp
    )
      
    SET(MXA_HDF5_HEADERS
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5MXADataFile.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5AttributeWriter.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5ReaderDelegate.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5WriterDelegate.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Lite.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Image.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Utilities.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5BmpIO.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5BmpImportDelegate.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5BmpImportDelegateFactory.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Dataset.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5MXARequiredMetaData.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5MXAUtilities.h
        ${PROJECT_CODE_DIR}/MXA/HDF5/H5Attribute.h
    )
    
    
    IF(MXA_USE_TIFF)
        #-------------------------------------------------------------------------------
        #  These sources depend on HDF5 and libTiff
        #-------------------------------------------------------------------------------
        SET(MXA_HDF5_SRCS
            ${MXA_HDF5_SRCS}
            ${PROJECT_CODE_DIR}/MXA/HDF5/H5TiffIO.cpp
            ${PROJECT_CODE_DIR}/MXA/HDF5/H5TiffImportDelegate.cpp
            ${PROJECT_CODE_DIR}/MXA/HDF5/H5TiffImportDelegateFactory.cpp
        )
            
        SET(MXA_HDF5_HEADERS
            ${MXA_HDF5_HEADERS}
            ${PROJECT_CODE_DIR}/MXA/HDF5/H5TiffIO.h
            ${PROJECT_CODE_DIR}/MXA/HDF5/H5TiffImportDelegate.h
            ${PROJECT_CODE_DIR}/MXA/HDF5/H5TiffImportDelegateFactory.h
        )
        SET (MXA_TIFF_SUPPORT "1" CACHE INTERNAL "")
    endif()

    OPTION(MXA_BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)
    IF(MXA_BUILD_VTK_HDF5)
    #-- Find VTK packages -----------------------------------------
        FIND_PACKAGE(VTK REQUIRED)
        INCLUDE(${VTK_USE_FILE})
          
        # List the kits from VTK that are needed by this project.
        SET(vtkLocal_LIBS vtkCommon vtksys vtkFiltering vtkIO vtkGraphics )
        
        SET (MXA_HDF5_SRCS ${MXA_HDF5_SRCS} ${PROJECT_CODE_DIR}/MXA/HDF5/vtkHDF5.cpp)
        SET (MXA_HDF5_HEADERS ${MXA_HDF5_HEADERS} ${PROJECT_CODE_DIR}/MXA/HDF5/vtkHDF5.h)
        SET (DEP_LIBS ${DEP_LIBS} ${vtkLocal_LIBS})
        SET(HDF5_VTK_BRIDGE "1")
    ENDIF (MXA_BUILD_VTK_HDF5)
  
    cmp_IDE_SOURCE_PROPERTIES( "MXA/HDF5" "${MXA_HDF5_HEADERS}" "${MXA_HDF5_SRCS}" ${install_files})
endif()
