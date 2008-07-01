# This will find the HDF5 libraries and define the following variables:
#  HDF5_FOUND - Is the base HDF5 library and include file found
#  HDF5_INCLUDE_DIR - The Include directory containing all the HDF5 include files
#  HDF5_LIBRARY - The Actual HDF5 library
#  HDF5_HL_FOUND - Is the High Level HDF5 API Found
#  HDF5_HL_INCLUDE_DIR - The High Level Include Directory
#  HDF5_HL_LIBRARY - The Actual High Level Library
#  HDF5_USE_HIGH_LEVEL - Set this to TRUE if you need to link against the HDF5 High level APIs.
#  HDF5_INSTALL - This is an Environment variable that can be used to help find the HDF5 libraries and Include Directories
#  HDF5_LIBRARIES - The List of HDF5 libraries that were found. This variable can be used in a LINK_LIBRARIES(...) command
#  HDF5_LIBRARY_DEBUG - Debug Version of HDF5 library
#  HDF5_LIBRARY_RELEASE - Release Version of HDF5 library

# MESSAGE(STATUS "Finding HDF5 Library... " )

#-- Clear the Library List 
# SET (HDF5_LIBRARIES "")
#-- Clear the include Directories
# SET (HDF5_INCLUDE_DIRS "")

# ------------------ START FIND HDF5 LIBS --------------------------------------
SET (HDF5_FOUND "NO")
SET (HDF5_HL_FOUND "NO")
SET (HDF5_INSTALL  $ENV{HDF5_INSTALL})

SET(HDF5_INCLUDE_SEARCH_DIRS
  $ENV{HDF5_INSTALL}/include
)

SET (HDF5_LIB_SEARCH_DIRS
  $ENV{HDF5_INSTALL}/lib
)

SET (HDF5_BIN_SEARCH_DIRS
  $ENV{HDF5_INSTALL}/bin
)

# -- Find the Include directory for HDF5
FIND_PATH(HDF5_INCLUDE_DIR 
  NAMES hdf5.h
  PATHS ${HDF5_INCLUDE_SEARCH_DIRS}
  NO_DEFAULT_PATH
)

IF (WIN32 AND NOT MINGW)
    SET (HDF5_SEARCH_DEBUG_NAMES "hdf5_D;libhdf5_D;hdf5dll_D")
    SET (HDF5_SEARCH_RELEASE_NAMES "hdf5;libhdf5;hdf5dll")
ELSE (WIN32 AND NOT MINGW)
    SET (HDF5_SEARCH_DEBUG_NAMES "hdf5_debug")
    SET (HDF5_SEARCH_RELEASE_NAMES "hdf5")
ENDIF(WIN32 AND NOT MINGW)

