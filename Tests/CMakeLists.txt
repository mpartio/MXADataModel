PROJECT (MXATests)


if ( NOT MXA_OUTPUT_DIRS_DEFINED )
# ---------- Setup output Directories -------------------------
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Libraries"
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Executables."
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all static libraries."
  )
endif()

# --------------------------------------------------------------------------
macro (DataImportTestSetup name alt_name start count end  increment index)
    SET (${name}_name ${name} )
    SET (${name}_alt_name ${alt_name} )
    SET (${name}_start ${start} )
    SET (${name}_count ${count} )
    SET (${name}_end ${end} )
    SET (${name}_increment ${increment} )
    SET (${name}_index ${index})
endmacro (DataImportTestSetup name alt_name start count end increment index)

# --------------------------------------------------------------------------
macro (ConvertPathToXMLCompatible INPUT)
    IF (WIN32)
      STRING(REPLACE "\\\\" "\\" ${INPUT} ${${INPUT}} )
      STRING(REPLACE "/" "\\" ${INPUT} ${${INPUT}}  )
    endif()
endmacro()

# --------------------------------------------------------------------------
macro (ConvertPathToHeaderCompatible INPUT)
    IF (WIN32)
      STRING(REPLACE "\\" "\\\\" ${INPUT} ${${INPUT}} )
      STRING(REPLACE "/" "\\\\" ${INPUT} ${${INPUT}}  )
    endif()
endmacro()

# --------------------------------------------------------------------------
macro(MXA_ADD_TEST testname Exename)
    if (NOT APPLE)
        ADD_TEST(${testname} ${Exename})
    else()
        foreach(arch ${CMAKE_OSX_ARCHITECTURES})
            ADD_TEST( ${arch}_${testname} ${OSX_TOOLS_DIR}/UBTest.sh ${arch} ${Exename} )
        endforeach()
    endif()
endmacro()

# --------------------------------------------------------------------
SET (TEST_Boost_LIBS ${Boost_TEST_EXEC_MONITOR_LIBRARY} )
        
OPTION(MXA_BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)

