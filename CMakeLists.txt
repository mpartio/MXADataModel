# set project's name
PROJECT( MXADataModel )
IF (NOT DEFINED MXA_SOURCE_DIR)
    SET (MXA_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(NOT DEFINED MXA_SOURCE_DIR)

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
  
#-------------------------------------------------------------------------------
#  Define the location of the Resources Directory
#-------------------------------------------------------------------------------
SET (MXA_RESOURCES_DIR ${MXA_SOURCE_DIR}/Resources)

#-------------------------------------------------------------------------------
#  Run all the configure checks
INCLUDE(${MXA_RESOURCES_DIR}/ConfigureChecks.cmake)

# ------- Enable the CTest testing. Use make test to run all tests ---------------
OPTION(BUILD_TESTING "Build MXADataModel Complete Testing" ON)
IF(BUILD_TESTING)
  ENABLE_TESTING()
ENDIF(BUILD_TESTING)

# Extend the include path to include the MXADataModel source directory
INCLUDE_DIRECTORIES( "${MXA_SOURCE_DIR}/src" )
# ----- Also include our binary directory as some headers are generated into that directory
INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}" ${INCLUDE_DIRECTORIES} )

# Build shared libraries
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
  SET (LIB_TYPE SHARED)
  IF (WIN32)
    ADD_DEFINITIONS("-DBUILD_SHARED_LIBS")
  ENDIF (WIN32)
ENDIF (BUILD_SHARED_LIBS)

#-- Find all the support libraries needed
INCLUDE (${MXA_RESOURCES_DIR}/FindSupportLibraries.cmake)

# ------- Set up the MXADataModel Project Source Files -----------
SET( MXADATAMODEL_SRCS
  ${MXA_SOURCE_DIR}/src/Common/IO/Reader64.cpp
  ${MXA_SOURCE_DIR}/src/Common/IO/Writer64.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataDimension.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataRecord.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataModel.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataSource.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXADataImport.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXAAbstractAttribute.cpp
  ${MXA_SOURCE_DIR}/src/Core/MXAAbstractData.cpp
  ${MXA_SOURCE_DIR}/src/DataImport/ImportDelegateManager.cpp
  ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpIO.cpp
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathIndexSection.cpp
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathTextSection.cpp
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathBuilder.cpp
)
  
#--- Adding headers is NOT needed unless you want Visual Studio or Xcode to have them in their projects 
SET (MXADATAMODEL_HEADERS
  ${MXA_SOURCE_DIR}/src/Common/LogTime.h
  ${MXA_SOURCE_DIR}/src/Common/DLLExport.h
  ${MXA_SOURCE_DIR}/src/Common/MXATypeDefs.h
  ${MXA_SOURCE_DIR}/src/Common/MXATypes.h
  ${MXA_SOURCE_DIR}/src/Common/MXAErrorDefinitions.h
  ${MXA_SOURCE_DIR}/src/Common/IO/Reader64.h
  ${MXA_SOURCE_DIR}/src/Common/IO/Writer64.h
  ${MXA_SOURCE_DIR}/src/Common/Cast/Cast.h
  ${MXA_SOURCE_DIR}/src/Base/IAttribute.h
  ${MXA_SOURCE_DIR}/src/Base/IAttributeHelper.h
  ${MXA_SOURCE_DIR}/src/Base/IAttributeWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IDataDimension.h
  ${MXA_SOURCE_DIR}/src/Base/IDataDimensionWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IDataFile.h
  ${MXA_SOURCE_DIR}/src/Base/IDataImport.h
  ${MXA_SOURCE_DIR}/src/Base/IImportDelegate.h
  ${MXA_SOURCE_DIR}/src/Base/IDataModel.h
  ${MXA_SOURCE_DIR}/src/Base/IDataModelReader.h
  ${MXA_SOURCE_DIR}/src/Base/IDataModelWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IDataRecord.h
  ${MXA_SOURCE_DIR}/src/Base/IDataRecordWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IDataSource.h
  ${MXA_SOURCE_DIR}/src/Base/IFileIODelegate.h
  ${MXA_SOURCE_DIR}/src/Base/INode.h
  ${MXA_SOURCE_DIR}/src/Base/IFileReader.h
  ${MXA_SOURCE_DIR}/src/Base/IFileWriter.h
  ${MXA_SOURCE_DIR}/src/Base/IAbstractDataArray.h
  ${MXA_SOURCE_DIR}/src/Base/IAbstractDataset.h
  ${MXA_SOURCE_DIR}/src/Base/IDataFileIO.h
  ${MXA_SOURCE_DIR}/src/Base/IDataset.h
  ${MXA_SOURCE_DIR}/src/Base/IRequiredMetaData.h
  ${MXA_SOURCE_DIR}/src/Core/AttributeHelpers.h
  ${MXA_SOURCE_DIR}/src/Core/MXAConstants.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataDimension.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataImport.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataModel.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataRecord.h
  ${MXA_SOURCE_DIR}/src/Core/MXADataSource.h
  ${MXA_SOURCE_DIR}/src/Core/MXAAbstractAttribute.h
  ${MXA_SOURCE_DIR}/src/Core/MXAAbstractData.h
  ${MXA_SOURCE_DIR}/src/Utilities/StringUtils.h
  ${MXA_SOURCE_DIR}/src/Utilities/IStringSection.h
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathIndexSection.h
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathTextSection.h
  ${MXA_SOURCE_DIR}/src/Utilities/DataSourcePathBuilder.h
  ${MXA_SOURCE_DIR}/src/DataImport/AbstractImportDelegateFactory.h
  ${MXA_SOURCE_DIR}/src/DataImport/ImportDelegateManager.h
  ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpIO.h
  ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpHeaders.h
)

