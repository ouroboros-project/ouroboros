
cmake_minimum_required (VERSION 2.8)

set (
  OPWIG_GENERATED_SRC
  ${CMAKE_CURRENT_BINARY_DIR}/mdparserbase.h
  ${CMAKE_CURRENT_BINARY_DIR}/mdparser.h
  ${CMAKE_CURRENT_BINARY_DIR}/mdparser.ih
  ${CMAKE_CURRENT_BINARY_DIR}/mdparser-parse.cxx
  PARENT_SCOPE
)

#add_custom_command (
#  OUTPUT  ${OPWIG_GENERATED_SRC}
#  COMMAND bisonc++ ARGS --parsefun-source=mdparser-parce.cxx
#                        --baseclass-header=mdparserbase.h
#                        --class-header=mdparser.h
#                        --implementation-header=mdparser.ih
#                        mdparser.gr
#  DEPENDS mdparser.gr
#  #WORKING_DIRECTORY .
#)

