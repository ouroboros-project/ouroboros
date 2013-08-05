
cmake_minimum_required(VERSION 2.8)

include_directories("src")

set (MILESTONE_EXECUTABLE milestone-01)

set (
  MILESTONE_SRC
  src/main.cxx
  src/out.h
  src/out.cxx
  src/input.h
  src/input.cxx
)

set (
  MILESTONE_GENERATED_SRC
  src/Lua_prompt_wrap.cxx
  src/Python_prompt_wrap.cxx
)

find_package (PythonLibs 2.6)
include_directories (${PYTHON_INCLUDE_DIRS})

find_package (Lua51)
include_directories (${LUA_INCLUDE_DIR})

add_custom_command (
  OUTPUT ${MILESTONE_GENERATED_SRC}
  COMMAND opwig
  ARGS    --module-name=prompt input.h out.h
  DEPENDS src/input.h src/out.h opwig
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src
)

set_source_files_properties (
  ${MILESTONE_GENERATED_SRC}
  PROPERTIES
  GENERATED 1
)

add_executable (
  ${MILESTONE_EXECUTABLE}
  EXCLUDE_FROM_ALL
  ${MILESTONE_SRC}
  ${MILESTONE_GENERATED_SRC}
)

target_link_libraries (
  ${MILESTONE_EXECUTABLE}
  libouroboros
  libouroboros-lua
  libouroboros-python
)

#configure_file (
#  "src/config.h.in"
#  "src/config.h"
#)