#--------- XML Sources ---------------------------------------------------
IF (MXA_USE_XML)
    SET ( MXA_XML_SRCS
        ${MXA_SOURCE_DIR}/src/XML/ExpatParser.cpp
        ${MXA_SOURCE_DIR}/src/XML/ExpatEvtHandler.cpp
        ${MXA_SOURCE_DIR}/src/XML/XMLIODelegate.cpp
        ${MXA_SOURCE_DIR}/src/XML/XMLDataModelWriter.cpp
        ${MXA_SOURCE_DIR}/src/XML/XMLDataModelReader.cpp
        ${MXA_SOURCE_DIR}/src/DataImport/DataImportXmlParser.cpp
    )
    
    SET (MXA_XML_HEADERS
        ${MXA_SOURCE_DIR}/src/XML/ExpatParser.h
        ${MXA_SOURCE_DIR}/src/XML/ExpatEvtHandler.h
        ${MXA_SOURCE_DIR}/src/XML/XMLIODelegate.h
        ${MXA_SOURCE_DIR}/src/XML/XMLDataModelWriter.h
        ${MXA_SOURCE_DIR}/src/XML/XMLDataModelReader.h
        ${MXA_SOURCE_DIR}/src/XML/XMLUserMetaDataWriter.h
        ${MXA_SOURCE_DIR}/src/DataImport/DataImportXmlParser.h
    )
    
    SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_XML_SRCS} ${MXA_XML_HEADERS})
    SET (XML_SUPPORT "1")
ENDIF(MXA_USE_XML)

#------- HDF5 Dependant Sources ------------------------------
IF ( MXA_USE_HDF5 )
  SET( MXA_HDF5_SRCS
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXADataFile.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Image.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Lite.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5Utilities.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelReader.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelWriter.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpIO.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpImportDelegate.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpImportDelegateFactory.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5AsciiStringData.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5RGBImage.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5AsciiStringAttribute.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXADataset.cpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXARequiredMetaData.cpp
  )
  
  SET(MXA_HDF5_HEADERS
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXADataFile.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5AttributeWriter.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelReader.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataModelWriter.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Lite.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Image.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Utilities.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpIO.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpImportDelegate.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5BmpImportDelegateFactory.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5AttributeArrayTemplate.hpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5DataArrayTemplate.hpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5AsciiStringData.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5AsciiStringAttribute.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5RGBImage.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5Data2DArray.hpp
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXADataset.h
    ${MXA_SOURCE_DIR}/src/HDF5/H5MXARequiredMetaData.h
  )
  
  SET(HDF5_SUPPORT "1")
  OPTION(BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)
  IF(BUILD_VTK_HDF5)
    #-- Find VTK packages -----------------------------------------
    FIND_PACKAGE(VTK REQUIRED)
    INCLUDE(${VTK_USE_FILE})
      
    # List the kits from VTK that are needed by this project.
    SET(vtkLocal_LIBS vtkCommon vtksys vtkFiltering vtkIO vtkGraphics )
    
    SET (MXA_HDF5_SRCS ${MXA_HDF5_SRCS} ${MXA_SOURCE_DIR}/src/HDF5/vtkHDF5.cpp)
    SET (MXA_HDF5_HEADERS ${MXA_HDF5_HEADERS} ${MXA_SOURCE_DIR}/src/HDF5/vtkHDF5.h)
    SET (DEP_LIBS ${DEP_LIBS} ${vtkLocal_LIBS})
    SET(HDF5_VTK_BRIDGE "1")
  ENDIF (BUILD_VTK_HDF5)
  #-- Finally include all the sources and headers from the HDf5 and vtk sections
  SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_SRCS} ${MXA_HDF5_HEADERS})
