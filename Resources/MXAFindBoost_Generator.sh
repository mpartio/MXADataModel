#! /bin/sh
# This is a shell script that generates part of the MXAFindBoost.cmake file
# This will only generate parts of the cmake file so don't loose the original
# file.


BOOST_LIB_LIST=(date_time filesystem iostreams prg_exec_monitor program_options python regex serialization signals test_exec_monitor thread unit_test_framework wserialization )
CMAKE_VAR_LIST=(DATE_TIME FILESYSTEM IOSTREAMS PRO_EXEC_MONITOR PROGRAM_OPTIONS PYTHON REGEX SERIALIZATION SIGNALS TEST_EXEC_MONITOR THREAD UNIT_TEST_FRAMEWORK WSERIALIZATION )
varnum=13

echo ""
echo "# Use these variables to control which libraries will be used for linking."
for ((i=0;i<$varnum;i++)); do
  libName=${BOOST_LIB_LIST[${i}]}
  cmVar=${CMAKE_VAR_LIST[${i}]} 
  echo "#  BOOST_USE_${cmVar}     Link against the ${libName} Library"

done

echo ""

for ((i=0;i<$varnum;i++)); do
  libName=${BOOST_LIB_LIST[${i}]}
  cmVar=${CMAKE_VAR_LIST[${i}]} 
  echo "#  BOOST_${cmVar}_LIBRARY     The Boost ${libName} libraries"
  echo "#  BOOST_${cmVar}_LIBRARY_DEBUG     The Boost ${libName} debug library"
  echo "#  BOOST_${cmVar}_LIBRARY_RELEASE   The Boost ${libName} release library"
  echo "#"
done

echo "# ------------------------------------------------------------------------"
echo "#  Begin finding boost libraries"
echo "# ------------------------------------------------------------------------"
 
  for ((i=0;i<$varnum;i++)); do
    libName=${BOOST_LIB_LIST[${i}]}
    cmVar=${CMAKE_VAR_LIST[${i}]}    
  
    echo "# -------- Find the ${libName} Library ------------- "
    echo " SET (BOOST_LIB ${libName})"
    echo " SET (BOOST_DEBUG_LIB_NAME boost_\${BOOST_LIB}\${BOOST_COMPILER}\${BOOST_MULTITHREADED}\${BOOST_ABI_TAG}-\${BOOST_CURRENT_VERSION})"
    echo " SET (BOOST_RELEASE_LIB_NAME boost_\${BOOST_LIB}\${BOOST_COMPILER}\${BOOST_MULTITHREADED}-\${BOOST_CURRENT_VERSION})"
    echo "#-- Find a Debug Library ---------------------------------------------"
    echo " FIND_LIBRARY(BOOST_${cmVar}_LIBRARY_DEBUG"
    echo "  NAMES \${BOOST_LIB_PREFIX}\${BOOST_DEBUG_LIB_NAME}"
    echo "  PATHS \${BOOST_LIBRARIES_SEARCH_DIRS}"
    echo "  NO_DEFAULT_PATH"
    echo " )"
    echo " IF (BOOST_${cmVar}_LIBRARY_DEBUG)"
    echo "  SET(BOOST_DEBUG_LIBRARIES \${BOOST_DEBUG_LIBRARIES} \${BOOST_${cmVar}_LIBRARY_DEBUG} )"
    echo " ENDIF (BOOST_${cmVar}_LIBRARY_DEBUG)"
    echo "#-- Find a Release Library -------------------------------------------"
    echo " FIND_LIBRARY(BOOST_${cmVar}_LIBRARY_RELEASE"
    echo "  NAMES \${BOOST_LIB_PREFIX}\${BOOST_RELEASE_LIB_NAME}"
    echo "  PATHS \${BOOST_LIBRARIES_SEARCH_DIRS}"
    echo "  NO_DEFAULT_PATH"
    echo " )"
    echo " IF (BOOST_${cmVar}_LIBRARY_RELEASE)"
    echo "  SET(BOOST_RELEASE_LIBRARIES \${BOOST_DEBUG_LIBRARIES} \${BOOST_${cmVar}_LIBRARY_RELEASE} )"
    echo " ENDIF (BOOST_${cmVar}_LIBRARY_RELEASE)" 
    echo "# -- Adjust all the library variables --------------------------------"
    echo "_BOOST_ADJUST_LIB_VARS(${cmVar})"
    echo ""


done

echo "# ------------------------------------------------------------------------"
echo "#  End finding boost libraries"
echo "# ------------------------------------------------------------------------"


