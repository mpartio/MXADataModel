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
  SET (MXADATAMODEL_LIB_NAME MXADataModel)
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
SET (MXADATAMODEL_RESOURCES_DIR ${PROJECT_RESOURCES_DIR})
SET (PROJECT_CMAKE_DIR ${PROJECT_RESOURCES_DIR}/CMake)
SET (PROJECT_CMAKE_MODULES_DIR ${PROJECT_CMAKE_DIR}/Modules)
SET (OSX_TOOLS_DIR ${PROJECT_RESOURCES_DIR}/OSX_Tools)
SET (MXA_SOURCE_DIR ${MXADataModel_SOURCE_DIR}/Source)
SET (MXATests_SOURCE_DIR ${MXADataModel_SOURCE_DIR}/Tests)
SET (MXAExamples_SOURCE_DIR ${MXADataModel_SOURCE_DIR}/Examples)
SET (MXA_DOCUMENTATION_DIR ${MXADataModel_SOURCE_DIR}/Documentation)
SET (PROJECT_PREFIX "MXA" CACHE STRING "The 3 Letter Prefix to be used for Preprocessor definitions")
SET (EXE_DEBUG_EXTENSION "_debug")

INCLUDE (${PROJECT_RESOURCES_DIR}/CMake/CMakeMacros.cmake )
INCLUDE (${PROJECT_RESOURCES_DIR}/CMake/ConfigureChecks.cmake)

# ------- Enable the CTest testing. Use make test to run all tests ---------------
if (NOT DEFINED MXA_BUILD_TESTING)
    OPTION(MXA_BUILD_TESTING "Build MXADataModel Unit Testing" ON)
endif()
IF(MXA_BUILD_TESTING)
  ENABLE_TESTING()
ENDIF(MXA_BUILD_TESTING)

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

# --------------------------------------------------------------------
# Find all the support libraries needed. Including the Boost Program
# options library which the ImportGenerator Needs
SET (Boost_USE_PROGRAM_OPTIONS TRUE)
INCLUDE (${PROJECT_CMAKE_DIR}/FindSupportLibraries.cmake)

#-- Include a Macro to setup source file properties
INCLUDE ( ${PROJECT_CMAKE_DIR}/CMakeMacros.cmake)


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


if ( ${CMAKE_GENERATOR} MATCHES "NMake Makefiles" )
    set (NMAKE_GENERATOR 1)
endif()

# --------------------------------------------------------------------
# Generate our header files
# --------------------------------------------------------------------
configure_file(${PROJECT_RESOURCES_DIR}/CMake/Configuration.h.in
               ${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Configuration.h )
configure_file(${PROJECT_RESOURCES_DIR}/CMake/PrimitiveTypes.h.in
               ${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Types.h )
configure_file(${PROJECT_SOURCE_DIR}/License.txt
                ${PROJECT_BINARY_DIR}/License.txt )
configure_file(${PROJECT_SOURCE_DIR}/ReadMe.txt
                ${PROJECT_BINARY_DIR}/ReadMe.txt )
SET(MXADATAMODEL_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Source;${MXADataModel_BINARY_DIR}"  )

# --------------------------------------------------------------------
# Generate a Header file with Compile Version variables
# --------------------------------------------------------------------
GenerateVersionString( ${PROJECT_NAME} "${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Version.h" "${PROJECT_PREFIX}" )

IDE_GENERATED_PROPERTIES( "Generated" 
"${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Version.h" 
"${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Configuration.h" 
"${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Types.h")               
                               
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

SET (MXADATAMODEL_HEADERS 
    ${MXADATAMODEL_HEADERS}
    ${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Version.h
    ${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Configuration.h
    ${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Types.h 
)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

# - Combine all the source files together -----------------------------
SET ( PROJECT_SRCS  ${MXADATAMODEL_SRCS} ${MXADATAMODEL_HEADERS})

IF (BUILD_SHARED_LIBS)
    SET (MXA_BUILT_AS_DYNAMIC_LIB 1)
endif()

# --------------------------------------------------------------------
# -- Add the MXADataModel Library and link to needed libraries 
# --------------------------------------------------------------------
ADD_LIBRARY (${MXADATAMODEL_LIB_NAME} ${LIB_TYPE} ${PROJECT_SRCS} )
TARGET_LINK_LIBRARIES (${MXADATAMODEL_LIB_NAME} ${DEP_LIBS} )
LibraryProperties( ${MXADATAMODEL_LIB_NAME} ${EXE_DEBUG_EXTENSION})

if ( ${MXA_INSTALL_FILES} EQUAL 1 )
    INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin 
		COMPONENT Runtime)
else()
# Include an install rule for NON-Apple systems. The specialized installer that ParaView uses
# will take care of copying the MXADataModel library into the proper location.
	if (NOT APPLE)
		INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} 
				LIBRARY DESTINATION bin
				ARCHIVE DESTINATION lib
				RUNTIME DESTINATION bin
				COMPONENT Runtime)
	endif()
endif()

set (lib_rt_dest lib)
if (WIN32 AND BUILD_SHARED_LIBS)
  set (lib_rt_dest bin)
endif()


#-----
IF (MINGW)
    SET (CMAKE_COLOR_MAKEFILE 0)
endif()

if (  ${MXA_INSTALL_FILES} EQUAL 1 )
    # Copy DLL libraries into the correct build location on MSVC platforms
    MXA_COPY_DEPENDENT_LIBRARIES ("tiff;expat;hdf5")
    
    # Generate Install Rules for the DLL Libraries
    MXA_LIBRARIES_INSTALL_RULES ("tiff;expat;hdf5" "bin" )
endif()

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
    include ( ${PROJECT_CMAKE_DIR}/MXAInstall.cmake)
    include ( ${PROJECT_CMAKE_DIR}/PackageProject.cmake)
endif()
