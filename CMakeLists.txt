# Requires CMake 2.8 or newer.
cmake_minimum_required (VERSION 2.8)

project (ouroboros CXX C)
set (FRAMEWORK_VERSION_MAJOR 0)
set (FRAMEWORK_VERSION_MINOR 1)
set (FRAMEWORK_VERSION_PATCH 0)
set (FRAMEWORK_BIGVERSION ${FRAMEWORK_VERSION_MAJOR}.${FRAMEWORK_VERSION_MINOR})
set (FRAMEWORK_VERSION ${FRAMEWORK_BIGVERSION}.${FRAMEWORK_VERSION_PATCH})
set (GROUP_NAME "Ouroboros Team")
set (HOME_URL "http://uspgamedev.org/")
set (CONTACT_MAIL "kazuo256@gmail.com;fernando_aluani@hotmail.com")
set (OUROBOROS_INSTALL_LOCATION share/ouroboros)

message ("=== Ouroboros Project version ${FRAMEWORK_VERSION} ===")

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

option(OUROBOROS_BUILD_TESTS "Set to ON to enable building tests." ON)
option(OUROBOROS_CREATE_BINDINGS "Set to ON to generate bindings for supported script languages." ON)
option(OUROBOROS_LUA_BINDINGS "Set to ON to enable Lua as one of the languages to create bindings." ON)
option(OUROBOROS_PYTHON_BINDINGS "Set to ON to enable Python as one of the languages to create bindings." ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET (CHECKMAC "#define ISMAC")
endif ()

IF (UNIVERSAL)
    # Necessary for a Mac Universal Binary
    SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
    SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
    SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF ()

# Important build locations.
set (SRC_DIR src)
set (LIB_DIR lib)
set (BIN_DIR bin)

set(FRAMEWORK_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR})

CONFIGURE_FILE(
    "doxystuff/Doxyfile.in"
    "doxystuff/Doxyfile"
)

#set (CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR}")
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${WRAPPER_DIR}")

include (${SRC_DIR}/src_list.cmake)
SET(OUROBOROS_MODULE_SRC 

)


# LDFLAGS: flags que voce queira enviar ao ligador
set (LDFLAGS "" CACHE STRING "flags enviadas ao linker")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    LIST(APPEND LDFLAGS "-mmacosx-version-min=10.5")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
if (UNIX OR MINGW)
    # se for o g++ para *nix ou o MinGW para Windows:
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -msse2 -std=gnu++0x)
elseif (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_VARIADIC_MAX=10 /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /fp:fast /MP)
endif ()

# ${OUROBOROS_SRC} está definido?
if (NOT OUROBOROS_SRC)
    message (FATAL_ERROR "Error: OUROBOROS_SRC not defined! Please do so in the file src/src_list.cmake!")
endif (NOT OUROBOROS_SRC)

include_directories ("src")
include (cmake/ouroboros.cmake)

##########################################
# Package Configuration

set (CPACK_GENERATOR DEB)
set (CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.txt)
set (CPACK_PACKAGE_NAME libouroboros)
set (CPACK_PACKAGE_VENDOR ${GROUP_NAME})
set (CPACK_PACKAGE_CONTACT "${GROUP_NAME} <${CONTACT_MAIL}>")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "ouroboros")
set (CPACK_PACKAGE_VERSION_MAJOR ${FRAMEWORK_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${FRAMEWORK_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${FRAMEWORK_VERSION_PATCH})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ouroboros Project, a system for integrating C++ and script languages.")
set (CPACK_DEBIAN_PACKAGE_NAME "libouroboros${FRAMEWORK_BIGVERSION}")
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE ${HOME_URL})
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.2), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0), lua5.1, python (>=2.6)")
set (DEBIAN_PACKAGE_BUILDS_DEPENDS "libc6 (>= 2.2), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0), swig2.0, lua5.1-dev, python-dev (>= 2.6), cmake")
set (CPACK_NSIS_CONTACT ${CONTACT_MAIL})
set (CPACK_NSIS_DISPLAY_NAME "Ouroboros Project")
set (CPACK_NSIS_PACKAGE_HOMEPAGE ${HOME_URL})
set (CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set (CPACK_RPM_PACKAGE_URL ${HOME_URL})

include (CPack)

#
##########################################
# Bit hacky way so all the output files are placed on the ${LIB_DIR} folder.
# Code found on http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2

# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${BIN_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


if (OUROBOROS_CREATE_BINDINGS)
    ouroboros_setup_package(SWIG)
    add_subdirectory( src/languages )
endif (OUROBOROS_CREATE_BINDINGS)

add_library (libouroboros ${OUROBOROS_SRC} ${GENERATED_SRC})

include (cmake/opwig.cmake)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (EXTRA_LIBRARIES m ${EXTRA_LIBRARIES})
endif (CMAKE_COMPILER_IS_GNUCXX)
if (MINGW)
    set (EXTRA_LIBRARIES mingw32 ${EXTRA_LIBRARIES})
endif (MINGW)
if (WIN32)
    set (EXTRA_LIBRARIES Ws2_32 ${EXTRA_LIBRARIES})
endif (WIN32)


# We could use LINK_PUBLIC here, but it's a new feature in cmake 2.8.7, so we'd rather not require the newest version.

message (" OUROBOROS LIBRARIES: ${OUROBOROS_LIBRARIES}")

target_link_libraries (libouroboros
                       ${EXTRA_LIBRARIES}
                       ${OUROBOROS_LIBRARIES})

target_link_libraries (libouroboros LINK_INTERFACE_LIBRARIES 
                       ${EXTRA_LIBRARIES}
                       ${OUROBOROS_LIBRARIES})
                      
set_target_properties (libouroboros PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME OUROBOROS
    OUTPUT_NAME ouroboros${FRAMEWORK_BIGVERSION}
    OUTPUT_NAME_DEBUG ouroboros${FRAMEWORK_BIGVERSION}-dbg
    SOVERSION ${FRAMEWORK_VERSION_PATCH}
    VERSION ${FRAMEWORK_VERSION}
)

if(OUROBOROS_BUILD_TESTS)
    add_subdirectory (test)
endif(OUROBOROS_BUILD_TESTS)


CONFIGURE_FILE(
  "${SRC_DIR}/opa/config.h.in"
  "${SRC_DIR}/opa/config.h"
)

install(TARGETS libouroboros
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY src/ouroboros DESTINATION include/ouroboros${FRAMEWORK_BIGVERSION} FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")

