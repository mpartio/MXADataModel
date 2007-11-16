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

# ---------- Find Boost Headers/Libraries -----------------------
SET (BOOST_USE_UNIT_TEST_FRAMEWORK TRUE)
SET (BOOST_USE_TEST_EXEC_MONITOR TRUE)
SET (BOOST_MXA_LIBS "")
SET (Boost_FIND_REQUIRED TRUE)
SET (Boost_FIND_QUIETLY TRUE)
INCLUDE(${PROJECT_SOURCE_DIR}/../../Resources/MXAFindBoost.cmake)
SET (BOOST_MXA_LIBS ${BOOST_MXA_LIBS} 
    ${BOOST_FILESYSTEM_LIBRARY}
    ${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${BOOST_TEST_EXEC_MONITOR_LIBRARY}
    )
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})  # Include the Boost Headers
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})

SET (HDF5_USE_HIGH_LEVEL FALSE)
INCLUDE( ${PROJECT_SOURCE_DIR}/../../Resources/MXAFindHDF5.cmake )
IF(NOT HDF5_FOUND)
  MESSAGE(FATAL_ERROR "HDF5 was not found. Set the include/lib manually or set the HDF_INSTALL environment variable")
ENDIF(NOT HDF5_FOUND)
  # Enable the HDF5_MODULES_ON when using the HDF dynamic libraries

  IF (MXA_BUILD_SHARED_LIBS)
      IF (WIN32)
        ADD_DEFINITIONS(-D_HDF5USEDLL_)
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
    TARGET_LINK_LIBRARIES ( DataModelTest MXADataModel ${BOOST_MXA_LIBS})
    ADD_TEST(DataModelTest ${EXECUTABLE_OUTPUT_PATH}/DataModelTest)
    
    # ------ DataImport Test
    SET (MXADATAIMPORT_TEST_SRCS
      DataImportTest.cpp
    )
    ADD_EXECUTABLE ( DataImportTest ${MXADATAIMPORT_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataImportTest  ${MXADATAMODEL_LIB_NAME} ${BOOST_MXA_LIBS})
    ADD_TEST(DataImportTest ${EXECUTABLE_OUTPUT_PATH}/DataImportTest)
    
    
    # ------- H5Lite Test
    SET (H5LITE_TEST_SRCS
      H5LiteTest.cpp
    )
    ADD_EXECUTABLE ( H5LiteTest ${H5LITE_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (H5LiteTest MXADataModel ${HDF5_LIBRARIES} ${BOOST_MXA_LIBS} )
    ADD_TEST(H5LiteTest ${EXECUTABLE_OUTPUT_PATH}/H5LiteTest)
    
    # --------- BMP IO Test
    SET (BMP_IO_TEST_SRCS
        BmpIOTest.cpp
    )
    ADD_EXECUTABLE(BmpIOTest ${BMP_IO_TEST_SRCS})
    TARGET_LINK_LIBRARIES (BmpIOTest MXADataModel ${HDF5_LIBRARIES} ${BOOST_MXA_LIBS})
    ADD_TEST(BmpIOTest ${EXECUTABLE_OUTPUT_PATH}/BmpIOTest)
    
    # ------- Vtk to HDF5 Bridge test
    IF(BUILD_VTK_HDF5)
        SET (VTK_HDF5_SRCS
          VtkHDF5Test.cpp
          DataFileGenerator.cpp
        )
        ADD_EXECUTABLE( vtkHDF5Test ${VTK_HDF5_SRCS} )
        TARGET_LINK_LIBRARIES (vtkHDF5Test MXADataModel  ${HDF5_LIBRARIES} ${BOOST_MXA_LIBS}  ${vtkLocal_LIBS})
        ADD_TEST(vtkHDF5Test ${EXECUTABLE_OUTPUT_PATH}/vtkHDF5Test)
    ENDIF(BUILD_VTK_HDF5)
    
    IF (MXA_USE_XML)
    SET (XML_TEST_SRCS
        XMLTest.cpp
        )
    ADD_EXECUTABLE( XMLTest ${XML_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (XMLTest MXADataModel  ${HDF5_LIBRARIES} ${BOOST_MXA_LIBS} )
    ADD_TEST(XMLTest ${EXECUTABLE_OUTPUT_PATH}/XMLTest)      
    ENDIF(MXA_USE_XML)
    
#    ADD_EXECUTABLE( UtilitiesTest UtilitiesTest.cpp )
#    TARGET_LINK_LIBRARIES(UtilitiesTest MXADataModel ${HDF5_LIBRARIES} ${BOOST_MXA_LIBS})
#    ADD_TEST(UtilitiesTest ${EXECUTABLE_OUTPUT_PATH}/UtilitiesTest)
    
    #-- Configure the XML file to correctly find the XML Model file
    SET (DATA_MODEL_XML_FILE ${MXA_SOURCE_DIR}/Resources/ModelTemplate.xml)
    CONFIGURE_FILE(${MXA_SOURCE_DIR}/Resources/DataImportTest.xml.in
                    ${CMAKE_BINARY_DIR}/DataImportTest.xml @ONLY IMMEDIATE)
                    
ENDIF (BUILD_TESTING)

