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

OPTION (MXA_BUILD_DATAIMPORT_EXAMPLE "Examples for importing data" OFF)
IF (MXA_BUILD_DATAIMPORT_EXAMPLE)
	SET (DATA_IMPORT_SRCS
	  main.cpp
	  DataImport/SimpleImportExample.cpp
	 # DataImport/ComplexImportExample.cpp
	 # DataImport/ExampleImportDelegate.cpp
	)
	
	SET (DATA_IMPORT_HEADERS
    DataImport/ComplexImportExample.h
    DataImport/ExampleImportDelegate.h
    DataImport/SimpleImportExample.h
    )
	
	ADD_EXECUTABLE ( DataImportExample ${DATA_IMPORT_SRCS} ${DATA_IMPORT_HEADERS} )
	
	TARGET_LINK_LIBRARIES ( DataImportExample
	  MXADataModel
	  ${HDF5_LIBRARIES}
	)
ENDIF(MXA_BUILD_DATAIMPORT_EXAMPLE)



