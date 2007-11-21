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
SET (BOOST_MXA_LIBS
    debug ${BOOST_FILESYSTEM_DEBUG_LIBRARY}
    debug ${BOOST_UNIT_TEST_FRAMEWORK_DEBUG_LIBRARY}
    debug ${BOOST_TEST_EXEC_MONITOR_DEBUG_LIBRARY}
    optimized ${BOOST_FILESYSTEM_RELEASE_LIBRARY}
    optimized ${BOOST_UNIT_TEST_FRAMEWORK_RELEASE_LIBRARY}
    optimized ${BOOST_TEST_EXEC_MONITOR_RELEASE_LIBRARY}
    )
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})  # Include the Boost Headers
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})

OPTION(BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)


IF (BUILD_TESTING)

    # -------- Data Model Test
    SET (MXADATAMODEL_TEST_SRCS
      DataModelTest.cpp
    )
    ADD_EXECUTABLE ( DataModelTest ${MXADATAMODEL_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataModelTest  ${MXADATAMODEL_LIB_NAME} ${BOOST_MXA_LIBS} ${DEP_LIBS} )
    ADD_TEST(DataModelTest ${EXECUTABLE_OUTPUT_PATH}/DataModelTest)
    
    # ------ DataImport Test
    SET (MXADATAIMPORT_TEST_SRCS
      DataImportTest.cpp
    )
    ADD_EXECUTABLE ( DataImportTest ${MXADATAIMPORT_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataImportTest ${MXADATAMODEL_LIB_NAME} ${BOOST_MXA_LIBS} ${DEP_LIBS} )
    ADD_TEST(DataImportTest ${EXECUTABLE_OUTPUT_PATH}/DataImportTest)
    #-- Configure the XML file to correctly find the XML Model file
    SET (DATA_MODEL_XML_FILE ${MXA_SOURCE_DIR}/Resources/ModelTemplate.xml)
    CONFIGURE_FILE(${MXA_SOURCE_DIR}/Resources/DataImportTest.xml.in
                    ${CMAKE_BINARY_DIR}/DataImportTest.xml @ONLY IMMEDIATE)
    
    # ------- H5Lite Test
    SET (H5LITE_TEST_SRCS
      H5LiteTest.cpp
    )
    ADD_EXECUTABLE ( H5LiteTest ${H5LITE_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (H5LiteTest ${MXADATAMODEL_LIB_NAME} ${BOOST_MXA_LIBS} ${DEP_LIBS} )
    ADD_TEST(H5LiteTest ${EXECUTABLE_OUTPUT_PATH}/H5LiteTest)
    
    # --------- BMP IO Test
    SET (BMP_IO_TEST_SRCS
        BmpIOTest.cpp
    )
    ADD_EXECUTABLE(BmpIOTest ${BMP_IO_TEST_SRCS})
    TARGET_LINK_LIBRARIES (BmpIOTest  ${MXADATAMODEL_LIB_NAME} ${BOOST_MXA_LIBS} ${DEP_LIBS} )
    ADD_TEST(BmpIOTest ${EXECUTABLE_OUTPUT_PATH}/BmpIOTest)
   
    IF (MXA_USE_XML)
    SET (XML_TEST_SRCS
        XMLTest.cpp
        )
    ADD_EXECUTABLE( XMLTest ${XML_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (XMLTest  ${MXADATAMODEL_LIB_NAME} ${BOOST_MXA_LIBS} ${DEP_LIBS} )
    ADD_TEST(XMLTest ${EXECUTABLE_OUTPUT_PATH}/XMLTest)      
    ENDIF(MXA_USE_XML)
    
#    ADD_EXECUTABLE( UtilitiesTest UtilitiesTest.cpp )
#    TARGET_LINK_LIBRARIES(UtilitiesTest MXADataModel ${HDF5_LIBRARIES} ${BOOST_MXA_LIBS})
#    ADD_TEST(UtilitiesTest ${EXECUTABLE_OUTPUT_PATH}/UtilitiesTest)

    # ------- Vtk to HDF5 Bridge test
    IF(BUILD_VTK_HDF5)
        SET (VTK_HDF5_SRCS
          VtkHDF5Test.cpp
          DataFileGenerator.cpp
        )
        ADD_EXECUTABLE( vtkHDF5Test ${VTK_HDF5_SRCS} )
        TARGET_LINK_LIBRARIES (vtkHDF5Test MXADataModel  
                ${MXADATAMODEL_LIB_NAME} 
                ${BOOST_MXA_LIBS} ${DEP_LIBS}   
                ${vtkLocal_LIBS}
        )
        ADD_TEST(vtkHDF5Test ${EXECUTABLE_OUTPUT_PATH}/vtkHDF5Test)
    ENDIF(BUILD_VTK_HDF5)
                   
ENDIF (BUILD_TESTING)

