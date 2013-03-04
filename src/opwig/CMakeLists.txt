
cmake_minimum_required (VERSION 2.8)

add_subdirectory (mdparser/)

add_executable (
  opwig
  main.cxx
  mdparser/mdparserbase.h
  mdparser/mdparser.h
  mdparser/mdparser.ih
  mdparser/mdparser-parse.cxx
)

