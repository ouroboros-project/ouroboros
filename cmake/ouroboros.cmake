
macro (ouroboros_setup_package package_name)
  include (cmake/${package_name}.cmake)
endmacro (ouroboros_setup_package package_name)

macro (ouroboros_add_langmodule name lib)
    list(APPEND OUROBOROS_LANGUAGES_LIST name)
    list(APPEND OUROBOROS_LIBRARIES lib)
endmacro (ouroboros_add_langmodule name lib)

set (OUROBOROS_LIBRARIES       "")
set (OUROBOROS_LIBRARIES_DEBUG "")
set (OUROBOROS_LANGUAGES_LIST "")

