
########################################################################
# CMake build script for Google Test.
#
# To run the tests for Google Test itself on Linux, use 'make test' or
# ctest.  You can select which tests to run using 'ctest -R regex'.
# For more options, run 'ctest --help'.

# When other libraries are using a shared version of runtime libraries,
# Google Test also has to use one.
option(
  gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  ON)

option(gtest_build_tests "Build all of gtest's own tests." OFF)
option(gtest_build_samples "Build gtest's sample programs." OFF)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." OFF)

# Defines pre_project_set_up_hermetic_build() and set_up_hermetic_build().
include(cmake/hermetic_build.cmake OPTIONAL)

if (COMMAND pre_project_set_up_hermetic_build)
  pre_project_set_up_hermetic_build()
endif()

########################################################################
#
# Project-wide settings
project(gtest CXX C)
cmake_minimum_required(VERSION 2.6.2)

# Name of the project.
#
# CMake files in this project can refer to the root source directory
# as ${gtest_SOURCE_DIR} and to the root binary directory as
# ${gtest_BINARY_DIR}.
# Language "C" is required for find_package(Threads).
if (COMMAND set_up_hermetic_build)
  set_up_hermetic_build()
endif()

# Define helper functions and macros used by Google Test.
include(internal_utils.cmake)

config_compiler_and_linker()  # Defined in internal_utils.cmake.

# Where Google Test's .h files can be found.
include_directories("." "gtest" "${CMAKE_CURRENT_BINARY_DIR}/src" "${gtest_BINARY_DIR}")

# Where Google Test's libraries can be found.
link_directories(${gtest_BINARY_DIR}/gtest_src)

########################################################################
#
# Defines the gtest & gtest_main libraries.  User tests should link
# with one of them.

# Google Test libraries.  We build them using more strict warnings than what
# are used for other targets, to ensure that gtest can be compiled by a user
# aggressive about warnings.
cxx_library(gtest "${cxx_strict}" gtest_src/gtest-all.cc)
cxx_library(gtest_main "${cxx_strict}" gtest_src/gtest_main.cc)
target_link_libraries(gtest_main gtest)

########################################################################
#
# Samples on how to link user tests with gtest or gtest_main.
#
# They are not built by default.  To build them, set the
# gtest_build_samples option to ON.  You can do it by running ccmake
# or specifying the -Dbuild_gtest_samples=ON flag when running cmake.

if(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:libcmtd.lib)
endif()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "" )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "" )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

include_directories("src")

cxx_executable (ouroboros_test src "gtest_main;libouroboros")

set (OUROBOROS_TEST_DUMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/dump)
configure_file (
  "src/config.h.in"
  "src/config.h"
)

add_executable (oldscript_test EXCLUDE_FROM_ALL script_test.cxx )
set_target_properties (oldscript_test PROPERTIES FOLDER "Tests")
target_link_libraries (
  oldscript_test
  libouroboros
  libouroboros-lua
  libouroboros-python
)

cxx_executable (opwig_test src gtest_main src/config.h)
target_link_libraries (opwig_test libopwig-common )
add_custom_command(TARGET opwig_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUROBOROS_TEST_DUMP_DIR}
)

cxx_executable (opa_test src gtest_main src/config.h)
target_link_libraries (opa_test libouroboros)