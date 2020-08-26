#--////////////////////////////////////////////////////////////////////////////
#--  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--////////////////////////////////////////////////////////////////////////////
PROJECT (MXAExamples)

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
ENDIF ( NOT MXA_OUTPUT_DIRS_DEFINED )

INCLUDE_DIRECTORIES(${PROJECT_CODE_DIR}/MXA)
include_directories(${MXADataModel_SOURCE_DIR})

#INCLUDE( ${CMP_MODULES_SOURCE_DIR}/FindHDF5.cmake )
INCLUDE(/usr/share/cmake3/Modules/FindHDF5.cmake)
SET (MXA_EXAMPLES_SOURCE_DIR ${PROJECT_CODE_DIR}/MXA/Examples)


SET (GETDATA_SRCS
  ${MXAExamples_SOURCE_DIR}/GetDataExample.cpp
  ${MXATests_SOURCE_DIR}/DataFileGenerator.cpp
)
set (GETDATA_HDRS  )

cmp_IDE_GENERATED_PROPERTIES("MXA/Examples" "${GETDATA_HDRS}" "${GETDATA_SRCS}" "0")

# ------------------------------------------------------------------------------
#  Configure the header file
if (NOT WIN32)
set (MXA_TEMP_DIR "/tmp/")
else()
SET (MXA_TEMP_DIR "C:\\\\Windows\\\\Temp\\\\")
endif()

CONFIGURE_FILE(${MXAExamples_SOURCE_DIR}/ExampleFileLocations.h.in
               ${PROJECT_BINARY_DIR}/ExampleFileLocations.h @ONLY IMMEDIATE)
cmp_IDE_GENERATED_PROPERTIES ("Generated" " ${PROJECT_BINARY_DIR}/ExampleFileLocations.h" "" "0")

# --------------------------------------------------------------------
# Setup the install rules for the various platforms
set(install_dir "tools")
if (WIN32)
    set (install_dir "bin")
endif()

# ------------------------------------------------------------------------------
#  Add Executable for GetDataExample
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 )
    #ADD_EXECUTABLE ( GetDataExample ${GETDATA_SRCS} )
    #TARGET_LINK_LIBRARIES ( GetDataExample  ${MXADATAMODEL_LIB_NAME} ${DEP_LIBS} )
    #cmp_ToolInstallationSupport (GetDataExample "_debug" ${MXADataModel_BINARY_DIR} "Examples")
    BuildToolBundle(
        TARGET GetDataExample
        SOURCES ${GETDATA_SRCS}
        DEBUG_EXTENSION ${EXE_DEBUG_EXTENSION}
        VERSION_MAJOR ${CMP_VER_MAJOR}
        VERSION_MINOR ${CMP_VER_MINOR}
        VERSION_PATCH ${CMP_VER_PATCH}
        BINARY_DIR    ${${PROJECT_NAME}_BINARY_DIR}
        LINK_LIBRARIES ${MXADATAMODEL_LIB_NAME} ${DEP_LIBS}
        LIB_SEARCH_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMPONENT     Applications
        INSTALL_DEST  "${install_dir}"
    )   
ENDIF ( MXA_USE_HDF5 )

# ------------------------------------------------------------------------------
#  Add Executable for Example 1
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 )
    cmp_IDE_GENERATED_PROPERTIES("MXA/Examples/Example1" "" "Example1.cpp" "0")
    BuildToolBundle(
        TARGET Example1
        SOURCES ${MXAExamples_SOURCE_DIR}/Example1/Example1.cpp
        DEBUG_EXTENSION ${EXE_DEBUG_EXTENSION}
        VERSION_MAJOR ${CMP_VER_MAJOR}
        VERSION_MINOR ${CMP_VER_MINOR}
        VERSION_PATCH ${CMP_VER_PATCH}
        BINARY_DIR    ${${PROJECT_NAME}_BINARY_DIR}
        LINK_LIBRARIES ${MXADATAMODEL_LIB_NAME}
        LIB_SEARCH_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMPONENT     Applications
        INSTALL_DEST  "${install_dir}"
    )   
ENDIF ( MXA_USE_HDF5 )

# ------------------------------------------------------------------------------
#  Add Executable for Example 2
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 )
    cmp_IDE_GENERATED_PROPERTIES("MXA/Examples/Example2" "" "Example2.cpp" "0")
    BuildToolBundle(
        TARGET Example2
        SOURCES ${MXAExamples_SOURCE_DIR}/Example2/Example2.cpp
        DEBUG_EXTENSION ${EXE_DEBUG_EXTENSION}
        VERSION_MAJOR ${CMP_VER_MAJOR}
        VERSION_MINOR ${CMP_VER_MINOR}
        VERSION_PATCH ${CMP_VER_PATCH}
        BINARY_DIR    ${${PROJECT_NAME}_BINARY_DIR}
        LINK_LIBRARIES ${MXADATAMODEL_LIB_NAME}
        LIB_SEARCH_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMPONENT     Applications
        INSTALL_DEST  "${install_dir}"
    )  
ENDIF ( MXA_USE_HDF5 )

# ------------------------------------------------------------------------------
#  Add Executable for DataWrapperExample
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 )
    BuildToolBundle(
        TARGET DataWrapperExample
        SOURCES ${MXAExamples_SOURCE_DIR}/DataWrapperExample.cpp
        DEBUG_EXTENSION ${EXE_DEBUG_EXTENSION}
        VERSION_MAJOR ${CMP_VER_MAJOR}
        VERSION_MINOR ${CMP_VER_MINOR}
        VERSION_PATCH ${CMP_VER_PATCH}
        BINARY_DIR    ${${PROJECT_NAME}_BINARY_DIR}
        LINK_LIBRARIES ${MXADATAMODEL_LIB_NAME}
        LIB_SEARCH_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMPONENT     Applications
        INSTALL_DEST  "${install_dir}"
    )  
