
SET(OUROBOROS_LUA_SRC 
    ./auxlib.h 
    ./converter.h 
    ./converter.cxx 
    ./basegear.cxx 
    ./basegear.h 
    ./bootstrapgear.cxx 
    ./bootstrapgear.h 
    ./datagear.cxx 
    ./datagear.h 
    ./defs.h 
    ./header.h 
    ./luadata.cxx 
    ./luadata.h 
    ./luamodules.cxx 
    ./luamachine.cxx 
    ./luamachine.h 
    ./modules.h 
    ./primitive.h 
    ./state.h 

    ./wrapperspecification.h
    ./wrapperspecification.cxx
    ./codes.h
    ./codes.cxx
    ./wraps.h
    ./wrapperstate.cxx
    ./wrapperstate.h

    ./wrap/module.h
    ./wrap/module.cxx
)

find_package (Lua51)

include_directories (${LUA_INCLUDE_DIR})

message ("-- Lua module added.")

add_library (libouroboros-lua ${OUROBOROS_LUA_SRC})

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

ouroboros_add_langmodule (Lua libouroboros-lua)

ouroboros_generate_opwig (
  lua
  languages/lua/wrapperspecification.h
  opa::lua::WrapperSpecification
)

install(TARGETS libouroboros-lua
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY . DESTINATION include/ouroboros${FRAMEWORK_BIGVERSION} FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")
