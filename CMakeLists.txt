# set project's name
PROJECT( MXADataModel )

# - Check to see if any other project is redefining the library names
IF ( NOT DEFINED  MXADATAMODEL_LIB_NAME)
  SET (MXADATAMODEL_LIB_NAME MXADataModel)
ENDIF ( NOT DEFINED MXADATAMODEL_LIB_NAME )

IF ( NOT DEFINED  QT_MXADATAMODEL_LIB_NAME)
  SET (QT_MXADATAMODEL_LIB_NAME QtMHDataModel)
ENDIF ( NOT DEFINED QT_MXADATAMODEL_LIB_NAME )

IF ( NOT DEFINED  MXA_HDF_INTERFACE_LIB_NAME)
	SET( MXA_HDF_INTERFACE_LIB_NAME HDFWrapper)
ENDIF ( NOT DEFINED MXA_HDF_INTERFACE_LIB_NAME )


# ---------- Setup output Directories -------------------------
SET (LIBRARY_OUTPUT_PATH
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Libraries"
  )

# --------- Setup the Executable output Directory -------------
SET (EXECUTABLE_OUTPUT_PATH
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Executables."
  )

IF ( CMAKE_BUILD_TYPE MATCHES Debug )
  ADD_DEFINITIONS(-DDEBUG)
  ADD_DEFINITIONS(-Wall)
ENDIF ( CMAKE_BUILD_TYPE MATCHES Debug )
IF(WIN32)
 ADD_DEFINITIONS(-DNODEFAULTLIB:library)
ENDIF(WIN32)


# ---------- Find TIFF Library --------------------------------
#FIND_PACKAGE(TIFF)
  
# ---------- Find ZLIB Library --------------------------------
#FIND_PACKAGE(ZLIB)

# ---------- Find the SZip Libraries -------------------------
OPTION(HDF5_USE_SZIP "HDF5 Library Requires SZip Library" OFF)
IF (HDF5_USE_SZIP)
  SET (SZLIB_FIND_REQUIRED TRUE)
  INCLUDE( ${PROJECT_SOURCE_DIR}/CMake/FindSZip.cmake )
ENDIF (HDF5_USE_SZIP)

# ---------- Find Boost Headers/Libraries -----------------------
INCLUDE (${PROJECT_SOURCE_DIR}/CMake/FindBoost.cmake)
IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Boost was not found. ${Boost_DIR_MESSAGE}")
ENDIF(NOT Boost_FOUND)

# ------ Make sure MXAFInterface Library is built First -------
#SUBDIRS( ${PROJECT_SOURCE_DIR}/../MXA-HDF5Interface/ )

# Extend the include path to include the MXADataModel source directory
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/src"  ${INCLUDE_DIRECTORIES} )

# ---------- Set up the Project Options -----------------------
# Build shared libraries
OPTION (BUILD_SHARED "Build Shared Libraries" OFF)

#-- Include the MXADataModel Testing Sources
SUBDIRS(${PROJECT_SOURCE_DIR}/src/Testing)

#-- Include the Examples Directory
SUBDIRS(${PROJECT_SOURCE_DIR}/src/Examples)

# ----------  Build Qt Data Model  -----------------------
OPTION (BUILD_Qt_MODULES "Builds the Qt Data Model" OFF)
IF ( BUILD_Qt_MODULES )
  SUBDIRS(src/QtMXADataModel)
ENDIF ( BUILD_Qt_MODULES )

# ------- Set up the MHDataModel Project Source Files -----------
# with SET() command you can change variables or define new ones
# here we define MXADATAMODEL_SRCS variable that contains a list 
# of all .cpp files note that we don't need \ at the end of line
SET( MXADATAMODEL_SRCS
	src/MXADataModel/MXAAttribute.cpp
  src/MXADataModel/MXANode.cpp
  src/MXADataModel/MXADataDimension.cpp
  src/MXADataModel/MXADataRecord.cpp
  src/MXADataModel/MXADataModel.cpp
  src/MXADataModel/MXADataSource.cpp
  src/MXADataModel/MXADataImport.cpp
  src/XML/XMLDataModelWriter.cpp
)
  
