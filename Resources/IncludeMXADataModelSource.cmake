IF (NOT MXA_RESOURCES_DIR)
  GET_FILENAME_COMPONENT(MXA_RESOURCES_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDIF (NOT MXA_RESOURCES_DIR)

# ---------- Find Boost Headers/Libraries -----------------------
SET (Boost_FIND_REQUIRED TRUE)
SET (Boost_FIND_QUIETLY TRUE)
SET (BOOST_USE_FILESYSTEM TRUE)
INCLUDE (${MXA_RESOURCES_DIR}/MXAFindBoost.cmake)
IF(NOT BOOST_FOUND)
  MESSAGE(FATAL_ERROR "MXAFindBoost: Boost was not found. ${Boost_DIR_MESSAGE}")
ENDIF(NOT BOOST_FOUND)
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})  # Include the Boost Headers
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})



SET( MXADATAMODEL_SRCS
    ${MXA_SOURCE_DIR}/src/Common/IO/Reader64.cpp
    ${MXA_SOURCE_DIR}/src/Common/IO/Writer64.cpp
    ${MXA_SOURCE_DIR}/src/Core/MXADataDimension.cpp
    ${MXA_SOURCE_DIR}/src/Core/MXADataRecord.cpp
    ${MXA_SOURCE_DIR}/src/Core/MXADataModel.cpp
    ${MXA_SOURCE_DIR}/src/Core/MXADataSource.cpp
    ${MXA_SOURCE_DIR}/src/Core/MXADataImport.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Image.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Lite.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Utilities.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelReader.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelWriter.cpp
)

SET (MXA_INCLUDE_DIRS

)

SET(HDF5_SUPPORT "1")
SET(HDF5_VTK_BRIDGE "0")
SET (TIFF_SUPPORT "0")
SET (XML_SUPPORT "0")
SET (BUILD_TESTING "0")
SET (BUILD_EXAMPELS "0")


#-- Configure the Header file and then add the Binary Directory to the Includes list
CONFIGURE_FILE(${MXA_SOURCE_DIR}/src/Common/MXAConfiguration.h.in
               ${PROJECT_BINARY_DIR}/MXAConfiguration.h @ONLY IMMEDIATE)
               
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})  # Include the Boost Headers
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS}) 

