# IF YOU CHANGE THIS FILE, MAKE SURE YOU UPDATE THE UseMXADataModel.cmake.in FILE ALSO.

IF ( NOT DEFINED  MXA_RESOURCES_DIR)
  SET (MXA_RESOURCES_DIR ${MXA_SOURCE_DIR}/Resources)
ENDIF ( NOT DEFINED  MXA_RESOURCES_DIR)

# ---------- Find Boost Headers/Libraries -----------------------
SET (Boost_FIND_REQUIRED TRUE)
SET (Boost_FIND_QUIETLY TRUE)
set (Boost_USE_MULTITHREADED TRUE)
set (Boost_USE_STATIC_LIBS TRUE)
SET (Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0")

if ( NOT MXA_BOOST_HEADERS_ONLY)
 set (MXA_BOOST_COMPONENTS program_options unit_test_framework test_exec_monitor)
endif()

FIND_PACKAGE(Boost COMPONENTS  ${MXA_BOOST_COMPONENTS} )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

#-------------------------------------------------------------------------------
#- Find Expat Library -------------
SET (MXA_XML_SUPPORT "0")
IF (NOT DEFINED MXA_USE_XML)
    OPTION(MXA_USE_XML "Adds Ability to read/write models to XML Files" ON)
ENDIF (NOT DEFINED MXA_USE_XML)

IF (MXA_USE_XML)
    IF (NOT DEFINED MXA_EXTERNAL_EXPAT_LIBRARY)
        INCLUDE ( ${MXA_CMAKE_DIR}/MXAFindExpat.cmake)
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
ENDIF (NOT DEFINED MXA_USE_HDF5)
IF ( MXA_USE_HDF5 )
  # ---------- DO NOT use the High Level APIs from the HDF5
  # group as those API's will not compile correctly under windows --------------
  # MXA_EXTERNAL_HDF5_LIBRARY is meant to be passed in from an external project
  # like a ParaView plugin that is building MXA and provides its own HDF5 implementation
  SET (HDF5_USE_HIGH_LEVEL FALSE)
  IF (NOT DEFINED MXA_EXTERNAL_HDF5_LIBRARY)
      INCLUDE( ${MXA_CMAKE_DIR}/MXAFindHDF5.cmake )
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
ENDIF (NOT DEFINED MXA_USE_TIFF)

IF(MXA_USE_TIFF)
    IF (NOT DEFINED MXA_EXTERNAL_TIFF_LIBRARY)
        INCLUDE ( ${MXA_CMAKE_DIR}/MXAFindTiff.cmake)
    ENDIF (NOT DEFINED MXA_EXTERNAL_TIFF_LIBRARY)
    IF (NOT TIFF_FOUND)
      MESSAGE (FATAL_ERROR " Tiff Library was NOT Found and is needed.")
    ENDIF (NOT TIFF_FOUND)
    INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})
    SET(DEP_LIBS ${DEP_LIBS} ${TIFF_LIBRARIES})
ENDIF(MXA_USE_TIFF)
 


    