#--////////////////////////////////////////////////////////////////////////////
#--  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--////////////////////////////////////////////////////////////////////////////
    
INSTALL (FILES ${PROJECT_BINARY_DIR}/MXA/MXAConfiguration.h ${PROJECT_BINARY_DIR}/MXA/MXAVersion.h 
         DESTINATION include/MXA 
         COMPONENT Headers           
)

CONFIGURE_FILE(${MXA_CMAKE_DIR}/UseMXADataModel.cmake.in 
                ${PROJECT_BINARY_DIR}/UseMXADataModel.cmake @ONLY IMMEDIATE)
CONFIGURE_FILE(${MXA_CMAKE_DIR}/InstallMXASupportLibraries.cmake.in 
                ${PROJECT_BINARY_DIR}/InstallMXASupportLibraries.cmake @ONLY IMMEDIATE)

SET (MXA_CMAKE_INSTALLED_RESOURCES
        ${PROJECT_BINARY_DIR}/UseMXADataModel.cmake
        ${PROJECT_BINARY_DIR}/InstallMXASupportLibraries.cmake
        ${MXA_CMAKE_DIR}/ConfigureChecks.cmake
        ${MXA_CMAKE_DIR}/MXAAdjustLibVars.cmake
        ${MXA_CMAKE_DIR}/MXAFindBoost.cmake
        ${MXA_CMAKE_DIR}/MXAFindExpat.cmake
        ${MXA_CMAKE_DIR}/MXAFindHDF5.cmake
        ${MXA_CMAKE_DIR}/MXAFindMinGW.cmake
        ${MXA_CMAKE_DIR}/MXAFindSZip.cmake
        ${MXA_CMAKE_DIR}/MXAFindTiff.cmake
        ${MXA_CMAKE_DIR}/GetTimeOfDayTest.cpp
        ${MXA_CMAKE_DIR}/FindSupportLibraries.cmake
        ${MXA_CMAKE_DIR}/GenerateVersionString.cpp
)

INSTALL (FILES ${MXA_CMAKE_INSTALLED_RESOURCES} 
        DESTINATION share/MXADataModel/Resources/CMake
        COMPONENT Resources)


SET (MXA_XML_INSTALLED_RESOURCES
        ${MXA_RESOURCES_DIR}/XML/ModelTemplate.xml
        ${MXA_RESOURCES_DIR}/XML/mxa_0.4.dtd
        ${MXA_RESOURCES_DIR}/XML/mxa_import.dtd
)

INSTALL (FILES ${MXA_XML_INSTALLED_RESOURCES} 
        DESTINATION doc/MXADataModel/xml
        COMPONENT Documentation)

SET (MISC_DOCS
  ${MXADataModel_SOURCE_DIR}/License.txt
)

INSTALL (FILES ${MISC_DOCS} 
        DESTINATION doc/MXADataModel
        COMPONENT Documentation)

#-- Install the HTML based documentation
INSTALL (DIRECTORY ${MXA_DOCUMENTATION_DIR}
            DESTINATION doc/MXADataModel
            COMPONENT Documentation )

#-- Install the generated Doxygen documentation
if (MXA_BUILD_API_DOCS)
    INSTALL(DIRECTORY ${MXADataModel_BINARY_DIR}/API-Docs
            DESTINATION doc/MXADataModel/Documentation
            COMPONENT Documentation )
endif()


# ------------------------------------------------------------------------------ 
# This CMake code sets up for CPack to be used to generate native installers
# ------------------------------------------------------------------------------
#INCLUDE (InstallRequiredSystemLibraries)
if (BUILD_SHARED_LIBS)
if (MXA_BUILD_EXAMPLES)
set (MSVC_COMP "Examples")
endif()
if (MXA_BUILD_UTILITIES)
    set (MSVC_COMP "Utilities")
endif()
if (MXA_BUILD_EXAMPLES OR MXA_BUILD_UTILITIES)
    INCLUDE (${MXA_RESOURCES_DIR}/CMake/InstallMSVCLibraries.cmake)
