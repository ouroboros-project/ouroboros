
# Parser stuff
set (
  OPWIG_MDPARER_GRAMMAR_SRC
  ${CMAKE_CURRENT_LIST_DIR}/parser.gr
)

set (
  OPWIG_MDPARSER_PERSISTENT_SRC
  ${CMAKE_CURRENT_LIST_DIR}/parser.h
  ${CMAKE_CURRENT_LIST_DIR}/parser.ih
)

set (
  OPWIG_MDPARSER_GENERATED_SRC
  ${CMAKE_CURRENT_LIST_DIR}/parserbase.h
  ${CMAKE_CURRENT_LIST_DIR}/parser-parse.cxx
)

add_custom_command (
  OUTPUT  ${OPWIG_MDPARSER_GENERATED_SRC}
  COMMAND bisonc++ ARGS --parsefun-source=parser-parse.cxx
                        --baseclass-header=parserbase.h
                        --class-header=parser.h
                        --implementation-header=parser.ih
                        #-V
                        parser.gr
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

# Lex stuff
set (
  OPWIG_MDSCANNER_LEX_SRC
  ${CMAKE_CURRENT_LIST_DIR}/scanner.lex
)

set (
  OPWIG_MDSCANNER_PERSISTENT_SRC
  ${CMAKE_CURRENT_LIST_DIR}/scanner.h
  ${CMAKE_CURRENT_LIST_DIR}/scanner.ih
)

set (
  OPWIG_MDSCANNER_GENERATED_SRC
  ${CMAKE_CURRENT_LIST_DIR}/scannerbase.h
  ${CMAKE_CURRENT_LIST_DIR}/scanner-lex.cxx
)

add_custom_command (
  OUTPUT  ${OPWIG_MDSCANNER_GENERATED_SRC}
  COMMAND flexc++ ARGS scanner.lex
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

