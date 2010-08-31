#--////////////////////////////////////////////////////////////////////////////
#--  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--////////////////////////////////////////////////////////////////////////////
PROJECT (MXAExamples)

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
ENDIF ( NOT MXA_OUTPUT_DIRS_DEFINED )

INCLUDE_DIRECTORIES(${MXA_CODE_DIR}/MXA)
include_directories(${MXADataModel_SOURCE_DIR})

INCLUDE( ${CMP_MODULES_SOURCE_DIR}/FindHDF5.cmake )
SET (MXA_EXAMPLES_SOURCE_DIR ${MXA_CODE_DIR}/MXA/Examples)


SET (GETDATA_SRCS
  ${MXAExamples_SOURCE_DIR}/GetDataExample.cpp
  ${MXATests_SOURCE_DIR}/DataFileGenerator.cpp
)
set (GETDATA_HDRS  )

cmp_IDE_GENERATED_PROPERTIES("MXA/Examples" "${GETDATA_HDRS}" "${GETDATA_SRCS}" "0")

# ------------------------------------------------------------------------------
#  Configure the header file
if (NOT WIN32)
set (MXA_TEMP_DIR "/tmp/")
else()
SET (MXA_TEMP_DIR "C:\\\\Windows\\\\Temp\\\\")
endif()

CONFIGURE_FILE(${MXAExamples_SOURCE_DIR}/ExampleFileLocations.h.in
               ${PROJECT_BINARY_DIR}/ExampleFileLocations.h @ONLY IMMEDIATE)
cmp_IDE_GENERATED_PROPERTIES ("Generated" " ${PROJECT_BINARY_DIR}/ExampleFileLocations.h" "" "0")

# ------------------------------------------------------------------------------
#  Add Executable for GetDataExample
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 )
    ADD_EXECUTABLE ( GetDataExample ${GETDATA_SRCS} )
    TARGET_LINK_LIBRARIES ( GetDataExample  ${MXADATAMODEL_LIB_NAME} ${DEP_LIBS} )
    ToolInstallationSupport (GetDataExample "_debug" ${MXADataModel_BINARY_DIR} "Examples")
ENDIF ( MXA_USE_HDF5 )

# ------------------------------------------------------------------------------
#  Add Executable for Example 1
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 )
    cmp_IDE_GENERATED_PROPERTIES("MXA/Examples/Example1" "" "Example1.cpp" "0")
    ADD_EXECUTABLE(Example1 ${MXAExamples_SOURCE_DIR}/Example1/Example1.cpp)
    TARGET_LINK_LIBRARIES(Example1 ${MXADATAMODEL_LIB_NAME} )
    ToolInstallationSupport (Example1 "_debug" ${MXADataModel_BINARY_DIR} "Examples" )
ENDIF ( MXA_USE_HDF5 )

# ------------------------------------------------------------------------------
#  Add Executable for Example 2
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 )
    cmp_IDE_GENERATED_PROPERTIES("MXA/Examples/Example2" "" "Example2.cpp" "0")
    ADD_EXECUTABLE(Example2 ${MXAExamples_SOURCE_DIR}/Example2/Example2.cpp)
    TARGET_LINK_LIBRARIES(Example2 ${MXADATAMODEL_LIB_NAME})
    ToolInstallationSupport (Example2 "_debug" ${MXADataModel_BINARY_DIR} "Examples")
ENDIF ( MXA_USE_HDF5 )

# ------------------------------------------------------------------------------
#  Add Executable for DataWrapperExample
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 )
    ADD_EXECUTABLE(DataWrapperExample ${MXAExamples_SOURCE_DIR}/DataWrapperExample.cpp)
    TARGET_LINK_LIBRARIES(DataWrapperExample ${MXADATAMODEL_LIB_NAME})
    ToolInstallationSupport (DataWrapperExample "_debug" ${MXADataModel_BINARY_DIR} "Examples")
ENDIF ( MXA_USE_HDF5 )

