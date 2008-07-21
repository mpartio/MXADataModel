PROJECT (MXATesting)

SET (TEST_BOOST_LIBS ${BOOST_TEST_EXEC_MONITOR_LIBRARY} )
        
OPTION(BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" OFF)

IF (BUILD_TESTING)

    IF (WIN32)
        SET (MXA_TEMP_DIR "C:\\\\WINDOWS\\\\Temp\\\\")
    ELSE (WIN32)
        SET (MXA_TEMP_DIR "/tmp/")
    ENDIF(WIN32)
    GET_FILENAME_COMPONENT(MXA_TEST_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
    SET (DATA_IMPORT_TEST_OUTPUT_FILE "${MXA_TEMP_DIR}XML_Based_Data_Import_Test.h5")
    FILE (TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/DataImportTest.xml" DATAIMPORT_TEST_IMPORT_XML_FILE)
    IF (WIN32)
     STRING(REPLACE "\\" "\\\\" DATAIMPORT_TEST_IMPORT_XML_FILE ${DATAIMPORT_TEST_IMPORT_XML_FILE} )
    ENDIF(WIN32)
    CONFIGURE_FILE(${MXA_TEST_SRC_DIR}/TestDataFileLocations.h.in
               ${PROJECT_BINARY_DIR}/TestDataFileLocations.h @ONLY IMMEDIATE)
    
    # -------- Dataset Test
    SET (MXADataset_TEST_SRCS
        DatasetTest.cpp

        ${MXA_SOURCE_DIR}/src/DataWrappers/MXAArrayTemplate.hpp
        ${MXA_SOURCE_DIR}/src/HDF5/H5Dataset.h
        ${MXA_SOURCE_DIR}/src/Base/IFileReader.h
        ${MXA_SOURCE_DIR}/src/Base/IFileWriter.h
        ${MXA_SOURCE_DIR}/src/Base/IMXAArray.h
        ${MXA_SOURCE_DIR}/src/Base/IDataFileIO.h
    )
    ADD_EXECUTABLE(DatasetTest ${MXADataset_TEST_SRCS})
   # MESSAGE(STATUS "MXADATAMODEL_LIB_NAME: ${MXADATAMODEL_LIB_NAME}")
    TARGET_LINK_LIBRARIES( DatasetTest ${MXADATAMODEL_LIB_NAME} ${TEST_BOOST_LIBS} ${DEP_LIBS})
    ADD_TEST(DatasetTest ${EXECUTABLE_OUTPUT_PATH}/DatasetTest)
    
    # -------- Data Model Test
    SET (MXADATAMODEL_TEST_SRCS
      DataModelTest.cpp
    )
    ADD_EXECUTABLE ( DataModelTest ${MXADATAMODEL_TEST_SRCS} )
    TARGET_LINK_LIBRARIES ( DataModelTest  ${MXADATAMODEL_LIB_NAME} ${TEST_BOOST_LIBS} ${DEP_LIBS} )
    ADD_TEST(DataModelTest ${EXECUTABLE_OUTPUT_PATH}/DataModelTest)
    
    # ------ DataImport Test
    IF (MXA_USE_XML)
        SET (MXADATAIMPORT_TEST_SRCS
          DataImportTest.cpp
        )
        ADD_EXECUTABLE ( DataImportTest ${MXADATAIMPORT_TEST_SRCS} )
        TARGET_LINK_LIBRARIES ( DataImportTest ${MXADATAMODEL_LIB_NAME} ${TEST_BOOST_LIBS} ${DEP_LIBS} )
        ADD_TEST(DataImportTest ${EXECUTABLE_OUTPUT_PATH}/DataImportTest)
        #-- Configure the XML file to correctly find the XML Model file
        SET (DATA_MODEL_XML_FILE ${MXA_SOURCE_DIR}/Resources/ModelTemplate.xml)
        CONFIGURE_FILE(${MXA_SOURCE_DIR}/Resources/DataImportTest.xml.in
                        ${CMAKE_BINARY_DIR}/DataImportTest.xml @ONLY IMMEDIATE)
    ENDIF(MXA_USE_XML)
    
    # ------- H5Lite Test
    SET (H5LITE_TEST_SRCS
      H5LiteTest.cpp
      ${PROJECT_BINARY_DIR}/TestDataFileLocations.h
    )
    ADD_EXECUTABLE ( H5LiteTest ${H5LITE_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (H5LiteTest ${MXADATAMODEL_LIB_NAME} ${TEST_BOOST_LIBS} ${DEP_LIBS})
    ADD_TEST(H5LiteTest ${EXECUTABLE_OUTPUT_PATH}/H5LiteTest)

    # ------- H5Lite Test
    SET (H5UTIL_TEST_SRCS
      H5UtilitiesTest.cpp
    )
    ADD_EXECUTABLE ( H5UtilitiesTest ${H5UTIL_TEST_SRCS} )
    TARGET_LINK_LIBRARIES (H5UtilitiesTest ${MXADATAMODEL_LIB_NAME} ${TEST_BOOST_LIBS} ${DEP_LIBS})
    ADD_TEST(H5UtilitiesTest ${EXECUTABLE_OUTPUT_PATH}/H5UtilitiesTest)
        
    # --------- BMP IO Test
    SET (BMP_IO_TEST_SRCS
        BmpIOTest.cpp
    )
    ADD_EXECUTABLE(BmpIOTest ${BMP_IO_TEST_SRCS})
    TARGET_LINK_LIBRARIES (BmpIOTest  ${MXADATAMODEL_LIB_NAME} ${TEST_BOOST_LIBS} ${DEP_LIBS} )
    ADD_TEST(BmpIOTest ${EXECUTABLE_OUTPUT_PATH}/BmpIOTest)
   
   # ------- H5ArrayTemplate Test
     SET (H5ARRAYTEMPLATE_TEST_SRCS
        H5ArrayTemplateTest.cpp
    )
    #ADD_EXECUTABLE(H5ArrayTemplateTest ${H5ARRAYTEMPLATE_TEST_SRCS})
    #TARGET_LINK_LIBRARIES (H5ArrayTemplateTest  ${MXADATAMODEL_LIB_NAME} ${TEST_BOOST_LIBS} ${DEP_LIBS} )
    #ADD_TEST(H5ArrayTemplateTest ${EXECUTABLE_OUTPUT_PATH}/H5ArrayTemplateTest)  
   
   # --------- XML Tests
    IF (MXA_USE_XML)
        SET (XML_TEST_SRCS
            XMLTest.cpp
            )
        ADD_EXECUTABLE( XMLTest ${XML_TEST_SRCS} )
        TARGET_LINK_LIBRARIES (XMLTest  ${MXADATAMODEL_LIB_NAME} ${TEST_BOOST_LIBS} ${DEP_LIBS})
        ADD_TEST(XMLTest ${EXECUTABLE_OUTPUT_PATH}/XMLTest)      
    ENDIF(MXA_USE_XML)
    
    
    
#    ADD_EXECUTABLE( UtilitiesTest UtilitiesTest.cpp )
#    TARGET_LINK_LIBRARIES(UtilitiesTest MXADataModel ${HDF5_LIBRARIES} ${BOOST_MXA_LIBS})
#    ADD_TEST(UtilitiesTest ${EXECUTABLE_OUTPUT_PATH}/UtilitiesTest)

    # ------- Vtk to HDF5 Bridge test
    IF(BUILD_VTK_HDF5)
        SET (VTK_HDF5_SRCS
          VtkHDF5Test.cpp
          DataFileGenerator.cpp
        )
        ADD_EXECUTABLE( vtkHDF5Test ${VTK_HDF5_SRCS} )
        TARGET_LINK_LIBRARIES (vtkHDF5Test MXADataModel  
                ${MXADATAMODEL_LIB_NAME} 
                ${TEST_BOOST_LIBS} 
                ${DEP_LIBS}   
                ${vtkLocal_LIBS}
        )
        ADD_TEST(vtkHDF5Test ${EXECUTABLE_OUTPUT_PATH}/vtkHDF5Test)
    ENDIF(BUILD_VTK_HDF5)
                   
ENDIF (BUILD_TESTING)