ENDIF ( MXA_USE_HDF5 )

# ------------------ Check for and Include Tif Lib dependent code --------------
IF(MXA_USE_TIFF)
    SET (TIFF_SUPPORT "1")
    
    SET(MXA_HDF5_TIFF_SRCS
        ${MXA_SOURCE_DIR}/src/HDF5/H5TiffIO.cpp
        ${MXA_SOURCE_DIR}/src/HDF5/H5TiffImportDelegate.cpp
        ${MXA_SOURCE_DIR}/src/HDF5/H5TiffImportDelegateFactory.cpp
        )
        
    SET(MXA_HDF5_TIFF_HEADERS
        ${MXA_SOURCE_DIR}/src/HDF5/H5TiffIO.h
        ${MXA_SOURCE_DIR}/src/HDF5/H5TiffImportDelegate.h
        ${MXA_SOURCE_DIR}/src/HDF5/H5TiffImportDelegateFactory.h
        )
    SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS} ${MXA_HDF5_TIFF_SRCS} ${MXA_HDF5_TIFF_HEADERS})
ENDIF (MXA_USE_TIFF)

# ---------- Set up the Project Options -----------------------
IF(WIN32)
 SET(GUI_TYPE WIN32)
ENDIF(WIN32)

# -------- Add some definitions for Debug Builds -----------------
IF ( CMAKE_BUILD_TYPE MATCHES Debug )
  ADD_DEFINITIONS(-DDEBUG)
  ADD_DEFINITIONS(-Wall)
ENDIF ( CMAKE_BUILD_TYPE MATCHES Debug )

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

#-- Configure the Header file and then add the Binary Directory to the Includes list
CONFIGURE_FILE(${MXA_SOURCE_DIR}/src/Common/MXAConfiguration.h.in
               ${PROJECT_BINARY_DIR}/MXAConfiguration.h @ONLY IMMEDIATE)
SET (MXADATAMODEL_HEADERS ${MXADATAMODEL_HEADERS} ${PROJECT_BINARY_DIR}/MXAConfiguration.h  )
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

#-- Configure our UseMXADataModel.cmake file
CONFIGURE_FILE ( ${MXA_RESOURCES_DIR}/UseMXADataModel.cmake.in
                 ${PROJECT_BINARY_DIR}/UseMXADataModel.cmake @ONLY IMMEDIATE)

# - Combine all the source files together -----------------------------
SET ( PROJECT_SRCS  ${MXADATAMODEL_SRCS} ${MXADATAMODEL_HEADERS})

#-- Add the MXADataModel Library and link to needed libraries-------------------
ADD_LIBRARY (${MXADATAMODEL_LIB_NAME} ${LIB_TYPE} ${PROJECT_SRCS} )
TARGET_LINK_LIBRARIES (${MXADATAMODEL_LIB_NAME} ${DEP_LIBS} )

# ----- OS X specific build settings
IF ( APPLE )
  SET_TARGET_PROPERTIES (${MXADATAMODEL_LIB_NAME}
    PROPERTIES INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib
    )  
ENDIF ( APPLE )

