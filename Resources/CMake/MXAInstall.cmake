#--////////////////////////////////////////////////////////////////////////////
#--  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--////////////////////////////////////////////////////////////////////////////
    
INSTALL (FILES  "${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Version.h" 
                "${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Configuration.h" 
                "${MXADataModel_BINARY_DIR}/${PROJECT_PREFIX}/${PROJECT_PREFIX}Types.h" 
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
 			 ${PROJECT_CMAKE_DIR}/CMakeMacros.cmake
       ${PROJECT_CMAKE_DIR}/ConfigureChecks.cmake
       ${PROJECT_CMAKE_DIR}/GenerateVersionString.cpp
       ${PROJECT_CMAKE_DIR}/GetTimeOfDayTest.cpp
       ${PROJECT_CMAKE_DIR}/TestBoolType.cxx
       ${PROJECT_CMAKE_DIR}/TestCharSignedness.cxx
       ${PROJECT_CMAKE_DIR}/TestCompareTypes.cxx
       ${PROJECT_CMAKE_DIR}/TestConvertTypes.cxx
       ${PROJECT_CMAKE_DIR}/TestMiscFeatures.c
			 ${PROJECT_CMAKE_DIR}/PrimitiveTypes.h.in
 			 ${PROJECT_CMAKE_DIR}/Version.h.in
)

INSTALL (FILES ${MXA_CMAKE_INSTALLED_RESOURCES} 
        DESTINATION share/MXADataModel/Resources/CMake
        COMPONENT Resources)

SET (MXA_INSTALLED_CMAKE_MODULES 
  ${PROJECT_CMAKE_MODULES_DIR}/AdjustLibVars.cmake
  ${PROJECT_CMAKE_MODULES_DIR}/MXAFindBoost.cmake
  ${PROJECT_CMAKE_MODULES_DIR}/MXAFindExpat.cmake
  ${PROJECT_CMAKE_MODULES_DIR}/MXAFindHDF5.cmake
  ${PROJECT_CMAKE_MODULES_DIR}/MXAFindMinGW.cmake
  ${PROJECT_CMAKE_MODULES_DIR}/MXAFindSZip.cmake
  ${PROJECT_CMAKE_MODULES_DIR}/MXAFindTiff.cmake
)


INSTALL (FILES ${MXA_INSTALLED_CMAKE_MODULES}
  DESTINATION share/MXADataModel/Resources/CMake/Modules
  COMPONENT Resources)

SET (MXA_XML_INSTALLED_RESOURCES
        ${PROJECT_RESOURCES_DIR}/XML/ModelTemplate.xml
        ${PROJECT_RESOURCES_DIR}/XML/mxa_0.4.dtd
        ${PROJECT_RESOURCES_DIR}/XML/mxa_import.dtd
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


