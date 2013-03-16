
cmake_minimum_required (VERSION 2.8)

include (mdparser/generate.cmake)

set (
  OPWIG_SRC
  ${CMAKE_CURRENT_BINARY_DIR}/mdnamespace.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/mdnamespace.h
  ${OPWIG_MDPARSER_SRC}
  ${OPWIG_MDSCANNER_SRC}
)

add_executable (
  opwig
  main.cxx
  ${OPWIG_SRC}
)

# Export to parent scope
set (OPWIG_SRC ${OPWIG_SRC} PARENT_SCOPE)

