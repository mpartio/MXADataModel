# set project's name
PROJECT( MXADataModel )
cmake_minimum_required(VERSION 2.6)

#-- Need this for link line stuff?
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# - Set the variable where to find the top level project sources
IF (NOT DEFINED MXADataModel_SOURCE_DIR)
    SET (MXADataModel_SOURCE_DIR ${MXADataModel_SOURCE_DIR})
ENDIF(NOT DEFINED MXADataModel_SOURCE_DIR)

# - Check to see if any other project is redefining the library names
IF ( NOT DEFINED  MXADATAMODEL_LIB_NAME)
  SET (MXADATAMODEL_LIB_NAME MXADataModel)
ENDIF ( NOT DEFINED MXADATAMODEL_LIB_NAME )

# - Check to see if another project is over riding the naming of the libraries
IF (NOT DEFINED MXA_USE_DEBUG_LIBRARY_NAME)
  SET (MXA_USE_DEBUG_LIBRARY_NAME TRUE)
ENDIF (NOT DEFINED MXA_USE_DEBUG_LIBRARY_NAME)

# - Set our Library Version
SET (MXADATAMODEL_VER_MAJOR "1")
SET (MXADATAMODEL_VER_MINOR "3")
SET (MXADATAMODEL_VER_PATCH "0")
SET (MXADATAMODEL_LIBRARY_VERSION ${MXADATAMODEL_VER_MAJOR}.${MXADATAMODEL_VER_MINOR}.${MXADATAMODEL_VER_PATCH})


# ---------- Setup output Directories -------------------------
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Libraries"
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Executables."
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all static libraries."
  )

#-------------------------------------------------------------------------------
#  Define the location of the Build Directory
SET (MXA_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

#-------------------------------------------------------------------------------
#  Define the location of the Resources Directory
#-------------------------------------------------------------------------------
SET (MXA_RESOURCES_DIR ${MXADataModel_SOURCE_DIR}/Resources)
SET (MXA_SOURCE_DIR ${MXADataModel_SOURCE_DIR}/Source)
SET (MXA_CMAKE_DIR ${MXA_RESOURCES_DIR}/CMake)
SET (MXATests_SOURCE_DIR ${MXADataModel_SOURCE_DIR}/Tests)
SET (MXAExamples_SOURCE_DIR ${MXADataModel_SOURCE_DIR}/Examples)
SET (PROJECT_RESOURCES_DIR ${MXA_RESOURCES_DIR})
SET (MXA_DOCUMENTATION_DIR ${MXADataModel_SOURCE_DIR}/Documentation)

#-------------------------------------------------------------------------------
INCLUDE (${MXA_CMAKE_DIR}/vtkTestTypes.cmake )
INCLUDE (${MXA_CMAKE_DIR}/ConfigureChecks.cmake)

# ------- Enable the CTest testing. Use make test to run all tests ---------------
OPTION(BUILD_TESTING "Build MXADataModel Unit Testing" ON)
IF(BUILD_TESTING)
  ENABLE_TESTING()
ENDIF(BUILD_TESTING)

# Extend the include path to include the MXADataModel source directory
INCLUDE_DIRECTORIES( "${MXA_SOURCE_DIR}" )
# ----- Also include our binary directory as some headers are generated into that directory
INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}" ${INCLUDE_DIRECTORIES} )

# Build shared libraries
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
SET (LIB_TYPE STATIC)
SET (MXA_BUILT_AS_DYNAMIC_LIB)
IF (BUILD_SHARED_LIBS)
  SET (LIB_TYPE SHARED)
  SET (MXA_BUILT_AS_DYNAMIC_LIB 1)
  IF (WIN32)
    ADD_DEFINITIONS("-DMXA_BUILT_AS_DYNAMIC_LIB")
  ENDIF (WIN32)
ENDIF (BUILD_SHARED_LIBS)

#-- Find all the support libraries needed
INCLUDE (${MXA_CMAKE_DIR}/FindSupportLibraries.cmake)

#-- Include a Macro to setup source file properties
INCLUDE ( ${MXA_CMAKE_DIR}/MXA_CMakeMacros.cmake)

#-- Include all the source files
INCLUDE (${MXA_SOURCE_DIR}/MXA/Base/Base_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/MXA/BMPIO/BMPIO_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/MXA/Common/Common_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/MXA/Core/Core_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/MXA/DataImport/DataImport_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/MXA/DataWrappers/DataWrappers_SourceList.cmake)
INCLUDE (${MXA_SOURCE_DIR}/MXA/Utilities/Utilities_SourceList.cmake)

# ------- Set up the MXADataModel Project Source Files -----------
SET( MXADATAMODEL_SRCS
    ${MXA_BASE_SRCS}
    ${MXA_COMMON_SRCS}
    ${MXA_CORE_SRCS}
    ${MXA_DATA_IMPORT_SRCS}
    ${MXA_DATA_WRAPPERS_SRCS}
    ${MXA_UTILITIES_SRCS}
)
  
