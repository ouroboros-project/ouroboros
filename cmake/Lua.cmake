
find_package (Lua51)

if (LUA51_FOUND)

  include_directories (${LUA_INCLUDE_DIR})
  LIST(APPEND OUROBOROS_SRC       ${OUROBOROS_LUA_SRC})
  LIST(APPEND OUROBOROS_LIBRARIES ${LUA_LIBRARIES})
  ouroboros_add_scriptlang (lua)
  
  set(OUROBOROS_LUA_ENABLED True)

  message ("-- Lua5.1 modules added.")
    
else (LUA51_FOUND)

  message ("Could NOT find Lua5.1. No Lua5.1 script modules will be compiled.")

endif (LUA51_FOUND)

