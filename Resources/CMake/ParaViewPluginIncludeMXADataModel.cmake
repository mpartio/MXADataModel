

MESSAGE (STATUS "[${MODULE_NAME}] MXADATAMODEL_TARGET_ADDED: '${MXADATAMODEL_TARGET_ADDED}'")
#IF (NOT DEFINED MXADATAMODEL_TARGET_ADDED OR NOT MXADATAMODEL_TARGET_ADDED)
    # ------ Add support for HDF5
    SET(MXA_USE_HDF5 ON)
    SET(MXA_EXTERNAL_HDF5_LIBRARY ${PARAVIEW_HDF5_LIBRARIES})
    SET(HDF5_FOUND 1)
    SET(HDF5_LIBRARIES ${PARAVIEW_HDF5_LIBRARIES})
    SET(HDF5_INCLUDE_DIRS "${ParaView_SOURCE_DIR}/Utilities/hdf5" ${ParaView_BINARY_DIR}/Utilities/hdf5)

    # ------- Add support for Expat and XML parsing
    SET (MXA_USE_XML  ON)
    SET (MXA_EXTERNAL_EXPAT_LIBRARY vtkexpat)
    SET (EXPAT_FOUND 1)
    SET (EXPAT_LIBRARIES vtkexpat)
    SET (EXPAT_INCLUDE_DIRS "${ParaView_SOURCE_DIR}/VTK/Utilities/vtkexpat")
    
    # ---- Add support for Tiff image functionality
    SET (MXA_USE_TIFF ON)
    SET (MXA_EXTERNAL_TIFF_LIBRARY vtktiff)
    SET (TIFF_FOUND 1)
    SET (TIFF_LIBRARIES vtktiff)
    SET (TIFF_INCLUDE_DIRS  ${ParaView_SOURCE_DIR}/VTK/Utilities/vtktiff
                            ${ParaView_BINARY_DIR}/VTK/Utilities/vtktiff )
    
    
    SET (MXA_BUILD_EXAMPLES FALSE)
    message (STATUS "BUILD_TESTING: ${BUILD_TESTING}")
    SET (MXA_BUILD_TESTING TRUE)
    
    SET (MXADATAMODEL_LIB_NAME MXADataModel)
    SET (MXA_USE_DEBUG_LIBRARY_NAME FALSE)
    set (MXA_BOOST_HEADERS_ONLY FALSE)
    
   # MESSAGE (STATUS "Plugin: [${MODULE_NAME}] adding MXADataModel Library")
    set (MXA_BUILD_TESTING_SAVE ${MXA_BUILD_TESTING})
    set (MXA_BUILD_TESTING ON)
    set (MXA_OUTPUT_DIRS_DEFINED 1)
    ADD_SUBDIRECTORY("${MXADataModel_SOURCE_DIR}" ${ParaView_BINARY_DIR}/MXADataModel)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})  # Include the Boost Headers
    set (MXA_BUILD_TESTING ${MXA_BUILD_TESTING_SAVE})
    SET (MXADATAMODEL_TARGET_ADDED TRUE )
    
    if (false)
    #-------------------------------------------------------------------------------
    # ---------- Find Boost Headers/Libraries -----------------------
    SET (Boost_FIND_REQUIRED TRUE)
    SET (Boost_FIND_QUIETLY TRUE)
    SET (Boost_USE_FILESYSTEM FALSE)
    SET (Boost_USE_SYSTEM FALSE)
    SET (Boost_USE_UNIT_TEST_FRAMEWORK FALSE)
    SET (Boost_USE_TEST_EXEC_MONITOR FALSE)
    SET (Boost_USE_PROGRAM_OPTIONS FALSE)
    #set (Boost_USE_MULTITHREADED TRUE)
    
    FIND_PACKAGE(Boost 1.39 )
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})  # Include the Boost Headers
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
    #- Add the required Libraries for MXA to function
    SET (PARAVIEW_ADDITIONAL_LIBRARIES 
        ${PARAVIEW_ADDITIONAL_LIBRARIES} 
        ${HDF5_LIBRARIES}
    )
    endif()
#ENDIF (NOT DEFINED MXADATAMODEL_TARGET_ADDED OR NOT MXADATAMODEL_TARGET_ADDED)

INCLUDE_DIRECTORIES (${MXADataModel_SOURCE_DIR}/Source)
INCLUDE_DIRECTORIES (${ParaView_BINARY_DIR}/MXADataModel)

