cmake_minimum_required (VERSION 2.8.11)
# 2.8.11 for target_include_directories

project (ouroboros CXX C)
set (OUROBOROS_VERSION_MAJOR 0)
set (OUROBOROS_VERSION_MINOR 1)
set (OUROBOROS_VERSION_PATCH 0)
set (OUROBOROS_BIGVERSION ${OUROBOROS_VERSION_MAJOR}.${OUROBOROS_VERSION_MINOR})
set (OUROBOROS_VERSION ${OUROBOROS_BIGVERSION}.${OUROBOROS_VERSION_PATCH})
set (GROUP_NAME "Ouroboros Team")
set (HOME_URL "http://uspgamedev.org/")
set (CONTACT_MAIL "kazuo256@gmail.com;fernando_aluani@hotmail.com")
set (ConfigPackageLocation share/cmake)

message ("=== Ouroboros Project version ${OUROBOROS_VERSION} ===")

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

option(OUROBOROS_BUILD_TESTS "Set to ON to enable building tests." ON)
option(OUROBOROS_LUA_BINDINGS "Set to ON to enable Lua as one of the languages to create bindings." ON)
option(OUROBOROS_PYTHON_BINDINGS "Set to ON to enable Python as one of the languages to create bindings." ON)


# FIXME: this is completely broken...
set(OUROBOROS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
CONFIGURE_FILE(
    "doxystuff/Doxyfile.in"
    "doxystuff/Doxyfile"
)

# Organize the generated project using folders.
set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

if (UNIX OR MINGW)
    # if g++ for *nix or MinGW for Windows:
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -msse2 -std=gnu++0x)
elseif (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /MP)
endif ()

##########################################
# Bit hacky way so all the output files are placed on the ${LIB_DIR} folder.
# Code found on http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2
set (LIB_DIR lib)
set (BIN_DIR bin)

# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${BIN_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LIB_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LIB_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUTCONFIG}/${BIN_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUTCONFIG}/${LIB_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUTCONFIG}/${LIB_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

##########################
## Add libouroboros target
add_subdirectory(opa)

##########################
# Create opwig
add_subdirectory( externals/libjson )
add_subdirectory( opwig )   
include (cmake/opwig.cmake)

##########################
# Handle the language implementation
add_subdirectory( languages )

#########################

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/OuroborosConfigVersion.cmake"
  VERSION ${OUROBOROS_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(EXPORT Ouroboros
  FILE OuroborosTargets.cmake
  DESTINATION ${ConfigPackageLocation}
)

install(
  FILES
    cmake/OuroborosConfig.cmake
    cmake/OuroborosOPWIG.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/OuroborosConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
)

#########################

if(OUROBOROS_BUILD_TESTS)
    add_subdirectory (test)
endif(OUROBOROS_BUILD_TESTS)

