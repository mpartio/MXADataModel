# - Find TIFF library
# - Derived from the FindTiff.cmake that is included with cmake
# Find the native TIFF includes and library
# This module defines
#  TIFF_INCLUDE_DIR, where to find tiff.h, etc.
#  TIFF_LIBRARIES, libraries to link against to use TIFF.
#  TIFF_FOUND, If false, do not try to use TIFF.
# also defined, but not for general use are
#  TIFF_LIBRARY, where to find the TIFF library.
#  TIFF_LIBRARY_DEBUG - Debug version of tiff library
#  TIFF_LIBRARY_RELEASE - Release Version of tiff library

# MESSAGE (STATUS "Finding Tiff library and headers..." )

# Look for the header file.
SET(TIFF_INCLUDE_SEARCH_DIRS
  $ENV{TIFF_INSTALL}/include
)

SET (TIFF_LIB_SEARCH_DIRS
  $ENV{TIFF_INSTALL}/lib
  )

FIND_PATH(TIFF_INCLUDE_DIR 
  NAMES tiff.h
  PATHS ${TIFF_INCLUDE_SEARCH_DIRS} 
  NO_DEFAULT_PATH
)

IF (WIN32)
    SET (TIFF_SEARCH_DEBUG_NAMES "tiff_D;libtiff_D")
    SET (TIFF_SEARCH_RELEASE_NAMES "tiff;libtiff")
ELSE (WIN32)
    SET (TIFF_SEARCH_DEBUG_NAMES "tiff")
    SET (TIFF_SEARCH_RELEASE_NAMES "tiff")
ENDIF(WIN32)

# Look for the library.
FIND_LIBRARY(TIFF_LIBRARY_DEBUG 
  NAMES ${TIFF_SEARCH_DEBUG_NAMES}
  PATHS ${TIFF_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )
  
FIND_LIBRARY(TIFF_LIBRARY_RELEASE 
  NAMES ${TIFF_SEARCH_RELEASE_NAMES}
  PATHS ${TIFF_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )

# MESSAGE(STATUS "TIFF_INCLUDE_DIR: ${TIFF_INCLUDE_DIR}")
# MESSAGE(STATUS "TIFF_LIBRARY_DEBUG: ${TIFF_LIBRARY_DEBUG}")
# MESSAGE(STATUS "TIFF_LIBRARY_RELEASE: ${TIFF_LIBRARY_RELEASE}")
# MESSAGE(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

IF (NOT MXA_RESOURCES_DIR)
  GET_FILENAME_COMPONENT(MXA_RESOURCES_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDIF (NOT MXA_RESOURCES_DIR)

# include the macro to adjust libraries
INCLUDE (${MXA_RESOURCES_DIR}/MXAAdjustLibVars.cmake) 
_MXA_ADJUST_LIB_VARS(TIFF)

IF(TIFF_INCLUDE_DIR AND TIFF_LIBRARY)
  SET(TIFF_FOUND 1)
  SET(TIFF_LIBRARIES ${TIFF_LIBRARY})
  SET(TIFF_INCLUDE_DIRS ${TIFF_INCLUDE_DIR})
ELSE(TIFF_INCLUDE_DIR AND TIFF_LIBRARY)
  SET(TIFF_FOUND 0)
  SET(TIFF_LIBRARIES)
  SET(TIFF_INCLUDE_DIRS)
ENDIF(TIFF_INCLUDE_DIR AND TIFF_LIBRARY)

# Report the results.
IF(NOT TIFF_FOUND)
  SET(TIFF_DIR_MESSAGE
    "Tiff was not found. Make sure TIFF_LIBRARY and TIFF_INCLUDE_DIR are set.")
  IF(NOT TIFF_FIND_QUIETLY)
    MESSAGE(STATUS "${TIFF_DIR_MESSAGE}")
  ELSE(NOT TIFF_FIND_QUIETLY)
    IF(TIFF_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Tiff was NOT found and is Required by this project")
    ENDIF(TIFF_FIND_REQUIRED)
  ENDIF(NOT TIFF_FIND_QUIETLY)
ENDIF(NOT TIFF_FOUND)

