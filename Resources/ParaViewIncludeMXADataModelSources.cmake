MESSAGE (STATUS "^^ParaViewIncludeMXADataModelSources.cmake")


IF (NOT MXA_RESOURCES_DIR)
  GET_FILENAME_COMPONENT(MXA_RESOURCES_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDIF (NOT MXA_RESOURCES_DIR)
#-- Include the Configure Checks
INCLUDE( "${MXA_RESOURCES_DIR}/ConfigureChecks.cmake")

# ---------- Find Boost Headers/Libraries -----------------------
SET (Boost_FIND_REQUIRED TRUE)
SET (Boost_FIND_QUIETLY TRUE)
SET (BOOST_USE_FILESYSTEM TRUE)
INCLUDE (${MXA_RESOURCES_DIR}/MXAFindBoost.cmake)
IF(NOT BOOST_FOUND)
  MESSAGE(FATAL_ERROR "MXAFindBoost: Boost was not found. ${Boost_DIR_MESSAGE}")
ENDIF(NOT BOOST_FOUND)

INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})  # Include the Boost Headers
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS}) 

#-- Add in the MXADataModel Sources
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
#-- Add the source files that depend on hdf5
  SET( MXADATAMODEL_SRCS ${MXADATAMODEL_SRCS}
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
  
  #-- Add the headers that depend on HDF5
  SET(MXADATAMODEL_HEADERS ${MXADATAMODEL_HEADERS}
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

INCLUDE_DIRECTORIES (${MXA_SOURCE_DIR}/src)

SET (HDF5_SUPPORT "1")
SET (HDF5_VTK_BRIDGE "0")
SET (TIFF_SUPPORT "0")
SET (XML_SUPPORT "0")
SET (BUILD_TESTING "0")
SET (BUILD_EXAMPLES "0")

#-- Configure the Header file and then add the Binary Directory to the Includes list
CONFIGURE_FILE(${MXA_SOURCE_DIR}/src/Common/MXAConfiguration.h.in
               ${PROJECT_BINARY_DIR}/MXAConfiguration.h @ONLY IMMEDIATE)
               
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")

SET (PARAVIEW_ADDITIONAL_LIBRARIES 
    ${PARAVIEW_ADDITIONAL_LIBRARIES} 
    ${BOOST_FILESYSTEM_LIBRARY}
    ${HDF_LIBRARY}
)

IF (PARAVIEW_VERSION_MAJOR EQUAL "2")
    MESSAGE (FATAL " ParaView 3.x only supported.")
  
ELSE (PARAVIEW_VERSION_MAJOR EQUAL "2")
    IF ( PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED)
        MESSAGE (STATUS "^^^^PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED was already configured")
    ELSE (PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED)
        MESSAGE (STATUS "^^^^^Configuring MXADataModel Sources")
        PARAVIEW_INCLUDE_SOURCES("${MXADATAMODEL_SRCS}")
        SET (PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED TRUE)
    ENDIF (PARAVIEW_MXADATAMODEL_SOURCES_INCLUDED)
ENDIF (PARAVIEW_VERSION_MAJOR EQUAL "2")




