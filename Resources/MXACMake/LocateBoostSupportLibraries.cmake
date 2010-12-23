# --------------------------------------------------------------------
#  This file is used to gather in one spot the locating of the boost
# libraries. It is called from several different CMake files.
# --------------------------------------------------------------------

# ---------- Find Boost Headers/Libraries -----------------------
SET (Boost_FIND_REQUIRED TRUE)
SET (Boost_FIND_QUIETLY TRUE)
set (Boost_USE_MULTITHREADED TRUE)
set (Boost_USE_STATIC_LIBS TRUE)
SET (Boost_ADDITIONAL_VERSIONS "1.36" "1.36.0" "1.41" "1.41.0" "1.44" "1.44.0")

# --------------------------------------------------------------------
# MXA_BOOST_HEADERS_ONLY determines if any boost libraries are going to
# be found with the boost headers. For MXADataModel itself NO boost 
# libraries are required but if you choose to build the ImportGenerator
# program then you will need the boost program_options library.
if ( NOT MXA_BOOST_HEADERS_ONLY)
 # set (MXA_BOOST_COMPONENTS program_options )
endif()

FIND_PACKAGE(Boost COMPONENTS  ${MXA_BOOST_COMPONENTS} )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
