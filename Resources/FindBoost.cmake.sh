#! /bin/bash

CMAKEFILE="/tmp/FindBoost.cmake"
echo "# - Find the Boost includes and libraries." > ${CMAKEFILE}
echo "# The following variables are set if Boost is found.  If Boost is not" >> ${CMAKEFILE}
echo "# found, Boost_FOUND is set to false." >> ${CMAKEFILE}
echo "#  Boost_FOUND        - True when the Boost include directory is found." >> ${CMAKEFILE}
echo "#  Boost_INCLUDE_DIRS - the path to where the boost include files are." >> ${CMAKEFILE}
echo "#  Boost_LIBRARY_DIRS - The path to where the boost library files are." >> ${CMAKEFILE}
echo "#  Boost_LIB_DIAGNOSTIC_DEFINITIONS - Only set if using Windows." >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "# ----------------------------------------------------------------------------" >> ${CMAKEFILE}
echo "# If you have installed Boost in a non-standard location or you have" >> ${CMAKEFILE}
echo "# just staged the boost files using bjam then you have three" >> ${CMAKEFILE}
echo "# options. In the following comments, it is assumed that <Your Path>" >> ${CMAKEFILE}
echo "# points to the root directory of the include directory of Boost. e.g" >> ${CMAKEFILE}
echo "# If you have put boost in C:\development\Boost then <Your Path> is" >> ${CMAKEFILE}
echo "# \"C:/development/Boost\" and in this directory there will be two" >> ${CMAKEFILE}
echo "# directories called \"include\" and \"lib\"." >> ${CMAKEFILE}
echo "# 1) After CMake runs, set Boost_INCLUDE_DIR to <Your Path>/include/boost<-version>" >> ${CMAKEFILE}
echo "# 2) Use CMAKE_INCLUDE_PATH to set a path to <Your Path>/include. This will allow FIND_PATH()" >> ${CMAKEFILE}
echo "#    to locate Boost_INCLUDE_DIR by utilizing the PATH_SUFFIXES option. e.g." >> ${CMAKEFILE}
echo "#    SET(CMAKE_INCLUDE_PATH \${CMAKE_INCLUDE_PATH} \"<Your Path>/include\")" >> ${CMAKEFILE}
echo "# 3) Set an environment variable called \${Boost_ROOT} that points to the root of where you have" >> ${CMAKEFILE}
echo "#    installed Boost, e.g. <Your Path>. It is assumed that there is at least a subdirectory called" >> ${CMAKEFILE}
echo "#    include in this path." >> ${CMAKEFILE}
echo "#" >> ${CMAKEFILE}
echo "# Note:" >> ${CMAKEFILE}
echo "#  1) If you are just using the boost headers, then you do not need to use" >> ${CMAKEFILE}
echo "#     Boost_LIBRARY_DIRS in your CMakeLists.txt file." >> ${CMAKEFILE}
echo "#  2) If Boost has not been installed, then when setting Boost_LIBRARY_DIRS" >> ${CMAKEFILE}
echo "#     the script will look for /lib first and, if this fails, then for /stage/lib." >> ${CMAKEFILE}
echo "#" >> ${CMAKEFILE}
echo "# Usage:" >> ${CMAKEFILE}
echo "# In your CMakeLists.txt file do something like this:" >> ${CMAKEFILE}
echo "# ..." >> ${CMAKEFILE}
echo "# # Boost" >> ${CMAKEFILE}
echo "# FIND_PACKAGE(Boost)" >> ${CMAKEFILE}
echo "# ..." >> ${CMAKEFILE}
echo "# INCLUDE_DIRECTORIES(\${Boost_INCLUDE_DIRS})" >> ${CMAKEFILE}
echo "# LINK_DIRECTORIES(\${Boost_LIBRARY_DIRS})" >> ${CMAKEFILE}
echo "#" >> ${CMAKEFILE}
echo "# In Windows, we make the assumption that, if the Boost files are installed, the default directory" >> ${CMAKEFILE}
echo "# will be C:\boost." >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "#" >> ${CMAKEFILE}
echo "# TODO:" >> ${CMAKEFILE}
echo "#" >> ${CMAKEFILE}
echo "# 1) Automatically find the Boost library files and eliminate the need" >> ${CMAKEFILE}
echo "#    to use Link Directories." >> ${CMAKEFILE}
echo "#" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "IF(WIN32)" >> ${CMAKEFILE}
echo "  # In windows, automatic linking is performed, so you do not have to specify the libraries." >> ${CMAKEFILE}
echo "  # If you are linking to a dynamic runtime, then you can choose to link to either a static or a" >> ${CMAKEFILE}
echo "  # dynamic Boost library, the default is to do a static link.  You can alter this for a specific" >> ${CMAKEFILE}
echo "  # library \"whatever\" by defining Boost_WHATEVER_DYN_LINK to force Boost library \"whatever\" to" >> ${CMAKEFILE}
echo "  # be linked dynamically.  Alternatively you can force all Boost libraries to dynamic link by" >> ${CMAKEFILE}
echo "  # defining Boost_ALL_DYN_LINK." >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  # This feature can be disabled for Boost library \"whatever\" by defining Boost_WHATEVER_NO_LIB," >> ${CMAKEFILE}
echo "  # or for all of Boost by defining Boost_ALL_NO_LIB." >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  # If you want to observe which libraries are being linked against then defining" >> ${CMAKEFILE}
echo "  # Boost_LIB_DIAGNOSTIC will cause the auto-linking code to emit a #pragma message each time" >> ${CMAKEFILE}
echo "  # a library is selected for linking." >> ${CMAKEFILE}
echo "  SET(Boost_LIB_DIAGNOSTIC_DEFINITIONS \"-DBoost_LIB_DIAGNOSTIC\")" >> ${CMAKEFILE}
echo "ENDIF(WIN32)" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "SET(Boost_INCLUDE_PATH_DESCRIPTION \"directory containing the boost include files. E.g /usr/local/include/boost-1_33_1 or c:/boost/include/boost-1_33_1\")" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "SET(Boost_DIR_MESSAGE \"Set the Boost_INCLUDE_DIR cmake cache entry to the \${Boost_INCLUDE_PATH_DESCRIPTION}\")" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "SET(Boost_DIR_SEARCH \$ENV{Boost_ROOT})" >> ${CMAKEFILE}
echo "IF(Boost_DIR_SEARCH)" >> ${CMAKEFILE}
echo "  FILE(TO_CMAKE_PATH \${Boost_DIR_SEARCH} Boost_DIR_SEARCH)" >> ${CMAKEFILE}
echo "  SET(Boost_DIR_SEARCH \${Boost_DIR_SEARCH}/include)" >> ${CMAKEFILE}
echo "ENDIF(Boost_DIR_SEARCH)" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "IF(WIN32)" >> ${CMAKEFILE}
echo "  SET(Boost_DIR_SEARCH" >> ${CMAKEFILE}
echo "    \${Boost_DIR_SEARCH}" >> ${CMAKEFILE}
echo "    C:/boost/include" >> ${CMAKEFILE}
echo "    D:/boost/include" >> ${CMAKEFILE}
echo "  )" >> ${CMAKEFILE}
echo "ENDIF(WIN32)" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "# Add in some path suffixes. These will have to be updated whenever a new Boost version comes out." >> ${CMAKEFILE}
echo "SET(SUFFIX_FOR_PATH" >> ${CMAKEFILE}
echo " boost-1_34_1" >> ${CMAKEFILE}
echo " boost-1_34_0" >> ${CMAKEFILE}
echo " boost-1_33_1" >> ${CMAKEFILE}
echo " boost-1_33_0" >> ${CMAKEFILE}
echo ")" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "SET (Boost_LIBRARIES \"\")" >> ${CMAKEFILE}
echo "SET (BOOST \"boost\")" >> ${CMAKEFILE}
echo "SET (Boost_VERSION \"-1_33_1\")" >> ${CMAKEFILE}
echo "SET (Boost_DEBUG \"-d\")" >> ${CMAKEFILE}
echo "# - Boost has a naming scheme that we have to match.." >> ${CMAKEFILE}
echo "IF (MINGW)" >> ${CMAKEFILE}
echo "SET (Boost_COMPILER \"-mgw\")" >> ${CMAKEFILE}
echo "ENDIF (MINGW)" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "#" >> ${CMAKEFILE}
echo "# Look for an installation." >> ${CMAKEFILE}
echo "#" >> ${CMAKEFILE}
echo "FIND_PATH(Boost_INCLUDE_DIR NAMES boost/config.hpp PATH_SUFFIXES \${SUFFIX_FOR_PATH} PATHS" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  # Look in other places." >> ${CMAKEFILE}
echo "  \${Boost_DIR_SEARCH}" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  # Help the user find it if we cannot." >> ${CMAKEFILE}
echo "  DOC \"The \${Boost_INCLUDE_PATH_DESCRIPTION}\"" >> ${CMAKEFILE}
echo ")" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "MARK_AS_ADVANCED(Boost_INCLUDE_DIR)" >> ${CMAKEFILE}
echo "# Assume we didn't find it." >> ${CMAKEFILE}
echo "SET(Boost_FOUND 0)" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "# Now try to get the include and library path." >> ${CMAKEFILE}
echo "IF(Boost_INCLUDE_DIR)" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  # Look for the boost library path." >> ${CMAKEFILE}
echo "  # Note that the user may not have installed any libraries" >> ${CMAKEFILE}
echo "  # so it is quite possible the Boost_LIBRARY_PATH may not exist." >> ${CMAKEFILE}
echo "  SET(Boost_LIBRARY_DIR \${Boost_INCLUDE_DIR})" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  IF(\"\${Boost_LIBRARY_DIR}\" MATCHES \"boost-[0-9]+\")" >> ${CMAKEFILE}
echo "    GET_FILENAME_COMPONENT(Boost_LIBRARY_DIR \${Boost_LIBRARY_DIR} PATH)" >> ${CMAKEFILE}
echo "  ENDIF (\"\${Boost_LIBRARY_DIR}\" MATCHES \"boost-[0-9]+\")" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  IF(\"\${Boost_LIBRARY_DIR}\" MATCHES \"/include\$\")" >> ${CMAKEFILE}
echo "    # Strip off the trailing \"/include\" in the path." >> ${CMAKEFILE}
echo "    GET_FILENAME_COMPONENT(Boost_LIBRARY_DIR \${Boost_LIBRARY_DIR} PATH)" >> ${CMAKEFILE}
echo "  ENDIF(\"\${Boost_LIBRARY_DIR}\" MATCHES \"/include\$\")" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  IF(EXISTS \"\${Boost_LIBRARY_DIR}/lib\")" >> ${CMAKEFILE}
echo "    SET (Boost_LIBRARY_DIR \${Boost_LIBRARY_DIR}/lib)" >> ${CMAKEFILE}
echo "  ELSE(EXISTS \"\${Boost_LIBRARY_DIR}/lib\")" >> ${CMAKEFILE}
echo "    IF(EXISTS \"\${Boost_LIBRARY_DIR}/stage/lib\")" >> ${CMAKEFILE}
echo "      SET(Boost_LIBRARY_DIR \${Boost_LIBRARY_DIR}/stage/lib)" >> ${CMAKEFILE}
echo "    ELSE(EXISTS \"\${Boost_LIBRARY_DIR}/stage/lib\")" >> ${CMAKEFILE}
echo "      SET(Boost_LIBRARY_DIR \"\")" >> ${CMAKEFILE}
echo "    ENDIF(EXISTS \"\${Boost_LIBRARY_DIR}/stage/lib\")" >> ${CMAKEFILE}
echo "  ENDIF(EXISTS \"\${Boost_LIBRARY_DIR}/lib\")" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  IF(EXISTS \"\${Boost_INCLUDE_DIR}\")" >> ${CMAKEFILE}
echo "    SET(Boost_INCLUDE_DIRS \${Boost_INCLUDE_DIR})" >> ${CMAKEFILE}
echo "    # We have found boost. It is possible that the user has not" >> ${CMAKEFILE}
echo "    # compiled any libraries so we set Boost_FOUND to be true here." >> ${CMAKEFILE}
echo "    SET(Boost_FOUND 1)" >> ${CMAKEFILE}
echo "  ENDIF(EXISTS \"\${Boost_INCLUDE_DIR}\")" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "  IF(Boost_LIBRARY_DIR AND EXISTS \"\${Boost_LIBRARY_DIR}\")" >> ${CMAKEFILE}
echo "    SET(Boost_LIBRARY_DIRS \${Boost_LIBRARY_DIR})" >> ${CMAKEFILE}
echo "  ENDIF(Boost_LIBRARY_DIR AND EXISTS \"\${Boost_LIBRARY_DIR}\")" >> ${CMAKEFILE}
echo "ENDIF(Boost_INCLUDE_DIR)" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "IF(NOT Boost_FOUND)" >> ${CMAKEFILE}
echo "  IF(NOT Boost_FIND_QUIETLY)" >> ${CMAKEFILE}
echo "    MESSAGE(STATUS \"Boost was not found. \${Boost_DIR_MESSAGE}\")" >> ${CMAKEFILE}
echo "  ELSE(NOT Boost_FIND_QUIETLY)" >> ${CMAKEFILE}
echo "    IF(Boost_FIND_REQUIRED)" >> ${CMAKEFILE}
echo "      MESSAGE(FATAL_ERROR \"Boost was not found. \${Boost_DIR_MESSAGE}\")" >> ${CMAKEFILE}
echo "    ENDIF(Boost_FIND_REQUIRED)" >> ${CMAKEFILE}
echo "  ENDIF(NOT Boost_FIND_QUIETLY)" >> ${CMAKEFILE}
echo "ENDIF(NOT Boost_FOUND)" >> ${CMAKEFILE}
echo "" >> ${CMAKEFILE}
echo "IF ( Boost_FOUND )" >> ${CMAKEFILE}
echo "  INCLUDE_DIRECTORIES( \${Boost_INCLUDE_DIRS} \${INCLUDE_DIRECTORIES})" >> ${CMAKEFILE}
echo "ENDIF ( Boost_FOUND )" >> ${CMAKEFILE}

echo "" >> ${CMAKEFILE}
echo "# ------------ Check for Each Boost Library ----------------" >> ${CMAKEFILE}
Boost_libs="date_time filesystem iostreams prg_exec_monitor program_options python regex serialization signals test_exec_monitor thread unit_test_framework wave wserialization"
for LOWER_VAR in ${Boost_libs}; do
  VAR=`echo "${LOWER_VAR}" | tr "[:lower:]" "[:upper:]"`
  echo ${LOWER_VAR} "  "${VAR} 
  echo "#-- Add the library to the Boost_LIBRARIES variable if found and the user asked for it" >> ${CMAKEFILE}
  echo "IF ( Boost_USE_${VAR} )" >> ${CMAKEFILE}
  echo "  #-- Set the Release and Debug library variables for ${VAR}" >> ${CMAKEFILE}
  echo "  SET (Boost_${VAR}_NAME \"${LOWER_VAR}\")" >> ${CMAKEFILE} 
  echo "" >> ${CMAKEFILE}
  echo "  #-- Try to find the release version of the ${LOWER_VAR} library" >> ${CMAKEFILE}
  echo "  FIND_LIBRARY(Boost_${VAR}_LIBRARY \"\${BOOST}_\${Boost_${VAR}_NAME}\${Boost_COMPILER}\${Boost_VERSION}\" " >> ${CMAKEFILE}
  echo "      \${Boost_LIBRARY_DIR}" >> ${CMAKEFILE}
  echo "  )" >> ${CMAKEFILE}
  echo "  MARK_AS_ADVANCED(Boost_${VAR}_LIBRARY)" >> ${CMAKEFILE}
  echo "" >> ${CMAKEFILE}
  echo "  #-- Try to find the debug version of the ${LOWER_VAR} library" >> ${CMAKEFILE}
  echo "  FIND_LIBRARY(Boost_${VAR}_LIBRARY_DEBUG \"\${BOOST}_\${Boost_${VAR}_NAME}\${Boost_COMPILER}\${Boost_DEBUG}\${Boost_VERSION}\"" >> ${CMAKEFILE} 
  echo "      \${Boost_LIBRARY_DIR}" >> ${CMAKEFILE}
  echo "  )" >> ${CMAKEFILE}
  echo "  MARK_AS_ADVANCED(Boost_${VAR}_LIBRARY_DEBUG)" >> ${CMAKEFILE}
  echo "" >> ${CMAKEFILE}
  echo "  #-- Check and add the release version of the library" >> ${CMAKEFILE}
  echo "  IF ( Boost_${VAR}_LIBRARY )" >> ${CMAKEFILE}
  echo "    IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)" >> ${CMAKEFILE}
  echo "      SET(Boost_LIBRARIES \${Boost_LIBRARIES}" >> ${CMAKEFILE}
  echo "        optimized \"\${Boost_${VAR}_LIBRARY}\"" >> ${CMAKEFILE}
  echo "        )" >> ${CMAKEFILE}
  echo "    ELSE (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)" >> ${CMAKEFILE}
  echo "      SET(Boost_LIBRARIES \${Boost_LIBRARIES} \"\${Boost_${VAR}_LIBRARY}\")" >> ${CMAKEFILE}
  echo "    ENDIF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)" >> ${CMAKEFILE}
  echo "  ENDIF ( Boost_${VAR}_LIBRARY )" >> ${CMAKEFILE}
  echo "  #-- Check and add the debug version of the library" >> ${CMAKEFILE}
  echo "  IF ( Boost_${VAR}_LIBRARY_DEBUG )" >> ${CMAKEFILE}
  echo "    IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)" >> ${CMAKEFILE}
  echo "      SET(Boost_LIBRARIES \${Boost_LIBRARIES}" >> ${CMAKEFILE}
  echo "        debug \"\${Boost_${VAR}_LIBRARY_DEBUG}\"" >> ${CMAKEFILE}
  echo "        )" >> ${CMAKEFILE}
  echo "    ELSE (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)" >> ${CMAKEFILE}
  echo "      SET(Boost_LIBRARIES \${Boost_LIBRARIES} \"\${Boost_${VAR}_LIBRARY}\")" >> ${CMAKEFILE}
  echo "    ENDIF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)" >> ${CMAKEFILE}
  echo "  ENDIF ( Boost_${VAR}_LIBRARY_DEBUG )	" >> ${CMAKEFILE}
  echo "ENDIF ( Boost_USE_${VAR} )" >> ${CMAKEFILE}
  echo "" >> ${CMAKEFILE}
done

exit