endif()
endif()
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MXADataModel SDK")
SET(CPACK_PACKAGE_VENDOR "BlueQuartz Software, Michael A. Jackson")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${MXADataModel_SOURCE_DIR}/ReadMe.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${MXADataModel_SOURCE_DIR}/License.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR ${MXADataModel_VER_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${MXADataModel_VER_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${MXADataModel_VER_PATCH})
SET(CPACK_PACKAGE_VERSION ${MXADataModel_VERSION})
SET(CPACK_COMPONENTS_ALL  Headers SDKLibrary Examples Resources Utilities Documentation)
set(CPACK_COMPONENT_SDKLIBRARY_DISPLAY_NAME "SDKLibrary")
set(CPACK_COMPONENT_SDKLIBRARY_DESCRIPTION  "The MXADataModel SDK Library")
set(CPACK_COMPONENT_SDKLIBRARY_REQUIRED 1)
set(CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "Examples")
set(CPACK_COMPONENT_EXAMPLES_DESCRIPTION  "The MXADataModel Examples")
set(CPACK_COMPONENT_EXAMPLES_REQUIRED 1)
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "Headers")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION  "The MXADataModel Headers")
set(CPACK_COMPONENT_HEADERS_REQUIRED 1)
set(CPACK_COMPONENT_RESOURCES_DISPLAY_NAME "Resources")
set(CPACK_COMPONENT_RESOURCES_DESCRIPTION  "The MXADataModel Resources")
set(CPACK_COMPONENT_RESOURCES_REQUIRED 1)
set(CPACK_COMPONENT_UTILITIES_DISPLAY_NAME "Utilities")
set(CPACK_COMPONENT_UTILITIES_DESCRIPTION  "The MXADataModel Utility Programs")
set(CPACK_COMPONENT_UTILITIES_REQUIRED 1)
set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "Documentation")
set(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION  "The MXADataModel API Documentation")
set(CPACK_COMPONENT_DOCUMENTATION_REQUIRED 0)
set(CPACK_PACKAGE_EXECUTABLES 
    ImportGenerator;ImportGenerator)
   
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
# SET(CPACK_PACKAGE_ICON "${MXADataModel_SOURCE_DIR}/Resources/InstallerIcon.bmp")
# SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "MXADataModel SDK")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.bluequartz.net")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.bluequartz.net")
  SET(CPACK_NSIS_CONTACT "mike.jackson@bluequartz.net")
  SET(CPACK_NSIS_MODIFY_PATH ON)
  SET(CPACK_SOURCE_GENERATOR "ZIP")
  SET(CPACK_GENERATOR "NSIS")
  SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "MXADataModel SDK")

ELSE(WIN32 AND NOT UNIX)
    SET(CPACK_BINARY_BUNDLE "OFF")
    SET(CPACK_BINARY_CYGWIN "OFF")
    SET(CPACK_BINARY_DEB "OFF")
    SET(CPACK_BINARY_DRAGNDROP "OFF")
    SET(CPACK_BINARY_NSIS "OFF")
    SET(CPACK_BINARY_OSXX11 "OFF")
    SET(CPACK_BINARY_PACKAGEMAKER "ON")
    SET(CPACK_BINARY_RPM "OFF")
    SET(CPACK_BINARY_STGZ "OFF")
    SET(CPACK_BINARY_TBZ2 "OFF")
    SET(CPACK_BINARY_TGZ "ON")
    SET(CPACK_BINARY_TZ "OFF")
    SET(CPACK_BINARY_ZIP "OFF")
    set(CPACK_PACKAGE_DEFAULT_LOCATION "/opt/local/MXADataModel")
ENDIF(WIN32 AND NOT UNIX)

SET(CPACK_SOURCE_GENERATOR "ZIP")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "MXADataModel-${MXADataModel_VERSION}-Source")
SET(CPACK_SOURCE_TOPLEVEL_TAG "Source")
SET(CPACK_IGNORE_FILES "/i386/;/x64/;/VS2008/;/zRel/;/Build/;/\\\\.git/;\\\\.*project")
SET(CPACK_SOURCE_IGNORE_FILES "/i386/;/x64/;/VS2008/;/zRel/;/Build/;/\\\\.git/;\\\\.*project")



# THIS MUST BE THE LAST LINE OF THIS FILE BECAUSE ALL THE CPACK VARIABLES MUST BE
# DEFINED BEFORE CPack IS INCLUDED
INCLUDE(CPack)


