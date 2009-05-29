#-------- Install the MXADataModel Library
IF (MSVC)

    SET (MXA_CONFIG_DIR_Debug "")
    SET (MXA_CONFIG_DIR_Release "")
    IF (MSVC)
        SET (MXA_CONFIG_DIR_Debug "Debug/")
        SET (MXA_CONFIG_DIR_Release "Release/")
    ENDIF (MSVC)
    # If we are building Shared Libraries put a copy of the DLL into the bin directory
    # so that the executables will work
    if (BUILD_SHARED_LIBS)
        INSTALL(FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${MXA_CONFIG_DIR_Debug}${MXA_LIBRARY_DEBUG}.dll 
            DESTINATION lib 
            CONFIGURATIONS Debug 
            COMPONENT Runtime)
        INSTALL(FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${MXA_CONFIG_DIR_Release}${MXA_LIBRARY_RELEASE}.dll 
            DESTINATION lib 
            CONFIGURATIONS Release 
            COMPONENT Runtime)
    endif(BUILD_SHARED_LIBS)
    INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} 
        LIBRARY DESTINATION bin 
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        COMPONENT Libraries
    )
   
ENDIF (MSVC)

IF (NOT MSVC)
    INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        FRAMEWORK DESTINATION lib
        COMPONENT Libraries
    )
ENDIF (NOT MSVC)  
    
INSTALL (FILES ${PROJECT_BINARY_DIR}/MXA/MXAConfiguration.h ${PROJECT_BINARY_DIR}/MXA/MXAVersion.h 
         DESTINATION include/MXA 
         COMPONENT Headers           
)

CONFIGURE_FILE(${MXA_CMAKE_DIR}/UseMXADataModel.cmake.in 
                ${PROJECT_BINARY_DIR}/UseMXADataModel.cmake @ONLY IMMEDIATE)
CONFIGURE_FILE(${MXA_CMAKE_DIR}/InstallMXASupportLibraries.cmake.in 
                ${PROJECT_BINARY_DIR}/InstallMXASupportLibraries.cmake @ONLY IMMEDIATE)

SET (MXA_INSTALLED_RESOURCES
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

INSTALL (FILES ${MXA_INSTALLED_RESOURCES} 
        DESTINATION share/MXADataModel/Resources/CMake
        COMPONENT Resources)

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
            DESTINATION doc/MXADataModel
            COMPONENT Documentation )
endif()


