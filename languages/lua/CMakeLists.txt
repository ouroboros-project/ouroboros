
SET(OUROBOROS_LUA_OPA_SRC 
    opa/auxlib.h 
    opa/converter.h 
    opa/converter.cxx 
    opa/basegear.cxx 
    opa/basegear.h 
    opa/bootstrapgear.cxx 
    opa/bootstrapgear.h 
    opa/datagear.cxx 
    opa/datagear.h 
    opa/defs.h 
    opa/header.h 
    opa/luadata.cxx 
    opa/luadata.h 
    opa/luamodules.cxx 
    opa/luamachine.cxx 
    opa/luamachine.h 
    opa/modules.h 
    opa/primitive.h 
    opa/state.h 
    
    opa/wrap/module.h
    opa/wrap/module.cxx
)

SET(OUROBOROS_LUA_OPWIG_SRC
    opwig/wrapperspecification.h
    opwig/wrapperspecification.cxx
    opwig/codes.h
    opwig/codes.cxx
    opwig/wraps.h
    opwig/wrapperstate.cxx
    opwig/wrapperstate.h
)

find_package (Lua51 REQUIRED)

message ("-- Lua module added.")

add_library (libouroboros-lua ${OUROBOROS_LUA_OPA_SRC})
target_include_directories(libouroboros-lua 
    PUBLIC
    ".."
    ${LUA_INCLUDE_DIR}
)

target_link_libraries (libouroboros-lua
    LINK_PUBLIC
    ${LUA_LIBRARIES}
    libouroboros
)

set_target_properties (libouroboros-lua PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME OUROBOROS-LUA
    OUTPUT_NAME ouroboros-lua${FRAMEWORK_BIGVERSION}
    OUTPUT_NAME_DEBUG ouroboros-lua${FRAMEWORK_BIGVERSION}-dbg
    SOVERSION ${FRAMEWORK_VERSION_PATCH}
    VERSION ${FRAMEWORK_VERSION}
)

#####

add_library(libopwig-lua ${OUROBOROS_LUA_OPWIG_SRC})
target_link_libraries(libopwig-lua LINK_PUBLIC libopwig-common)
target_include_directories(libopwig-lua PUBLIC "..")

ouroboros_generate_opwig (
  lua
  lua/opwig/wrapperspecification.h
  opa::lua::WrapperSpecification
  libopwig-lua
)

install(TARGETS libouroboros-lua
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY . DESTINATION include/ouroboros${FRAMEWORK_BIGVERSION} FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")
