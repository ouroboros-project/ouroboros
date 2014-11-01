
SET(OUROBOROS_OPA_SRC 
    src/opa/script.h 
    src/opa/baseproxy.h 
    src/opa/converter.h
    src/opa/defs.h
    src/opa/exceptions.h
    src/opa/virtualmachine.h 
    src/opa/module.h 
    src/opa/scriptmanager.cxx 
    src/opa/scriptmanager.h 
    src/opa/type.h
    src/opa/type/conversionregistry.h
    src/opa/type/conversionregistry.cxx
    src/opa/virtualdata.h 
    src/opa/virtualobj.cxx 
    src/opa/virtualobj.h 
    src/opa/virtualprimitive.h 
    src/opa/utils/idgenerator.cxx 
    src/opa/utils/idgenerator.h 
    src/opa/utils/uncopyable.h
)

CONFIGURE_FILE(
  src/opa/config.h.in
  src/opa/config.h
)

add_library (libouroboros ${OUROBOROS_OPA_SRC})
target_include_directories(libouroboros PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>"
)
                     
set_target_properties (libouroboros PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME OUROBOROS
    OUTPUT_NAME ouroboros
    OUTPUT_NAME_DEBUG ouroboros-dbg
    FOLDER "opa"
)

install(TARGETS libouroboros
    EXPORT Ouroboros
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(
  DIRECTORY   src/opa/ouroboros
  DESTINATION include/ouroboros
  FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc"
)
