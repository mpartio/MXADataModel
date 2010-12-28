SET (MXA_BMPIO_SRCS
    ${PROJECT_CODE_DIR}/MXA/BMPIO/MXABmpIO.cpp
)

SET (MXA_BMPIO_HDRS
  ${PROJECT_CODE_DIR}/MXA/BMPIO/MXABmpIO.h
  ${PROJECT_CODE_DIR}/MXA/BMPIO/MXABmpHeaders.h
)
if ( DEFINED MXA_INSTALL_FILES)
    if( ${MXA_INSTALL_FILES} EQUAL 1 )
        set (install_files "1")
    endif()
endif()
cmp_IDE_SOURCE_PROPERTIES( "MXA/BMPIO" "${MXA_BMPIO_HDRS}" "${MXA_BMPIO_SRCS}" ${install_files})
