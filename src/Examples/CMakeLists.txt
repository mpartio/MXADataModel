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

IF (MXA_BUILD_SHARED_LIBS)
    IF (WIN32)
      ADD_DEFINITIONS(-D_HDF5USEDLL_)
    ENDIF (WIN32)
ENDIF (MXA_BUILD_SHARED_LIBS)


SET (GETDATA_SRCS
  GetDataExample.cpp
  ${MXA_SOURCE_DIR}/src/Testing/DataFileGenerator.cpp
)

ADD_EXECUTABLE ( GetDataExample ${GETDATA_SRCS} )
TARGET_LINK_LIBRARIES ( GetDataExample
  MXADataModel
  ${HDF5_LIBRARIES}
  ${DEP_LIBS} 
)

# ------------------------------------------------------------------------------
#  Add Executable for Example 1
# ------------------------------------------------------------------------------
ADD_EXECUTABLE(Example1 Example1/Example1.cpp)
TARGET_LINK_LIBRARIES(Example1 MXADataModel ${HDF5_LIBRARIES})


# ------------------------------------------------------------------------------
#  Add Executable for Example 2
# ------------------------------------------------------------------------------
ADD_EXECUTABLE(Example2 Example2/Example2.cpp)
TARGET_LINK_LIBRARIES(Example2 MXADataModel ${HDF5_LIBRARIES})



