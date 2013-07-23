
cmake_minimum_required(VERSION 2.8)

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

add_executable (milestone-00 EXCLUDE_FROM_ALL src/script_test.cxx )
target_link_libraries (
  milestone-00
  libouroboros
  libouroboros-lua
  libouroboros-python
)

#configure_file (
#  "src/config.h.in"
#  "src/config.h"
#)