# Look for the library.
FIND_LIBRARY(HDF5_LIBRARY_DEBUG 
  NAMES ${HDF5_SEARCH_DEBUG_NAMES}
  PATHS ${HDF5_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )
  
FIND_LIBRARY(HDF5_LIBRARY_RELEASE 
  NAMES ${HDF5_SEARCH_RELEASE_NAMES}
  PATHS ${HDF5_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )
  
SET (HDF5_DUMP_PROG_NAME "h5dump")
IF (WIN32)
    SET (HDF5_DUMP_PROG_NAME "h5dump.exe")
ENDIF(WIN32)

FIND_PROGRAM(HDF5_DUMP_PROG
    NAMES ${HDF5_DUMP_PROG_NAME}
    PATHS ${HDF5_BIN_SEARCH_DIRS} 
    NO_DEFAULT_PATH
)
MARK_AS_ADVANCED(HDF5_DUMP_PROG)


IF (NOT MXA_RESOURCES_DIR)
  GET_FILENAME_COMPONENT(MXA_RESOURCES_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDIF (NOT MXA_RESOURCES_DIR)

# include the macro to adjust libraries
INCLUDE (${MXA_RESOURCES_DIR}/MXAAdjustLibVars.cmake)
_MXA_ADJUST_LIB_VARS(HDF5)

#IF(NOT HDF5_FIND_QUIETLY)
# MESSAGE(STATUS "HDF5_INCLUDE_SEARCH_DIRS: ${HDF5_INCLUDE_SEARCH_DIRS}")
# MESSAGE(STATUS "HDF5_LIB_SEARCH_DIRS: ${HDF5_LIB_SEARCH_DIRS}")
# MESSAGE(STATUS "HDF5_INCLUDE_DIR: ${HDF5_INCLUDE_DIR}")
# MESSAGE(STATUS "HDF5_LIBRARY_DEBUG: ${HDF5_LIBRARY_DEBUG}")
# MESSAGE(STATUS "HDF5_LIBRARY: ${HDF5_LIBRARY}")
# MESSAGE(STATUS "HDF5_LIBRARY_RELEASE: ${HDF5_LIBRARY_RELEASE}")
#ENDIF(NOT HDF5_FIND_QUIETLY)

IF(HDF5_INCLUDE_DIR AND HDF5_LIBRARY)
  SET(HDF5_FOUND 1) 
  SET(HDF5_LIBRARIES ${HDF5_LIBRARY})
  SET(HDF5_INCLUDE_DIRS ${HDF5_INCLUDE_DIR})
  IF (HDF5_LIBRARY_DEBUG)
    GET_FILENAME_COMPONENT(HDF5_LIBRARY_PATH ${HDF5_LIBRARY_DEBUG} PATH)
    SET(HDF5_LIB_DIR  ${HDF5_LIBRARY_PATH})
  ELSEIF(HDF5_LIBRARY_RELEASE)
    GET_FILENAME_COMPONENT(HDF5_LIBRARY_PATH ${HDF5_LIBRARY_RELEASE} PATH)
    SET(HDF5_LIB_DIR  ${HDF5_LIBRARY_PATH})
  ENDIF(HDF5_LIBRARY_DEBUG)
  
  IF (HDF5_DUMP_PROG)
    GET_FILENAME_COMPONENT(HDF5_BIN_PATH ${HDF5_DUMP_PROG} PATH)
    SET(HDF5_BIN_DIR  ${HDF5_BIN_PATH})
  ENDIF (HDF5_DUMP_PROG)
  
ELSE(HDF5_INCLUDE_DIR AND HDF5_LIBRARY)
  SET(HDF5_FOUND 0)
  SET(HDF5_LIBRARIES)
  SET(HDF5_INCLUDE_DIRS)
ENDIF(HDF5_INCLUDE_DIR AND HDF5_LIBRARY)

# Report the results.
IF(NOT HDF5_FOUND)
  SET(HDF5_DIR_MESSAGE
    "HDF5 was not found. Make sure HDF5_LIBRARY and HDF5_INCLUDE_DIR are set.")
  IF(NOT HDF5_FIND_QUIETLY)
    MESSAGE(STATUS "${HDF5_DIR_MESSAGE}")
  ELSE(NOT HDF5_FIND_QUIETLY)
    IF(HDF5_FIND_REQUIRED)
      # MESSAGE(FATAL_ERROR "${HDF5_DIR_MESSAGE}")
      MESSAGE(FATAL_ERROR "Expat was NOT found and is Required by this project")
    ENDIF(HDF5_FIND_REQUIRED)
  ENDIF(NOT HDF5_FIND_QUIETLY)
ENDIF(NOT HDF5_FOUND)


# ------------------ START FIND HDF5 HL LIBS -----------------------------------
IF ( HDF5_USE_HIGH_LEVEL )

  FIND_PATH(HDF5_HL_INCLUDE_DIR 
    NAMES H5LT.h
    PATHS ${HDF5_INCLUDE_SEARCH_DIRS}
    #  NO_DEFAULT_PATH
  )
  
  FIND_LIBRARY(HDF5_HL_LIBRARY 
    NAMES hdf5_hl
    PATHS ${HDF5_LIB_SEARCH_DIRS}
    #  NO_DEFAULT_PATH
  )

  IF (HDF5_HL_INCLUDE_DIR )
    IF (HDF5_HL_LIBRARY)
      SET (HDF5_HL_FOUND "YES")
      SET (HDF5_LIBRARIES ${HDF5_HL_LIBRARY} ${HDF5_LIBRARIES} )
      INCLUDE_DIRECTORIES( ${HDF5_HL_INCLUDE_DIR} )
    ELSE (HDF5_HL_LIBRARY)
      SET (HDF5_HL_FOUND "NO")
    ENDIF (HDF5_HL_LIBRARY)
  ENDIF (HDF5_HL_INCLUDE_DIR)
ENDIF ( HDF5_USE_HIGH_LEVEL )


IF (HDF5_FOUND)
  INCLUDE(CheckSymbolExists)
  #############################################
  # Find out if HDF5 was build using dll's
  #############################################
  # Save required variable
  SET(CMAKE_REQUIRED_INCLUDES_SAVE ${CMAKE_REQUIRED_INCLUDES})
  SET(CMAKE_REQUIRED_FLAGS_SAVE    ${CMAKE_REQUIRED_FLAGS})
  # Add HDF5_INCLUDE_DIR to CMAKE_REQUIRED_INCLUDES
  SET(CMAKE_REQUIRED_INCLUDES "${CMAKE_REQUIRED_INCLUDES};${HDF5_INCLUDE_DIRS}")

  CHECK_SYMBOL_EXISTS(HDF5_BUILT_AS_DYNAMIC_LIB "H5config.h" HAVE_HDF5_DLL)

    IF (HAVE_HDF5_DLL STREQUAL "TRUE")
        SET (HAVE_HDF5_DLL "1")
    ENDIF (HAVE_HDF5_DLL STREQUAL "TRUE")

  # Restore CMAKE_REQUIRED_INCLUDES and CMAKE_REQUIRED_FLAGS variables
  SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES_SAVE})
  SET(CMAKE_REQUIRED_FLAGS    ${CMAKE_REQUIRED_FLAGS_SAVE})
  #
  #############################################

ENDIF (HDF5_FOUND)
