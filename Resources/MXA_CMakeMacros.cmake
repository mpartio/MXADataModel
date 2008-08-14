
MACRO (MXA_SOURCE_PROPERTIES NAME HEADERS SOURCES)

INSTALL (FILES ${HEADERS}
            DESTINATION include/MXADataModel/${NAME}
            COMPONENT Headers           
)

source_group(src\\${NAME} FILES ${HEADERS} ${SOURCES})

#-- The following is needed if we ever start to use OS X Frameworks but only
#--  works on CMake 2.6 and greater
#set_property(SOURCE ${HEADERS}
# PROPERTY MACOSX_PACKAGE_LOCATION Headers/${NAME}
#)

ENDMACRO (MXA_SOURCE_PROPERTIES NAME HEADERS SOURCES)
