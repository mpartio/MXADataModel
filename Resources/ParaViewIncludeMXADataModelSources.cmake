#MESSAGE (STATUS "^^ParaViewIncludeMXADataModelSources.cmake")
IF (NOT MXA_RESOURCES_DIR)
  GET_FILENAME_COMPONENT(MXA_RESOURCES_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDIF (NOT MXA_RESOURCES_DIR)
#-- Include the Configure Checks
INCLUDE( "${MXA_RESOURCES_DIR}/ConfigureChecks.cmake")
INCLUDE ("${MXA_RESOURCES_DIR}/IncludeMXADataModelSource.cmake")

SET (PARAVIEW_ADDITIONAL_LIBRARIES 
    ${PARAVIEW_ADDITIONAL_LIBRARIES} 
    ${BOOST_FILESYSTEM_LIBRARY}
    ${HDF_LIBRARY}
)

IF (PARAVIEW_VERSION_MAJOR EQUAL "2")
    MESSAGE (FATAL " ParaView 3.x only supported.")
  
ELSE (PARAVIEW_VERSION_MAJOR EQUAL "2")
    IF ( PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED)
       # MESSAGE (STATUS "^^^^PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED was already configured")
    ELSE (PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED)
        # MESSAGE (STATUS "^^^^^Configuring MXADataModel Sources")
        PARAVIEW_INCLUDE_SOURCES("${MXADATAMODEL_SRCS}")
        SET (PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED TRUE)
    ENDIF (PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED)
ENDIF (PARAVIEW_VERSION_MAJOR EQUAL "2")
