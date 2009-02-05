#-------- Install the MXADataModel Library
IF (WIN32)

    SET (MXA_CONFIG_DIR_Debug "")
    SET (MXA_CONFIG_DIR_Release "")
    IF (MSVC)
        SET (MXA_CONFIG_DIR_Debug "Debug/")
        SET (MXA_CONFIG_DIR_Release "Release/")
    ENDIF (MSVC)
    # If we are building Shared Libraries put a copy of the DLL into the bin directory
    # so that the executables will work
    if (BUILD_SHARED_LIBS)
        INSTALL(FILES ${LIBRARY_OUTPUT_PATH}/${MXA_CONFIG_DIR_Debug}${MXA_LIBRARY_DEBUG}.dll 
            DESTINATION lib 
            CONFIGURATIONS Debug 
            COMPONENT Runtime)
        INSTALL(FILES ${LIBRARY_OUTPUT_PATH}/${MXA_CONFIG_DIR_Release}${MXA_LIBRARY_RELEASE}.dll 
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
   
ENDIF (WIN32)

IF (NOT WIN32)
    INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        COMPONENT Libraries
    )
ENDIF (NOT WIN32)  
    
INSTALL (FILES ${PROJECT_BINARY_DIR}/MXAConfiguration.h 
            DESTINATION include/MXADataModel 
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
)

INSTALL (FILES ${MXA_INSTALLED_RESOURCES} 
        DESTINATION share/MXADataModel/Resources/CMake
        COMPONENT Resources)

SET (DOCS
  ${MXA_DOCUMENTATION_DIR}/Background.html
  ${MXA_DOCUMENTATION_DIR}/CompileNotesUnix.html
  ${MXA_DOCUMENTATION_DIR}/CompileNotesXP.html
  ${MXA_DOCUMENTATION_DIR}/default.css
  ${MXA_DOCUMENTATION_DIR}/Examples.html
  ${MXA_DOCUMENTATION_DIR}/HowToWriteDataImportDelegate.html
  ${MXA_DOCUMENTATION_DIR}/ImportingData.html
  ${MXA_DOCUMENTATION_DIR}/index.html
  ${MXA_DOCUMENTATION_DIR}/MXAUtilities.html
  ${MXA_DOCUMENTATION_DIR}/MXASpecification.html
  ${MXA_DOCUMENTATION_DIR}/Navigation.js
  ${MXA_DOCUMENTATION_DIR}/site.css
  ${MXA_DOCUMENTATION_DIR}/SystemRequirements.html
  ${MXA_DOCUMENTATION_DIR}/Todo.html
  ${MXA_DOCUMENTATION_DIR}/UsingMXAInOtherProjects.html
  ${MXA_DOCUMENTATION_DIR}/XML_Usage.html
  ${MXA_DOCUMENTATION_DIR}/img/li.gif
  ${MXA_DOCUMENTATION_DIR}/img/quote.gif
)

#-- Install the HTML based documentation
INSTALL (DIRECTORY ${MXA_DOCUMENTATION_DIR}
            DESTINATION doc/MXADataModel
            COMPONENT Documentation
            PATTERN "CVS" EXCLUDE)

#-- Install the generated Doxygen documentation
INSTALL(DIRECTORY ${MXA_SOURCE_DIR}/API-Docs
        DESTINATION doc/MXADataModel
        COMPONENT Documentation)

INSTALL(FILES ${MXA_SOURCE_DIR}/Resources/ReadMe.txt
        DESTINATION .
        COMPONENT Documentation)
        
