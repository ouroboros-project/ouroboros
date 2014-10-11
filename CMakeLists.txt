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
set (OUROBOROS_INSTALL_LOCATION share/ouroboros)

message ("=== Ouroboros Project version ${OUROBOROS_VERSION} ===")

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

option(OUROBOROS_BUILD_TESTS "Set to ON to enable building tests." ON)
option(OUROBOROS_CREATE_BINDINGS "Set to ON to generate bindings for supported script languages." ON)
option(OUROBOROS_LUA_BINDINGS "Set to ON to enable Lua as one of the languages to create bindings." ON)
option(OUROBOROS_PYTHON_BINDINGS "Set to ON to enable Python as one of the languages to create bindings." ON)

# Important build locations.
set (SRC_DIR src)
set (LIB_DIR lib)
set (BIN_DIR bin)

set(OUROBOROS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR})

CONFIGURE_FILE(
    "doxystuff/Doxyfile.in"
    "doxystuff/Doxyfile"
)

# LDFLAGS: flags for the linker
set (LDFLAGS "" CACHE STRING "flags sent to the linker")

if (UNIX OR MINGW)
    # if g++ for *nix or MinGW for Windows:
    add_definitions(-Wall -ansi -U_FORTIFY_SOURCE -msse2 -std=gnu++0x)
elseif (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_VARIADIC_MAX=10 /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /fp:fast /MP)
endif ()

include_directories (${OUROBOROS_SRC_DIR})

##########################################
# Package Configuration
# TODO check dependencies

set (CPACK_GENERATOR DEB)
set (CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.txt)
set (CPACK_PACKAGE_NAME libouroboros)
set (CPACK_PACKAGE_VENDOR ${GROUP_NAME})
set (CPACK_PACKAGE_CONTACT "${GROUP_NAME} <${CONTACT_MAIL}>")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "ouroboros")
set (CPACK_PACKAGE_VERSION_MAJOR ${OUROBOROS_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${OUROBOROS_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${OUROBOROS_VERSION_PATCH})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ouroboros Project, a system for automated integration between C++ and script languages.")
set (CPACK_DEBIAN_PACKAGE_NAME "libouroboros${OUROBOROS_BIGVERSION}")
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE ${HOME_URL})
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.2), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0), lua5.1, python (>=2.6)")
set (DEBIAN_PACKAGE_BUILDS_DEPENDS "libc6 (>= 2.2), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0), lua5.1-dev, python-dev (>= 2.6), cmake")
set (CPACK_NSIS_CONTACT ${CONTACT_MAIL})
set (CPACK_NSIS_DISPLAY_NAME "Ouroboros Project")
set (CPACK_NSIS_PACKAGE_HOMEPAGE ${HOME_URL})
set (CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set (CPACK_RPM_PACKAGE_URL ${HOME_URL})

include (CPack)

##########################################
# Bit hacky way so all the output files are placed on the ${LIB_DIR} folder.
# Code found on http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2

# First for the generic no-config case (e.g. with mingw)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LIB_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LIB_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_BINARY_DIR}/${LIB_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_BINARY_DIR}/${LIB_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

##########################
## Add libouroboros target
add_subdirectory(opa)

##########################
# Create opwig

if (OUROBOROS_CREATE_BINDINGS)
    add_subdirectory( externals/libjson )
    add_subdirectory( opwig )
       
    include (cmake/opwig.cmake)
    add_subdirectory( languages )
    
    find_program(CLANGINTROSPECTOR_EXECUTABLE clang-introspector)
    function(ouroboros_export_metadata target)
        get_target_property(TARGET_INCLUDE_DIRECTORIES ${target} INCLUDE_DIRECTORIES)

        set(INCLUDE_COMMMAND)
        foreach(it ${TARGET_INCLUDE_DIRECTORIES})
            list(APPEND INCLUDE_COMMMAND "-I${it}")
        endforeach()
        
        set(METADATA_FILES)
        foreach(it ${ARGN})
            set(OUTPUT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/metadata/${it}.json)
            add_custom_command(OUTPUT ${OUTPUT_FILE_NAME}
                                COMMAND ${CLANGINTROSPECTOR_EXECUTABLE}
                                ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${it} "--" -x c++ ${INCLUDE_COMMMAND} > ${OUTPUT_FILE_NAME}
                                DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${it})
            list(APPEND METADATA_FILES ${OUTPUT_FILE_NAME})
        endforeach()
        add_custom_target(${target}-metadata
                            DEPENDS ${METADATA_FILES})
        set(OUROBOROS_METADATAFILES_${target} ${METADATA_FILES} PARENT_SCOPE)
    endfunction(ouroboros_export_metadata)

    function(ouroboros_create_wrappers TARGET LANGUAGE)
        if(DEFINED ARGV2)
            set(OUTDIR "${ARGV2}")
        else()
            set(OUTDIR "${CMAKE_CURRENT_BINARY_DIR}")
        endif()
        string(REPLACE "-" "_" MODULE_NAME ${TARGET})
        set(WRAPPER_FILE "${CMAKE_CURRENT_BINARY_DIR}/${LANGUAGE}_${MODULE_NAME}_wrap.cxx")
        set(OUROBOROS_SPECIFIC_EXECUTABLE opwig-${LANGUAGE})
        
        if (UNIX OR MINGW)
            set_source_files_properties (
                ${WRAPPER_FILE}
                PROPERTIES
                GENERATED 1
                COMPILE_FLAGS -std=c++11
            )
        endif()
        add_custom_command (
            OUTPUT  ${WRAPPER_FILE}
            COMMAND ${OUROBOROS_SPECIFIC_EXECUTABLE}
            ARGS    --module-name=${MODULE_NAME}
                    --output-dir="${OUTDIR}"
                    ${OUROBOROS_METADATAFILES_${TARGET}}
            DEPENDS ${OUROBOROS_SPECIFIC_EXECUTABLE} ${OUROBOROS_METADATAFILES_${TARGET}}
        )
        add_library(${TARGET}-${LANGUAGE} ${WRAPPER_FILE})
        target_link_libraries(${TARGET}-${LANGUAGE} ${TARGET} libouroboros-${LANGUAGE})
        
        unset (OUROBOROS_SPECIFIC_EXECUTABLE)
    endfunction(ouroboros_create_wrappers)
endif (OUROBOROS_CREATE_BINDINGS)




#########################


#########################

if(OUROBOROS_BUILD_TESTS)
    add_subdirectory (test)
endif(OUROBOROS_BUILD_TESTS)

