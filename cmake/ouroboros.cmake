
macro (ouroboros_setup_package package_name)
  include (cmake/${package_name}.cmake)
endmacro (ouroboros_setup_package package_name)

set (OUROBOROS_LIBRARIES       "")
set (OUROBOROS_LIBRARIES_DEBUG "")

