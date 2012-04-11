
# ------ Add support for HDF5
SET(MXA_USE_HDF5 ON)
SET(MXA_EXTERNAL_HDF5_LIBRARY ${PARAVIEW_HDF5_LIBRARIES})
SET(HDF5_FOUND 1)
SET(HDF5_LIBRARIES ${PARAVIEW_HDF5_LIBRARIES})
SET(HDF5_LIBRARY  ${PARAVIEW_HDF5_LIBRARIES} CACHE FILEPATH "" )

INCLUDE_DIRECTORIES( ${HDF5_INCLUDE_DIR} )

# ------- Add support for Expat and XML parsing using the Expat that comes with ParaView/VTK
SET (MXA_USE_XML  ON)
SET (MXA_EXTERNAL_EXPAT_LIBRARY vtkexpat)
SET (EXPAT_FOUND 1)
SET (EXPAT_LIBRARIES vtkexpat)
SET (EXPAT_INCLUDE_DIRS "${ParaView_SOURCE_DIR}/VTK/Utilities/vtkexpat")

# ---- Remove support for Tiff image functionality
SET (MXA_USE_TIFF OFF)

#-- Remove MXA Examples from being built
SET (MXA_BUILD_EXAMPLES OFF CACHE BOOL "" )

#-- Remove the MXA Testing from being built
SET (MXA_BUILD_TESTING FALSE)

#-- Remove the MXA Utilities from being built
SET (MXA_BUILD_UTILITIES FALSE)

#-- Set the Name of the MXA Library. This should ALWAYS be MXADataModel
SET (MXADATAMODEL_LIB_NAME MXADataModel)

#-- ParaView does not like it when we use specific Debug suffixes for library names
#-- so turn it off here
SET (MXA_USE_DEBUG_LIBRARY_NAME FALSE)

#-- We are NOT going to install the default set of files from the MXA Distribution
set (MXA_INSTALL_FILES 0 CACHE INTERNAL "Ignore the MXA Install Rules, defaulting to the enclosing projects install rules")
set (MXA_BUILD_TESTING_SAVE ${MXA_BUILD_TESTING})

#-- ParaView already defines where to place output files like libraries and executables
SET (MXA_OUTPUT_DIRS_DEFINED 1 CACHE INTERNAL "The CMake output directories have already been defined")

#-- We need some more boost libraries?
set (MXA_BOOST_HEADERS_ONLY 0)
INCLUDE ("${MXADataModel_SOURCE_DIR}/Resources/MXACMake/LocateBoostSupportLibraries.cmake" )

# --------------------------------------------------------------------
#-- Build the MXADataModel library
# --------------------------------------------------------------------
ADD_SUBDIRECTORY("${MXADataModel_SOURCE_DIR}" ${ParaView_BINARY_DIR}/${MXADATAMODEL_LIB_NAME})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})  # Include the Boost Headers
SET (MXA_BUILD_TESTING ${MXA_BUILD_TESTING_SAVE})
SET (MXADATAMODEL_TARGET_ADDED TRUE )
MARK_AS_ADVANCED(MXA_BUILD_WITH_INSTALL_NAME)


SET (MXADATAMODEL_TARGET_ADDED "1" CACHE INTERNAL "MXADataModel Added to ParaView as a libary")
MESSAGE (STATUS "[${MODULE_NAME}] Added the MXADataModel library.")

#-- Set some inlucde directories that will be needed later on in the Build
INCLUDE_DIRECTORIES (${MXADataModel_SOURCE_DIR}/Source)
INCLUDE_DIRECTORIES (${ParaView_BINARY_DIR}/MXADataModel)

#-- Include a CMake file with some Macros that will be needed.
#include (${MXADataModel_SOURCE_DIR}/Resources/cmp/cmpCMakeMacros.cmake)
#CMP_LIBRARIES_INSTALL_RULES ("hdf5" "bin")
