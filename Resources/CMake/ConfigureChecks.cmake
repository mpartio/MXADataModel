# In this file we are doing all of our 'configure' checks. Things like checking
# for headers, functions, libraries, types and size of types.

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckCXXSourceCompiles.cmake)

CHECK_INCLUDE_FILE("stddef.h" HAVE_STDDEF_H)
CHECK_INCLUDE_FILE("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)

if (MSVC)
 set(HAVE_TIME_GETTIMEOFDAY "TRUE")
endif()
 
IF (NOT MSVC)
TRY_COMPILE(HAVE_TIME_GETTIMEOFDAY
      ${CMAKE_BINARY_DIR}
      ${MXA_CMAKE_DIR}/GetTimeOfDayTest.cpp
      COMPILE_DEFINITIONS -DTRY_TIME_H
      OUTPUT_VARIABLE OUTPUT)
IF (HAVE_TIME_GETTIMEOFDAY STREQUAL "TRUE")
    SET (HAVE_TIME_GETTIMEOFDAY "1")
ENDIF (HAVE_TIME_GETTIMEOFDAY STREQUAL "TRUE")
 #MESSAGE(STATUS "HAVE_TIME_GETTIMEOFDAY: ${HAVE_TIME_GETTIMEOFDAY}")
 #MESSAGE(STATUS "OUTPUT: ${OUTPUT}")

TRY_COMPILE(HAVE_SYS_TIME_GETTIMEOFDAY
      ${CMAKE_BINARY_DIR}
      ${MXA_CMAKE_DIR}/GetTimeOfDayTest.cpp
      COMPILE_DEFINITIONS -DTRY_SYS_TIME_H
      OUTPUT_VARIABLE OUTPUT)
IF (HAVE_SYS_TIME_GETTIMEOFDAY STREQUAL "TRUE")
    SET (HAVE_SYS_TIME_GETTIMEOFDAY "1")
ENDIF (HAVE_SYS_TIME_GETTIMEOFDAY STREQUAL "TRUE")
      
 #MESSAGE(STATUS "HAVE_SYS_TIME_GETTIMEOFDAY: ${HAVE_SYS_TIME_GETTIMEOFDAY}")
 #MESSAGE(STATUS "OUTPUT: ${OUTPUT}")


if (NOT HAVE_SYS_TIME_GETTIMEOFDAY AND NOT HAVE_TIME_GETTIMEOFDAY)
message(STATUS "---------------------------------------------------------------")
message(STATUS "Function 'gettimeofday()' was not found. MXADataModel will use its")
message(STATUS "  own implementation.. This can happen on older versions of")
message(STATUS "  MinGW on Windows. Consider upgrading your MinGW installation")
message(STATUS "  to a newer version such as MinGW 3.12")
message(STATUS "---------------------------------------------------------------")
endif()

ENDIF (NOT MSVC)