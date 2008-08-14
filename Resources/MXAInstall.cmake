#-------- Install the MXADataModel Library
IF (WIN32 AND BUILD_SHARED_LIBS)
    SET (MXA_CONFIG_DIR_Debug "")
    SET (MXA_CONFIG_DIR_Release "")
    IF (MSVC)
        SET (MXA_CONFIG_DIR_Debug "Debug/")
        SET (MXA_CONFIG_DIR_Release "Release/")
    ENDIF (MSVC)  
    INSTALL(FILES ${LIBRARY_OUTPUT_PATH}/${MXA_CONFIG_DIR_Debug}${MXA_LIBRARY_DEBUG}.dll 
        DESTINATION bin 
        CONFIGURATIONS Debug 
        COMPONENT Runtime)
    INSTALL(FILES ${LIBRARY_OUTPUT_PATH}/${MXA_CONFIG_DIR_Release}${MXA_LIBRARY_RELEASE}.dll 
        DESTINATION bin 
        CONFIGURATIONS Release 
        COMPONENT Runtime)
    INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} 
        LIBRARY DESTINATION bin 
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        COMPONENT Libraries
    )
ENDIF (WIN32 AND BUILD_SHARED_LIBS)

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

SET (MXA_INSTALLED_RESOURCES
        ${PROJECT_BINARY_DIR}/UseMXADataModel.cmake
        ${MXA_RESOURCES_DIR}/ConfigureChecks.cmake
   #     ${MXA_RESOURCES_DIR}/IncludeMXADataModelSource.cmake
        ${MXA_RESOURCES_DIR}/MXAAdjustLibVars.cmake
        ${MXA_RESOURCES_DIR}/MXAFindBoost.cmake
        ${MXA_RESOURCES_DIR}/MXAFindExpat.cmake
        ${MXA_RESOURCES_DIR}/MXAFindHDF5.cmake
        ${MXA_RESOURCES_DIR}/MXAFindMinGW.cmake
        ${MXA_RESOURCES_DIR}/MXAFindSZip.cmake
        ${MXA_RESOURCES_DIR}/MXAFindTiff.cmake
)

#-- Add Apple specific resources
#IF (APPLE)
#    SET (MXA_INSTALLED_RESOURCES ${MXA_INSTALLED_RESOURCES}
#        ${MXA_RESOURCES_DIR}/PackageMXALibsForOSXAppBundle.sh.in
#        ${MXA_RESOURCES_DIR}/CreateOSXBundle.sh.in
#        )
#ENDIF (APPLE)

INSTALL (FILES ${MXA_INSTALLED_RESOURCES} 
        DESTINATION share/MXADataModel/Resources/ 
        COMPONENT Resources)


