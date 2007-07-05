PROJECT (MXATesting)

# ---------- Setup output Directories -------------------------
SET (LIBRARY_OUTPUT_PATH
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Libraries"
  )

# --------- Setup the Executable output Directory -------------
SET (EXECUTABLE_OUTPUT_PATH
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Executables."
  )

# ----------- Find the Boost stuff
SET (Boost_USE_UNIT_TEST_FRAMEWORK TRUE)
SET (Boost_USE_TEST_EXEC_MONITOR TRUE)
SET (Boost_USE_SERIALIZATION TRUE)
INCLUDE(${PROJECT_SOURCE_DIR}/../../Resources/FindBoost.cmake)
LINK_DIRECTORIES(${Boost_LIBRARY_DIR})



SET (HDF5_USE_HIGH_LEVEL FALSE)
  INCLUDE( ${PROJECT_SOURCE_DIR}/../../Resources/FindHDF.cmake )
  IF(NOT HDF5_FOUND)
    MESSAGE(FATAL_ERROR "HDF5 was not found. Set the include/lib manually or set the HDF_INSTALL environment variable")
  ENDIF(NOT HDF5_FOUND)
  # Enable the HDF5_MODULES_ON when using the HDF dynamic libraries

  IF (MXA_BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DHDF5_MODULES_ON)
      IF (WIN32)
        ADD_DEFINITIONS(-D_HDF5USEDLL_)
        ADD_DEFINITIONS(-DMXA_BUILD_SHARED_LIBS)
      ENDIF (WIN32)
  ENDIF (MXA_BUILD_SHARED_LIBS)

OPTION(BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)
IF(BUILD_VTK_HDF5)
     #-- Find VTK packages -----------------------------------------
    FIND_PACKAGE(VTK REQUIRED)
    INCLUDE(${VTK_USE_FILE})
    
    # List the kits from VTK that are needed by this project.
    SET(vtkLocal_LIBS vtkCommon vtksys vtkFiltering vtkIO vtkGraphics )

    SET (DEP_LIBS ${DEP_LIBS} ${vtkLocal_LIBS})
    SET(HDF5_VTK_BRIDGE "1")
ENDIF(BUILD_VTK_HDF5) 

IF (BUILD_TESTING)

    # -------- Data Model Test
    SET (MXADATAMODEL_TEST_SRCS
      DataModelTest.cpp
    )
    ADD_EXECUTABLE ( DataModelTest ${MXADATAMODEL_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataModelTest MXADataModel ${Boost_LIBRARIES} ${vtkLocal_LIBS})
    ADD_TEST(DataModelTest ${EXECUTABLE_OUTPUT_PATH}/DataModelTest)
    
    # ------ DataImport Test
    SET (MXADATAIMPORT_TEST_SRCS
      DataImportTest.cpp
    )
    ADD_EXECUTABLE ( DataImportTest ${MXADATAIMPORT_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataImportTest  ${MXADATAMODEL_LIB_NAME} ${Boost_LIBRARIES} ${vtkLocal_LIBS})
    ADD_TEST(DataImportTest ${EXECUTABLE_OUTPUT_PATH}/DataImportTest)
    
    
    # ------- H5Lite Test
    SET (H5LITE_TEST_SRCS
      H5LiteTest.cpp
    )
    ADD_EXECUTABLE ( H5LiteTest ${H5LITE_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (H5LiteTest MXADataModel ${HDF5_LIBRARIES} ${Boost_LIBRARIES} ${vtkLocal_LIBS})
    ADD_TEST(H5LiteTest ${EXECUTABLE_OUTPUT_PATH}/H5LiteTest)
    
    # ------- Vtk to HDF5 Bridge test
    IF(BUILD_VTK_HDF5)
        SET (VTK_HDF5_SRCS
          VtkHDF5Test.cpp
          DataFileGenerator.cpp
        )
        ADD_EXECUTABLE( vtkHDF5Test ${VTK_HDF5_SRCS} )
        TARGET_LINK_LIBRARIES (vtkHDF5Test MXADataModel  ${HDF5_LIBRARIES} ${Boost_LIBRARIES} ${vtkLocal_LIBS})
        ADD_TEST(vtkHDF5Test ${EXECUTABLE_OUTPUT_PATH}/vtkHDF5Test)
    ENDIF(BUILD_VTK_HDF5)
    
    
    SET (XML_TEST_SRCS
        XMLTest.cpp
        )
    ADD_EXECUTABLE( XMLTest ${XML_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (XMLTest MXADataModel  ${HDF5_LIBRARIES} ${Boost_LIBRARIES} )
    ADD_TEST(XMLTest ${EXECUTABLE_OUTPUT_PATH}/XMLTest)      
            
ENDIF (BUILD_TESTING)

