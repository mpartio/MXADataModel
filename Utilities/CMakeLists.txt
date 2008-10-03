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

