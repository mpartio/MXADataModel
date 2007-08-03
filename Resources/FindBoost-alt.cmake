# - Try to find Boost
# Once done this will define
#
#  BOOST_FOUND - System has Boost
#  BOOST_INCLUDE_DIRS - Boost include directory
#  BOOST_LIBRARIES - Link these to use Boost
#  BOOST_LIBRARY_DIRS - The path to where the Boost library files are.
#  BOOST_DEFINITIONS - Compiler switches required for using Boost
#  BOOST_LIBRARIES_SUFFIX - Boost libraries name suffix (e.g -vc71-mt-gd-1_34, -gcc41-mt...)
#
#  BOOST_DATE_TIME_FOUND               True IF Boost Date Time was found.
#  BOOST_FILESYSTEM_FOUND              True IF Boost Filesystem was found.
#  BOOST_IOSTREAMS_FOUND               True IF Boost Iostream was found.
#  BOOST_PRG_EXEC_MONITOR_FOUND        True IF Boost Program Exec Monitor was found.
#  BOOST_PROGRAM_OPTIONS_FOUND         True IF Boost Program Options was found.
#  BOOST_PYTHON_FOUND                  True IF Boost Python was found.
#  BOOST_REGEX_FOUND                   True IF Boost Regex was found.
#  BOOST_SERIALIZATION_FOUND           True IF Boost Serialization was found.
#  BOOST_SIGNALS_FOUND                 True IF Boost Signals was found.
#  BOOST_TEST_EXEC_MONITOR_FOUND       True IF Boost Test Exec Monitor was found.
#  BOOST_THREAD-MT_FOUND               True IF Boost Thread was found.
#  BOOST_UNIT_TEST_FRAMEWORK_FOUND     True IF Boost Unit Test Framework was found.
#  BOOST_WSERIALIZATION_FOUND          True IF Boost WSerialization was found.
#
#  BOOST_DATE_TIME_LIBRARY             The Boost Date Time libary.
#  BOOST_FILESYSTEM_LIBRARY            The Boost Filesystem libary.
#  BOOST_IOSTREAMS_LIBRARY             The Boost Iostream libary.
#  BOOST_PRG_EXEC_MONITOR_LIBRARY      The Boost Program libary.
#  BOOST_PROGRAM_OPTIONS_LIBRARY       The Boost Program libary.
#  BOOST_PYTHON_LIBRARY                The Boost Python libary.
#  BOOST_REGEX_LIBRARY                 The Boost Regex libary.
#  BOOST_SERIALIZATION_LIBRARY         The Boost Serialization libary.
#  BOOST_SIGNALS_LIBRARY               The Boost Signals libary.
#  BOOST_TEST_EXEC_MONITOR_LIBRARY     The Boost Test Exec Monitor libary.
#  BOOST_THREAD_LIBRARY                The Boost Thread libary.
#  BOOST_UNIT_TEST_FRAMEWORK_LIBRARY   The Boost Unit Test Framework libary.
#  BOOST_WSERIALIZATION_LIBRARY        The Boost WSerialization libary.
#
#  Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#  Copyright (c) 2007 Wengo
#
#  Redistribution AND use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


IF (BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)
  # in cache already
  SET(BOOST_FOUND TRUE)
