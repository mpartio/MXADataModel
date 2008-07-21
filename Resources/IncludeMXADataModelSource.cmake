#MESSAGE (STATUS "IncludeMXADataModelSource.cmake")
IF (NOT MXA_RESOURCES_DIR)
  GET_FILENAME_COMPONENT(MXA_RESOURCES_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDIF (NOT MXA_RESOURCES_DIR)
#-- Include the Configure Checks
#-------------------------------------------------------------------------------
INCLUDE( "${MXA_RESOURCES_DIR}/vtkTestTypes.cmake" )
INCLUDE( "${MXA_RESOURCES_DIR}/ConfigureChecks.cmake")


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

#--- Include the MXADataModel/src directory on the INCLUDE path
INCLUDE_DIRECTORIES (${MXA_SOURCE_DIR}/src)

#-- Include all the source files
INCLUDE (${MXA_SOURCE_DIR}/src/Base/Base_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/src/BMPIO/BMPIO_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/src/Common/Common_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/src/Core/Core_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/src/DataImport/DataImport_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/src/DataWrappers/DataWrappers_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/src/Utilities/Utilities_SourceList.cmake)

SET (MXA_USE_HDF5 1)
SET (MXA_USE_XML 0)
SET (MXA_USE_TIFF 0)
SET (HDF5_SUPPORT "1")
SET (HDF5_VTK_BRIDGE "0")
SET (MXA_USE_HDF5_PRIMITIVE_TYPES 1)
SET (H5LITE_USE_MXA_CONSTRUCTS 1)
SET (TIFF_SUPPORT "0")
SET (XML_SUPPORT "0")
SET (BUILD_TESTING "0")
SET (BUILD_EXAMPLES "0")

# ------- Set up the MXADataModel Project Source Files -----------
SET( MXADATAMODEL_SRCS
    ${MXA_BASE_SRCS}
    ${MXA_BMPIO_SRCS}
    ${MXA_COMMON_SRCS}
    ${MXA_CORE_SRCS}
    ${MXA_DATA_IMPORT_SRCS}
    ${MXA_DATA_WRAPPERS_SRCS}
    ${MXA_UTILITIES_SRCS}
)
  
#--- Adding headers is NOT needed unless you want Visual Studio or Xcode to have them in their projects 
SET (MXADATAMODEL_HEADERS
    ${MXA_BASE_HEADERS}
    ${MXA_BMPIO_HEADERS}
    ${MXA_COMMON_HEADERS}
    ${MXA_CORE_HEADERS}
    ${MXA_DATA_IMPORT_HEADERS}
    ${MXA_DATA_WRAPPERS_HEADERS}
    ${MXA_UTILITIES_HEADERS}
)

#--------- XML Sources ---------------------------------------------------
IF (MXA_USE_XML)
    INCLUDE (${MXA_SOURCE_DIR}/src/XML/XML_SourceList.cmake)
    SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_XML_SRCS} ${MXA_XML_HEADERS})
    SET (XML_SUPPORT "1")
ENDIF(MXA_USE_XML)

#------- HDF5 Dependant Sources ------------------------------
IF ( MXA_USE_HDF5 )
  INCLUDE (${MXA_SOURCE_DIR}/src/HDF5/HDF5_SourceList.cmake)
  MESSAGE(STATUS "MXA has HDF5 sources")
  
  SET(HDF5_SUPPORT "1")
 # OPTION(BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)
  IF(BUILD_VTK_HDF5)
    #-- Find VTK packages -----------------------------------------
    FIND_PACKAGE(VTK REQUIRED)
    INCLUDE(${VTK_USE_FILE})
      
    # List the kits from VTK that are needed by this project.
    SET(vtkLocal_LIBS vtkCommon vtksys vtkFiltering vtkIO vtkGraphics )
    
    SET (MXA_HDF5_SRCS ${MXA_HDF5_SRCS} ${MXA_SOURCE_DIR}/src/HDF5/vtkHDF5.cpp)
    SET (MXA_HDF5_HEADERS ${MXA_HDF5_HEADERS} ${MXA_SOURCE_DIR}/src/HDF5/vtkHDF5.h)
    SET (DEP_LIBS ${DEP_LIBS} ${vtkLocal_LIBS})
    SET(HDF5_VTK_BRIDGE "1")
  ENDIF (BUILD_VTK_HDF5)
  #-- Finally include all the sources and headers from the HDf5 and vtk sections
  SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_SRCS} ${MXA_HDF5_HEADERS})
  
  # ------------------ Check for and Include Tif Lib dependent code --------------
  IF(MXA_USE_TIFF)
    SET (TIFF_SUPPORT "1")
    SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_TIFF_SRCS} ${MXA_HDF5_TIFF_HEADERS})
  ENDIF (MXA_USE_TIFF)

ENDIF ( MXA_USE_HDF5 )


#-- Configure the Header file and then add the Binary Directory to the Includes list
CONFIGURE_FILE(${MXA_SOURCE_DIR}/src/Common/MXAConfiguration.h.in
               ${PROJECT_BINARY_DIR}/MXAConfiguration.h @ONLY IMMEDIATE)
               
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")


