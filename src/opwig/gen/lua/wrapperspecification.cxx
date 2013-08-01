
#include <opwig/gen/lua/wrapperspecification.h>
#include <opwig/md/function.h>
#include <sstream>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using std::stringstream;
using md::Ptr;

string WrapperSpecification::FileHeader () const {
    return
        "\n"
        "// This is a generated file.\n\n"
        "#include <languages/lua/luawrapper.h>\n"
        "#include <languages/lua/converter.h>\n"
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

string WrapperSpecification::MiddleBlock() const {
    /***********
        WIL, OLHA BEEEIIIIN
    ************/
    return "";
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
        "    { nullptr, nullptr }\n"
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
        "} // extern \"C\"\n\n";
}

string WrapperSpecification::WrapFunction(const md::Ptr<const md::Function>& obj) {
    wrapped_functions_.push_back(obj->name());
    stringstream func_code, call_code;
    func_code << "int OPWIG_wrap_" << obj->name() << " (lua_State* L) {\n"
              << "    opa::lua::Converter convert(L);\n";
    call_code << obj->name() << "(";
    for (size_t i = 0; i < obj->num_parameters(); ++i) {
        string type = obj->parameter_type(i);
        func_code << "    " << type << " arg_" << i
                            << " = convert.ScriptToType<" << type << ">(" << (i+1) << ");\n";
        call_code << "arg_" << i;
        if (i+1 < obj->num_parameters())
            call_code << ", ";
    }
    call_code << ")";
    func_code << "    " << obj->return_type() << " result = " << call_code.str() << ";\n";
    func_code << "    int stack = convert.TypeToScript<" << obj->return_type() << ">(result);\n";
    func_code << "    return stack;\n"
              << "}\n\n";

    return func_code.str();
}

string WrapperSpecification::WrapVariable(const md::Ptr<const md::Variable>& obj) {
    return "";
}

string WrapperSpecification::WrapClass(const md::Ptr<const md::Class>& obj) {
    return "";
}

string WrapperSpecification::WrapNamespace(const md::Ptr<const md::Namespace>& obj, bool closing) {
    return "";
}

string WrapperSpecification::WrapEnum(const md::Ptr<const md::Enum>& obj) {
    return "";
}

} // namespace lua
} // namespace gen
} // namespace opwig