# ------------------------------------------------------------------------------
#  Add Executable for DataImport Example
# ------------------------------------------------------------------------------
SET (DATAIMPORT_EXAMPLE_SOURCES
    ${MXAExamples_SOURCE_DIR}/DataImport/main.cpp
    ${MXAExamples_SOURCE_DIR}/DataImport/SimpleImportExample.cpp
    ${MXAExamples_SOURCE_DIR}/DataImport/ExampleImportDelegate.cpp
    ${MXAExamples_SOURCE_DIR}/DataImport/ExampleImportDelegateFactory.cpp
)
SET (DATAIMPORT_EXAMPLE_HEADERS
  ${MXAExamples_SOURCE_DIR}/DataImport/SimpleImportExample.h
  ${MXAExamples_SOURCE_DIR}/DataImport/ExampleImportDelegate.h
  ${MXAExamples_SOURCE_DIR}/DataImport/ExampleImportDelegateFactory.h
)
cmp_IDE_GENERATED_PROPERTIES("MXA/Examples/DataImport" "${DATAIMPORT_EXAMPLE_HEADERS}" "${DATAIMPORT_EXAMPLE_SOURCES}" "0")

IF ( MXA_USE_HDF5 )
    ADD_EXECUTABLE(DataImportExample ${DATAIMPORT_EXAMPLE_SOURCES})
    TARGET_LINK_LIBRARIES(DataImportExample ${MXADATAMODEL_LIB_NAME} ) 
    ToolInstallationSupport (DataImportExample "_debug" "${MXADataModel_BINARY_DIR}" "Examples")
ENDIF ( MXA_USE_HDF5 )


# ------------------------------------------------------------------------------
#  Add Executable for MXAProperty Example
# ------------------------------------------------------------------------------
SET (MXA_PROPERTY_EXAMPLE_SRCS
    ${MXAExamples_SOURCE_DIR}/MXAPropertyExample/main.cpp
    ${MXAExamples_SOURCE_DIR}/MXAPropertyExample/MyPropertyClass.cpp
)
SET (MXA_PROPERTY_EXAMPLE_HEADERS
    ${MXAExamples_SOURCE_DIR}/MXAPropertyExample/MyPropertyClass.h
)
cmp_IDE_GENERATED_PROPERTIES("MXA/Examples/MXAPropertyExample" "${MXA_PROPERTY_EXAMPLE_HEADERS}" "${MXA_PROPERTY_EXAMPLE_SRCS}" "0")

ADD_EXECUTABLE(MXAPropertyExample ${MXA_PROPERTY_EXAMPLE_SRCS} ${MXA_PROPERTY_EXAMPLE_HEADERS})
TARGET_LINK_LIBRARIES(MXAPropertyExample ${MXADATAMODEL_LIB_NAME} ) 
ToolInstallationSupport (MXAPropertyExample "_debug" ${MXADataModel_BINARY_DIR} "Examples")

# ------------------------------------------------------------------------------
#  Add Executable for Support File Example
# ------------------------------------------------------------------------------
IF ( MXA_USE_HDF5 AND MXA_USE_TIFF)
    SET (SUPPORT_FILE_EXAMPLE_SRCS
        ${MXAExamples_SOURCE_DIR}/SupportFileExample.cpp
        ${MXATests_SOURCE_DIR}/TiffMaker.cpp
    )
    SET (SUPPORT_FILE_EXAMPLE_HEADERS
        
    )
    cmp_IDE_GENERATED_PROPERTIES("MXA/Examples" "${SUPPORT_FILE_EXAMPLE_HEADERS}" "${SUPPORT_FILE_EXAMPLE_SRCS}" "0")
 
    ADD_EXECUTABLE(SupportFileExample ${SUPPORT_FILE_EXAMPLE_SRCS} ${SUPPORT_FILE_EXAMPLE_HEADERS})
    TARGET_LINK_LIBRARIES(SupportFileExample ${MXADATAMODEL_LIB_NAME} )
    ToolInstallationSupport (SupportFileExample "_debug" ${MXADataModel_BINARY_DIR} "Examples")
ENDIF ( MXA_USE_HDF5 AND MXA_USE_TIFF )

