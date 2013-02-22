
SET(OUROBOROS_LUA_SRC 
    ./src/languages/lua/auxlib.h 
    ./src/languages/lua/basegear.cxx 
    ./src/languages/lua/basegear.h 
    ./src/languages/lua/bootstrapgear.cxx 
    ./src/languages/lua/bootstrapgear.h 
    ./src/languages/lua/datagear.cxx 
    ./src/languages/lua/datagear.h 
    ./src/languages/lua/defs.h 
    ./src/languages/lua/header.h 
    ./src/languages/lua/luadata.cxx 
    ./src/languages/lua/luadata.h 
    ./src/languages/lua/luamodules.cxx 
    ./src/languages/lua/luawrapper.cxx 
    ./src/languages/lua/luawrapper.h 
    ./src/languages/lua/modules.h 
    ./src/languages/lua/primitive.h 
    ./src/languages/lua/state.h 
    ./src/languages/lua/swigluarun.h 
)

find_package (Lua51)

include_directories (${LUA_INCLUDE_DIR})
ouroboros_add_scriptlang (lua)

message ("-- Lua module added.")

add_library (libouroboros-lua ${OUROBOROS_LUA_SRC})

target_link_libraries (libouroboros-lua  ${LUA_LIBRARIES})

target_link_libraries (libouroboros-lua LINK_INTERFACE_LIBRARIES ${LUA_LIBRARIES})

ouroboros_add_langmodule(Lua libouroboros-lua)