#--- Adding headers is NOT needed unless you want Visual Studio or Xcode to have them in their projects 
SET (MXADATAMODEL_HEADERS
    ${MXA_BASE_HEADERS}
    ${MXA_COMMON_HEADERS}
    ${MXA_CORE_HEADERS}
    ${MXA_DATA_IMPORT_HEADERS}
    ${MXA_DATA_WRAPPERS_HEADERS}
    ${MXA_UTILITIES_HEADERS}
)

#--------- XML Sources ---------------------------------------------------
IF (MXA_USE_XML)
    INCLUDE (${MXA_SOURCE_DIR}/MXA/XML/XML_SourceList.cmake)
    SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_XML_SRCS} ${MXA_XML_HEADERS})
    SET (MXA_XML_SUPPORT "1")
ENDIF(MXA_USE_XML)

#------- HDF5 Dependant Sources ------------------------------
IF ( MXA_USE_HDF5 )
  INCLUDE (${MXA_SOURCE_DIR}/MXA/HDF5/HDF5_SourceList.cmake)
  SET(MXA_HDF5_SUPPORT "1")
  OPTION(MXA_BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)
  IF(MXA_BUILD_VTK_HDF5)
    #-- Find VTK packages -----------------------------------------
    FIND_PACKAGE(VTK REQUIRED)
    INCLUDE(${VTK_USE_FILE})
      
    # List the kits from VTK that are needed by this project.
    SET(vtkLocal_LIBS vtkCommon vtksys vtkFiltering vtkIO vtkGraphics )
    
    SET (MXA_HDF5_SRCS ${MXA_HDF5_SRCS} ${MXA_SOURCE_DIR}/MXA/HDF5/vtkHDF5.cpp)
    SET (MXA_HDF5_HEADERS ${MXA_HDF5_HEADERS} ${MXA_SOURCE_DIR}/MXA/HDF5/vtkHDF5.h)
    SET (DEP_LIBS ${DEP_LIBS} ${vtkLocal_LIBS})
    SET(HDF5_VTK_BRIDGE "1")
  ENDIF (MXA_BUILD_VTK_HDF5)
  #-- Finally include all the sources and headers from the HDf5 and vtk sections
  SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_SRCS} ${MXA_HDF5_HEADERS})
  
  # ------------------ Check for and Include Tif Lib dependent code --------------
  IF(MXA_USE_TIFF)
    SET (MXA_TIFF_SUPPORT "1")
    SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_TIFF_SRCS} ${MXA_HDF5_TIFF_HEADERS})
  ENDIF (MXA_USE_TIFF)
  
  #-- The BMP IO Classes are dependent on HDF5
  SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_BMPIO_SRCS} ${MXA_BMPIO_HEADERS})
  
ENDIF ( MXA_USE_HDF5 )

# ---------- Set up the Project Options -----------------------
IF(WIN32)
 SET(GUI_TYPE WIN32)
 SET(WINDOWS_LARGE_FILE_SUPPORT "1")
ENDIF(WIN32)

#-- Configure the Header file and then add the Binary Directory to the Includes list
CONFIGURE_FILE(${MXA_SOURCE_DIR}/MXA/Common/MXAConfiguration.h.in
               ${PROJECT_BINARY_DIR}/MXAConfiguration.h @ONLY IMMEDIATE)
                 
# -------- Add some definitions for Debug Builds -----------------
IF ( CMAKE_BUILD_TYPE MATCHES Debug )
 # ADD_DEFINITIONS(-DDEBUG)
 ADD_DEFINITIONS(-Wall)
ENDIF ( CMAKE_BUILD_TYPE MATCHES Debug )

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


SET (MXADATAMODEL_HEADERS ${MXADATAMODEL_HEADERS} ${PROJECT_BINARY_DIR}/MXAConfiguration.h  )
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

# - Combine all the source files together -----------------------------
SET ( PROJECT_SRCS  ${MXADATAMODEL_SRCS} ${MXADATAMODEL_HEADERS})

IF (MXA_USE_DEBUG_LIBRARY_NAME)

    IF (BUILD_SHARED_LIBS)
        SET (MXA_BUILT_AS_DYNAMIC_LIB 1)
        IF (WIN32 AND NOT MINGW)
            SET(MXA_LIBRARY_RELEASE "MXADataModel" CACHE INTERNAL "" FORCE)
            SET(MXA_LIBRARY_DEBUG "MXADataModel_D" CACHE INTERNAL "" FORCE)
        ELSE (WIN32 AND NOT MINGW)
            SET(MXA_LIBRARY_RELEASE "MXADataModel" CACHE INTERNAL "" FORCE)
            SET(MXA_LIBRARY_DEBUG "MXADataModel_debug" CACHE INTERNAL "" FORCE)
        ENDIF(WIN32 AND NOT MINGW)
    ELSE (BUILD_SHARED_LIBS)
        SET (MXA_BUILT_AS_DYNAMIC_LIB)
        IF (WIN32 AND NOT MINGW)
            SET(MXA_LIBRARY_RELEASE "libMXADataModel" CACHE INTERNAL "" FORCE)
            SET(MXA_LIBRARY_DEBUG "libMXADataModel_D" CACHE INTERNAL "" FORCE)
        ELSE (WIN32 AND NOT MINGW)
            SET(MXA_LIBRARY_RELEASE "MXADataModel" CACHE INTERNAL "" FORCE)
            SET(MXA_LIBRARY_DEBUG "MXADataModel_debug" CACHE INTERNAL "" FORCE)
        ENDIF(WIN32 AND NOT MINGW)
    ENDIF (BUILD_SHARED_LIBS)
