
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

set(FRAMEWORK_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR})

CONFIGURE_FILE(
    "doxystuff/Doxyfile.in"
    "doxystuff/Doxyfile"
)

#set (CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR}")
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${WRAPPER_DIR}")

include (${SRC_DIR}/src_list.cmake)
include (${SRC_DIR}/lua_src_list.cmake)
include (${SRC_DIR}/py_src_list.cmake)
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


set(OUROBOROS_LANGUAGES_LIST "")
if (OUROBOROS_CREATE_BINDINGS)
    if (OUROBOROS_LUA_BINDINGS)
        list(APPEND OUROBOROS_LANGUAGES_LIST Lua)
    endif()
    if (OUROBOROS_PYTHON_BINDINGS)
        list(APPEND OUROBOROS_LANGUAGES_LIST Python)
    endif()
    ouroboros_setup_package(SWIG)
endif (OUROBOROS_CREATE_BINDINGS)

# Bit hacky way so all the output files are placed on the ${LIB_DIR} folder.
# Code found on http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2

# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIB_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

add_library (libouroboros ${OUROBOROS_SRC} ${GENERATED_SRC})

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

if (UNIX AND PYTHONLIBS_FOUND)
    set (OUROBOROS_INSTALL_LOCATION_DEFINE "#define OUROBOROS_INSTALL_LOCATION \"/usr/${OUROBOROS_INSTALL_LOCATION}\"")
endif (UNIX AND PYTHONLIBS_FOUND)

set(OUROBOROS_EXTERNAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src;${SDL_INCLUDE_DIR};${SDLTTF_INCLUDE_DIR} CACHE STRING "OUROBOROS include directories" FORCE)

CONFIGURE_FILE(
    "${SRC_DIR}/ouroboros/config.h.in"
    "${SRC_DIR}/ouroboros/config.h"
)

install(TARGETS libouroboros
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY src/ouroboros DESTINATION include/ouroboros${FRAMEWORK_BIGVERSION} FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")

if (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
    install(DIRECTORY src/generated/ 
            DESTINATION ${OUROBOROS_INSTALL_LOCATION}/${FRAMEWORK_BIGVERSION}/python
            FILES_MATCHING PATTERN "*.py")
endif (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")

