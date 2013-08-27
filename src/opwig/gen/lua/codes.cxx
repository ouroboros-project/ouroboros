
#include <opwig/gen/lua/codes.h>

#include <sstream>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using std::stringstream;
using std::list;
using md::Ptr;
using md::MetadataObject;

string WrapList (const md::Ptr<ModuleWrap>& module, WrappedMember member, const string& type) {
    const auto& wraps = (*module).*member;
    stringstream code;
    code  << "luaL_Reg "<< type << "s[] = {\n";
    for (auto wrap : wraps)
        code 
          << "    { "
              << "\"" + wrap.name + "\", "
              << "generated::" + GetWrapName(type, wrap.name)
          << " },\n";
    code  << "    { nullptr, nullptr }\n"
             "};\n\n";
    return code.str();
}

string MiddleBlockCode (const string& module_name) {
    return
        "#include <languages/lua/aux/exportmodule.h>\n"
        "#include <languages/lua/luawrapper.h>\n"
        "#include <languages/lua/converter.h>\n"
        "#include <languages/lua/header.h>\n"
        "#include <opa/scriptmanager.h>\n"
        "#include <opa/module.h>\n"
        "#include <opa/converter.h>\n"
        "#include <iostream>\n"
        "#include <string>\n"
        "#include <list>\n"
        "#include <stdexcept>\n"
        "\n"
        "using std::list;\n"
        "using std::string;\n"
        "using std::cout;\n"
        "using std::endl;\n"
        "using std::runtime_error;\n"
        "using opa::Module;\n"
        "using opa::lua::LuaWrapper;\n"
        "using opa::lua::State;\n"
        "using opa::lua::Constant;\n"
        "using opa::lua::aux::ModuleInfo;\n"
        "using opa::lua::aux::ExportModule;\n"
        "\n"
        "// Begin wrappers\n\n"
        "namespace generated {\n\n";
}

string CheckAndCloseNamespace (bool open, const string& name) {
    return
        open ? ("} // namespace "+name+"\n\n") : "";
}

std::string CloseModuleBlock (const Ptr<ModuleWrap>& module) {
    stringstream code;

    code  << "namespace {\n\n"
          << "/// Forward declaration.\n"
          << "int init (lua_State* L_);\n\n"
          << WrapList(module, &ModuleWrap::functions, "function")
          << WrapList(module, &ModuleWrap::getters, "getter")
          << WrapList(module, &ModuleWrap::setters, "setter")
          << "ModuleInfo info(\n"
          << "    \""+module->name+"\", init, getters, setters, functions,\n"
          << "    {";
    for (auto child : module->children())
        code
          << " &" << (child->is_class() ? "class_" : "") << child->name << "::info,";
    code  << " }\n"
          << ");\n\n"
          << "/// [-(1|2),+1,e]\n"
          << "int init (lua_State* L) {\n"
          << "    return ExportModule(L, &info);\n"
          << "}\n\n"
          << "} // unnamed namespace\n\n";

    return code.str();
}

} // namespace lua
} // namespace gen
} // namespace opwig

