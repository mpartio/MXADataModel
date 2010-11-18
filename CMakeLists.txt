#--////////////////////////////////////////////////////////////////////////////
#--  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--////////////////////////////////////////////////////////////////////////////
# Set project's name

PROJECT( MXADataModel )
cmake_minimum_required(VERSION 2.6)

if (NOT DEFINED MXA_INSTALL_FILES)
    set (MXA_INSTALL_FILES 1 CACHE INTERNAL "Use the Normal Install rules for the MXADataModel Project")
endif()
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
  SET (MXADATAMODEL_LIB_NAME MXADataModel CACHE STRING "The Name of the Library")
ENDIF ( NOT DEFINED MXADATAMODEL_LIB_NAME )

# - Check to see if another project is over riding the naming of the libraries
IF (NOT DEFINED MXA_USE_DEBUG_LIBRARY_NAME)
  SET (MXA_USE_DEBUG_LIBRARY_NAME TRUE)
ENDIF (NOT DEFINED MXA_USE_DEBUG_LIBRARY_NAME)

if ( NOT MXA_OUTPUT_DIRS_DEFINED )
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
endif()

if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
endif()

if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif()

if (NOT DEFINED EXECUTABLE_OUTPUT_PATH)
  set (EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if (NOT DEFINED LIBRARY_OUTPUT_PATH)
  set (LIBRARY_OUTPUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()

if(0)
message(STATUS "EXECUTABLE_OUTPUT_PATH:         ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "LIBRARY_OUTPUT_PATH:            ${LIBRARY_OUTPUT_PATH}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

#-------------------------------------------------------------------------------
#  Define the location of the Build Directory
SET (MXA_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

#-------------------------------------------------------------------------------
#  Define the location of the Resources Directory
SET (PROJECT_RESOURCES_DIR ${MXADataModel_SOURCE_DIR}/Resources)
SET (PROJECT_CODE_DIR ${MXADataModel_SOURCE_DIR}/Code)

# - Include all the cmake code to make sure that the MXA Library is built
if(NOT EXISTS ${PROJECT_RESOURCES_DIR}/cmp/cmpProject.cmake)
	message(STATUS "CMP is a git submodule of MXADataModel and does not seem to be cloned")
	message(STATUS "from the repository properly. Use the following command to pull")
	message(STATUS "the CMP project from the repository")
	message(STATUS "git submodule update --init")
	message(FATAL_ERROR "")
endif()

#-------------------------------------------------------------------------------
# Include the cmp project to perform all the basic configuration tests for the 
# project.
#------------------------------------------------------------------------------- 
set (CMP_HEADER_DIR ${MXADataModel_BINARY_DIR}/MXA)
set (CMP_CONFIGURATION_FILE_NAME "MXACMPConfiguration.h")
set (CMP_TYPES_FILE_NAME "MXATypes.h")
set (CMP_VERSION_HEADER_FILE_NAME "MXAVersion.h")
set (CMP_EXTRA_CONFIGURATION_FILE "MXAConfiguration.h")
SET (CMP_PROJECT_PREFIX "MXA" CACHE STRING "The 3 Letter Prefix to be used for Preprocessor definitions")
set (CMP_PROJECT_NAMESPACE "MXA")
INCLUDE (${PROJECT_RESOURCES_DIR}/cmp/cmpProject.cmake)

# --------------------------------------------------------------------
# Project Specific Variables
SET (MXATests_SOURCE_DIR ${MXADataModel_SOURCE_DIR}/Tests)
SET (MXAExamples_SOURCE_DIR ${MXADataModel_SOURCE_DIR}/Examples)
SET (MXA_DOCUMENTATION_DIR ${MXADataModel_SOURCE_DIR}/Documentation)
SET (MXA_CMAKE_DIR ${PROJECT_RESOURCES_DIR}/MXACMake)
SET (EXE_DEBUG_EXTENSION "_debug")


# ------- Enable the CTest testing. Use make test to run all tests ---------------
if (NOT DEFINED MXA_BUILD_TESTING)
    OPTION(MXA_BUILD_TESTING "Build MXADataModel Unit Testing" ON)
endif()
IF(MXA_BUILD_TESTING)
  ENABLE_TESTING()
ENDIF(MXA_BUILD_TESTING)

# Extend the include path to include the MXADataModel source directory
INCLUDE_DIRECTORIES( "${PROJECT_CODE_DIR}" )
# ----- Also include our binary directory as some headers are generated into that directory
INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}")

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

                               
# -------- Add some definitions for Debug Builds -----------------
IF ( CMAKE_BUILD_TYPE MATCHES Debug )
  ADD_DEFINITIONS(-DDEBUG)
  if (NOT WIN32)
    ADD_DEFINITIONS(-Wall)
  endif()
  IF (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  ENDIF()
ENDIF ( CMAKE_BUILD_TYPE MATCHES Debug )


IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


# --------------------------------------------------------------------
# Find all the support libraries needed. Including the Boost Program
# options library which the ImportGenerator Needs
INCLUDE (${MXA_CMAKE_DIR}/FindSupportLibraries.cmake)


# ---------- Set up the Project Options -----------------------
IF(WIN32)
 SET(GUI_TYPE WIN32)
 SET(WINDOWS_LARGE_FILE_SUPPORT "1")
ENDIF(WIN32)


if ( ${CMAKE_GENERATOR} MATCHES "NMake Makefiles" )
    set (NMAKE_GENERATOR 1)
endif()

# --------------------------------------------------------------------
# Add our MXA Library            
add_subdirectory(${MXADataModel_SOURCE_DIR}/Code ${PROJECT_BINARY_DIR}/MXALib)

#-- Include the MXADataModel Testing Sources
IF ( MXA_BUILD_TESTING )
  ADD_SUBDIRECTORY(${MXADataModel_SOURCE_DIR}/Tests ${PROJECT_BINARY_DIR}/Tests)
ENDIF ( MXA_BUILD_TESTING )

#-- Include the Examples Directory
OPTION(MXA_BUILD_EXAMPLES "Build Programs that demonstrate the use of the API" ON)
IF(MXA_BUILD_EXAMPLES)
    ADD_SUBDIRECTORY(${MXADataModel_SOURCE_DIR}/Examples ${PROJECT_BINARY_DIR}/Examples)
ENDIF(MXA_BUILD_EXAMPLES)

OPTION(MXA_BUILD_UTILITIES "Build programs that help with extending MXADataModel" ON)
IF (MXA_BUILD_UTILITIES)
    ADD_SUBDIRECTORY(${MXADataModel_SOURCE_DIR}/Utilities ${PROJECT_BINARY_DIR}/Utilities)
ENDIF (MXA_BUILD_UTILITIES)

option(MXA_BUILD_API_DOCS "Use Doxygen to create the HTML based API documentation" OFF)
if(MXA_BUILD_API_DOCS)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  configure_file(${PROJECT_RESOURCES_DIR}/MXADataModel.doxyfile.in 
                 ${MXADataModel_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)

  add_custom_target (Docs COMMAND ${DOXYGEN_EXECUTABLE} ${MXADataModel_BINARY_DIR}/Doxyfile
													SOURCES ${MXADataModel_BINARY_DIR}/Doxyfile)
endif(MXA_BUILD_API_DOCS)


#--- Set up the installation Code for Make Install
if ( ${MXA_INSTALL_FILES} EQUAL 1 )
    include ( ${PROJECT_RESOURCES_DIR}/MXACMake/MXAInstall.cmake)
    include ( ${PROJECT_RESOURCES_DIR}/CPack/PackageProject.cmake)
endif()