ENDIF ( MXA_USE_HDF5 )

# ------------------------------------------------------------------------------
#  Add Executable for DataImport Example
# ------------------------------------------------------------------------------
SET (DATAIMPORT_EXAMPLE_SOURCES
    ${MXAExamples_SOURCE_DIR}/DataImport/main.cpp
    ${MXAExamples_SOURCE_DIR}/DataImport/SimpleImportExample.cpp
    ${MXAExamples_SOURCE_DIR}/DataImport/ExampleImportDelegate.cpp
    ${MXAExamples_SOURCE_DIR}/DataImport/ExampleImportDelegateFactory.cpp
)
SET (DATAIMPORT_EXAMPLE_HEADERS
  ${MXAExamples_SOURCE_DIR}/DataImport/SimpleImportExample.h
  ${MXAExamples_SOURCE_DIR}/DataImport/ExampleImportDelegate.h
  ${MXAExamples_SOURCE_DIR}/DataImport/ExampleImportDelegateFactory.h
)
cmp_IDE_GENERATED_PROPERTIES("MXA/Examples/DataImport" "${DATAIMPORT_EXAMPLE_HEADERS}" "${DATAIMPORT_EXAMPLE_SOURCES}" "0")

IF ( MXA_USE_HDF5 )
    BuildToolBundle(
        TARGET DataImportExample
        SOURCES ${DATAIMPORT_EXAMPLE_SOURCES}
        DEBUG_EXTENSION ${EXE_DEBUG_EXTENSION}
        VERSION_MAJOR ${CMP_VER_MAJOR}
        VERSION_MINOR ${CMP_VER_MINOR}
        VERSION_PATCH ${CMP_VER_PATCH}
        BINARY_DIR    ${${PROJECT_NAME}_BINARY_DIR}
        LINK_LIBRARIES ${MXADATAMODEL_LIB_NAME}
        LIB_SEARCH_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMPONENT     Applications
        INSTALL_DEST  "${install_dir}"
    )  
ENDIF ( MXA_USE_HDF5 )


# ------------------------------------------------------------------------------
#  Add Executable for MXAProperty Example
# ------------------------------------------------------------------------------
SET (MXA_PROPERTY_EXAMPLE_SRCS
    ${MXAExamples_SOURCE_DIR}/MXAPropertyExample/main.cpp
    ${MXAExamples_SOURCE_DIR}/MXAPropertyExample/MyPropertyClass.cpp
)
SET (MXA_PROPERTY_EXAMPLE_HEADERS
    ${MXAExamples_SOURCE_DIR}/MXAPropertyExample/MyPropertyClass.h
)
cmp_IDE_GENERATED_PROPERTIES("MXA/Examples/MXAPropertyExample" "${MXA_PROPERTY_EXAMPLE_HEADERS}" "${MXA_PROPERTY_EXAMPLE_SRCS}" "0")

BuildToolBundle(
    TARGET MXAPropertyExample
    SOURCES  ${MXA_PROPERTY_EXAMPLE_SRCS};${MXA_PROPERTY_EXAMPLE_HEADERS}
    DEBUG_EXTENSION ${EXE_DEBUG_EXTENSION}
    VERSION_MAJOR ${CMP_VER_MAJOR}
    VERSION_MINOR ${CMP_VER_MINOR}
    VERSION_PATCH ${CMP_VER_PATCH}
    BINARY_DIR    ${${PROJECT_NAME}_BINARY_DIR}
    LINK_LIBRARIES ${MXADATAMODEL_LIB_NAME}
    LIB_SEARCH_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMPONENT     Applications
    INSTALL_DEST  "${install_dir}"
)  

# ------------------------------------------------------------------------------
#  Add Executable for Support File Example
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 AND MXA_USE_TIFF)
    SET (SUPPORT_FILE_EXAMPLE_SRCS
        ${MXAExamples_SOURCE_DIR}/SupportFileExample.cpp
        ${MXATests_SOURCE_DIR}/TiffMaker.cpp
    )
    SET (SUPPORT_FILE_EXAMPLE_HEADERS
        
    )
    cmp_IDE_GENERATED_PROPERTIES("MXA/Examples" "${SUPPORT_FILE_EXAMPLE_HEADERS}" "${SUPPORT_FILE_EXAMPLE_SRCS}" "0")

    BuildToolBundle(
        TARGET SupportFileExample
        SOURCES  ${SUPPORT_FILE_EXAMPLE_SRCS};${SUPPORT_FILE_EXAMPLE_HEADERS}
        DEBUG_EXTENSION ${EXE_DEBUG_EXTENSION}
        VERSION_MAJOR ${CMP_VER_MAJOR}
        VERSION_MINOR ${CMP_VER_MINOR}
        VERSION_PATCH ${CMP_VER_PATCH}
        BINARY_DIR    ${${PROJECT_NAME}_BINARY_DIR}
        LINK_LIBRARIES ${MXADATAMODEL_LIB_NAME}
        LIB_SEARCH_DIRS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMPONENT     Applications
        INSTALL_DEST  "${install_dir}"
    )  

ENDIF ( MXA_USE_HDF5 AND MXA_USE_TIFF )

