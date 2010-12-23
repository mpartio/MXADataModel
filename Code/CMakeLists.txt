project(MXALib)

set (MXALib_PROJECT_FILES "")
# --------------------------------------------------------------------
Macro(Add_MXALib_Dir DirName)
    include(${MXALib_SOURCE_DIR}/MXA/${DirName}/SourceList.cmake)
    set (MXALib_PROJECT_FILES ${MXALib_PROJECT_FILES}
                                        ${MXA_${DirName}_SRCS}
                                        ${MXA_${DirName}_HDRS} )
endMacro()

Add_MXALib_Dir(Base)
Add_MXALib_Dir(BMPIO)
Add_MXALib_Dir(Common)
Add_MXALib_Dir(Core)
Add_MXALib_Dir(DataImport)
Add_MXALib_Dir(DataWrappers)
Add_MXALib_Dir(HDF5)
Add_MXALib_Dir(Utilities)
Add_MXALib_Dir(XML)

# --------------------------------------------------------------------
# Include the generated Headers
SET (MXALib_PROJECT_FILES 
    ${MXALib_PROJECT_FILES}
    ${MXADataModel_BINARY_DIR}/${CMP_PROJECT_PREFIX}/MXAVersion.h
    ${MXADataModel_BINARY_DIR}/${CMP_PROJECT_PREFIX}/MXAConfiguration.h
    ${MXADataModel_BINARY_DIR}/${CMP_PROJECT_PREFIX}/MXATypes.h 
)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")


IF (BUILD_SHARED_LIBS)
    SET (MXA_BUILT_AS_DYNAMIC_LIB "1" CACHE INTERNAL "")
endif()


# --------------------------------------------------------------------
# Generate our header files
# --------------------------------------------------------------------
configure_file(${MXADataModel_SOURCE_DIR}/License.txt
                ${MXADataModel_BINARY_DIR}/License.txt )
configure_file(${MXADataModel_SOURCE_DIR}/ReadMe.txt
                ${MXADataModel_BINARY_DIR}/ReadMe.txt )
configure_file(${PROJECT_RESOURCES_DIR}/MXACMake/MXAConfiguration.h.in
               ${CMP_HEADER_DIR}/MXAConfiguration.h)
SET(MXADATAMODEL_INCLUDE_DIRS "${PROJECT_CODE_DIR};${MXADataModel_BINARY_DIR}"  )


# --------------------------------------------------------------------
# -- Add the MXADataModel Library and link to needed libraries 
# --------------------------------------------------------------------
ADD_LIBRARY (${MXADATAMODEL_LIB_NAME} ${LIB_TYPE} ${MXALib_PROJECT_FILES} )
TARGET_LINK_LIBRARIES (${MXADATAMODEL_LIB_NAME} ${DEP_LIBS} )
LibraryProperties( ${MXADATAMODEL_LIB_NAME} ${EXE_DEBUG_EXTENSION})

if ( ${MXA_INSTALL_FILES} EQUAL 1 )
    INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin 
        COMPONENT Runtime)
else()
# Include an install rule for NON-Apple systems. The specialized installer that ParaView uses
# will take care of copying the MXADataModel library into the proper location.
    INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME}
            EXPORT ${PV_INSTALL_EXPORT_NAME}
          RUNTIME DESTINATION ${PV_INSTALL_BIN_DIR} COMPONENT Runtime
          LIBRARY DESTINATION ${PV_INSTALL_LIB_DIR} COMPONENT Runtime
          ARCHIVE DESTINATION ${PV_INSTALL_LIB_DIR} COMPONENT Development)
endif()

set (lib_rt_dest lib)
if (WIN32 AND BUILD_SHARED_LIBS)
  set (lib_rt_dest bin)
endif()


#-----
IF (MINGW)
    SET (CMAKE_COLOR_MAKEFILE 0)
endif()

if (  ${MXA_INSTALL_FILES} EQUAL 1 )
    # Copy DLL libraries into the correct build location on MSVC platforms
    CMP_COPY_DEPENDENT_LIBRARIES ("tiff;expat;hdf5")
    
    # Generate Install Rules for the DLL Libraries
    CMP_LIBRARIES_INSTALL_RULES ("tiff;expat;hdf5" "bin" )
endif()