#--- Adding headers is NOT needed unless you want Visual Studio or Xcode to have them in their projects 
SET (MXADATAMODEL_HEADERS
  src/Headers/MXATypeDefs.h
  src/Headers/MXATypes.h
  src/Interfaces/IAttribute.h
  src/Interfaces/IAttributeHelper.h
  src/Interfaces/IAttributeWriter.h
  src/Interfaces/IDataDimension.h
  src/Interfaces/IDataImport.h
  src/Interfaces/IDataImportDelegate.h
  src/Interfaces/IDataModel.h
  src/Interfaces/IDataModelReader.h
  src/Interfaces/IDataModelWriter.h
  src/Interfaces/IDataRecord.h
  src/Interfaces/IFileIODelegate.h
  src/Interfaces/INode.h
  src/MXADataModel/AttributeHelpers.h
  src/MXADataModel/MXAAttribute.h
  src/MXADataModel/MXAConstants.h
  src/MXADataModel/MXADataDimension.h
  src/MXADataModel/MXADataImport.h
  src/MXADataModel/MXADataModel.h
  src/MXADataModel/MXADataRecord.h
  src/MXADataModel/MXADataSource.h
  src/MXADataModel/MXANode.h
  src/XML/XMLDataModelWriter.h
  src/Utilities/StringUtils.h
  )
  
OPTION (BUILD_HDF5_MODULES "Compiles Sources dependent on HDF5 Libraries" ON)
IF ( BUILD_HDF5_MODULES )

  # ---------- Find the HDF Libraries ---------------------------
  SET (HDF5_USE_HIGH_LEVEL FALSE)
  INCLUDE( ${PROJECT_SOURCE_DIR}/CMake/FindHDF.cmake )
  IF(NOT HDF5_FOUND)
    MESSAGE(FATAL_ERROR "HDF5 was not found. Set the include/lib manually or set the HDF_INSTALL environment variable")
  ENDIF(NOT HDF5_FOUND)
  
  ADD_DEFINITIONS(-DHDF5_MODULES_ON)
  # ---------- Include the HDF5 Dependant source in the Build -------------
  SET( MXA_HDF5_SRCS
    src/HDF5/H5Lite.cpp
    src/HDF5/H5Utilities.cpp
    src/HDF5/H5IODelegate.cpp
    src/HDF5/H5DataModelReader.cpp
    src/HDF5/H5DataModelWriter.cpp
  )
  
  SET(MXA_HDF5_HEADERS 
    src/HDF5/H5AttributeWriter.h
    src/HDF5/H5DataModelReader.h
    src/HDF5/H5DataModelWriter.h
    src/HDF5/H5IODelegate.h
    src/HDF5/H5Lite.h
    src/HDF5/H5Utilities.h
  )
  SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_SRCS} ${MXA_HDF5_HEADERS})
ENDIF ( BUILD_HDF5_MODULES )

#--------- XML Sources
SET( MXA_XML_SRCS
  src/XMLTools/MHDXMLGenerator.cpp
  src/XMLTools/MHDXML.cpp
)  

#--- Tiff Sources
SET (MXA_TIFF_IO_SRCS
  src/ImageIO/MHDTiffIO.cpp
  src/ImageIO/TiffToHDFDelegate.cpp  
  )

# - Combine all the source files together
SET ( PROJECT_SRCS  ${MXADATAMODEL_SRCS} ${MXADATAMODEL_HEADERS})

# Include Libraries
SET (EXTRA_LIBS 
  ${ZLIB_LIBRARIES}
  ${HDF5_LIBRARIES}
  ${TIFF_LIBRARIES}
  ${SZLIB_LIBRARIES}
)


#---------- Build the MXADataModel Library ----------------------
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED)
  SET (LIB_TYPE SHARED)
  IF (WIN32)
    ADD_DEFINITIONS("-D_HDF5USEDLL_")
  ENDIF (WIN32)
ENDIF (BUILD_SHARED)

IF ( APPLE )
  SET (OSX_EMBEDDED_LIBRARY ON)
ENDIF ( APPLE )


ADD_LIBRARY (${MXADATAMODEL_LIB_NAME} ${LIB_TYPE} ${PROJECT_SRCS} )
TARGET_LINK_LIBRARIES (${MXADATAMODEL_LIB_NAME} ${EXTRA_LIBS} )


#ADD_EXECUTABLE(MiscTesting src/Misc/main.cpp)
#TARGET_LINK_LIBRARIES(MiscTesting ${HDF5_LIBRARIES})

