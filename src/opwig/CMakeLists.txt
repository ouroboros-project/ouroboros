
cmake_minimum_required (VERSION 2.8)

include (mdparser/generate.cmake)

add_executable (
  opwig
  main.cxx
  ${OPWIG_MDPARSER_SRC}
  ${OPWIG_MDSCANNER_SRC}
)

