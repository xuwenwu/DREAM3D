SET (ANG_HDF5_SRCS
  ${TSLAngLib_SOURCE_DIR}/HDF5/H5Lite.cpp
  ${TSLAngLib_SOURCE_DIR}/HDF5/H5Utilities.cpp
)

SET (ANG_HDF5_HDRS
  ${TSLAngLib_SOURCE_DIR}/HDF5/H5Lite.h
  ${TSLAngLib_SOURCE_DIR}/HDF5/H5Utilities.h
)
cmp_IDE_SOURCE_PROPERTIES( "TSLAngLib/HDF5" "${ANG_HDF5_SRCS}" "${ANG_HDF5_HDRS}" "0")

if ( ${MXA_INSTALL_FILES} EQUAL 1 )
    INSTALL (FILES ${ANG_HDF5_HDRS}
            DESTINATION include/ANG/HDF5
            COMPONENT Headers   )
endif()
