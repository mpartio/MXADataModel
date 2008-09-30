PROJECT (MXAExamples)

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

INCLUDE_DIRECTORIES(${MXA_SOURCE_DIR}/src)

INCLUDE( ${PROJECT_SOURCE_DIR}/../../Resources/MXAFindHDF5.cmake )
SET (MXA_EXAMPLES_SOURCE_DIR ${MXA_SOURCE_DIR}/src/Examples)

IF(NOT HDF5_FOUND)
  MESSAGE(FATAL_ERROR "HDF5 was not found. Set the include/lib manually or set the HDF_INSTALL environment variable")
ENDIF(NOT HDF5_FOUND)

OPTION(BUILD_VTK_HDF5 "Include Code to bridge HDF5 to VTK Datasets" ON)
IF(BUILD_VTK_HDF5)
     #-- Find VTK packages -----------------------------------------
    FIND_PACKAGE(VTK REQUIRED)
    INCLUDE(${VTK_USE_FILE})
    
    # List the kits from VTK that are needed by this project.
    SET(vtkLocal_LIBS vtkCommon vtksys vtkFiltering vtkIO vtkGraphics )
    
    SET (DEP_LIBS ${DEP_LIBS} ${vtkLocal_LIBS})
    SET(HDF5_VTK_BRIDGE "1")
ENDIF(BUILD_VTK_HDF5) 


SET (GETDATA_SRCS
  GetDataExample.cpp
  ${MXA_SOURCE_DIR}/src/Testing/DataFileGenerator.cpp
)

# ------------------------------------------------------------------------------
#  Configure the header file
SET (MXA_TEMP_DIR "${MXA_BUILD_DIR}/Bin/")


CONFIGURE_FILE(${MXA_EXAMPLES_SOURCE_DIR}/ExampleFileLocations.h.in
               ${PROJECT_BINARY_DIR}/ExampleFileLocations.h @ONLY IMMEDIATE)


# ------------------------------------------------------------------------------
#  Add Executable for GetDataExample
# ------------------------------------------------------------------------------
ADD_EXECUTABLE ( GetDataExample ${GETDATA_SRCS} )
TARGET_LINK_LIBRARIES ( GetDataExample
  ${MXADATAMODEL_LIB_NAME}
  ${DEP_LIBS} 
)

# ------------------------------------------------------------------------------
#  Add Executable for Example 1
# ------------------------------------------------------------------------------
ADD_EXECUTABLE(Example1 Example1/Example1.cpp)
TARGET_LINK_LIBRARIES(Example1 ${MXADATAMODEL_LIB_NAME} )


# ------------------------------------------------------------------------------
#  Add Executable for Example 2
# ------------------------------------------------------------------------------
ADD_EXECUTABLE(Example2 Example2/Example2.cpp)
TARGET_LINK_LIBRARIES(Example2 ${MXADATAMODEL_LIB_NAME})


# ------------------------------------------------------------------------------
#  Add Executable for DataWrapperExample
# ------------------------------------------------------------------------------
ADD_EXECUTABLE(DataWrapperExample DataWrapperExample.cpp)
TARGET_LINK_LIBRARIES(DataWrapperExample ${MXADATAMODEL_LIB_NAME})

# ------------------------------------------------------------------------------
#  Add Executable for DataImport Example
# ------------------------------------------------------------------------------
SET (DATAIMPORT_EXAMPLE_SOURCES
    ${MXA_SOURCE_DIR}/src/Examples/DataImport/main.cpp
    ${MXA_SOURCE_DIR}/src/Examples/DataImport/SimpleImportExample.cpp
    ${MXA_SOURCE_DIR}/src/Examples/DataImport/ExampleImportDelegate.cpp
    ${MXA_SOURCE_DIR}/src/Examples/DataImport/ExampleImportDelegateFactory.cpp
)
SET (DATAIMPORT_EXAMPLE_HEADERS
  ${MXA_SOURCE_DIR}/src/Examples/DataImport/SimpleImportExample.h
  ${MXA_SOURCE_DIR}/src/Examples/DataImport/ExampleImportDelegate.h
  ${MXA_SOURCE_DIR}/src/Examples/DataImport/ExampleImportDelegateFactory.h
)
source_group(src\\Examples\\DataImport FILES ${DATAIMPORT_EXAMPLE_SOURCES} ${DATAIMPORT_EXAMPLE_HEADERS})
ADD_EXECUTABLE(DataImportExample ${DATAIMPORT_EXAMPLE_SOURCES})
TARGET_LINK_LIBRARIES(DataImportExample ${MXADATAMODEL_LIB_NAME} ) 



# ------------------------------------------------------------------------------
#  Add Executable for MXAProperty Example
# ------------------------------------------------------------------------------
SET (MXA_PROPERTY_EXAMPLE_SRCS
${MXA_SOURCE_DIR}/src/Examples/MXAPropertyExample/main.cpp
${MXA_SOURCE_DIR}/src/Examples/MXAPropertyExample/MyPropertyClass.cpp
)
SET (MXA_PROPERTY_EXAMPLE_HEADERS
${MXA_SOURCE_DIR}/src/Examples/MXAPropertyExample/MyPropertyClass.h
)
#source_group(src\\Examples\\MXAPropertyExample FILES ${MXA_PROPERTY_EXAMPLE_SRCS} ${MXA_PROPERTY_EXAMPLE_HEADERS})
ADD_EXECUTABLE(MXAPropertyExample ${MXA_PROPERTY_EXAMPLE_SRCS} ${MXA_PROPERTY_EXAMPLE_HEADERS})
TARGET_LINK_LIBRARIES(MXAPropertyExample ${MXADATAMODEL_LIB_NAME} ) 


# ------------------------------------------------------------------------------
#  Add Executable for Arbitrary File Example
# ------------------------------------------------------------------------------
SET (SUPPORT_FILE_EXAMPLE_SRCS
    ${MXA_SOURCE_DIR}/src/Examples/SupportFileExample.cpp
    ${MXA_SOURCE_DIR}/src/Testing/TiffMaker.cpp
)
SET (SUPPORT_FILE_EXAMPLE_HEADERS
    
)
#source_group(src\\Examples\\MXAPropertyExample FILES ${MXA_PROPERTY_EXAMPLE_SRCS} ${MXA_PROPERTY_EXAMPLE_HEADERS})
ADD_EXECUTABLE(SupportFileExample ${SUPPORT_FILE_EXAMPLE_SRCS} ${SUPPORT_FILE_EXAMPLE_HEADERS})
TARGET_LINK_LIBRARIES(SupportFileExample ${MXADATAMODEL_LIB_NAME} ) 


