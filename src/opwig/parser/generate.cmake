
# Parser stuff
set (
  OPWIG_MDPARSER_GRAMMAR_SRC
  ${CMAKE_CURRENT_LIST_DIR}/mdparser.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/class_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/declaration_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/declarator_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/expression_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/identifier_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/member_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/namespace_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/semantictypes.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/toplevel_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/type_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/enum_rules.gr
  ${CMAKE_CURRENT_LIST_DIR}/grammar/function_rules.gr
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

if(NOT WIN32)
  add_custom_command (
    OUTPUT  ${OPWIG_MDPARSER_GENERATED_SRC}
    COMMAND bisonc++ ARGS --parsefun-source=mdparser-parse.cxx
                          --baseclass-header=mdparserbase.h
                          --class-header=mdparser.h
                          --implementation-header=mdparser.ih
                          -V
                          #-t
                          #--error-verbose
                          mdparser.gr
    DEPENDS ${OPWIG_MDPARSER_GRAMMAR_SRC}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif(NOT WIN32)
if(MSVC)
  FILE(READ ${CMAKE_CURRENT_LIST_DIR}/mdparserbase.h FILE_CONTENT)
  STRING(REPLACE 
          "SType(SType &&tmp) = default;"
          "SType(SType &&tmp) : std::shared_ptr<Base>(std::move(static_cast<std::shared_ptr<Base>&&>(tmp))) {}"
          FILE_MOD "${FILE_CONTENT}")
  STRING(REPLACE 
          "SType &operator=(SType &&tmp) = default;"
          "SType &operator=(SType &&tmp) { *static_cast<std::shared_ptr<Base>*>(this) = std::move(static_cast<std::shared_ptr<Base>&&>(tmp)); return *this; }"
          FILE_OUT "${FILE_MOD}")
  FILE(WRITE ${CMAKE_CURRENT_LIST_DIR}/mdparserbase.h "${FILE_OUT}")
  UNSET(FILE_CONTENT)
  UNSET(FILE_MOD)
  UNSET(FILE_OUT)
endif(MSVC)

set_source_files_properties (
  ${OPWIG_MDPARSER_GENERATED_SRC}
  PROPERTIES
  GENERATED 1
)

set (
  OPWIG_MDPARSER_SRC
  ${OPWIG_MDPARSER_PERSISTENT_SRC}
  ${OPWIG_MDPARSER_GENERATED_SRC}
  ${CMAKE_CURRENT_LIST_DIR}/semantictypes.h
  ${CMAKE_CURRENT_LIST_DIR}/declarator.h
  ${CMAKE_CURRENT_LIST_DIR}/declspecifier.h
  ${CMAKE_CURRENT_LIST_DIR}/basespecifier.h
  ${CMAKE_CURRENT_LIST_DIR}/utilities.h
  ${CMAKE_CURRENT_LIST_DIR}/utilities.cxx
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

if(NOT WIN32)
  add_custom_command (
    OUTPUT  ${OPWIG_MDSCANNER_GENERATED_SRC}
    COMMAND flexc++ ARGS mdscanner.lex
    DEPENDS ${OPWIG_MDSCANNER_LEX_SRC} ${OPWIG_MDPARSER_GENERATED_SRC}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif(NOT WIN32)

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