IF (MXA_BUILD_TESTING)
    include_directories(${MXATests_BINARY_DIR})
    set(MXA_TEMP_DIR ${MXADataModel_BINARY_DIR}/Testing/)

    GET_FILENAME_COMPONENT(MXA_TEST_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
    FILE(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/DataImportTest.xml" DATAIMPORT_TEST_IMPORT_XML_FILE)
    file(TO_NATIVE_PATH "${MXATests_BINARY_DIR}" MXATests_BINARY_DIR_NATIVE)
    file(TO_NATIVE_PATH "${MXADataModel_BINARY_DIR}" MXADataModel_BINARY_DIR_NATIVE)
    #-- Convert all '\' to '\\' so that they are properly escaped in the header file
    SET (DATAIMPORT_TEST_XMLH5TIFF_OUTPUT_FILE "${MXA_TEMP_DIR}DataImportTest_XMLH5Tiff.h5")
    ConvertPathToHeaderCompatible(MXA_TEMP_DIR)
    ConvertPathToHeaderCompatible(DATAIMPORT_TEST_XMLH5TIFF_OUTPUT_FILE)
    ConvertPathToHeaderCompatible(DATAIMPORT_TEST_IMPORT_XML_FILE)
    ConvertPathToHeaderCompatible(MXATests_BINARY_DIR_NATIVE)
    ConvertPathToHeaderCompatible(MXADataModel_BINARY_DIR_NATIVE)

    CONFIGURE_FILE(${MXA_TEST_SRC_DIR}/TestDataFileLocations.h.in
               ${PROJECT_BINARY_DIR}/MXAUnitTestDataFileLocations.h @ONLY IMMEDIATE)

# --------------------------------------------------------------------
# --------- FileReader/Writer Test
    SET (FILEREADERWRITER_TEST_SRCS
        ${MXATests_SOURCE_DIR}/MXAFileReaderWriterTest.cpp
    )
    ADD_EXECUTABLE(MXA_FileReaderWriterTest ${FILEREADERWRITER_TEST_SRCS})
    TARGET_LINK_LIBRARIES (MXA_FileReaderWriterTest ${MXADATAMODEL_LIB_NAME}  ${DEP_LIBS} )
    MXA_ADD_TEST(MXA_FileReaderWriterTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_FileReaderWriterTest) 

# --------------------------------------------------------------------
# -------- Dataset Test
    SET (MXADataset_TEST_SRCS
        ${MXATests_SOURCE_DIR}/DatasetTest.cpp
    )
    SET (MXADataset_TEST_HEADERS 
      ${MXA_SOURCE_DIR}/MXA/DataWrappers/MXAArrayTemplate.hpp
      ${MXA_SOURCE_DIR}/MXA/HDF5/H5Dataset.h
      ${MXA_SOURCE_DIR}/MXA/Base/IFileReader.h
      ${MXA_SOURCE_DIR}/MXA/Base/IFileWriter.h
      ${MXA_SOURCE_DIR}/MXA/Base/IMXAArray.h
      ${MXA_SOURCE_DIR}/MXA/Base/IDataFileIO.h
      )
   # source_group(src\\Testing FILES ${MXADataset_TEST_SRCS} ${MXADataset_TEST_HEADERS})
   IF ( MXA_USE_HDF5 AND MXA_USE_TIFF )
    ADD_EXECUTABLE(MXA_DatasetTest ${MXADataset_TEST_SRCS} ${MXADataset_TEST_HEADERS})
    TARGET_LINK_LIBRARIES( MXA_DatasetTest ${MXADATAMODEL_LIB_NAME}  ${DEP_LIBS})
    MXA_ADD_TEST(MXA_DatasetTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_DatasetTest)
   ENDIF ( MXA_USE_HDF5 AND MXA_USE_TIFF)
    

# --------------------------------------------------------------------
# -------- Data Model Test
    SET (MXADATAMODEL_TEST_SRCS
      ${MXATests_SOURCE_DIR}/DataModelTest.cpp
    )
  #  source_group(src\\Testing FILES ${MXADATAMODEL_TEST_SRCS})
  IF ( MXA_USE_HDF5 )
    ADD_EXECUTABLE ( MXA_DataModelTest ${MXADATAMODEL_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( MXA_DataModelTest  ${MXADATAMODEL_LIB_NAME}   )
    MXA_ADD_TEST(MXA_DataModelTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_DataModelTest)
  ENDIF ( MXA_USE_HDF5 )

# -------------------------------------------------------------------- 
# ------ DataImport Test
IF (MXA_USE_XML AND MXA_USE_HDF5 AND MXA_USE_TIFF)
    SET (MXADATAIMPORT_TEST_SRCS
      ${MXATests_SOURCE_DIR}/DataImportTest.cpp
      ${MXATests_SOURCE_DIR}/TiffMaker.cpp
    )
    SET (MXADATAIMPORT_TEST_HEADERS
      ${MXATests_SOURCE_DIR}/H5ImportTest.h
      ${MXATests_SOURCE_DIR}/TiffMaker.h
    )

    DataImportTestSetup("Dim_0" "Vol Frac" 0 4 75 25 0)
    DataImportTestSetup("Dim_1" "Rnd Seed" 0 3 50 25 1)
    DataImportTestSetup("Dim_2" "TS" 0 4 75 25 2)
    DataImportTestSetup("Dim_3" "Slice" 0 3 2 1 3)
    FILE(MAKE_DIRECTORY "@MXA_TEMP_DIR@DataImportTest/test_data/" )
    ADD_EXECUTABLE ( MXA_DataImportTest ${MXADATAIMPORT_TEST_SRCS} ${MXADATAIMPORT_TEST_HEADERS})
    TARGET_LINK_LIBRARIES ( MXA_DataImportTest  ${MXADATAMODEL_LIB_NAME}  ${DEP_LIBS})
    MXA_ADD_TEST(MXA_DataImportTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_DataImportTest)

    #-- Configure the XML file to correctly find the XML Model file
    SET(DATA_MODEL_XML_FILE ${MXADataModel_SOURCE_DIR}/Resources/XML/ModelTemplate.xml)
    SET(DATAIMPORT_TEST_XMLH5TIFF_OUTPUT_FILE "${MXA_TEMP_DIR}DataImportTest_XMLH5Tiff.h5")
    set(DATAIMPORT_TEST_IMPORT_DIR "${MXA_TEMP_DIR}DataImportTest/test_data/")
    set(DATAIMPORT_TEST_IMPORT_XML_FILE "${CMAKE_BINARY_DIR}/DataImportTest.xml")
    ConvertPathToXMLCompatible(MXA_TEMP_DIR)
    ConvertPathToXMLCompatible(DATA_MODEL_XML_FILE)
    ConvertPathToXMLCompatible(DATAIMPORT_TEST_IMPORT_XML_FILE)
    ConvertPathToXMLCompatible(DATAIMPORT_TEST_XMLH5TIFF_OUTPUT_FILE)
    ConvertPathToXMLCompatible(DATAIMPORT_TEST_IMPORT_DIR)
    CONFIGURE_FILE(${MXADataModel_SOURCE_DIR}/Resources/XML/DataImportTest.xml.in
                    ${CMAKE_BINARY_DIR}/DataImportTest.xml @ONLY IMMEDIATE)
ENDIF(MXA_USE_XML AND MXA_USE_HDF5 AND MXA_USE_TIFF)

# --------------------------------------------------------------------
# ------- H5Lite Test
IF ( MXA_USE_HDF5 )
    SET (H5LITE_TEST_SRCS
      ${MXATests_SOURCE_DIR}/H5LiteTest.cpp
      ${PROJECT_BINARY_DIR}/MXAUnitTestDataFileLocations.h
    )
    ADD_EXECUTABLE ( MXA_H5LiteTest ${H5LITE_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (MXA_H5LiteTest ${MXADATAMODEL_LIB_NAME}  )
    MXA_ADD_TEST(MXA_H5LiteTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_H5LiteTest)


# --------------------------------------------------------------------
# ------- H5Utilities Test
    SET (H5UTIL_TEST_SRCS
      ${MXATests_SOURCE_DIR}/H5UtilitiesTest.cpp
    )
    ADD_EXECUTABLE ( MXA_H5UtilitiesTest ${H5UTIL_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (MXA_H5UtilitiesTest ${MXADATAMODEL_LIB_NAME}  )
    MXA_ADD_TEST(MXA_H5UtilitiesTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_H5UtilitiesTest)
ENDIF ( MXA_USE_HDF5 )

# --------------------------------------------------------------------
# ------- DataSourcePathIndexSection  Test
SET (DATASOURCEPATHINDEXSECTION_TEST_SRCS
  ${MXATests_SOURCE_DIR}/DataSourcePathIndexSectionTest.cpp
)
ADD_EXECUTABLE ( MXA_DataSourcePathIndexSectionTest ${DATASOURCEPATHINDEXSECTION_TEST_SRCS} )
TARGET_LINK_LIBRARIES (MXA_DataSourcePathIndexSectionTest ${MXADATAMODEL_LIB_NAME} )
MXA_ADD_TEST(MXA_DataSourcePathIndexSectionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_DataSourcePathIndexSectionTest)

# --------------------------------------------------------------------    
# --------- BMP IO Test
SET (BMP_IO_TEST_SRCS
    ${MXATests_SOURCE_DIR}/BmpIOTest.cpp
)
IF ( MXA_USE_HDF5 )
    ADD_EXECUTABLE(MXA_BmpIOTest ${BMP_IO_TEST_SRCS})
    TARGET_LINK_LIBRARIES (MXA_BmpIOTest ${MXADATAMODEL_LIB_NAME} )
    MXA_ADD_TEST(MXA_BmpIOTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_BmpIOTest)
ENDIF ( MXA_USE_HDF5 )

# --------------------------------------------------------------------
# ------- H5ArrayTemplate Test
SET (H5ARRAYTEMPLATE_TEST_SRCS
    ${MXATests_SOURCE_DIR}/H5ArrayTemplateTest.cpp
)
#ADD_EXECUTABLE(MXA_H5ArrayTemplateTest ${H5ARRAYTEMPLATE_TEST_SRCS})
#TARGET_LINK_LIBRARIES (MXA_H5ArrayTemplateTest  ${MXADATAMODEL_LIB_NAME}  ${DEP_LIBS} )
#MXA_ADD_TEST(MXA_H5ArrayTemplateTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_H5ArrayTemplateTest)  


# -------------------------------------------------------------------- 
# --------- XML Tests
IF (MXA_USE_XML)
    SET (XML_TEST_SRCS  ${MXATests_SOURCE_DIR}/XMLTest.cpp )
    ADD_EXECUTABLE( MXA_XMLTest ${XML_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (MXA_XMLTest ${MXADATAMODEL_LIB_NAME} )
    MXA_ADD_TEST(MXA_XMLTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_XMLTest)      
ENDIF(MXA_USE_XML)

# --------------------------------------------------------------------
# --------- IImportProperty Test
SET (IIMPORT_PROPERTY_TEST_SRCS
    ${MXATests_SOURCE_DIR}/IImportPropertyTest.cpp
)
ADD_EXECUTABLE(MXA_IImportPropertyTest ${IIMPORT_PROPERTY_TEST_SRCS})
TARGET_LINK_LIBRARIES (MXA_IImportPropertyTest  ${MXADATAMODEL_LIB_NAME} )
MXA_ADD_TEST(MXA_IImportPropertyTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_IImportPropertyTest) 

# --------------------------------------------------------------------
# --------- SupportFile Test
   IF ( MXA_USE_HDF5 AND MXA_USE_TIFF)
    SET (SUPPORT_FILE_TEST_SRCS
        ${MXATests_SOURCE_DIR}/SupportFileTest.cpp
        ${MXATests_SOURCE_DIR}/TiffMaker.cpp
    )
    ADD_EXECUTABLE(MXA_SupportFileTest ${SUPPORT_FILE_TEST_SRCS})
    TARGET_LINK_LIBRARIES (MXA_SupportFileTest  
                            ${MXADATAMODEL_LIB_NAME}
                            ${DEP_LIBS} )
    MXA_ADD_TEST(MXA_SupportFileTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_SupportFileTest) 
ENDIF ( MXA_USE_HDF5 AND MXA_USE_TIFF ) 

# --------------------------------------------------------------------
# ----- MXAFileSystemPath Test
set(MXAFileSystemPathTest_SRCS 
    ${MXATests_SOURCE_DIR}/MXAFileSystemPathTest.cpp
)
add_executable(MXA_FileSystemPathTest ${MXAFileSystemPathTest_SRCS})
target_link_libraries(MXA_FileSystemPathTest 
                      ${MXADATAMODEL_LIB_NAME} )
MXA_ADD_TEST(MXA_FileSystemPathTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_FileSystemPathTest)

# --------------------------------------------------------------------
# ----- MXALogger Test
set(MXALoggerTest_SRCS 
    ${MXATests_SOURCE_DIR}/MXALoggerTest.cpp
)
add_executable(MXA_LoggerTest ${MXALoggerTest_SRCS})
target_link_libraries(MXA_LoggerTest 
                      ${MXADATAMODEL_LIB_NAME} )
MXA_ADD_TEST(MXA_LoggerTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_LoggerTest)

# --------------------------------------------------------------------
#---- Writer Delegate Test
set (WriterDelegateTest_SRCS
    ${MXATests_SOURCE_DIR}/WriterDelegateTest.cpp
)
add_executable(MXA_WriterDelegateTest ${WriterDelegateTest_SRCS} )
target_link_libraries(MXA_WriterDelegateTest 
                      ${MXADATAMODEL_LIB_NAME} )
MXA_ADD_TEST(MXA_WriterDelegateTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_WriterDelegateTest)

# --------------------------------------------------------------------
# ------- Vtk to HDF5 Bridge test
IF(MXA_BUILD_VTK_HDF5)
    SET (VTK_HDF5_SRCS
      ${MXATests_SOURCE_DIR}/VtkHDF5Test.cpp
      ${MXATests_SOURCE_DIR}/DataFileGenerator.cpp
    )
    ADD_EXECUTABLE( MXA_vtkHDF5Test ${VTK_HDF5_SRCS} )
    TARGET_LINK_LIBRARIES (MXA_vtkHDF5Test MXADataModel  
            ${MXADATAMODEL_LIB_NAME}
            ${DEP_LIBS}   
            ${vtkLocal_LIBS}
    )
    MXA_ADD_TEST(MXA_vtkHDF5Test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXA_vtkHDF5Test)
ENDIF(MXA_BUILD_VTK_HDF5)

# --------------------------------------------------------------------
#-- Copy all the dependent DLLs into the current build directory so that the test can run.
if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(TYPE "DEBUG")
    else()
        set(TYPE "RELEASE")
    endif()

    SET (mxa_lib_list tiff expat hdf5)
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MinSizeRel)
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo)
    FOREACH(lib ${mxa_lib_list})
      STRING(TOUPPER ${lib} upperlib)
      if (HAVE_${upperlib}_DLL)
        GET_FILENAME_COMPONENT(lib_path ${${upperlib}_LIBRARY_${TYPE}} PATH)
        get_filename_component(lib_name ${${upperlib}_LIBRARY_${TYPE}} NAME_WE)
        ADD_CUSTOM_COMMAND(TARGET DataModelTest POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${${upperlib}_BIN_DIR}/${lib_name}.dll
                      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/ 
                      COMMENT "Copying ${lib_name}.dll to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/")
      endif()
    ENDFOREACH()
endif()


ENDIF (MXA_BUILD_TESTING)
