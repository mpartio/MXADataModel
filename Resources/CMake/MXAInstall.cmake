#--////////////////////////////////////////////////////////////////////////////
#--  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--////////////////////////////////////////////////////////////////////////////
    
INSTALL (FILES ${PROJECT_BINARY_DIR}/MXA/MXAConfiguration.h ${PROJECT_BINARY_DIR}/MXA/MXAVersion.h 
         DESTINATION include/MXA 
         COMPONENT Headers           
)

CONFIGURE_FILE(${PROJECT_CMAKE_DIR}/UseMXADataModel.cmake.in 
                ${PROJECT_BINARY_DIR}/UseMXADataModel.cmake @ONLY IMMEDIATE)
CONFIGURE_FILE(${PROJECT_CMAKE_DIR}/InstallMXASupportLibraries.cmake.in 
                ${PROJECT_BINARY_DIR}/InstallMXASupportLibraries.cmake @ONLY IMMEDIATE)

SET (MXA_CMAKE_INSTALLED_RESOURCES
        ${PROJECT_BINARY_DIR}/UseMXADataModel.cmake
        ${PROJECT_BINARY_DIR}/InstallMXASupportLibraries.cmake
        ${PROJECT_CMAKE_DIR}/ConfigureChecks.cmake
        ${PROJECT_CMAKE_MODULES_DIR}/AdjustLibVars.cmake
        ${PROJECT_CMAKE_MODULES_DIR}/MXAFindBoost.cmake
        ${PROJECT_CMAKE_MODULES_DIR}/MXAFindExpat.cmake
        ${PROJECT_CMAKE_MODULES_DIR}/MXAFindHDF5.cmake
        ${PROJECT_CMAKE_MODULES_DIR}/MXAFindMinGW.cmake
        ${PROJECT_CMAKE_MODULES_DIR}/MXAFindSZip.cmake
        ${PROJECT_CMAKE_MODULES_DIR}/MXAFindTiff.cmake
        ${PROJECT_CMAKE_DIR}/GetTimeOfDayTest.cpp
        ${PROJECT_CMAKE_DIR}/FindSupportLibraries.cmake
        ${PROJECT_CMAKE_DIR}/GenerateVersionString.cpp
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


