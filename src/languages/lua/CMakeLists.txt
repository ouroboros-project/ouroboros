
SET(OUROBOROS_LUA_SRC 
    ./auxlib.h 
    ./converter.h 
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
    ./luawrapper.cxx 
    ./luawrapper.h 
    ./modules.h 
    ./primitive.h 
    ./state.h 
    ./swigluarun.h 
)

find_package (Lua51)

include_directories (${LUA_INCLUDE_DIR})
ouroboros_add_scriptlang (lua)

message ("-- Lua module added.")

add_library (libouroboros-lua ${OUROBOROS_LUA_SRC})

target_link_libraries (libouroboros-lua ${LUA_LIBRARIES})

target_link_libraries (libouroboros-lua LINK_INTERFACE_LIBRARIES ${LUA_LIBRARIES})

target_link_libraries (libouroboros-lua libouroboros)

target_link_libraries (libouroboros-lua LINK_INTERFACE_LIBRARIES libouroboros)

set_target_properties (libouroboros-lua PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME OUROBOROS-LUA
    OUTPUT_NAME ouroboros-lua${FRAMEWORK_BIGVERSION}
    OUTPUT_NAME_DEBUG ouroboros-lua${FRAMEWORK_BIGVERSION}-dbg
    SOVERSION ${FRAMEWORK_VERSION_PATCH}
    VERSION ${FRAMEWORK_VERSION}
)

ouroboros_add_langmodule(Lua libouroboros-lua)

install(TARGETS libouroboros-lua
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY . DESTINATION include/ouroboros${FRAMEWORK_BIGVERSION} FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")
