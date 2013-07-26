
#include <opwig/gen/lua/wrapperspecification.h>
#include <opwig/md/function.h>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using md::Ptr;

string WrapperSpecification::FileHeader () const {
    return
        "\n"
        "// This is a generated file.\n\n"
        "#include <languages/lua/luawrapper.h>\n"
        "#include <opa/scriptmanager.h>\n"
        "#include <opa/module.h>\n"
        "#include <lua5.1/lauxlib.h>\n"
        "#include <iostream>\n"
        "#include <string>\n"
        "\n"
        "using std::string;\n"
        "using std::cout;\n"
        "using std::endl;\n"
        "using opa::Module;\n"
        "using opa::lua::LuaWrapper;\n"
        "\n"
        "namespace {\n\n"
        "const char *MODULE_NAME = \""+module_name_+"\";\n\n"
        "} // unnamed namespace\n\n";
}

string WrapperSpecification::FinishFile () const {
    string functions_wrap_code =
        "namespace {\n\n"
        "// List of wrapped functions\n"
        "luaL_Reg wrapped_functions[] = {\n";
    for (auto func_name : wrapped_functions_)
        functions_wrap_code +=
            "    { \""+func_name+"\", OPWIG_wrap_"+func_name+" },\n";
    functions_wrap_code +=
        "    { NULL, NULL }\n"
        "};\n\n"
        "} // unnamed namespace\n\n";
    return
        functions_wrap_code+
        // Loader function
        "extern \"C\" {\n\n"
        "/// [-1,+1,e]\n"
        "int luaopen_"+module_name_+" (lua_State* L) {\n"
        "    const char *mod_name = luaL_checkstring(L, 1);\n"
        "    lua_settop(L, 0);\n"
        "    luaL_register(L, mod_name, wrapped_functions);\n"
        "    lua_setfield(L, LUA_GLOBALSINDEX, MODULE_NAME);\n"
        "    return 1;\n"
        "}\n\n"
        "} // extern \"C\"\n\n"
        // Bootstrap implementation
        "namespace {\n\n"
        // Bootstrap class
        "class Bootstrap final {\n"
        "  public:\n"
        "    Bootstrap ();\n"
        "};\n\n"
        // Bootstrap object
        "Bootstrap entry_point;\n\n"
        "Bootstrap::Bootstrap () {\n"
        "    cout << \"Bootstrapping Lua module \\\"\" << MODULE_NAME << \"\\\"\" << endl;\n"
        "    LuaWrapper *wrapper = dynamic_cast<LuaWrapper*>(\n"
        "        SCRIPT_MANAGER()->GetWrapper(\"Lua\")\n"
        "    );\n"
        "    if (wrapper == NULL) {\n"
        "        wrapper = new LuaWrapper;\n"
        "        SCRIPT_MANAGER()->Register(wrapper);\n"
        "    }\n"
        "    wrapper->RegisterModule("
                "Module<int(*)(lua_State*)>(MODULE_NAME, luaopen_"+module_name_+")"
             ");\n"
        "}\n\n"
        "} // unnamed namespace\n";
}

string WrapperSpecification::WrapFunction(const md::Ptr<const md::Function>& obj) {
    wrapped_functions_.push_back(obj->name());
    return
      "int OPWIG_wrap_"+obj->name()+" (lua_State* L) {\n"
      "    return 0;\n"
      "}\n\n";
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

