
cmake_minimum_required (VERSION 2.8)

include (mdparser/src.cmake)

add_executable (
  opwig
  main.cxx
  ${OPWIG_MDPARSER_SRC}
)

