# ------------------------------------------------------------------------------ 
# This CMake code installs the needed support libraries on NON OSX platforms
# ------------------------------------------------------------------------------ 

#------- Macro to find and install the proper dynamic libraries ----------------
MACRO (MXA_INSTALL_SUPPORT_LIBRARY basename)
    IF (HAVE_${basename}_DLL)
      IF (WIN32)
        GET_FILENAME_COMPONENT(${basename}_LIB_DEBUG_NAME ${${basename}_LIBRARY_DEBUG} NAME_WE)
        GET_FILENAME_COMPONENT(${basename}_LIB_RELEASE_NAME ${${basename}_LIBRARY_RELEASE} NAME_WE)
        INSTALL(FILES ${${basename}_BIN_DIR}/${${basename}_LIB_DEBUG_NAME}.dll 
            DESTINATION bin 
            CONFIGURATIONS Debug 
            COMPONENT Runtime)
        INSTALL(FILES ${${basename}_BIN_DIR}/${${basename}_LIB_RELEASE_NAME}.dll 
            DESTINATION bin 
            CONFIGURATIONS Release 
            COMPONENT Runtime)
      ENDIF (WIN32)
    ENDIF (HAVE_${basename}_DLL)
ENDMACRO(MXA_INSTALL_SUPPORT_LIBRARY basename)

#-- Install the support libraries if needed
IF ( MXA_USE_HDF5 )
MXA_INSTALL_SUPPORT_LIBRARY(HDF5)
ENDIF ( MXA_USE_HDF5 )

IF (MXA_USE_XML)
MXA_INSTALL_SUPPORT_LIBRARY(EXPAT)
ENDIF (MXA_USE_XML)

IF(MXA_USE_TIFF)
MXA_INSTALL_SUPPORT_LIBRARY(TIFF)
ENDIF(MXA_USE_TIFF)

# -------------------------- Find the MinGW Runtime library --------------------
IF (MINGW)
    INCLUDE ( ${MXA_RESOURCES_DIR}/MXAFindMinGW.cmake )
    IF (MINGW_FOUND)
        INSTALL(FILES ${MINGW_LIBRARIES}
                DESTINATION bin
                COMPONENT Runtime)
    ENDIF (MINGW_FOUND)
ENDIF (MINGW)
