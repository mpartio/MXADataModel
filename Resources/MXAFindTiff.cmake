# - Find TIFF library
# - Derived from the FindTiff.cmake that is included with cmake
# Find the native TIFF includes and library
# This module defines
#  TIFF_INCLUDE_DIR, where to find tiff.h, etc.
#  TIFF_LIBRARIES, libraries to link against to use TIFF.
#  TIFF_FOUND, If false, do not try to use TIFF.
# also defined, but not for general use are
#  TIFF_LIBRARY, where to find the TIFF library.

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

SET(TIFF_NAMES ${TIFF_NAMES} tiff)

FIND_LIBRARY(TIFF_LIBRARY
  NAMES ${TIFF_NAMES}
  PATHS ${TIFF_LIB_SEARCH_DIRS}
  NO_DEFAULT_PATH
)

IF(TIFF_INCLUDE_DIR)
  IF(TIFF_LIBRARY)
    SET( TIFF_FOUND "YES" )
    SET( TIFF_LIBRARIES ${TIFF_LIBRARY} )
    MARK_AS_ADVANCED(${TIFF_LIBRARY})
    MARK_AS_ADVANCED(${TIFF_INCLUDE_DIR})
  ENDIF(TIFF_LIBRARY)
ENDIF(TIFF_INCLUDE_DIR)

