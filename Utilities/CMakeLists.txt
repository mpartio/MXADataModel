# set project's name
PROJECT( ImportGenerator )
cmake_minimum_required(VERSION 2.6)


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


#INCLUDE_DIRECTORIES(${ImportGenerator_SOURCE_DIR}/src)

# List the source files here
SET (ImportGenerator_SRCS
    ImportGenerator.cpp
)

SET (UTILITIES_DIR ${MXA_SOURCE_DIR}/Utilities)
CONFIGURE_FILE(${UTILITIES_DIR}/UtilityFileLocations.h.in ${PROJECT_BINARY_DIR}/UtilityFileLocations.h)

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

ADD_EXECUTABLE(ImportGenerator ${ImportGenerator_SRCS} )
TARGET_LINK_LIBRARIES(ImportGenerator ${MXADATAMODEL_LIB_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} )

# - Installation Code for the binary
INSTALL(TARGETS ImportGenerator RUNTIME DESTINATION bin/)
        
SET (IMPORTGENERATOR_INSTALLED_RESOURCES
        ${UTILITIES_DIR}/Templates/CMakeLists.txt.in
        ${UTILITIES_DIR}/Templates/ImportDelegate.cpp.in
        ${UTILITIES_DIR}/Templates/ImportDelegate.h.in
        ${UTILITIES_DIR}/Templates/ImportDelegateFactory.cpp.in
        ${UTILITIES_DIR}/Templates/ImportDelegateFactory.h.in
        ${UTILITIES_DIR}/Templates/main.cpp.in
)

INSTALL (FILES ${IMPORTGENERATOR_INSTALLED_RESOURCES} 
        DESTINATION share/MXADataModel/Resources/Templates
        COMPONENT Resources)

INSTALL (FILES ${MXA_CMAKE_DIR}/FindMXADataModel.cmake
        DESTINATION share/MXADataModel/Resources/CMake
        COMPONENT Resources
        )
        
