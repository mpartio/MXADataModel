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

#--- Data Model Test Program --------------------
OPTION (BUILD_MXADATAMODEL_TEST "Builds a Program to test the User meta data read/write classes" OFF)
IF ( BUILD_MXADATAMODEL_TEST )
  SET (MXADATAMODEL_TEST_SRCS
    DataModelTest.cpp
  )
  
  ADD_EXECUTABLE ( DataModelTest ${MXADATAMODEL_TEST_SRCS} )
  TARGET_LINK_LIBRARIES ( DataModelTest 
    MXADataModel
    ${Boost_LIBRARIES}
  )
ENDIF ( BUILD_MXADATAMODEL_TEST )

#---- Data Import Test Program -----------------------
OPTION (BUILD_MXADATAIMPORT_TEST "Builds a Program to test the User meta data read/write classes" OFF)
IF ( BUILD_MXADATAIMPORT_TEST )
  SET (MXADATAIMPORT_TEST_SRCS
    DataImportTest.cpp
  )

  ADD_EXECUTABLE ( DataImportTest ${MXADATAIMPORT_TEST_SRCS} )
  TARGET_LINK_LIBRARIES ( DataImportTest 
    ${MXADATAMODEL_LIB_NAME}
    ${Boost_LIBRARIES}
  )
ENDIF ( BUILD_MXADATAIMPORT_TEST )

#------------------- Build the HD5Lite Testing ---------------
IF ( BUILD_HDF5_MODULES )
  OPTION(BUILD_H5LITE_TEST "Builds a program that tests the H5Lite methods" OFF)
  IF ( BUILD_H5LITE_TEST )
    SET (H5LITE_TEST_SRCS
      H5LiteTest.cpp
      ${PROJECT_SOURCE_DIR}/../HDF5/H5Lite.cpp
    )
  
    ADD_EXECUTABLE ( H5LiteTest ${H5LITE_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( 
	H5LiteTest 
    ${HDF5_LIBRARIES}
	${Boost_LIBRARIES}
    )
  ENDIF ( BUILD_H5LITE_TEST )
ENDIF ( BUILD_HDF5_MODULES )