ELSE (MXA_USE_DEBUG_LIBRARY_NAME)
    SET(MXA_LIBRARY_RELEASE "MXADataModel" CACHE INTERNAL "" FORCE)
    SET(MXA_LIBRARY_DEBUG "MXADataModel" CACHE INTERNAL "" FORCE)
ENDIF (MXA_USE_DEBUG_LIBRARY_NAME)

#-- Add the MXADataModel Library and link to needed libraries-------------------
ADD_LIBRARY (${MXADATAMODEL_LIB_NAME} ${LIB_TYPE} ${PROJECT_SRCS} )
TARGET_LINK_LIBRARIES (${MXADATAMODEL_LIB_NAME} ${DEP_LIBS} )

#-- Set the Debug and Release names for the libraries
SET_TARGET_PROPERTIES( ${MXADATAMODEL_LIB_NAME} 
    PROPERTIES
    DEBUG_OUTPUT_NAME ${MXA_LIBRARY_DEBUG}
    RELEASE_OUTPUT_NAME ${MXA_LIBRARY_RELEASE}
)

IF (APPLE)
  OPTION (MXA_BUILD_WITH_INSTALL_NAME "Build Libraries with the install_name set to the installation prefix. This is good if you are going to run from the installation location" OFF)
  IF(MXA_BUILD_WITH_INSTALL_NAME)
  SET_TARGET_PROPERTIES(${MXADATAMODEL_LIB_NAME}
     PROPERTIES
     LINK_FLAGS "-current_version ${MXADATAMODEL_LIBRARY_VERSION} -compatibility_version ${MXADATAMODEL_LIBRARY_VERSION}"
     INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
     BUILD_WITH_INSTALL_RPATH ${MXA_BUILD_WITH_INSTALL_NAME}
  )
  ENDIF(MXA_BUILD_WITH_INSTALL_NAME)
ENDIF (APPLE)

#----- Use MSVC Naming conventions for Shared Libraries for MinGW --------------
IF (MINGW AND BUILD_SHARED_LIBS)
    SET_TARGET_PROPERTIES( ${MXADATAMODEL_LIB_NAME}
        PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
    )
ENDIF (MINGW AND BUILD_SHARED_LIBS)

#-- Include the MXADataModel Testing Sources
IF ( BUILD_TESTING )
  ADD_SUBDIRECTORY(${MXADataModel_SOURCE_DIR}/Tests ${PROJECT_BINARY_DIR}/Tests)
ENDIF ( BUILD_TESTING )

#-- Include the Examples Directory
OPTION(MXA_BUILD_EXAMPLES "Build Programs that demonstrate the use of the API" OFF)
IF(MXA_BUILD_EXAMPLES)
    ADD_SUBDIRECTORY(${MXADataModel_SOURCE_DIR}/Examples ${PROJECT_BINARY_DIR}/Examples)
ENDIF(MXA_BUILD_EXAMPLES)

OPTION(MXA_BUILD_UTILITIES "Build programs that help with extending MXADataModel" OFF)
IF (MXA_BUILD_UTILITIES)
    ADD_SUBDIRECTORY(${MXADataModel_SOURCE_DIR}/Utilities ${PROJECT_BINARY_DIR}/Utilities)
ENDIF (MXA_BUILD_UTILITIES)

option(MXA_BUILD_API_DOCS "Use Doxygen to create the HTML based API documentation" OFF)
if(MXA_BUILD_API_DOCS)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  configure_file(${MXA_RESOURCES_DIR}/MXADataModel.doxyfile.in 
                 ${MXADataModel_BINARY_DIR}/MXADataModel.doxyfile  @ONLY IMMEDIATE)
  add_custom_command(TARGET ${MXADATAMODEL_LIB_NAME} POST_BUILD COMMAND ${DOXYGEN_EXECUTABLE} ${MXADataModel_BINARY_DIR}/MXADataModel.doxyfile)
  
endif(MXA_BUILD_API_DOCS)


#--- Set up the installation Code for Make Install
include ( "${MXA_CMAKE_DIR}/MXAInstall.cmake")
