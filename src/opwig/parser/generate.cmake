
# Parser stuff
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
                        #-V
                        #-t
                        #--error-verbose
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
  ${CMAKE_CURRENT_LIST_DIR}/declarator.h
)

# Lex stuff
set (
  OPWIG_MDSCANNER_LEX_SRC
  ${CMAKE_CURRENT_LIST_DIR}/mdscanner.lex
)

set (
  OPWIG_MDSCANNER_PERSISTENT_SRC
  ${CMAKE_CURRENT_LIST_DIR}/mdscanner.h
  ${CMAKE_CURRENT_LIST_DIR}/mdscanner.ih
)

set (
  OPWIG_MDSCANNER_GENERATED_SRC
  ${CMAKE_CURRENT_LIST_DIR}/mdscannerbase.h
  ${CMAKE_CURRENT_LIST_DIR}/mdscanner-lex.cxx
)

add_custom_command (
  OUTPUT  ${OPWIG_MDSCANNER_GENERATED_SRC}
  COMMAND flexc++ ARGS mdscanner.lex
  DEPENDS ${OPWIG_MDSCANNER_LEX_SRC} ${OPWIG_MDPARSER_GENERATED_SRC}
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

set_source_files_properties (
  ${OPWIG_MDSCANNER_GENERATED_SRC}
  PROPERTIES
  GENERATED 1
)

set (
  OPWIG_MDSCANNER_SRC
  ${OPWIG_MDSCANNER_PERSISTENT_SRC}
  ${OPWIG_MDSCANNER_GENERATED_SRC}
)

