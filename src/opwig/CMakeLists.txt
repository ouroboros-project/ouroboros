
cmake_minimum_required (VERSION 2.8)

include (parser/generate.cmake)

set (
  OPWIG_SRC
  ${CMAKE_CURRENT_BINARY_DIR}/md/accessspecifier.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/container.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/scope.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/namespace.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/md/namespace.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/variable.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/class.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/md/class.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/semanticerror.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/function.h
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

