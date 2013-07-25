
#include <opwig/gen/lua/wrapperspecification.h>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using md::Ptr;

Ptr<ConverterProvider> WrapperSpecification::GetConverterProvider () const {
    return Ptr<ConverterProvider>();
}

string WrapperSpecification::FileHeader () const {
    return
        "#include <lua5.1/lauxlib.h>\n"
        "#include <iostream>\n"
        "using std::cout;\n"
        "using std::endl;\n";
}

string WrapperSpecification::FinishFile () const {
    return
        "namespace {\n\n"
        "luaL_Reg wrapped_functions[] = {\n"
        "    { NULL, NULL }\n"
        "};\n\n"
        "class Bootstrap final {\n"
        "  public:\n"
        "    Bootstrap ();\n"
        "};\n\n"
        "Bootstrap entry_point;\n\n"
        "} // unnamed namespace\n\n"
        "extern \"C\" {\n\n"
        "/// [-1,+1,e]\n"
        "int luaopen_"+module_name_+" (lua_State* L) {\n"
        "    const char *mod_name = luaL_checkstring(L, 1);\n"
        "    lua_settop(L, 0);\n"
        "    luaL_register(L, mod_name, wrapped_functions);\n"
        "    lua_setfield(L, LUA_GLOBALSINDEX, \""+module_name_+"\");\n"
        "    return 1;\n"
        "}\n\n"
        "} // extern \"C\"\n\n"
        "namespace {\n\n"
        "Bootstrap::Bootstrap () {\n"
        "    cout << \"Bootstrapping Lua module \\\""+module_name_+"\\\"\" << endl;\n"
        "}\n\n"
        "} // unnamed namespace\n";
}

string WrapperSpecification::WrapFunction(const md::Ptr<const md::Function>& obj) {
    return "";
}

string WrapperSpecification::WrapVariable(const md::Ptr<const md::Variable>& obj) {
    return "";
}

string WrapperSpecification::WrapClass(const md::Ptr<const md::Class>& obj) {
    return "";
}

string WrapperSpecification::WrapNamespace(const md::Ptr<const md::Namespace>& obj) {
    return "";
}

string WrapperSpecification::WrapEnum(const md::Ptr<const md::Enum>& obj) {
    return "";
}

} // namespace lua
} // namespace gen
} // namespace opwig

