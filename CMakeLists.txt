cmake_minimum_required (VERSION 3.8.2)

# rSGD Version
set (RSGD_MAJOR    1)
set (RSGD_MINOR    0)
set (RSGD_REVISION 0)
set (RSGD_VERSION ${RSGD_MAJOR}.${RSGD_MINOR}.${RSGD_REVISION})

project (rSGD LANGUAGES CXX VERSION ${RSGD_VERSION} DESCRIPTION "Replicated Focusing Belief Propagation")
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions (-DMAJOR=${RSGD_MAJOR} -DMINOR=${RSGD_MINOR} -DREVISION=${RSGD_REVISION})

#################################################################
#                         COMPILE OPTIONS                       #
#################################################################

option (OMP        "Enable OpenMP                support" OFF)
option (VERBOSE    "Enable verbose stdout        support" ON)
option (PYrsgd     "Enable Python wrap compilation      " OFF)

#################################################################
#                         SETTING VARIABLES                     #
#################################################################

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/" ${CMAKE_MODULE_PATH})

if ( NOT APPLE )
  set (CMAKE_SKIP_BUILD_RPATH             FALSE )
  set (CMAKE_BUILD_WITH_INSTALL_RPATH     FALSE )
  set (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE  )
endif()

# make sure that the default is a RELEASE
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if ( CMAKE_COMPILER_IS_GNUCXX )
  add_compile_options (-Wall -Wextra -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-narrowing -Wpedantic)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    string (REGEX REPLACE "-O[0123]" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
    set (CMAKE_CXX_FLAGS_RELEASE "")
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    string (REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
    set (CMAKE_CXX_FLAGS_DEBUG "")
  endif()
  #list (APPEND linked_libs stdc++fs)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  add_compile_options(-Wno-deprecated -Wno-writable-strings)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    string (REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    string (REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
  endif()
  #list (APPEND linked_libs stdc++fs)
endif()

if (MSVC)
  set (CMAKE_CXX_FLAGS "/wd4013 /wd4018 /wd4028 /wd4047 /wd4068 /wd4090 /wd4101 /wd4113 /wd4133 /wd4190 /wd4244 /wd4267 /wd4305 /wd4477 /wd4996 /wd4819 /fp:fast ${CMAKE_CXX_FLAGS}")
  string (REGEX REPLACE "/O2" "/Ox" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  add_definitions (-D_CRT_RAND_S)
  add_definitions (-DNOMINMAX)
  add_definitions (-D_USE_MATH_DEFINES)
  add_definitions (-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
  set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()


#################################################################
#                         PARSE OPTIONS                         #
#################################################################

if (OMP)
  find_package(OpenMP REQUIRED)
  if (OPENMP_FOUND)
    message(STATUS "OpenMP found")
    if (OpenMP_CXX_VERSION_MAJOR LESS 4 AND OpenMP_CXX_VERSION_MINOR LESS 5)
      message(FATAL_ERROR " Your OpenMP is too old. Required OpenMP 4.5. Please upgrade.")
    endif()
    message(STATUS "OpenMP found")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    if (APPLE)
      list(APPEND linked_libs OpenMP::OpenMP_CXX)
    endif()
  endif()
else()
  message(STATUS "OpenMP disabled")
endif()

if (VERBOSE)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERBOSE" )
  add_definitions (-DVERBOSE)
  message (STATUS "Verbose state: ON")
else()
  message (STATUS "Verbose disabled")
endif()

#################################################################
#                         SETTING DIRECTORIES                   #
#################################################################

set(SRC_DIR  ${CMAKE_SOURCE_DIR}/src                                 CACHE PATH "Path where find cpp files"                        )
set(INC_DIR  ${CMAKE_SOURCE_DIR}/include                             CACHE PATH "Path where find header files"                     )
set(EXAMPLE  ${CMAKE_SOURCE_DIR}/example                             CACHE PATH "Path where find main files"                       )
set(PYC_DIR  ${CMAKE_SOURCE_DIR}/ReplicatedStochasticGradientDescent CACHE PATH "Path where find cython files"                     )         # cython directory
set(LIB_DIR  ${CMAKE_SOURCE_DIR}/lib                                 CACHE PATH "Path where lib will be installed"            FORCE)
set(INST_DIR ${CMAKE_SOURCE_DIR}/share/rsgd                          CACHE PATH "Path where cmake configs will be installed"  FORCE)
set(OUT_DIR  ${CMAKE_SOURCE_DIR}/bin                                 CACHE PATH "Path where outputs will be installed"        FORCE)
set(OUT_TEST ${CMAKE_SOURCE_DIR}/test/bin                            CACHE PATH "Path where test outputs will be installed"   FORCE)

set(INC_SCORER ${CMAKE_SOURCE_DIR}/scorer/include             CACHE PATH "Path where to find scorer header"            FORCE)

set(rsgdlib rsgd)
set(rsgdtestmain test_main)
set(rsgdtrainmain train_main)

file(GLOB SRC           "${SRC_DIR}/*.cpp" )
file(GLOB HEADER        "${INC_DIR}/*.h"   )
file(GLOB SCORER_HEADER "${INC_SCORER}/*.h")
# file(GLOB EXE    "${EXAMPLE}/*.cpp")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.9")
  list (APPEND SRC ${CMAKE_SOURCE_DIR}/scorer/src/scorer.cpp)
endif()

include_directories(${INC_DIR} ${INC_SCORER})

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

if (PYrsgd)
  include( UseCython )
endif()

#################################################################
#                          SUMMARY                              #
#################################################################

message(STATUS ""                                                                    )
message(STATUS "=================== rSGD configuration Summary =================="   )
message(STATUS "   rSGD version: ${RSGD_MAJOR}.${RSGD_MINOR}.${RSGD_REVISION}"       )
message(STATUS ""                                                                    )
message(STATUS "   Build type : ${CMAKE_BUILD_TYPE}"                                 )
message(STATUS "   C++ :"                                                            )
message(STATUS "      C++ Compiler : ${CMAKE_CXX_COMPILER}"                          )
message(STATUS "      C++ flags    :"                                                )
foreach(FLAG ${CMAKE_CXX_FLAGS_LIST})
  message(STATUS "                    * ${FLAG}"                                     )
endforeach(FLAG)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "      C++ Debug flags:"                                            )
  foreach(FLAG ${CMAKE_CXX_FLAGS_DEBUG})
    message(STATUS "                    * ${FLAG}"                                   )
  endforeach(FLAG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "      C++ Release flags  :"                                        )
  foreach(FLAG ${CMAKE_CXX_FLAGS_RELEASE})
    message(STATUS "                    * ${FLAG}"                                   )
  endforeach(FLAG)
endif()
message(STATUS "      Linker flags : "                                               )
foreach(FLAG ${linked_libs})
  message(STATUS "                    * ${FLAG}"                                     )
endforeach(FLAG)
message(STATUS ""                                                                    )
message(STATUS "   OpenMP support : ${OMP}"                                          )
message(STATUS "   VERBOSE level  : ${VERBOSE}"                                      )
message(STATUS ""                                                                    )
message(STATUS ""                                                                    )
message(STATUS "   Compile Pythonize version : ${PYrsgd}"                            )
message(STATUS ""                                                                    )


#################################################################
#                         MAIN RULES                            #
#################################################################

if (PYrsgd)
  add_subdirectory(${PYC_DIR}/source)
endif()

add_library(${rsgdlib} SHARED ${SRC} ${HEADER} ${SCORER_HEADER})
set_property(TARGET ${rsgdlib} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${rsgdlib} ${linked_libs})

add_executable(${rsgdtrainmain} ${EXAMPLE}/train_main.cpp)
target_link_libraries(${rsgdtrainmain} ${linked_libs} ${rsgdlib})

add_executable(${rsgdtestmain} ${EXAMPLE}/test_main.cpp)
target_link_libraries(${rsgdtestmain} ${linked_libs} ${rsgdlib})


#################################################################
#                          INSTALLERS                           #
#################################################################

install(TARGETS ${rsgdlib}          EXPORT rSGDTargets
                                    RUNTIME DESTINATION ${OUT_DIR}
                                    LIBRARY DESTINATION ${LIB_DIR}
                                    ARCHIVE DESTINATION ${LIB_DIR}
                                    PUBLIC_HEADER DESTINATION ${LIB_DIR}/include
                                    COMPONENT dev
                                    )
install(TARGETS ${rsgdtrainmain}   DESTINATION ${OUT_DIR})
install(TARGETS ${rsgdtestmain}    DESTINATION ${OUT_DIR})


#################################################################
#                          EXPORT RSGD                          #
#################################################################

install (EXPORT rSGDTargets
         FILE rSGDTargets.cmake
         NAMESPACE rSGD::
         DESTINATION "${INST_DIR}"
        )

# Export the package for use from the build-tree (this registers the build-tree with a global CMake-registry)
export (PACKAGE rSGD)

# Create the rSGDConfig.cmake
# First of all we compute the relative path between the cmake config file and the include path
file (RELATIVE_PATH REL_INCLUDE_DIR "${INST_DIR}" "${LIB_DIR}/include")
set (CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file (rSGDConfig.cmake.in "${PROJECT_BINARY_DIR}/rSGDConfig.cmake" @ONLY)
set (CONF_INCLUDE_DIRS "\${rSGD_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file (rSGDConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/rSGDConfig.cmake" @ONLY)

# Create the rSGDConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file ("${PROJECT_BINARY_DIR}/rSGDConfigVersion.cmake"
                                  COMPATIBILITY SameMajorVersion
                                 )

install (FILES
         "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/rSGDConfig.cmake"
         "${PROJECT_BINARY_DIR}/rSGDConfigVersion.cmake"
         DESTINATION "${INST_DIR}"
         )
