
SET(OUROBOROS_OPA_SRC 
    script.h 
    baseproxy.h 
    converter.h
    defs.h
    exceptions.h
    initscripts.cxx 
    virtualmachine.h 
    module.h 
    scriptmanager.cxx 
    scriptmanager.h 
    type.h
    type/conversionregistry.h
    type/conversionregistry.cxx
    virtualdata.h 
    virtualobj.cxx 
    virtualobj.h 
    virtualprimitive.h 
    utils/idgenerator.cxx 
    utils/idgenerator.h 
    utils/uncopyable.h
)

CONFIGURE_FILE(
  config.h.in
  config.h
)

add_library (libouroboros ${OUROBOROS_OPA_SRC})
                     
set_target_properties (libouroboros PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME OUROBOROS
    OUTPUT_NAME ouroboros${OUROBOROS_BIGVERSION}
    OUTPUT_NAME_DEBUG ouroboros${OUROBOROS_BIGVERSION}-dbg
    SOVERSION ${OUROBOROS_VERSION_PATCH}
    VERSION ${OUROBOROS_VERSION}
)

install(TARGETS libouroboros
    DESTINATION lib
#    CONFIGURATIONS Release
)
install(
  DIRECTORY   src/ouroboros
  DESTINATION include/ouroboros${OUROBOROS_BIGVERSION}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc"
)

# Export relevant variables to parent scope in case add_subdirectory has been
# used.

if (NOT ${CMAKE_HOME_DIRECTORY} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
  set (OUROBOROS_INCLUDE_DIR ${OUROBOROS_SRC_DIR} PARENT_SCOPE)
  set (
    OUROBOROS_WRAP_HELPER
    ${CMAKE_CURRENT_LIST_DIR}/cmake/wrap_helper.cmake
    PARENT_SCOPE
  )
endif ()