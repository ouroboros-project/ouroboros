
#include <opwig/gen/lua/codes.h>

#include <sstream>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using std::stringstream;
using std::list;

string WrapList (const md::Ptr<Module>& module, WrappedMember member, const string& type) {
    const auto& wraps = (*module).*member;
    stringstream code;
    code  << "luaL_Reg "<< module->path+module->name << "_" << type << "s[] = {\n";
    for (auto wrap : wraps)
        code 
          << "    { \"" + wrap.name + "\", " + wrap.nesting + WrapName(type, wrap.name) + " },\n";
    code  << "    { nullptr, nullptr }\n"
             "};\n\n";
    return code.str();
}

string Utilities () {
    return
        " void OPWIG_Lua_MakeParentModule (State& L, const string& parent_name) {\n"
        "     // This function uses the table on top of the stack to check for the parent module.\n"
        "     // If it is not there, the function creates it.\n"
        "     L.getfield(-1, parent_name);\n"
        "     if (L.isnil(-1)) {\n"
        "         L.pop(1);\n"
        "         L.newtable();\n"
        "         L.pushvalue(-1);\n"
        "         L.setfield(-3, parent_name);\n"
        "     }\n"
        "     L.remove(-2);\n"
        " }\n\n"
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
        "int OPWIG_Lua_UniversalGetter (lua_State *L_) {\n"
        "    State L(L_);\n"
        "    L.pushvalue(lua_upvalueindex(1));\n"
        "    if (L.isnil(-1))\n"
        "       return luaL_error(L, \"Universal getter failed.\");\n"
        "    L.pushvalue(1);\n"
        "    L.gettable(2);\n"
        "    L.insert(1);\n"
        "    L.settop(1);\n"
        "    return 1;\n"
        "}\n\n";
}

} // namespace lua
} // namespace gen
} // namespace opwig