#IF (WIN32)
    SET (MXA_LIB_DEBUG_NAME "")
    SET (MXA_LIB_RELEASE_NAME "")
    SET (MXA_LIB_STATIC_PREFIX "")
    IF (WIN32)
       SET (MXA_LIB_STATIC_PREFIX "lib")
    ENDIF(WIN32)
    IF (BUILD_SHARED_LIBS)
        SET(MXA_LIB_RELEASE_NAME "MXADataModel")
        SET(MXA_LIB_DEBUG_NAME "MXADataModel_D")
    ELSE (BUILD_SHARED_LIBS)
        SET(MXA_LIB_RELEASE_NAME "${MXA_LIB_STATIC_PREFIX}MXADataModel")
        SET(MXA_LIB_DEBUG_NAME "${MXA_LIB_STATIC_PREFIX}MXADataModel_D")
    ENDIF(BUILD_SHARED_LIBS)
    SET_TARGET_PROPERTIES( ${MXADATAMODEL_LIB_NAME}
        PROPERTIES
        DEBUG_OUTPUT_NAME ${MXA_LIB_DEBUG_NAME}
        RELEASE_OUTPUT_NAME ${MXA_LIB_RELEASE_NAME}
    )
#ENDIF (WIN32)


#--- Set up the installation Code for Make Install
INSTALL(TARGETS ${MXADATAMODEL_LIB_NAME} 
    LIBRARY DESTINATION lib 
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib
    )
    
SET (INSTALL_INCLUDES 
  ${MXA_SOURCE_DIR}/src/Base
  ${MXA_SOURCE_DIR}/src/BMPIO
  ${MXA_SOURCE_DIR}/src/Common
  ${MXA_SOURCE_DIR}/src/Core
  ${MXA_SOURCE_DIR}/src/DataImport
  ${MXA_SOURCE_DIR}/src/HDF5
  ${MXA_SOURCE_DIR}/src/Utilities
  ${MXA_SOURCE_DIR}/src/XML
  )
  
INSTALL (FILES ${PROJECT_BINARY_DIR}/MXAConfiguration.h DESTINATION include/MXADataModel )

SET (MXA_INSTALLED_RESOURCES
        ${PROJECT_BINARY_DIR}/UseMXADataModel.cmake
        ${MXA_RESOURCES_DIR}/ConfigureChecks.cmake
        ${MXA_RESOURCES_DIR}/CreateOSXBundle.sh.in
        ${MXA_RESOURCES_DIR}/IncludeMXADataModelSource.cmake
        ${MXA_RESOURCES_DIR}/MXAAdjustLibVars.cmake
        ${MXA_RESOURCES_DIR}/MXAFindBoost.cmake
        ${MXA_RESOURCES_DIR}/MXAFindBoost_Generator.sh
        ${MXA_RESOURCES_DIR}/MXAFindExpat.cmake
        ${MXA_RESOURCES_DIR}/MXAFindHDF5.cmake
        ${MXA_RESOURCES_DIR}/MXAFindSZip.cmake
        ${MXA_RESOURCES_DIR}/MXAFindTiff.cmake
        ${MXA_RESOURCES_DIR}/PackageMXALibsForOSXAppBundle.sh.in
)

INSTALL (FILES ${MXA_INSTALLED_RESOURCES} DESTINATION share/MXADataModel/Resources/ )

INSTALL(DIRECTORY ${INSTALL_INCLUDES}  DESTINATION include/MXADataModel
  PATTERN "CVS" EXCLUDE
  PATTERN "*.c*" EXCLUDE
  PATTERN "*.in" EXCLUDE
)

#-- Include the MXADataModel Testing Sources
IF ( BUILD_TESTING )
  ADD_SUBDIRECTORY(${MXA_SOURCE_DIR}/src/Testing ${PROJECT_BINARY_DIR}/Testing)
ENDIF ( BUILD_TESTING )

#-- Include the Examples Directory
OPTION(BUILD_EXAMPLES "Build Programs that demonstrate the use of the API" OFF)
IF(BUILD_EXAMPLES)
    ADD_SUBDIRECTORY(${MXA_SOURCE_DIR}/src/Examples ${PROJECT_BINARY_DIR}/Examples)
ENDIF(BUILD_EXAMPLES)

# ------------------------------------------------------------------------------
#  Build simple test programs. This should always be commented out
# ------------------------------------------------------------------------------
#ADD_EXECUTABLE(OverLoadTest ${MXA_SOURCE_DIR}/src/Misc/OverLoadTest.cpp)
#TARGET_LINK_LIBRARIES(OverLoadTest ${MXADATAMODEL_LIB_NAME} ${DEP_LIBS})


