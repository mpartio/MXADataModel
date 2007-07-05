# set project's name
PROJECT( MXADataModel )

# - Check to see if any other project is redefining the library names
IF ( NOT DEFINED  MXADATAMODEL_LIB_NAME)
  SET (MXADATAMODEL_LIB_NAME MXADataModel)
ENDIF ( NOT DEFINED MXADATAMODEL_LIB_NAME )

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

# ------- Enable the CTest testing. Use make test to run all tests ---------------
OPTION(BUILD_TESTING "Build ParaView Complete Testing" ON)
IF(BUILD_TESTING)
  ENABLE_TESTING()
ENDIF(BUILD_TESTING)

# -------- Add some definitions for Debug Builds -----------------
IF ( CMAKE_BUILD_TYPE MATCHES Debug )
  ADD_DEFINITIONS(-DDEBUG)
  ADD_DEFINITIONS(-Wall)
ENDIF ( CMAKE_BUILD_TYPE MATCHES Debug )


# ---------- Find Boost Headers/Libraries -----------------------
SET (Boost_USE_FILESYSTEM TRUE)
INCLUDE (${PROJECT_SOURCE_DIR}/Resources/FindBoost.cmake)
IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Boost was not found. ${Boost_DIR_MESSAGE}")
ENDIF(NOT Boost_FOUND)


# Extend the include path to include the MXADataModel source directory
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/src" )
# ----- Also include our binary directory as some headers are generated into that directory
INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}" ${INCLUDE_DIRECTORIES} )

#-- Include the MXADataModel Testing Sources
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/Testing ${PROJECT_BINARY_DIR}/Testing)

#-- Include the Examples Directory
OPTION(BUILD_EXAMPLES "Build Programs that demonstrate the use of the API" OFF)
IF(BUILD_EXAMPLES)
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/Examples ${PROJECT_BINARY_DIR}/Examples)
ENDIF(BUILD_EXAMPLES)

# Build shared libraries
OPTION (MXA_BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
SET (LIB_TYPE STATIC)
IF (MXA_BUILD_SHARED_LIBS)
  SET (LIB_TYPE SHARED)
  IF (WIN32)
    ADD_DEFINITIONS("-DMXA_BUILD_SHARED_LIBS")
  ENDIF (WIN32)
ENDIF (MXA_BUILD_SHARED_LIBS)

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
)
  
#--- Adding headers is NOT needed unless you want Visual Studio or Xcode to have them in their projects 
SET (MXADATAMODEL_HEADERS
  src/Headers/LogTime.h
  src/Headers/DLLExport.h
  src/Headers/MXATypeDefs.h
  src/Headers/MXATypes.h
  src/Headers/MXAErrorDefinitions.h
  src/Interfaces/IAttribute.h
  src/Interfaces/IAttributeHelper.h
  src/Interfaces/IAttributeWriter.h
  src/Interfaces/IDataDimension.h
  src/Interfaces/IDataDimensionWriter.h
  src/Interfaces/IDataImport.h
  src/Interfaces/IDataImportDelegate.h
  src/Interfaces/IDataModel.h
  src/Interfaces/IDataModelReader.h
  src/Interfaces/IDataModelWriter.h
  src/Interfaces/IDataRecord.h
  src/Interfaces/IDataRecordWriter.h
  src/Interfaces/IDataSource.h
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
  src/Utilities/StringUtils.h
  )

# Build with the HDF5 Libraries Enabled. This is the default
SET(HDF5_SUPPORT "0")
SET(HDF5_VTK_BRIDGE "0")
OPTION (MXA_USE_HDF5 "Compiles Sources dependent on HDF5 Libraries" ON)
IF ( MXA_USE_HDF5 )
  # ---------- Find the HDF Libraries - DO NOT use the High Level APIs from the HDF5
  # group as those API's will not compile correctly under windows --------------
  SET (HDF5_USE_HIGH_LEVEL FALSE)
  INCLUDE( ${PROJECT_SOURCE_DIR}/Resources/FindHDF.cmake )
  IF(NOT HDF5_FOUND)
    MESSAGE(FATAL_ERROR "HDF5 was not found. Set the include/lib manually or set the HDF_INSTALL environment variable")
  ENDIF(NOT HDF5_FOUND)
  # Enable the HDF5_MODULES_ON when using the HDF dynamic libraries

  IF (MXA_BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DHDF5_MODULES_ON)
      IF (WIN32)
        ADD_DEFINITIONS(-D_HDF5USEDLL_)
        ADD_DEFINITIONS(-DMXA_BUILD_SHARED_LIBS)
      ENDIF (WIN32)
  ENDIF (MXA_BUILD_SHARED_LIBS)
  
  # ---------- Include the HDF5 Dependant source in the Build -------------
  SET( MXA_HDF5_SRCS
    src/HDF5/H5Image.cpp
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
    src/HDF5/H5Image.h
    src/HDF5/H5Utilities.h
  )
  

  SET(DEP_LIBS ${DEP_LIBS} ${HDF5_LIBRARIES})
  SET(HDF5_SUPPORT "1")
  OPTION(BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)
  IF(BUILD_VTK_HDF5)
    #-- Find VTK packages -----------------------------------------
    FIND_PACKAGE(VTK REQUIRED)
    INCLUDE(${VTK_USE_FILE})
    
    # List the kits from VTK that are needed by this project.
    SET(vtkLocal_LIBS vtkCommon vtksys vtkFiltering vtkIO vtkGraphics )
    
    SET (MXA_HDF5_SRCS ${MXA_HDF5_SRCS} src/HDF5/vtkHDF5.cpp)
    SET (MXA_HDF5_HEADERS ${MXA_HDF5_HEADERS} src/HDF5/vtkHDF5.h)
    SET (DEP_LIBS ${DEP_LIBS} ${vtkLocal_LIBS})
    SET(HDF5_VTK_BRIDGE "1")
  ENDIF (BUILD_VTK_HDF5)
  #-- Finally include all the sources and headers from the HDf5 and vtk sections
  SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_SRCS} ${MXA_HDF5_HEADERS})
