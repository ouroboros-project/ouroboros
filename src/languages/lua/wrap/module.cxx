
#include <languages/lua/wrap/module.h>

#include <iostream>

namespace opa {
namespace lua {
namespace wrap {

using std::string;

namespace {

/// This function uses the table on top of the stack to check for the parent module.
/** If it is not there, the function creates it. Recursive. 
 */
void MakeParentModule (State& L, const ModuleInfo* parent) {
    if (!parent) return;
    MakeParentModule(L, parent->parent());
    L.getfield(-1, parent->name());
    if (L.isnil(-1)) {
        L.pop(1);
        L.newtable();
        L.pushvalue(-1);
        L.setfield(-3, parent->name());
    }
    L.remove(-2);
}

/// This function exports the given submodule into the table at the stack's top.
/**
 */
void ExportSubmodule (State& L, const string& name, lua_CFunction init) {
    // Stack: [module]
    L.pushcfunction(init);
    L.pushprimitive(name);
    L.pushvalue(1);
    // Stack: [module, cfunction, string, module]
    L.call(2, 1);
    // Stack: [module, submodule]
    L.setfield(1, name);
}

/// [-2,+1,-]
int UniversalGetter (lua_State *L_) {
    // Stack: [table, key]
    State L(L_);
    L.remove(1);
    // Stack: [key]
    L.pushvalue(lua_upvalueindex(1));
    // Stack: [key, getters]
    L.insert(1);
    // Stack: [getters, key]
    L.gettable(1);
    // Stack: [getters, getter]
    if (L.isnil(2)) {
        L.settop(0);
        L.pushnil();
        return 1;
    }
    L.remove(1);
    // Stack: [getter]
    L.call(0, 1);
    // Stack: [value]
    return 1;
}

/// [-3,+1,-]
int UniversalSetter (lua_State *L_) {
    // Stack: [table, key, value]
    State L(L_);
    L.remove(1);
    // Stack: [key, value]
    L.pushvalue(lua_upvalueindex(1));
    // Stack: [key, value, setters]
    L.pushvalue(1);
    // Stack: [key, value, setters, key]
    L.gettable(3);
    // Stack: [key, value, setters, setter]
    if (L.isnil(4)) {
        L.settop(0);
        return luaL_error(
            L,
            "Attempt to write to nonexistent variable or field."
        );
    }
    L.remove(3);
    // Stack: [key, value, setter]
    L.insert(2);
    // Stack: [key, setter, value]
    L.call(1, 0);
    // Stack: [key]
    L.settop(0);
    // Stack: []
    return 0;
}

/// [-2,+1,-]
int UniversalMemberGetter (lua_State *L_) {
    // Stack: [obj, key]
    State L(L_);
    L.pushvalue(lua_upvalueindex(1));
    // Stack: [obj, key, getters]
    L.pushvalue(2);
    // Stack: [obj, key, getters, key]
    L.gettable(3);
    // Stack: [obj, key, getters, getter]
    if (!L.isnil(4)) {
        // Found an attributre.
        L.insert(1);
        // Stack: [getter, obj, ...]
        L.settop(2);
        // Stack: [getter, obj]
        L.call(1, 1);
        // Stack: [value]
        return 1;
    } else {
        // It wasn't an attribute, try a method.
        L.settop(2);
        // Stack: [obj, key]
        L.pushvalue(lua_upvalueindex(2));
        // Stack: [obj, key, methods]
        L.pushvalue(2);
        // Stack: [obj, key, methods, key]
        L.gettable(3);
        // Stack: [obj, key, methods, method]
        if (L.isnil(4)) {
            // It wasn't a method either. Return nil.
            L.settop(0);
            L.pushnil();
            return 1;
        }
        L.insert(1);
        // Stack: [method, ...]
        L.settop(1);
        // Stack: [method]
        return 1;
    }
}

/// [-3,+1,-]
int UniversalMemberSetter (lua_State *L_) {
    // Stack: [obj, key, value]
    State L(L_);
    // Stack: [obj, key, value]
    L.pushvalue(lua_upvalueindex(1));
    // Stack: [obj, key, value, setters]
    L.pushvalue(2);
    // Stack: [obj, key, value, setters, key]
    L.gettable(4);
    // Stack: [obj, key, value, setters, setter]
    if (L.isnil(5)) {
        { // Avoiding memory leak...
          string attr_name = L.tostring(2);
          L.settop(0);
          lua_pushfstring(
              L,
              "Attempt to write to nonexistent attribute '%s'.",
              attr_name.c_str()
          );
        }
        return lua_error(L);
    }
    L.insert(1);
    // Stack: [setter, obj, key, value, ...]
    L.remove(3);
    // Stack: [setter, obj, value, ...]
    L.settop(3);
    // Stack: [setter, obj, value]
    L.call(2, 0);
    // Stack: []
    L.settop(0); // Yes, I'm neurotic.
    // Stack: []
    return 0;
}

/// [-1,+1,-]
void PrepareObjMetatable (State& L, const ModuleInfo* info) {
    // Stack: [module, mttable]
    L.newtable();
    // Stack: [module, mttable, vtable]
    /* Getters and methods */ {
        L.newtable();
        // Stack: [module, mttable, vtable, getters]
        luaL_register(L, NULL, info->member_getters());
        L.newtable();
        // Stack: [module, mttable, vtable, getters, methods]
        luaL_register(L, NULL, info->member_functions());
        // Stack: [module, mttable, vtable, getters, methods]
        L.pushcfunction(UniversalMemberGetter, 2);
        // Stack: [module, mttable, vtable, __index]
        L.setfield(3, "__index");
    }
    // Stack: [module, mttable, vtable]
    /* Setters */ {
        L.newtable();
        // Stack: [module, mttable, vtable, setters]
        luaL_register(L, NULL, info->member_setters());
        // Stack: [module, mttable, vtable, setters]
        L.pushcfunction(UniversalMemberSetter, 1);
        // Stack: [module, mttable, vtable, __newindex]
        L.setfield(3, "__newindex");
    }
    // Stack: [module, mttable, vtable]
    /* Destructor */ {
        L.pushcfunction(info->destructor());
        // Stack: [module, mttable, vtable, __gc]
        L.setfield(3, "__gc");
    }
    // Stack: [module, mttable, vtable]
    L.setfield(2, "__vtable");
    // Stack: [module, mttable]
}

/// [-1,+1,-]
void PrepareMetatable (State& L, const ModuleInfo* info) {
    // Stack: [module]
    L.newtable();
    // Stack: [module, mttable]
    /* Getters */ {
        L.newtable();
        // Stack: [module, mttable, getters]
        luaL_register(L, NULL, info->getters());
        // Stack: [module, mttable, getters]
        L.pushcfunction(UniversalGetter, 1);
        // Stack: [module, mttable, __index]
        L.setfield(2, "__index");
    }
    // Stack: [module, mttable]
    /* Setters */ {
        L.newtable();
        // Stack: [module, mttable, setters]
        luaL_register(L, NULL, info->setters());
        // Stack: [module, mttable, setters]
        L.pushcfunction(UniversalSetter, 1);
        // Stack: [module, mttable, __newindex]
        L.setfield(2, "__newindex");
    }
    if (info->is_class()) {
        // Stack: [module, mttable]
        if (info->constructor()) {
            L.pushcfunction(info->constructor());
            // Stack: [module, mttable, __call]
            L.setfield(2, "__call");
        }
        // Stack: [module, mttable]
        PrepareObjMetatable(L, info);
    }
    // Stack: [module, mttable]
    L.setmetatable(1);
    // Stack: [module]
}

} // unnamed namespace

int ExportModule (State&& L, const ModuleInfo* info) {
    if (L.gettop() > 1) {
        L.remove(1);
        L.settop(1);
    } else {
        L.settop(0);
        L.pushvalue(Constant::GLOBALSINDEX());
        MakeParentModule(L, info->parent());
    }
    // Stack: [nesting-table]
    L.newtable();
    L.pushvalue(-1);
    // Stack: [nesting-table, module, module]
    L.setfield(1, info->name());
    // Leave only the module table in the stack
    L.remove(1);
    L.settop(1);
    // Stack: [module]
    // Register module's submodules.
    for (auto submodule_info : info->children())
        ExportSubmodule(L, submodule_info->name(), submodule_info->init_function());
    // Stack: [module]
    // Register module's functions.
    luaL_register(L, NULL, info->functions());
    // Stack: [module];
    // Set module metatable.
    PrepareMetatable(L, info);
    // Return de module itself
    return 1;
}

} // namespace wrap
} // namespace lua
} // namespace opa

