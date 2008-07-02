SET (MXA_BMPIO_SRCS
    ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpIO.cpp
)

SET (MXA_BMPIO_HEADERS
  ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpIO.h
  ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpHeaders.h
)

INSTALL (FILES ${MXA_BMPIO_HEADERS}
            DESTINATION include/MXADataModel/BMPIO
            COMPONENT Headers           
)

