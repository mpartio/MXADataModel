#///////////////////////////////////////////////////////////////////////////////
#  Copyright (c) 2010, Michael A. Jackson. BlueQuartz Software
#  All rights reserved.
#  BSD License: http://www.opensource.org/licenses/bsd-license.html
#///////////////////////////////////////////////////////////////////////////////

project(MXA)


SET (MXA_BUILT_AS_DYNAMIC_LIB)
IF (BUILD_SHARED_LIBS)
    SET (MXA_BUILT_AS_DYNAMIC_LIB 1)
    IF (WIN32)
        ADD_DEFINITIONS("-DMXA_BUILT_AS_DYNAMIC_LIB")
    ENDIF (WIN32)
ENDIF (BUILD_SHARED_LIBS)

SET (PROJECT_CODE_DIR ${PROJECT_SOURCE_DIR}/Source)
if (NOT PROJECT_RESOURCES_DIR)
SET (PROJECT_RESOURCES_DIR ${MXA_SOURCE_DIR}/Resources)
endif()
if (NOT DEFINED CMP_SOURCE_DIR)
set(CMP_SOURCE_DIR ${MXA_SOURCE_DIR}/Support/cmp)
endif()

SET (PROJECT_PREFIX "MXA" CACHE STRING "The Prefix to be used for Preprocessor definitions")
set (MXA_INSTALL_FILES 0)
#-------------------------------------------------------------------------------
# Include the cmp project to perform all the basic configuration tests for the 
# project.
#------------------------------------------------------------------------------- 
set (CMP_HEADER_DIR ${MXA_BINARY_DIR})
set (CMP_CONFIGURATION_FILE_NAME "MXAConfiguration.h")
set (CMP_TYPES_FILE_NAME "MXATypes.h")
set (CMP_VERSION_HEADER_FILE_NAME "MXAVersion.h")
set (CMP_VERSION_SOURCE_FILE_NAME "MXAVersion.cpp")
set (CMP_TOP_HEADER_FILE "MXA.h")
set (CMP_PROJECT_NAMESPACE "MXA")
set (CMP_PROJECT_NAME "MXA")
set (CMP_ENABLE_PLUGINS "0")
set (CMP_LIB_SEARCH_DIRS "")
INCLUDE (${CMP_SOURCE_DIR}/cmpProject.cmake)



set (MXA_PROJECT_FILES "")
# --------------------------------------------------------------------
Macro(Add_MXA_Dir DirName)
    include(${MXA_SOURCE_DIR}/MXA/${DirName}/SourceList.cmake)
    set (MXA_PROJECT_FILES ${MXA_PROJECT_FILES}
                                        ${MXA_${DirName}_SRCS}
                                        ${MXA_${DirName}_HDRS} )
endMacro()

Add_MXA_Dir(Base)
Add_MXA_Dir(BMPIO)
Add_MXA_Dir(Common)
Add_MXA_Dir(Core)
Add_MXA_Dir(DataImport)
Add_MXA_Dir(DataWrappers)
Add_MXA_Dir(HDF5)
Add_MXA_Dir(Utilities)
Add_MXA_Dir(XML)

# --------------------------------------------------------------------
# Generate a Header file with Compile Version variables
# --------------------------------------------------------------------
cmpVersionStringsFromGit(GENERATED_HEADER_FILE_PATH ${CMP_HEADER_DIR}/${CMP_VERSION_HEADER_FILE_NAME} 
                         GENERATED_SOURCE_FILE_PATH ${CMP_HEADER_DIR}/${CMP_VERSION_SOURCE_FILE_NAME}
                         NAMESPACE "MXA"
                         cmpProjectName "MXA" 
                         WORKING_DIRECTORY ${MXADataModel_SOURCE_DIR}
                         EXPORTS_DEFINE  MXA_EXPORT)
                         

configure_file(${MXA_SOURCE_DIR}/MXAConfiguration.h.in
               ${CMP_HEADER_DIR}/${CMP_TOP_HEADER_FILE})

cmp_IDE_GENERATED_PROPERTIES("Generated" 
                             "${CMP_HEADER_DIR}/${CMP_TOP_HEADER_FILE};${CMP_HEADER_DIR}/${CMP_TYPES_FILE_NAME};${CMP_HEADER_DIR}/${CMP_CONFIGURATION_FILE_NAME};${CMP_HEADER_DIR}/${CMP_VERSION_HEADER_FILE_NAME}"
                             "${CMP_HEADER_DIR}/${CMP_VERSION_SOURCE_FILE_NAME}")


      
      
# --------------------------------------------------------------------
# Include the generated Headers
SET (MXA_PROJECT_FILES 
    ${MXA_PROJECT_FILES}
    ${MXA_BINARY_DIR}/${CMP_CONFIGURATION_FILE_NAME}
    ${MXA_BINARY_DIR}/${CMP_TYPES_FILE_NAME}
    ${MXA_BINARY_DIR}/${CMP_VERSION_HEADER_FILE_NAME}
    ${MXA_BINARY_DIR}/${CMP_VERSION_SOURCE_FILE_NAME}
    ${MXA_BINARY_DIR}/${CMP_TOP_HEADER_FILE}
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
#configure_file(${PROJECT_RESOURCES_DIR}/MXACMake/MXAConfiguration.h.in
#               ${CMP_HEADER_DIR}/MXAConfiguration.h)
SET(MXADATAMODEL_INCLUDE_DIRS "${PROJECT_CODE_DIR};${MXADataModel_BINARY_DIR}"  )


# --------------------------------------------------------------------
# -- Add the MXADataModel Library and link to needed libraries 
# --------------------------------------------------------------------
ADD_LIBRARY (${MXADATAMODEL_LIB_NAME} ${LIB_TYPE} ${MXA_PROJECT_FILES} )
TARGET_LINK_LIBRARIES (${MXADATAMODEL_LIB_NAME} ${DEP_LIBS} )
LibraryProperties( ${MXADATAMODEL_LIB_NAME} ${EXE_DEBUG_EXTENSION})

set(install_dir "tools")
set(lib_install_dir "lib")
if (WIN32)
    set (install_dir ".")
    set(lib_install_dir "")
endif()

if (BUILD_SHARED_LIBS AND NOT APPLE)
    INSTALL(TARGETS ${PROJECT_NAME} 
        COMPONENT Applications
        RUNTIME DESTINATION ${install_dir}
        LIBRARY DESTINATION ${lib_install_dir}
        ARCHIVE DESTINATION ${lib_install_dir}       
        BUNDLE DESTINATION "."
    ) 
endif() 

if ( ${MXA_INSTALL_FILES} EQUAL 1 )
    include ( ${PROJECT_RESOURCES_DIR}/MXACMake/MXAInstall.cmake)
endif()
