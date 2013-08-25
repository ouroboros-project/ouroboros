
#include <opwig/gen/lua/codes.h>

#include <sstream>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using std::stringstream;
using std::list;

string WrapList (const md::Ptr<ModuleWrap>& module, WrappedMember member, const string& type) {
    const auto& wraps = (*module).*member;
    stringstream code;
    code  << "luaL_Reg "<< module->path+module->name << "_" << type << "s[] = {\n";
    for (auto wrap : wraps)
        code 
          << "    { "
              << "\"" + wrap.name + "\", "
              <<  wrap.nesting + GetWrapName(type, wrap.name)
          << " },\n";
    code  << "    { nullptr, nullptr }\n"
             "};\n\n";
    return code.str();
}

string MiddleBlockCode (const string& module_name) {
    return
        "#include <languages/lua/luawrapper.h>\n"
        "#include <languages/lua/converter.h>\n"
        "#include <languages/lua/header.h>\n"
        "#include <opa/scriptmanager.h>\n"
        "#include <opa/module.h>\n"
        "#include <opa/converter.h>\n"
        "#include <iostream>\n"
        "#include <string>\n"
        "#include <stdexcept>\n"
        "\n"
        "using std::string;\n"
        "using std::cout;\n"
        "using std::endl;\n"
        "using std::runtime_error;\n"
        "using opa::Module;\n"
        "using opa::lua::LuaWrapper;\n"
        "using opa::lua::State;\n"
        "using opa::lua::Constant;\n"
        "\n"
        "namespace {\n\n"
        "const char *MODULE_NAME = \""+module_name+"\";\n\n"
        "struct ModuleInfo {\n"
        "    luaL_Reg getters[];\n"
        "    luaL_Reg setters[];\n"
        "    luaL_Reg funcions[];\n"
        "};\n\n"
        "} // unnamed namespace\n\n";
}

string Utilities () {
    return
        "void OPWIG_Lua_MakeParentModule (State& L, const string& parent_name) {\n"
        "    // This function uses the table on top of the stack to check for the parent module.\n"
        "    // If it is not there, the function creates it.\n"
        "    L.getfield(-1, parent_name);\n"
        "    if (L.isnil(-1)) {\n"
        "        L.pop(1);\n"
        "        L.newtable();\n"
        "        L.pushvalue(-1);\n"
        "        L.setfield(-3, parent_name);\n"
        "    }\n"
        "    L.remove(-2);\n"
        "}\n\n"
        "void OPWIG_Lua_ExportSubmodule (State& L, const string& name, lua_CFunction init) {\n"
        "    // This function exports the given submodule into the table at the stack's top.\n"
        "    // Stack: [module]\n"
        "    L.pushcfunction(init);\n"
        "    L.pushprimitive(name);\n"
        "    L.pushvalue(1);\n"
        "    // Stack: [module, cfunction, string, module]\n"
        "    L.call(2, 1);\n"
        "    // Stack: [module, submodule]\n"
        "    L.setfield(1, name);\n"
        "}\n\n"
        "/// [-2,+1,-]\n"
        "int OPWIG_Lua_UniversalGetter (lua_State *L_) {\n"
        "    // Stack: [table, key]\n"
        "    State L(L_);\n"
        "    L.remove(1);\n"
        "    // Stack: [key]\n"
        "    L.pushvalue(lua_upvalueindex(1));\n"
        "    // Stack: [key, getters]\n"
        "    L.insert(1);\n"
        "    // Stack: [getters, key]\n"
        "    L.gettable(1);\n"
        "    // Stack: [getters, getter]\n"
        "    if (L.isnil(2)) {\n"
        "        L.settop(0);\n"
        "        L.pushnil();\n"
        "        return 1;\n"
        "    }\n"
        "    L.remove(1);\n"
        "    // Stack: [getter]\n"
        "    L.call(0, 1);\n"
        "    // Stack: [value]\n"
        "    return 1;\n"
        "}\n\n"
        "/// [-3,+1,-]\n"
        "int OPWIG_Lua_UniversalSetter (lua_State *L_) {\n"
        "    // Stack: [table, key, value]\n"
        "    State L(L_);\n"
        "    L.remove(1);\n"
        "    // Stack: [key, value]\n"
        "    L.pushvalue(lua_upvalueindex(1));\n"
        "    // Stack: [key, value, setters]\n"
        "    L.pushvalue(1);\n"
        "    // Stack: [key, value, setters, key]\n"
        "    L.gettable(3);\n"
        "    // Stack: [key, value, setters, setter]\n"
        "    if (L.isnil(4)) {\n"
        "        L.settop(0);\n"
        "        return luaL_error(\n"
        "            L,\n"
        "            \"Attempt to write to nonexistent variable.\"\n"
        "        );\n"
        "    }\n"
        "    L.remove(3);\n"
        "    // Stack: [key, value, setter]\n"
        "    L.insert(2);\n"
        "    // Stack: [key, setter, value]\n"
        "    L.call(1, 0);\n"
        "    // Stack: [key]\n"
        "    L.settop(0);\n"
        "    // Stack: []\n"
        "    return 0;\n"
        "}\n\n"
        "/// [-1,+1,-]\n"
        "void OPWIG_Lua_PrepareMetatable (State& L, luaL_Reg getters[], luaL_Reg setters[]) {\n"
        "    // Stack: [module]\n"
        "    L.newtable();\n"
        "    // Stack: [module, mttable]\n"
        "    L.pushvalue(-1);\n"
        "    // Stack: [module, mttable, mttable]\n"
        "    L.setmetatable(1);\n"
        "    // Stack: [module, mttable]\n"
        "    /* Getters */ {\n"
        "       L.newtable();\n"
        "       // Stack: [module, mttable, getters]\n"
        "       luaL_register(L, NULL, getters);\n"
        "       // Stack: [module, mttable, getters]\n"
        "       L.pushcfunction(OPWIG_Lua_UniversalGetter, 1);\n"
        "       // Stack: [module, mttable, __index]\n"
        "       L.setfield(2, \"__index\");\n"
        "    }\n"
        "    // Stack: [module, mttable]\n"
        "    /* Setters */ {\n"
        "       L.newtable();\n"
        "       // Stack: [module, mttable, setters]\n"
        "       luaL_register(L, NULL, setters);\n"
        "       // Stack: [module, mttable, setters]\n"
        "       L.pushcfunction(OPWIG_Lua_UniversalSetter, 1);\n"
        "       // Stack: [module, mttable, __newindex]\n"
        "       L.setfield(2, \"__newindex\");\n"
        "    }\n"
        "    // Stack: [module, mttable]\n"
        "    // Leave only the module table in the stack.\n"
        "    L.settop(1);\n"
        "    // Stack: [module]\n"
        "}\n\n";
}

} // namespace lua
} // namespace gen
} // namespace opwig

