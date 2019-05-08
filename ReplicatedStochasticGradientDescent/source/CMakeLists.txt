# If the pyx file is a C++ file, we should specify that here.

set(PYC_SRC_DIR    ${PYC_DIR}/source       CACHE PATH "Path where find pyx files" )
set(PYC_INC_DIR    ${PYC_DIR}/include      CACHE PATH "Path where find pxd files" )

set_source_files_properties( ${PYC_SRC_DIR}/rFBP.pyx PROPERTIES CYTHON_IS_CXX TRUE )

# Include *.pxd directory

include_directories(${PYC_INC_DIR})

# Multi-file cython modules do not appear to be working at the moment.
cython_add_module( rSGD  ${PYC_SRC_DIR}/rSGD.pyx
                                                  ${INC_DIR}/rsgd.h
                                                  ${INC_DIR}/pattern.h
                                                  ${INC_SCORER}/scorer.h
                                                                        ${SRC_DIR}/rsgd.cpp
                                                                        ${SRC_DIR}/pattern.cpp
                                                                        )


# Installation of targets
install(TARGETS rSGD            DESTINATION ${LIB_DIR}/ReplicatedStochasticGradientDescent)
