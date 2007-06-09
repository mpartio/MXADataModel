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
INCLUDE(${PROJECT_SOURCE_DIR}/../../CMake/FindBoost.cmake)

SET (HDF5_USE_HIGH_LEVEL FALSE)
  INCLUDE( ${PROJECT_SOURCE_DIR}/../../CMake/FindHDF.cmake )
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

IF (BUILD_TESTING)

    # -------- Data Model Test
    SET (MXADATAMODEL_TEST_SRCS
      DataModelTest.cpp
    )
    ADD_EXECUTABLE ( DataModelTest ${MXADATAMODEL_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataModelTest MXADataModel ${Boost_LIBRARIES})
    ADD_TEST(DataModelTest ${EXECUTABLE_OUTPUT_PATH}/DataModelTest)
    
    # ------ DataImport Test
    SET (MXADATAIMPORT_TEST_SRCS
      DataImportTest.cpp
    )
    ADD_EXECUTABLE ( DataImportTest ${MXADATAIMPORT_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataImportTest  ${MXADATAMODEL_LIB_NAME} ${Boost_LIBRARIES})
    ADD_TEST(DataImportTest ${EXECUTABLE_OUTPUT_PATH}/DataImportTest)
    
    
    # ------- H5Lite Test
    SET (H5LITE_TEST_SRCS
      H5LiteTest.cpp
    )
    ADD_EXECUTABLE ( H5LiteTest ${H5LITE_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (H5LiteTest MXADataModel ${HDF5_LIBRARIES} ${Boost_LIBRARIES})
    ADD_TEST(H5LiteTest ${EXECUTABLE_OUTPUT_PATH}/H5LiteTest)

ENDIF (BUILD_TESTING)