ENDIF ( MXA_USE_HDF5 )

# ------------------ Check for and Include Tif Lib dependent code --------------
SET (TIFF_SUPPORT "0")
OPTION(MXA_USE_TIFF "Compiles source code that depends on having the Tif Lib installed" ON)
IF(MXA_USE_TIFF)

	FIND_PACKAGE(TIFF REQUIRED)
	SET(MXA_HDF5_TIFF_SRCS
		src/HDF5/H5TiffIO.cpp
		src/HDF5/H5TiffImportDelegate.cpp
		)
		
	SET(MXA_HDF5_TIFF_HEADERS
		src/HDF5/H5TiffIO.h
		src/HDF5/H5TiffImportDelegate.h
		)
		
	SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_TIFF_SRCS} ${MXA_HDF5_TIFF_HEADERS})
	SET(DEP_LIBS ${DEP_LIBS} ${TIFF_LIBRARIES})
	SET (TIFF_SUPPORT "1")
ENDIF (MXA_USE_TIFF)

#--------- XML Sources ---------------------------------------------------
SET (XML_SUPPORT "0")
OPTION(MXA_USE_XML "Adds Ability to read/write models to XML Files" ON)
IF (MXA_USE_XML)
    FIND_PACKAGE(EXPAT REQUIRED)
    IF ( NOT EXPAT_FOUND )
       MESSAGE(FATAL_ERROR "Expat was not found. Set the include/lib manually. The XML Functionality depends on it")
    ENDIF ( NOT EXPAT_FOUND)
   
    SET ( MXA_XML_SRCS
        src/XML/ExpatParser.cpp
        src/XML/ExpatEvtHandler.cpp
        src/XML/XMLIODelegate.cpp
        src/XML/XMLDataModelWriter.cpp
        src/XML/XMLDataModelReader.cpp
    )
    
    SET (MXA_XML_HEADERS
        src/XML/ExpatParser.h
        src/XML/ExpatEvtHandler.h
        src/XML/XMLIODelegate.h
        src/XML/XMLDataModelWriter.h
        src/XML/XMLDataModelReader.h
        src/XML/XMLUserMetaDataWriter.h
    )
    
    SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_XML_SRCS} ${MXA_XML_HEADERS})
    SET(DEP_LIBS ${DEP_LIBS} ${EXPAT_LIBRARIES})
    SET (XML_SUPPORT "1")
ENDIF(MXA_USE_XML)


# ---------- Set up the Project Options -----------------------
IF(WIN32)
 SET(GUI_TYPE WIN32)
ENDIF(WIN32)



#-- Configure the Header file and then add the Binary Directory to the Includes list
CONFIGURE_FILE(${MXADataModel_SOURCE_DIR}/src/Headers/MXAConfiguration.h.in
               ${PROJECT_BINARY_DIR}/MXAConfiguration.h @ONLY IMMEDIATE)
SET (MXADATAMODEL_HEADERS ${MXADATAMODEL_HEADERS} ${PROJECT_BINARY_DIR}/MXAConfiguration.h  )
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")
LINK_DIRECTORIES(${Boost_LIBRARY_DIR}) 

# - Combine all the source files together -----------------------------
SET ( PROJECT_SRCS  ${MXADATAMODEL_SRCS} ${MXADATAMODEL_HEADERS})

#-- Add the MXADataModel Library and link to needed libraries-------------------
ADD_LIBRARY (${MXADATAMODEL_LIB_NAME} ${LIB_TYPE} ${PROJECT_SRCS} )
TARGET_LINK_LIBRARIES (${MXADATAMODEL_LIB_NAME} ${DEP_LIBS} ${Boost_LIBRARIES} )
# ----- OS X specific build settings
IF ( APPLE )
#  SET (OSX_EMBEDDED_LIBRARY ON)
  SET_TARGET_PROPERTIES (${MXADATAMODEL_LIB_NAME}
    PROPERTIES INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib
    )
    
ENDIF ( APPLE )


#--- Set up the installation Code for Make Install
INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
SET (INSTALL_INCLUDES 
  src/MXADataModel
  src/HDF5
  src/Headers
  src/Interfaces
  src/Utilities
  src/XML
  )
  
INSTALL ( FILES ${PROJECT_BINARY_DIR}/MXAConfiguration.h DESTINATION include/MXADataModel/Headers )
  
INSTALL(DIRECTORY ${INSTALL_INCLUDES}  DESTINATION include/MXADataModel
  PATTERN "CVS" EXCLUDE
  PATTERN "*.c*" EXCLUDE
  PATTERN "*.in" EXCLUDE
  )
  
  