ELSE (BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)
  # Add in some path suffixes. These will have to be updated whenever
  # a new Boost version comes out.
  SET(BOOST_PATH_SUFFIX
    boost-1_34_1
    boost-1_34_0
    boost-1_34
    boost-1_33_1
    boost-1_33_0
    boost-1_33
  )

  IF (WIN32)
    # In windows, automatic linking is performed, so you do not have to specify the libraries.
    # If you are linking to a dynamic runtime, then you can choose to link to either a static or a
    # dynamic Boost library, the default is to do a static link.  You can alter this for a specific
    # library "whatever" by defining BOOST_WHATEVER_DYN_LINK to force Boost library "whatever" to
    # be linked dynamically.  Alternatively you can force all Boost libraries to dynamic link by
    # defining BOOST_ALL_DYN_LINK.

    # This feature can be disabled for Boost library "whatever" by defining BOOST_WHATEVER_NO_LIB,
    # or for all of Boost by defining BOOST_ALL_NO_LIB.

    # If you want to observe which libraries are being linked against then defining
    # BOOST_LIB_DIAGNOSTIC will cause the auto-linking code to emit a #pragma MESSAGE each time
    # a library is selected for linking.
    SET(BOOST_LIB_DIAGNOSTIC_DEFINITIONS "-DBOOST_LIB_DIAGNOSTIC")

    SET(BOOST_INCLUDE_SEARCH_DIRS
      $ENV{BOOSTINCLUDEDIR}
      $ENV{BOOST_ROOT}/include
      C:/boost/include
      "C:/Program Files/boost/boost_1_34_0"
      "C:/Program Files/boost/boost_1_33_1"
      # D: is very often the cdrom drive, IF you don't have a
      # cdrom inserted it will popup a very annoying dialog
      #D:/boost/include
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
    )

    SET(BOOST_LIBRARIES_SEARCH_DIRS
      $ENV{BOOSTLIBDIR}
      $ENV{BOOST_ROOT}/lib
      C:/boost/lib
      "C:/Program Files/boost/boost_1_34_0/lib"
      "C:/Program Files/boost/boost_1_33_1/lib"
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
    )

    IF (MSVC71)
      IF (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(BOOST_LIBRARIES_SUFFIXES
          -vc71-mt-gd-1_34
          -vc71-mt-gd-1_33_1
        )
      ELSE (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(BOOST_LIBRARIES_SUFFIXES
          -vc71-mt-1_34
          -vc71-mt-1_33_1
        )
      ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
    ENDIF(MSVC71)

    IF (MSVC80)
      IF (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(BOOST_LIBRARIES_SUFFIXES
          -vc80-mt-gd-1_34
          -vc80-mt-gd-1_33_1
        )
      ELSE (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(BOOST_LIBRARIES_SUFFIXES
          -vc80-mt-1_34
          -vc80-mt-1_33_1
        )
      ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
    ENDIF(MSVC80)

    IF (MINGW)
      IF (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(BOOST_LIBRARIES_SUFFIXES
          -mgw-mt-d
        )
      ELSE (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(BOOST_LIBRARIES_SUFFIXES
          -mgw-mt
        )
      ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
    ENDIF(MINGW)

    IF (CYGWIN)
      IF (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(BOOST_LIBRARIES_SUFFIXES
          -gcc-mt-d
        )
      ELSE (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(BOOST_LIBRARIES_SUFFIXES
          -gcc-mt
        )
      ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
    ENDIF(CYGWIN)

  ELSE (WIN32)
    SET(BOOST_LIBRARIES_SUFFIXES
        "-1_34_1"
        "-gcc-mt"
        "-gcc41-mt"
        "-mt-1_34_1"
    )
  ENDIF(WIN32)

  FIND_PATH(BOOST_INCLUDE_DIR
    NAMES
      boost/config.hpp
    PATHS
      ${BOOST_INCLUDE_SEARCH_DIRS}
    PATH_SUFFIXES
      ${BOOST_PATH_SUFFIX}
  )

  #MESSAGE (STATUS "BOOST_LIBRARIES_SUFFIXES==> ${BOOST_LIBRARIES_SUFFIXES}")

  FOREACH (TMP_BOOST_LIBRARIES_SUFFIX ${BOOST_LIBRARIES_SUFFIXES})
    
    FIND_LIBRARY(BOOST_DATE_TIME_LIBRARY 
        boost_date_time${TMP_BOOST_LIBRARIES_SUFFIX}
        /usr/local/lib)
#    MESSAGE (STATUS "TMP_BOOST_LIBRARIES_SUFFIX==> ${TMP_BOOST_LIBRARIES_SUFFIX}")
#    MESSAGE (STATUS "boost_date_time${TMP_BOOST_LIBRARIES_SUFFIX}")
#    MESSAGE (STATUS "Date/Time ==> ${BOOST_DATE_TIME_LIBRARY}")
#    MESSAGE (STATUS "-----")
    
    IF (NOT BOOST_DATE_TIME_LIBRARY)
      FIND_LIBRARY(BOOST_DATE_TIME_LIBRARY
        NAMES
          boost_date_time${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )

      IF (BOOST_DATE_TIME_LIBRARY)
        # BOOST_DATE_TIME_LIBRARY was found
        # SETs the libraries suffix, this code is ugly
        # but CMake does not allow to break a loop :/
        SET(BOOST_LIBRARIES_SUFFIX
          ${TMP_BOOST_LIBRARIES_SUFFIX}
          CACHE INTERNAL "" FORCE
        )
        
      ENDIF(BOOST_DATE_TIME_LIBRARY)
    ENDIF(NOT BOOST_DATE_TIME_LIBRARY)

    IF (NOT BOOST_FILESYSTEM_LIBRARY)
      FIND_LIBRARY(BOOST_FILESYSTEM_LIBRARY
       # NAMES
          boost_filesystem${TMP_BOOST_LIBRARIES_SUFFIX}
       # PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_FILESYSTEM_LIBRARY)

    IF (NOT BOOST_IOSTREAMS_LIBRARY)
      FIND_LIBRARY(BOOST_IOSTREAMS_LIBRARY
        NAMES
          boost_iostreams${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_IOSTREAMS_LIBRARY)

    IF (NOT BOOST_PRG_EXEC_MONITOR_LIBRARY)
      FIND_LIBRARY(BOOST_PRG_EXEC_MONITOR_LIBRARY
        NAMES
          boost_prg_exec_monitor${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_PRG_EXEC_MONITOR_LIBRARY)

    IF (NOT BOOST_PROGRAM_OPTIONS_LIBRARY)
      FIND_LIBRARY(BOOST_PROGRAM_OPTIONS_LIBRARY
        NAMES
          boost_program_options${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_PROGRAM_OPTIONS_LIBRARY)

    IF (NOT BOOST_PYTHON_LIBRARY)
      FIND_LIBRARY(BOOST_PYTHON_LIBRARY
        NAMES
          boost_python${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_PYTHON_LIBRARY)

    IF (NOT BOOST_REGEX_LIBRARY)
      FIND_LIBRARY(BOOST_REGEX_LIBRARY
        NAMES
          boost_regex${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_REGEX_LIBRARY)

    IF (NOT BOOST_SERIALIZATION_LIBRARY)
      FIND_LIBRARY(BOOST_SERIALIZATION_LIBRARY
        NAMES
          boost_serialization${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_SERIALIZATION_LIBRARY)

    IF (NOT BOOST_SIGNALS_LIBRARY)
      FIND_LIBRARY(BOOST_SIGNALS_LIBRARY
        NAMES
          boost_signals${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_SIGNALS_LIBRARY)

    IF (NOT BOOST_TEST_EXEC_MONITOR_LIBRARY)
      IF (WIN32)
        SET (_name libboost_test_exec_monitor${TMP_BOOST_LIBRARIES_SUFFIX})
      ELSE (WIN32)
        SET (_name boost_test_exec_monitor${TMP_BOOST_LIBRARIES_SUFFIX})
      ENDIF(WIN32)
      FIND_LIBRARY(BOOST_TEST_EXEC_MONITOR_LIBRARY
        NAMES
          ${_name}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_TEST_EXEC_MONITOR_LIBRARY)

    IF (NOT BOOST_THREAD_LIBRARY)
      FIND_LIBRARY(BOOST_THREAD_LIBRARY
        NAMES
          boost_thread${TMP_BOOST_LIBRARIES_SUFFIX}
          boost_thread-mt
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_THREAD_LIBRARY)

    IF (NOT BOOST_UNIT_TEST_FRAMEWORK_LIBRARY)
      SET (_boost_unit_test_lib_name "")
      IF (WIN32)
        SET (_boost_unit_test_lib_name libboost_unit_test_framework${TMP_BOOST_LIBRARIES_SUFFIX})
      ELSE (WIN32)
        SET (_boost_unit_test_lib_name boost_unit_test_framework${TMP_BOOST_LIBRARIES_SUFFIX})
      ENDIF(WIN32)
      FIND_LIBRARY(BOOST_UNIT_TEST_FRAMEWORK_LIBRARY
        NAMES
          ${_boost_unit_test_lib_name}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_UNIT_TEST_FRAMEWORK_LIBRARY)

    IF (NOT BOOST_WSERIALIZATION_LIBRARY)
      FIND_LIBRARY(BOOST_WSERIALIZATION_LIBRARY
        NAMES
          boost_wserialization${TMP_BOOST_LIBRARIES_SUFFIX}
        PATHS
          ${BOOST_LIBRARIES_SEARCH_DIRS}
      )
    ENDIF(NOT BOOST_WSERIALIZATION_LIBRARY)

    IF (BOOST_DATE_TIME_LIBRARY)
      SET(BOOST_DATE_TIME_FOUND TRUE)
    ENDIF(BOOST_DATE_TIME_LIBRARY)
    IF (BOOST_FILESYSTEM_LIBRARY)
      SET(BOOST_FILESYSTEM_FOUND TRUE)
    ENDIF(BOOST_FILESYSTEM_LIBRARY)
    IF (BOOST_IOSTREAMS_LIBRARY)
      SET(BOOST_IOSTREAMS_FOUND TRUE)
    ENDIF(BOOST_IOSTREAMS_LIBRARY)
    IF (BOOST_PRG_EXEC_MONITOR_LIBRARY)
      SET(BOOST_PRG_EXEC_MONITOR_FOUND TRUE)
    ENDIF(BOOST_PRG_EXEC_MONITOR_LIBRARY)
    IF (BOOST_PROGRAM_OPTIONS_LIBRARY)
      SET(BOOST_PROGRAM_OPTIONS_FOUND TRUE)
    ENDIF(BOOST_PROGRAM_OPTIONS_LIBRARY)
    IF (BOOST_PYTHON_LIBRARY)
      SET(BOOST_PYTHON_FOUND TRUE)
    ENDIF(BOOST_PYTHON_LIBRARY)
    IF (BOOST_REGEX_LIBRARY)
      SET(BOOST_REGEX_FOUND TRUE)
    ENDIF(BOOST_REGEX_LIBRARY)
    IF (BOOST_SERIALIZATION_LIBRARY)
      SET(BOOST_SERIALIZATION_FOUND TRUE)
    ENDIF(BOOST_SERIALIZATION_LIBRARY)
    IF (BOOST_SIGNALS_LIBRARY)
      SET(BOOST_SIGNALS_FOUND TRUE)
    ENDIF(BOOST_SIGNALS_LIBRARY)
    IF (BOOST_TEST_EXEC_MONITOR_LIBRARY)
      SET(BOOST_TEST_EXEC_MONITOR_FOUND TRUE)
    ENDIF(BOOST_TEST_EXEC_MONITOR_LIBRARY)
    IF (BOOST_THREAD_LIBRARY)
      SET(BOOST_THREAD-MT_FOUND TRUE)
    ENDIF(BOOST_THREAD_LIBRARY)
    IF (BOOST_UNIT_TEST_FRAMEWORK_LIBRARY)
      SET(BOOST_UNIT_TEST_FRAMEWORK_FOUND TRUE)
    ENDIF(BOOST_UNIT_TEST_FRAMEWORK_LIBRARY)
    IF (BOOST_WSERIALIZATION_LIBRARY)
      SET(BOOST_WSERIALIZATION_FOUND TRUE)
    ENDIF(BOOST_WSERIALIZATION_LIBRARY)

  ENDFOREACH (TMP_BOOST_LIBRARIES_SUFFIX)

  SET(BOOST_INCLUDE_DIRS
    ${BOOST_INCLUDE_DIR}
  )

  IF (BOOST_DATE_TIME_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_DATE_TIME_LIBRARY}
    )
  ENDIF(BOOST_DATE_TIME_FOUND )
  IF (BOOST_FILESYSTEM_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_FILESYSTEM_LIBRARY}
    )
  ENDIF(BOOST_FILESYSTEM_FOUND)
  IF (BOOST_IOSTREAMS_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_IOSTREAMS_LIBRARY}
    )
  ENDIF(BOOST_IOSTREAMS_FOUND )
  IF (BOOST_PRG_EXEC_MONITOR_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_PRG_EXEC_MONITOR_LIBRARY}
    )
  ENDIF(BOOST_PRG_EXEC_MONITOR_FOUND )
  IF (BOOST_PROGRAM_OPTIONS_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_PROGRAM_OPTIONS_LIBRARY}
    )
  ENDIF(BOOST_PROGRAM_OPTIONS_FOUND )
  IF (BOOST_PYTHON_FOUND)
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_PYTHON_LIBRARY}
    )
  ENDIF(BOOST_PYTHON_FOUND)
  IF (BOOST_REGEX_FOUND)
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_REGEX_LIBRARY}
    )
  ENDIF(BOOST_REGEX_FOUND)
  IF (BOOST_SERIALIZATION_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_SERIALIZATION_LIBRARY}
    )
  ENDIF(BOOST_SERIALIZATION_FOUND)
  IF (BOOST_SIGNALS_FOUND)
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_SIGNALS_LIBRARY}
    )
  ENDIF(BOOST_SIGNALS_FOUND )
  IF (BOOST_TEST_EXEC_MONITOR_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_TEST_EXEC_MONITOR_LIBRARY}
    )
  ENDIF(BOOST_TEST_EXEC_MONITOR_FOUND )
  IF (BOOST_THREAD-MT_FOUND)
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_THREAD_LIBRARY}
    )
  ENDIF(BOOST_THREAD-MT_FOUND )
  IF (BOOST_UNIT_TEST_FRAMEWORK_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
  ENDIF(BOOST_UNIT_TEST_FRAMEWORK_FOUND )
  IF (BOOST_WSERIALIZATION_FOUND )
    SET(BOOST_LIBRARIES
      ${BOOST_LIBRARIES}
      ${BOOST_WSERIALIZATION_LIBRARY}
    )
  ENDIF(BOOST_WSERIALIZATION_FOUND )

  IF (BOOST_INCLUDE_DIRS AND BOOST_LIBRARIES)
    SET(BOOST_FOUND TRUE)
  ENDIF(BOOST_INCLUDE_DIRS AND BOOST_LIBRARIES)

  IF (BOOST_FOUND)
    IF (NOT Boost_FIND_QUIETLY)
      MESSAGE(STATUS "Found Boost: ${BOOST_LIBRARIES}")
    ENDIF(NOT Boost_FIND_QUIETLY)
  ELSE (BOOST_FOUND)
    IF (Boost_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Please install the Boost libraries AND development packages")
    ENDIF(Boost_FIND_REQUIRED)
  ENDIF(BOOST_FOUND)

  FOREACH (BOOST_LIBDIR ${BOOST_LIBRARIES})
    GET_FILENAME_COMPONENT(BOOST_LIBRARY_DIRS ${BOOST_LIBDIR} PATH)
  ENDFOREACH (BOOST_LIBDIR ${BOOST_LIBRARIES})

  # Under Windows, automatic linking is performed, so no need to specify the libraries.
  IF (WIN32)
    SET(BOOST_LIBRARIES "")
  ENDIF(WIN32)

  # show the BOOST_INCLUDE_DIRS AND BOOST_LIBRARIES variables only in the advanced view
  MARK_AS_ADVANCED(BOOST_INCLUDE_DIRS 
    BOOST_LIBRARIES 
    BOOST_LIBRARY_DIRS 
    BOOST_DEFINITIONS 
    BOOST_LIBRARIES_SUFFIX
    )

ENDIF(BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)
