
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

list (APPEND OUROBOROS_LANGS lua python)

function (ouroboros_wrap_module MODULE_NAME OUTDIR GENERATED_SRC_VAR)
  message (STATUS "Wrapping module ${MODULE_NAME}")
  message (STATUS "Outputing to ${GENERATED_SRC_VAR}")
  ## TODO: When opwig is separated, fix all this
  list (APPEND OUROBOROS_GENERATED_SRC)
  foreach (LANGUAGE IN LISTS OUROBOROS_LANGS)
    find_package (${OUROBOROS_LANG_PACKAGE_${LANGUAGE}} ${OUROBOROS_LANG_VERSION_${LANGUAGE}})
    if (NOT ${OUROBOROS_LANG_FOUND_${LANGUAGE}})
      message (FATAL_ERROR "Could not find ${OUROBOROS_LANG_LONGNAME_${LANGUAGE}}")
    endif (NOT ${OUROBOROS_LANG_FOUND_${LANGUAGE}})
    include_directories (${${OUROBOROS_LANG_INCLUDE_DIR_${LANGUAGE}}})
    list (
      APPEND
      OUROBOROS_GENERATED_SRC
      "${OUTDIR}/${OUROBOROS_LANG_LONGNAME_${LANGUAGE}}_${MODULE_NAME}_wrap.cxx"
    )
  endforeach(LANGUAGE)
  set (${GENERATED_SRC_VAR} ${OUROBOROS_GENERATED_SRC} PARENT_SCOPE)
  message (STATUS ${CMAKE_CURRENT_LIST_DIR})
  #add_custom_command (
  #  OUTPUT  ${OUROBOROS_GENERATED_SRC}
  #  COMMAND opwig
  #  ARGS    --module-name=${MODULE_NAME} ${ARGN}
  #  DEPENDS opwig ${ARGN}
  #  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  #)
endfunction (ouroboros_wrap_module)

