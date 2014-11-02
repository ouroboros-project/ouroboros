
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

#########
# OPA

add_library (libouroboros-lua ${OUROBOROS_LUA_OPA_SRC})
target_link_libraries (libouroboros-lua LINK_PUBLIC libouroboros ${LUA_LIBRARIES})
target_include_directories(libouroboros-lua PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>" ${LUA_INCLUDE_DIR})

set_target_properties (libouroboros-lua PROPERTIES 
    PROJECT_NAME OUROBOROS-LUA
    OUTPUT_NAME ouroboros-lua
    OUTPUT_NAME_DEBUG ouroboros-lua-dbg
    FOLDER "opa"
)

install(TARGETS libouroboros-lua
    EXPORT OuroborosLua
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

########
# OPWIG

add_library(libopwig-lua ${OUROBOROS_LUA_OPWIG_SRC})
target_link_libraries(libopwig-lua LINK_PUBLIC libopwig-common)
target_include_directories(libopwig-lua PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")

set_target_properties (libopwig-lua PROPERTIES 
    OUTPUT_NAME opwig-lua
    OUTPUT_NAME_DEBUG opwig-lua-dbg
    FOLDER "opwig"
)

ouroboros_generate_opwig (
  lua
  lua/opwig/wrapperspecification.h
  opa::lua::WrapperSpecification
  libopwig-lua
)

install(TARGETS opwig-lua
    EXPORT OuroborosLua
    RUNTIME DESTINATION bin
    CONFIGURATIONS Release
)

#install(DIRECTORY .
#    DESTINATION include/ouroboros
#    FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc"
#)

install(EXPORT OuroborosLua
    FILE OuroborosLuaTargets.cmake
    DESTINATION ${ConfigPackageLocation}
)
