# set project's name
PROJECT( MXAUtilities )
cmake_minimum_required(VERSION 2.6)


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


CONFIGURE_FILE(${MXAUtilities_SOURCE_DIR}/UtilityFileLocations.h.in ${PROJECT_BINARY_DIR}/UtilityFileLocations.h)

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
include_directories(${MXAUtilities_SOURCE_DIR})


# List the source files here
SET (ImportGenerator_SRCS
    ${MXAUtilities_SOURCE_DIR}/ImportGenerator.cpp
)
ADD_EXECUTABLE(ImportGenerator ${ImportGenerator_SRCS} )
TARGET_LINK_LIBRARIES(ImportGenerator ${MXADATAMODEL_LIB_NAME} )
#cmp_ToolInstallationSupport (ImportGenerator "_debug" ${MXADataModel_BINARY_DIR} "Utilities")

        
SET (IMPORTGENERATOR_INSTALLED_RESOURCES
    ${MXAUtilities_SOURCE_DIR}/Templates/CMakeLists.txt.in
    ${MXAUtilities_SOURCE_DIR}/Templates/ImportDelegate.cpp.in
    ${MXAUtilities_SOURCE_DIR}/Templates/ImportDelegate.h.in
    ${MXAUtilities_SOURCE_DIR}/Templates/ImportDelegateFactory.cpp.in
    ${MXAUtilities_SOURCE_DIR}/Templates/ImportDelegateFactory.h.in
    ${MXAUtilities_SOURCE_DIR}/Templates/main.cpp.in
)

INSTALL (FILES ${IMPORTGENERATOR_INSTALLED_RESOURCES} 
        DESTINATION share/MXADataModel/Resources/Templates
        COMPONENT Resources)
