
MACRO (MXA_SOURCE_PROPERTIES NAME HEADERS SOURCES)

  INSTALL (FILES ${HEADERS}
              DESTINATION include/MXA/${NAME}
              COMPONENT Headers           
  )

    set(source_group_path "Source/MXA/${NAME}")
    STRING(REPLACE "/" "\\\\" source_group_path ${source_group_path}  )
    source_group(${source_group_path} FILES ${HEADERS} ${SOURCES})
  
  #-- The following is needed if we ever start to use OS X Frameworks but only
  #--  works on CMake 2.6 and greater
  #set_property(SOURCE ${HEADERS}
  #             PROPERTY MACOSX_PACKAGE_LOCATION Headers/${NAME}
  #)

ENDMACRO (MXA_SOURCE_PROPERTIES NAME HEADERS SOURCES)
