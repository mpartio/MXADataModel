message(FATAL_ERROR "THIS IS NOT BEING USED ANYMORE")



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


