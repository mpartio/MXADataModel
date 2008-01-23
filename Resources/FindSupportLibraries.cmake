# IF YOU CHANGE THIS FILE, MAKE SURE YOU UPDATE THE UseMXADataModel.cmake.in FILE ALSO.

IF ( NOT DEFINED  MXA_RESOURCES_DIR)
  SET (MXA_RESOURCES_DIR ${MXA_SOURCE_DIR}/Resources)
ENDIF ( NOT DEFINED  MXA_RESOURCES_DIR)

# ---------- Find Boost Headers/Libraries -----------------------
SET (Boost_FIND_REQUIRED TRUE)
SET (Boost_FIND_QUIETLY TRUE)
SET (BOOST_USE_FILESYSTEM TRUE)
INCLUDE (${MXA_RESOURCES_DIR}/MXAFindBoost.cmake)
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})  # Include the Boost Headers
SET(DEP_LIBS ${DEP_LIBS} ${BOOST_FILESYSTEM_LIBRARY})
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS}) 

#---- Find Expat -------------
SET (XML_SUPPORT "0")
OPTION(MXA_USE_XML "Adds Ability to read/write models to XML Files" ON)
IF (MXA_USE_XML)
    INCLUDE ( ${MXA_RESOURCES_DIR}/MXAFindExpat.cmake)
    IF (NOT EXPAT_FOUND)
      MESSAGE (FATAL_ERROR "Expat Library was NOT Found and is needed.")
    ENDIF (NOT EXPAT_FOUND)
    INCLUDE_DIRECTORIES( ${EXPAT_INCLUDE_DIRS} )
    SET(DEP_LIBS ${DEP_LIBS} ${EXPAT_LIBRARIES})
ENDIF (MXA_USE_XML)


#--- Find the HDF5 library ------------------------------------------------
# Build with the HDF5 Libraries Enabled. This is the default
SET(HDF5_SUPPORT "0")
SET(HDF5_VTK_BRIDGE "0")
OPTION (MXA_USE_HDF5 "Compiles Sources dependent on HDF5 Libraries" ON)
IF ( MXA_USE_HDF5 )
  # ---------- DO NOT use the High Level APIs from the HDF5
  # group as those API's will not compile correctly under windows --------------
  SET (HDF5_USE_HIGH_LEVEL FALSE)
  INCLUDE( ${MXA_RESOURCES_DIR}/MXAFindHDF5.cmake )
  IF(NOT HDF5_FOUND)
    MESSAGE(FATAL_ERROR " HDF5 was not found. Set the include/lib manually or set the HDF_INSTALL environment variable")
  ENDIF(NOT HDF5_FOUND)
  INCLUDE_DIRECTORIES( ${HDF5_INCLUDE_DIRS} )
  SET(DEP_LIBS ${DEP_LIBS} ${HDF5_LIBRARIES})
ENDIF ( MXA_USE_HDF5 )   
 
#---- Find TIFF -------------------------------------------------------------
SET (TIFF_SUPPORT "0")
OPTION(MXA_USE_TIFF "Compiles source code that depends on having the Tif Lib installed" ON)
IF(MXA_USE_TIFF)
    INCLUDE ( ${MXA_RESOURCES_DIR}/MXAFindTiff.cmake)
    IF (NOT TIFF_FOUND)
      MESSAGE (FATAL_ERROR " Tiff Library was NOT Found and is needed.")
    ENDIF (NOT TIFF_FOUND)
    INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})
    SET(DEP_LIBS ${DEP_LIBS} ${TIFF_LIBRARIES})
ENDIF(MXA_USE_TIFF)
 
   
