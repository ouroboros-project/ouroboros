
cmake_minimum_required(VERSION 2.8)

include_directories("src")

set (MILESTONE_00_EXECUTABLE milestone-00)

set (
  MILESTONE_00_SRC
  src/main.cxx
  src/prompt.h
  src/prompt.cxx
)

set (
  MILESTONE_00_GENERATED_SRC
  src/Lua_prompt_wrap.cxx
  src/Python_prompt_wrap.cxx
)

find_package (PythonLibs 2.6)
include_directories (${PYTHON_INCLUDE_DIRS})

find_package (Lua51)
include_directories (${LUA_INCLUDE_DIR})

add_custom_command (
  OUTPUT ${MILESTONE_00_GENERATED_SRC}
  COMMAND opwig
  ARGS    --module-name=prompt prompt.h
  DEPENDS src/prompt.h opwig
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src
)

set_source_files_properties (
  ${MILESTONE_00_GENERATED_SRC}
  PROPERTIES
  GENERATED 1
)

add_executable (
  ${MILESTONE_00_EXECUTABLE}
  EXCLUDE_FROM_ALL
  ${MILESTONE_00_SRC}
  ${MILESTONE_00_GENERATED_SRC}
)

target_link_libraries (
  ${MILESTONE_00_EXECUTABLE}
  libouroboros
  libouroboros-lua
  libouroboros-python
)

#configure_file (
#  "src/config.h.in"
#  "src/config.h"
#)

