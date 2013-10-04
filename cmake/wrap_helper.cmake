
set (OUROBOROS_LANG_PACKAGE_lua     Lua51)
set (OUROBOROS_LANG_VERSION_lua     )
set (OUROBOROS_LANG_FOUND_lua       LUA51_FOUND)
set (OUROBOROS_LANG_LONGNAME_lua    Lua)
set (OUROBOROS_LANG_INCLUDE_DIR_lua LUA_INCLUDE_DIR)

set (OUROBOROS_LANG_PACKAGE_python      PythonLibs)
set (OUROBOROS_LANG_VERSION_python      2.6)
set (OUROBOROS_LANG_FOUND_python        PYTHONLIBS_FOUND)
set (OUROBOROS_LANG_LONGNAME_python     Python)
set (OUROBOROS_LANG_INCLUDE_DIR_python  PYTHON_INCLUDE_DIRS)

function (ouroboros_wrap_module MODULE_NAME LANGUAGE OUTDIR GENERATED_SRC_VAR)
  # Notify
  message (STATUS "Wrapping module ${MODULE_NAME}")
  # Find language package
  find_package (${OUROBOROS_LANG_PACKAGE_${LANGUAGE}} ${OUROBOROS_LANG_VERSION_${LANGUAGE}})
  if (NOT ${OUROBOROS_LANG_FOUND_${LANGUAGE}})
    message (FATAL_ERROR "Could not find ${OUROBOROS_LANG_LONGNAME_${LANGUAGE}}")
  endif (NOT ${OUROBOROS_LANG_FOUND_${LANGUAGE}})
  # Setup include directories
  include_directories (${${OUROBOROS_LANG_INCLUDE_DIR_${LANGUAGE}}})
  # Define generated source and provide it to the parent scope
  set (
    OUROBOROS_GENERATED_SRC
    "${OUTDIR}/${OUROBOROS_LANG_LONGNAME_${LANGUAGE}}_${MODULE_NAME}_wrap.cxx"
  )
  set (${GENERATED_SRC_VAR} ${OUROBOROS_GENERATED_SRC} PARENT_SCOPE)
  # Prepare list of include directories
  get_directory_property (OUROBOROS_INCLUDE_LIST INCLUDE_DIRECTORIES)
  set (OUROBOROS_INCLUDES)
  foreach (INCLUDE_DIR IN LIST OUROBOROS_INCLUDE_LIST)
    list (APPEND OUROBOROS_INCLUDES "-I${INCLUDE_DIR}")
  endforeach (INCLUDE_DIR)
  # Add custom target for generated source code
  set (OUROBOROS_SPECIFIC_EXECUTABLE opwig-${LANGUAGE})
  add_custom_command (
    OUTPUT  ${OUROBOROS_GENERATED_SRC}
    COMMAND ${OUROBOROS_SPECIFIC_EXECUTABLE}
    ARGS    --module-name=${MODULE_NAME}
            --output-dir=${OUTDIR}
            ${OUROBOROS_INCLUDES}
            ${ARGN}
    DEPENDS ${OUROBOROS_SPECIFIC_EXECUTABLE} ${ARGN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
  # Clean up variables
  unset (OUROBOROS_GENERATED_SRC)
  unset (OUROBOROS_SPECIFIC_EXECUTABLE)
  unset (OUROBOROS_INCLUDE_LIST)
  unset (OUROBOROS_INCLUDES)
endfunction (ouroboros_wrap_module)

