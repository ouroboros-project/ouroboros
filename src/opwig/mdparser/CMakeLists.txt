
cmake_minimum_required (VERSION 2.8)

set (
  OPWIG_GENERATED_SRC
  mdparserbase.h
  mdparser.h
  mdparser.ih
  mdparser-parse.cxx
)

add_custom_command (
  OUTPUT  ${OPWIG_GENERATED_SRC}
  COMMAND bisonc++ ARGS --parsefun-source=mdparser-parce.cxx
                        --baseclass-header=mdparserbase.h
                        --class-header=mdparser.h
                        --implementation-header=mdparser.ih
                        mdparser.gr
  DEPENDS mdparser.gr
  #WORKING_DIRECTORY .
)

set_source_files_properties (${OPWIG_GENERATED_SRC} PROPERTIES GENERATED 1)

