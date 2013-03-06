
set (
  OPWIG_MDPARER_GRAMMAR_SRC
  ${CMAKE_CURRENT_LIST_DIR}/mdparser.gr
)

set (
  OPWIG_MDPARSER_PERSISTENT_SRC
  ${CMAKE_CURRENT_LIST_DIR}/mdparser.h
  ${CMAKE_CURRENT_LIST_DIR}/mdparser.ih
)

set (
  OPWIG_MDPARSER_GENERATED_SRC
  ${CMAKE_CURRENT_LIST_DIR}/mdparserbase.h
  ${CMAKE_CURRENT_LIST_DIR}/mdparser-parse.cxx
)

add_custom_command (
  OUTPUT  ${OPWIG_MDPARSER_GENERATED_SRC}
  COMMAND bisonc++ ARGS --parsefun-source=mdparser-parse.cxx
                        --baseclass-header=mdparserbase.h
                        --class-header=mdparser.h
                        --implementation-header=mdparser.ih
                        mdparser.gr
  DEPENDS ${OPWIG_MDPARER_GRAMMAR_SRC}
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

set_source_files_properties (
  ${OPWIG_MDPARSER_GENERATED_SRC}
  PROPERTIES
  GENERATED 1
)

set (
  OPWIG_MDPARSER_SRC
  ${OPWIG_MDPARSER_PERSISTENT_SRC}
  ${OPWIG_MDPARSER_GENERATED_SRC}
)

