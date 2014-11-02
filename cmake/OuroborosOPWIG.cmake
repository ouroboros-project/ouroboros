
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
        get_filename_component(OUTPUT_FILE_DIR ${OUTPUT_FILE_NAME} PATH)
        add_custom_command(OUTPUT ${OUTPUT_FILE_NAME}
                            COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_FILE_DIR}
                            COMMAND ${CLANGINTROSPECTOR_EXECUTABLE}
                            ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${it} "--" -x c++ ${INCLUDE_COMMMAND} > ${OUTPUT_FILE_NAME}
                            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${it})
        list(APPEND METADATA_FILES ${OUTPUT_FILE_NAME})
    endforeach()
    add_custom_target(${target}-metadata
                        DEPENDS ${METADATA_FILES})
    set(OUROBOROS_METADATAFILES_${target} ${METADATA_FILES} PARENT_SCOPE)
    
    get_target_property(TARGET_FOLDER ${target} FOLDER)
    if(TARGET_FOLDER)
        set_target_properties (${target}-metadata PROPERTIES FOLDER ${TARGET_FOLDER}/Generated)
    endif()
endfunction(ouroboros_export_metadata)

function(ouroboros_create_wrappers TARGET LANGUAGE)
    if(DEFINED ARGV2)
        set(OUTDIR "${ARGV2}")
    else()
        set(OUTDIR "${CMAKE_CURRENT_BINARY_DIR}")
    endif()
    string(REPLACE "-" "_" MODULE_NAME ${TARGET})
    set(WRAPPER_FILE "${OUTDIR}/${LANGUAGE}_${MODULE_NAME}_wrap.cxx")
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
                --output-file="${WRAPPER_FILE}"
                ${OUROBOROS_METADATAFILES_${TARGET}}
        DEPENDS ${OUROBOROS_SPECIFIC_EXECUTABLE} ${OUROBOROS_METADATAFILES_${TARGET}}
    )
    add_library(${TARGET}-${LANGUAGE} ${WRAPPER_FILE})
    target_link_libraries(${TARGET}-${LANGUAGE} ${TARGET} libouroboros-${LANGUAGE})
    
    get_target_property(TARGET_FOLDER ${TARGET} FOLDER)
    if(TARGET_FOLDER)
        set_target_properties (${TARGET}-${LANGUAGE} PROPERTIES FOLDER ${TARGET_FOLDER}/Generated)
    endif()
    
    unset (OUROBOROS_SPECIFIC_EXECUTABLE)
endfunction(ouroboros_create_wrappers)

