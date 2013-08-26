
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

} // namespace lua
} // namespace gen
} // namespace opwig

