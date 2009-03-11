PROJECT (MXATests)

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
    
SET (TEST_Boost_LIBS ${Boost_TEST_EXEC_MONITOR_LIBRARY} )
        
OPTION(MXA_BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)

IF (BUILD_TESTING)

    IF (WIN32)
        SET (MXA_TEMP_DIR "C:\\\\WINDOWS\\\\Temp\\\\")
    ELSE (WIN32)
        SET (MXA_TEMP_DIR "/tmp/")
    ENDIF(WIN32)
    GET_FILENAME_COMPONENT(MXA_TEST_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
    SET (DATAIMPORT_TEST_XMLH5TIFF_OUTPUT_FILE "${MXA_TEMP_DIR}DataImportTest_XMLH5Tiff.h5")
    FILE (TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/DataImportTest.xml" DATAIMPORT_TEST_IMPORT_XML_FILE)
    IF (WIN32)
     STRING(REPLACE "\\" "\\\\" DATAIMPORT_TEST_IMPORT_XML_FILE ${DATAIMPORT_TEST_IMPORT_XML_FILE} )
    ENDIF(WIN32)
    
    CONFIGURE_FILE(${MXA_TEST_SRC_DIR}/TestDataFileLocations.h.in
               ${PROJECT_BINARY_DIR}/MXAUnitTestDataFileLocations.h @ONLY IMMEDIATE)
  
# --------- FileReader/Writer Test
    SET (FILEREADERWRITER_TEST_SRCS
        ${MXATests_SOURCE_DIR}/MXAFileReaderWriterTest.cpp
    )
    ADD_EXECUTABLE(MXAFileReaderWriterTest ${FILEREADERWRITER_TEST_SRCS})
    TARGET_LINK_LIBRARIES (MXAFileReaderWriterTest  ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS} )
    ADD_TEST(MXAFileReaderWriterTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXAFileReaderWriterTest) 

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
    ADD_EXECUTABLE(DatasetTest ${MXADataset_TEST_SRCS} ${MXADataset_TEST_HEADERS})
    TARGET_LINK_LIBRARIES( DatasetTest ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} )
    ADD_TEST(DatasetTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DatasetTest)
   ENDIF ( MXA_USE_HDF5 AND MXA_USE_TIFF)
    
    # -------- Data Model Test
    SET (MXADATAMODEL_TEST_SRCS
      ${MXATests_SOURCE_DIR}/DataModelTest.cpp
    )
  #  source_group(src\\Testing FILES ${MXADATAMODEL_TEST_SRCS})
  IF ( MXA_USE_HDF5 )
    ADD_EXECUTABLE ( DataModelTest ${MXADATAMODEL_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataModelTest  ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS}  )
    ADD_TEST(DataModelTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DataModelTest)
  ENDIF ( MXA_USE_HDF5 )
  
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


    include_directories(${MXATests_BINARY_DIR})
    ADD_EXECUTABLE ( DataImportTest ${MXADATAIMPORT_TEST_SRCS} ${MXADATAIMPORT_TEST_HEADERS})
    TARGET_LINK_LIBRARIES ( DataImportTest ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS} )
    ADD_TEST(DataImportTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DataImportTest)

    #-- Configure the XML file to correctly find the XML Model file
    SET (DATA_MODEL_XML_FILE ${MXADataModel_SOURCE_DIR}/Resources/XML/ModelTemplate.xml)
    CONFIGURE_FILE(${MXADataModel_SOURCE_DIR}/Resources/XML/DataImportTest.xml.in
                    ${CMAKE_BINARY_DIR}/DataImportTest.xml @ONLY IMMEDIATE)
ENDIF(MXA_USE_XML AND MXA_USE_HDF5 AND MXA_USE_TIFF)
    
# ------- H5Lite Test
    SET (H5LITE_TEST_SRCS
      ${MXATests_SOURCE_DIR}/H5LiteTest.cpp
      ${PROJECT_BINARY_DIR}/MXAUnitTestDataFileLocations.h
    )
    IF ( MXA_USE_HDF5 AND MXA_USE_TIFF)
        ADD_EXECUTABLE ( H5LiteTest ${H5LITE_TEST_SRCS} )
        TARGET_LINK_LIBRARIES (H5LiteTest ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS})
        ADD_TEST(H5LiteTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/H5LiteTest)
    ENDIF ( MXA_USE_HDF5 AND MXA_USE_TIFF)
    
# ------- H5Utilities Test
    SET (H5UTIL_TEST_SRCS
      ${MXATests_SOURCE_DIR}/H5UtilitiesTest.cpp
    )
    IF ( MXA_USE_HDF5 )
        ADD_EXECUTABLE ( H5UtilitiesTest ${H5UTIL_TEST_SRCS} )
        TARGET_LINK_LIBRARIES (H5UtilitiesTest ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS})
        ADD_TEST(H5UtilitiesTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/H5UtilitiesTest)
    ENDIF ( MXA_USE_HDF5 )
    
