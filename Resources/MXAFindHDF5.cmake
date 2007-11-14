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

#-- Clear the Library List 
SET (HDF5_LIBRARIES "")
#-- Clear the include Directories
SET (HDF5_INCLUDE_DIRS "")

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

# -- Find the Include directory for HDF5
FIND_PATH(HDF5_INCLUDE_DIR 
  NAMES hdf5.h
  PATHS ${HDF5_INCLUDE_SEARCH_DIRS}
#  NO_DEFAULT_PATH
)


SET (HDF5_SEARCH_LIBRARY "hdf5")
#--- By Default try to find the Dynamic Link Library on Windows
IF(WIN32)
  SET (HDF5_SEARCH_LIBRARY "hdf5dll")
ENDIF(WIN32)

FIND_LIBRARY(HDF5_LIBRARY 
  NAMES ${HDF5_SEARCH_LIBRARY} hdf5
  PATHS ${HDF5_LIB_SEARCH_DIRS}
#  NO_DEFAULT_PATH
)

IF (HDF5_INCLUDE_DIR)
  IF (HDF5_LIBRARY)
    SET (HDF5_FOUND "YES")
    SET (HDF5_LIBRARIES ${HDF5_LIBRARY} ${HDF5_LIBRARIES} )
    INCLUDE_DIRECTORIES( ${HDF5_INCLUDE_DIR} )
  ELSE (HDF5_LIBRARY)
    SET (HDF5_FOUND "NO")
  ENDIF (HDF5_LIBRARY)
ENDIF (HDF5_INCLUDE_DIR)


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

