


MACRO (MXA_COPY_DEPENDENT_LIBRARIES TARGET_NAME)

#-- Copy all the dependent DLLs into the current build directory so that the test
#-- can run.
if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(TYPE "DEBUG")
    else()
        set(TYPE "RELEASE")
    endif()
    set(TYPE "DEBUG")
    SET (mxa_lib_list tiff expat hdf5)
    FOREACH(lib ${mxa_lib_list})
      STRING(TOUPPER ${lib} upperlib)
      if (HAVE_${upperlib}_DLL)
            file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
            file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
            file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MinSizeRel)
            file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo)
            message(STATUS "${upperlib}_BIN_DIR: ${${upperlib}_BIN_DIR}")
            message(STATUS "CMAKE_CFG_INTDIR: ${variable}")
            message (STATUS "${upperlib}_LIBRARY_${TYPE}: ${${upperlib}_LIBRARY_${TYPE}} ")
      
            get_filename_component(lib_path ${${upperlib}_LIBRARY_${TYPE}} PATH)
            get_filename_component(lib_name ${${upperlib}_LIBRARY_${TYPE}} NAME_WE)
            
            message(STATUS "${${upperlib}_BIN_DIR}/${lib_name}.dll")
            message(STATUS "MXADATAMODEL_LIB_NAME: ${MXADATAMODEL_LIB_NAME}")
            message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
            message(STATUS "${TARGET_NAME}")
            ADD_CUSTOM_COMMAND(TARGET ${TARGET_NAME}  POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${${upperlib}_BIN_DIR}/${lib_name}.dll
                      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/ 
                      COMMENT "Copying ${lib_name}.dll to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/")
      endif()
    ENDFOREACH()
endif()
    
    
endmacro()


#-------------------------------------------------------------------------------
MACRO (IDE_GENERATED_PROPERTIES SOURCE_PATH HEADERS SOURCES)
    #set(source_group_path "Source/AIM/${NAME}")
    STRING(REPLACE "/" "\\\\" source_group_path ${SOURCE_PATH}  )
    source_group(${source_group_path} FILES ${HEADERS} ${SOURCES})

  #-- The following is needed if we ever start to use OS X Frameworks but only
  #--  works on CMake 2.6 and greater
  #set_property(SOURCE ${HEADERS}
  #             PROPERTY MACOSX_PACKAGE_LOCATION Headers/${NAME}
  #)

ENDMACRO (IDE_GENERATED_PROPERTIES SOURCE_PATH HEADERS SOURCES)


#-------------------------------------------------------------------------------

MACRO (IDE_SOURCE_PROPERTIES SOURCE_PATH HEADERS SOURCES)
    INSTALL (FILES ${HEADERS}
             DESTINATION include/${SOURCE_PATH}
             COMPONENT Headers           
    )

    STRING(REPLACE "/" "\\\\" source_group_path ${SOURCE_PATH}  )
    source_group(${source_group_path} FILES ${HEADERS} ${SOURCES})

  #-- The following is needed if we ever start to use OS X Frameworks but only
  #--  works on CMake 2.6 and greater
  #set_property(SOURCE ${HEADERS}
  #             PROPERTY MACOSX_PACKAGE_LOCATION Headers/${NAME}
  #)

ENDMACRO (IDE_SOURCE_PROPERTIES NAME HEADERS SOURCES)

# --------------------------------------------------------------------

macro(LibraryProperties targetName )
message(STATUS "Adding Install support for ${targetName}")
if (0)
    if (WIN32 AND NOT MINGW)
        SET (DEBUG_EXTENSION "_d")
    else()
        SET (DEBUG_EXTENSION "_debug")
    endif()
    
    IF (WIN32 AND NOT MINGW)
        SET(LIBRARY_RELEASE_NAME "lib${targetName}" CACHE INTERNAL "" FORCE)
        SET(LIBRARY_DEBUG_NAME "lib${targetName}${DEBUG_EXTENSION}" CACHE INTERNAL "" FORCE)
    ELSE (WIN32 AND NOT MINGW)
        SET(LIBRARY_RELEASE_NAME "${targetName}" CACHE INTERNAL "" FORCE)
        SET(LIBRARY_DEBUG_NAME "${targetName}${DEBUG_EXTENSION}" CACHE INTERNAL "" FORCE)
    ENDIF(WIN32 AND NOT MINGW)
 endif()
 
    if ( NOT BUILD_SHARED_LIBS AND MSVC)
      SET_TARGET_PROPERTIES( ${targetName} 
        PROPERTIES
        DEBUG_OUTPUT_NAME lib${targetName}
        RELEASE_OUTPUT_NAME lib${targetName}  )
    endif()
    
    #-- Set the Debug and Release names for the libraries
    SET_TARGET_PROPERTIES( ${targetName} 
        PROPERTIES
        DEBUG_POSTFIX "_d" )
    
    IF (APPLE AND BUILD_SHARED_LIBS)
      OPTION (MXA_BUILD_WITH_INSTALL_NAME "Build Libraries with the install_name set to the installation prefix. This is good if you are going to run from the installation location" OFF)
      IF(MXA_BUILD_WITH_INSTALL_NAME)
          SET_TARGET_PROPERTIES(${MXADATAMODEL_LIB_NAME}
             PROPERTIES
             LINK_FLAGS "-current_version ${${PROJECT_NAME}_VERSION} -compatibility_version ${${PROJECT_NAME}_VERSION}"
             INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
             BUILD_WITH_INSTALL_RPATH ${MXA_BUILD_WITH_INSTALL_NAME}
          )
     ENDIF(MXA_BUILD_WITH_INSTALL_NAME)
   ENDIF (APPLE AND BUILD_SHARED_LIBS)

endmacro(LibraryProperties)

