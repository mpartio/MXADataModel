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


SET (GETDATA_SRCS
  GetDataExample.cpp
  DataFileGenerator.cpp
)


ADD_EXECUTABLE ( GetDataExample ${GETDATA_SRCS}  )

TARGET_LINK_LIBRARIES ( GetDataExample
  MXADataModel
  ${HDF5_LIBRARIES}
)




