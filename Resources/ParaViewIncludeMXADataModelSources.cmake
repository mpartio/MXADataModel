# This cmake file finds the MXA DataModel sources and conditionally adds them to 
# the list of files to compile. This step should only be performed once because
# on successful addition a cache variable is set "PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED"
# to true. Additionally, only ParaView 3.x and greater is supported.

IF (NOT MXA_RESOURCES_DIR)
  GET_FILENAME_COMPONENT(MXA_RESOURCES_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDIF (NOT MXA_RESOURCES_DIR)

#-- Gather the list of MXA Sources
INCLUDE ("${MXA_RESOURCES_DIR}/IncludeMXADataModelSource.cmake")
INCLUDE_DIRECTORIES (${MXA_SOURCE_DIR}/src)
        
IF (PARAVIEW_VERSION_MAJOR GREATER "2")
    IF (NOT PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED)
        # MESSAGE (STATUS "+++ Adding  MXADataModel Sources")

        #- Add the required Libraries for MXA to function
        SET (PARAVIEW_ADDITIONAL_LIBRARIES 
            ${PARAVIEW_ADDITIONAL_LIBRARIES} 
            ${BOOST_FILESYSTEM_LIBRARY}
            ${HDF_LIBRARY}
        )
        
        #-- Add the MXADataModel sources to the list of paraview sources
        PARAVIEW_INCLUDE_SOURCES("${MXADATAMODEL_SRCS}")
        
        #-- Make sure we only do this step once in case we are including multiple
        #-- projects that all use MXADataModel
        SET (PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED TRUE CACHE INTERNAL "DocString")
    ENDIF (NOT PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED)
ENDIF (PARAVIEW_VERSION_MAJOR GREATER "2")