# ------- DataSourcePathIndexSection  Test
    SET (DATASOURCEPATHINDEXSECTION_TEST_SRCS
      ${MXATests_SOURCE_DIR}/DataSourcePathIndexSectionTest.cpp
    )
    ADD_EXECUTABLE ( DataSourcePathIndexSectionTest ${DATASOURCEPATHINDEXSECTION_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (DataSourcePathIndexSectionTest ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS})
    ADD_TEST(DataSourcePathIndexSectionTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DataSourcePathIndexSectionTest)
           
# --------- BMP IO Test
    SET (BMP_IO_TEST_SRCS
        ${MXATests_SOURCE_DIR}/BmpIOTest.cpp
    )
    IF ( MXA_USE_HDF5 )
        ADD_EXECUTABLE(BmpIOTest ${BMP_IO_TEST_SRCS})
        TARGET_LINK_LIBRARIES (BmpIOTest  ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS} )
        ADD_TEST(BmpIOTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/BmpIOTest)
    ENDIF ( MXA_USE_HDF5 )
       
# ------- H5ArrayTemplate Test
     SET (H5ARRAYTEMPLATE_TEST_SRCS
        ${MXATests_SOURCE_DIR}/H5ArrayTemplateTest.cpp
    )
    #ADD_EXECUTABLE(H5ArrayTemplateTest ${H5ARRAYTEMPLATE_TEST_SRCS})
    #TARGET_LINK_LIBRARIES (H5ArrayTemplateTest  ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS} )
    #ADD_TEST(H5ArrayTemplateTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/H5ArrayTemplateTest)  
   
# --------- XML Tests
    IF (MXA_USE_XML)
        SET (XML_TEST_SRCS
            ${MXATests_SOURCE_DIR}/XMLTest.cpp
            )
        ADD_EXECUTABLE( XMLTest ${XML_TEST_SRCS} )
        TARGET_LINK_LIBRARIES (XMLTest  ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS})
        ADD_TEST(XMLTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/XMLTest)      
    ENDIF(MXA_USE_XML)
    
# --------- IImportProperty Test
    SET (IIMPORT_PROPERTY_TEST_SRCS
        ${MXATests_SOURCE_DIR}/IImportPropertyTest.cpp
    )
    ADD_EXECUTABLE(IImportPropertyTest ${IIMPORT_PROPERTY_TEST_SRCS})
    TARGET_LINK_LIBRARIES (IImportPropertyTest  ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS} )
    ADD_TEST(IImportPropertyTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/IImportPropertyTest) 
  
# --------- SupportFile Test
   IF ( MXA_USE_HDF5 AND MXA_USE_TIFF)
        SET (SUPPORT_FILE_TEST_SRCS
            ${MXATests_SOURCE_DIR}/SupportFileTest.cpp
            ${MXATests_SOURCE_DIR}/TiffMaker.cpp
        )
        ADD_EXECUTABLE(SupportFileTest ${SUPPORT_FILE_TEST_SRCS})
        TARGET_LINK_LIBRARIES (SupportFileTest  ${MXADATAMODEL_LIB_NAME} ${TEST_Boost_LIBS} ${DEP_LIBS} )
        ADD_TEST(SupportFileTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SupportFileTest) 
    ENDIF ( MXA_USE_HDF5 AND MXA_USE_TIFF ) 
    
# ----- MXAFileSystemPath Test
    set(MXAFileSystemPathTest_SRCS 
        ${MXATests_SOURCE_DIR}/MXAFileSystemPathTest.cpp
        ${MXA_SOURCE_DIR}/MXA/Utilities/MXAFileSystemPath.cpp
    )
    add_executable(MXAFileSystemPathTest ${MXAFileSystemPathTest_SRCS})
    target_link_libraries(MXAFileSystemPathTest ${Boost_TEST_EXEC_MONITOR_LIBRARY} ${Boost_SYSTEM_LIBRARY})
    add_test(MXAFileSystemPathTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MXAFileSystemPathTest)
    
# ------- Vtk to HDF5 Bridge test
    IF(MXA_BUILD_VTK_HDF5)
        SET (VTK_HDF5_SRCS
          ${MXATests_SOURCE_DIR}/VtkHDF5Test.cpp
          ${MXATests_SOURCE_DIR}/DataFileGenerator.cpp
        )
        ADD_EXECUTABLE( vtkHDF5Test ${VTK_HDF5_SRCS} )
        TARGET_LINK_LIBRARIES (vtkHDF5Test MXADataModel  
                ${MXADATAMODEL_LIB_NAME} 
                ${TEST_Boost_LIBS} 
                ${DEP_LIBS}   
                ${vtkLocal_LIBS}
        )
        ADD_TEST(vtkHDF5Test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vtkHDF5Test)
    ENDIF(MXA_BUILD_VTK_HDF5)
                   
ENDIF (BUILD_TESTING)

