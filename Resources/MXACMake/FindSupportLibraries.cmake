# IF YOU CHANGE THIS FILE, MAKE SURE YOU UPDATE THE UseMXADataModel.cmake.in FILE ALSO.

include ( ${MXA_CMAKE_DIR}/LocateBoostSupportLibraries.cmake )

#-------------------------------------------------------------------------------
#- Find Expat Library -------------
SET (MXA_XML_SUPPORT "0" CACHE INTERNAL "")
IF (NOT DEFINED MXA_USE_XML)
    OPTION(MXA_USE_XML "Adds Ability to read/write models to XML Files" ON)
    mark_as_advanced(MXA_USE_XML)
ENDIF (NOT DEFINED MXA_USE_XML)

IF (MXA_USE_XML)
    IF (NOT DEFINED MXA_EXTERNAL_EXPAT_LIBRARY)
        INCLUDE ( ${CMP_MODULES_SOURCE_DIR}/FindExpat.cmake)
    ENDIF (NOT DEFINED MXA_EXTERNAL_EXPAT_LIBRARY)
    IF (NOT EXPAT_FOUND)
        MESSAGE (FATAL_ERROR "Expat Library was NOT Found and is needed.")
    ENDIF (NOT EXPAT_FOUND)
    INCLUDE_DIRECTORIES( ${EXPAT_INCLUDE_DIRS} )
    SET(DEP_LIBS ${DEP_LIBS} ${EXPAT_LIBRARIES})
ENDIF (MXA_USE_XML)

#-------------------------------------------------------------------------------
#- Find the HDF5 library ------------------------------------------------
#- Build with the HDF5 Libraries Enabled. This is the default
SET(MXA_HDF5_SUPPORT "0")
SET(MXA_USE_HDF5_PRIMITIVE_TYPES)
SET(H5LITE_USE_MXA_CONSTRUCTS)
SET(HDF5_VTK_BRIDGE "0")
IF (NOT DEFINED MXA_USE_HDF5)
    OPTION (MXA_USE_HDF5 "Compiles Sources dependent on HDF5 Libraries" ON)
    mark_as_advanced(MXA_USE_HDF5)
ENDIF (NOT DEFINED MXA_USE_HDF5)
IF ( MXA_USE_HDF5 )
  # ---------- DO NOT use the High Level APIs from the HDF5
  # group as those API's will not compile correctly under windows --------------
  # MXA_EXTERNAL_HDF5_LIBRARY is meant to be passed in from an external project
  # like a ParaView plugin that is building MXA and provides its own HDF5 implementation
  SET (HDF5_USE_HIGH_LEVEL FALSE)
  IF (NOT DEFINED MXA_EXTERNAL_HDF5_LIBRARY)
      INCLUDE( ${CMP_MODULES_SOURCE_DIR}/FindHDF5.cmake )
  ENDIF (NOT DEFINED MXA_EXTERNAL_HDF5_LIBRARY)
  IF(NOT HDF5_FOUND)
    MESSAGE(FATAL_ERROR " HDF5 was not found. Set the include/lib manually or set the HDF_INSTALL environment variable")
  ENDIF(NOT HDF5_FOUND)
  INCLUDE_DIRECTORIES( ${HDF5_INCLUDE_DIRS} )
  SET(DEP_LIBS ${DEP_LIBS} ${HDF5_LIBRARIES})
  SET(MXA_USE_HDF5_PRIMITIVE_TYPES 1)
  SET(H5LITE_USE_MXA_CONSTRUCTS 1)
ENDIF ( MXA_USE_HDF5 )   

#-------------------------------------------------------------------------------
# Find TIFF Library 
SET (MXA_TIFF_SUPPORT "0")
IF (NOT DEFINED MXA_USE_TIFF)
    OPTION(MXA_USE_TIFF "Compiles source code that depends on having the Tif Lib installed" ON)
    mark_as_advanced(MXA_USE_TIFF)
ENDIF (NOT DEFINED MXA_USE_TIFF)

IF(MXA_USE_TIFF)
    IF (NOT DEFINED MXA_EXTERNAL_TIFF_LIBRARY)
        INCLUDE ( ${CMP_MODULES_SOURCE_DIR}/FindTiff.cmake)
    ENDIF (NOT DEFINED MXA_EXTERNAL_TIFF_LIBRARY)
    IF (NOT TIFF_FOUND)
      MESSAGE (FATAL_ERROR " Tiff Library was NOT Found and is needed.")
    ENDIF (NOT TIFF_FOUND)
    INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})
    SET(DEP_LIBS ${DEP_LIBS} ${TIFF_LIBRARIES})
ENDIF(MXA_USE_TIFF)
 


    