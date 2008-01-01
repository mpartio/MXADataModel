# In this file we are doing all of our 'configure' checks. Things like checking
# for headers, functions, libraries, types and size of types.

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)

CHECK_INCLUDE_FILE("stddef.h" HAVE_STDDEF_H)
CHECK_INCLUDE_FILE("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)


CHECK_TYPE_SIZE(size_t         SIZEOF_SIZE_T)
MESSAGE (STATUS "SIZEOF_SIZE_T: ${SIZEOF_SIZE_T}")
CHECK_TYPE_SIZE(ssize_t        SIZEOF_SSIZE_T)
MESSAGE (STATUS "SIZEOF_SSIZE_T: ${SIZEOF_SSIZE_T}")
