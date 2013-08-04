
#include <opwig/gen/lua/wrapperspecification.h>
#include <opwig/md/function.h>
#include <opwig/md/namespace.h>
#include <sstream>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using std::stringstream;
using md::Ptr;

WrapperSpecification::WrapperSpecification () {
    Ptr<Module> root(new Module);
    modules_.push_back(root);
    module_stack_.push_back(root);
}

string WrapperSpecification::FileHeader () const {
    current_module()->name = module_name_;
    return
        "\n"
        "// This is a generated file.\n\n";
}

string WrapperSpecification::MiddleBlock() const {
    return
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

string WrapperSpecification::FinishFile () const {
    string functions_wrap_code =
        "namespace {\n\n"
        "// List of wrapped functions\n";
    for (auto module : modules_) {
        functions_wrap_code +=
            "luaL_Reg "+module->path+module->name+"_functions[] = {\n";
        for (auto func : module->functions)
            functions_wrap_code +=
                "    { \""+func.name+"\", "+func.nesting+"OPWIG_wrap_"+func.name+" },\n";
        functions_wrap_code +=
            "    { nullptr, nullptr }\n"
            "};\n\n";
    }
    functions_wrap_code +=
        "} // unnamed namespace\n\n";
    string init_functions_code =
        // Loader function
        "extern \"C\" {\n\n"
        "// Forward declarations\n\n";
    for (auto module : modules_)
        init_functions_code +=
            "int luaopen_"+module->path+module->name+" (lua_State *L);\n\n";
    for (auto module : modules_) {
        init_functions_code +=
            "/// [-(1|2),+1,e]\n"
            "int luaopen_"+module->path+module->name+" (lua_State* L) {\n"
            "    //const char *mod_name = luaL_checkstring(L, 1);\n"
            "    if (lua_gettop(L) > 1) {\n"
            "        lua_remove(L, 1);\n"
            "        lua_settop(L, 1);\n"
            "    } else {\n"
            "        lua_settop(L, 0);\n"
            "        lua_pushvalue(L, LUA_GLOBALSINDEX);\n"
            "    }\n"
            "    // Nesting table is at index 1.\n"
            "    lua_newtable(L);\n"
            "    lua_pushvalue(L, -1);\n"
            "    // Module table is at index 2 and 3.\n"
            "    luaL_register(L, NULL, "+module->path+module->name+"_functions);\n"
            "    lua_setfield(L, 1, MODULE_NAME);\n"
            "    // Leave only the module table in the stack\n"
            "    lua_remove(L, 1);\n"
            "    lua_settop(L, 1);\n";
        for (auto submodule : module->children) {
            init_functions_code +=
                "    // Add submodule \""+submodule->name+"\"\n"
                "    lua_pushcfunction(L, luaopen_"+submodule->path+submodule->name+");\n"
                "    lua_pushstring(L, \""+submodule->name+"\");\n"
                "    lua_pushvalue(L, 1);\n"
                "    // Stack: [module, cfunction, string, module]\n"
                "    lua_call(L, 2, 1);\n"
                "    // Stack: [module, submodule]\n"
                "    lua_setfield(L, 1, \""+submodule->name+"\");\n"
                ;
        }
        init_functions_code +=
            "    // Return de module itself\n"
            "    return 1;\n"
            "}\n\n";
    }
    init_functions_code +=
        "} // extern \"C\"\n\n";
    return functions_wrap_code + init_functions_code;
}

string WrapperSpecification::WrapFunction (const md::Ptr<const md::Function>& obj) {
    module_stack_.back()->functions.push_back({obj->name(), DumpNamespaceNesting()+"generated::"});
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
    if (obj->return_type() == "void") {
        func_code << "    " << call_code.str() << ";\n";
        func_code << "    int stack = 0;\n";
    } else {
        func_code << "    " << obj->return_type() << " result = " << call_code.str() << ";\n";
        func_code << "    int stack = convert.TypeToScript<" << obj->return_type()
                                                             << ">(result);\n";
    }
    func_code << "    return stack;\n"
              << "}\n\n";

    return func_code.str();
}

string WrapperSpecification::WrapVariable (const md::Ptr<const md::Variable>& obj) {
    return "";
}

string WrapperSpecification::WrapClass (const md::Ptr<const md::Class>& obj) {
    return "";
}

string WrapperSpecification::WrapNamespace (const md::Ptr<const md::Namespace>& obj, bool closing) {
    if (!closing) {
        Ptr<Module> new_module(new Module);

        new_module->name = obj->name();
        for (auto module : module_stack_)
          new_module->path += module->name+"_";

        current_module()->children.push_back(new_module);
        modules_.push_back(new_module);
        module_stack_.push_back(new_module);

        return
            "namespace "+new_module->name+" {\n"
            "namespace generated {\n\n";
    } else {
        module_stack_.pop_back();

        return
            "} // namespace generated\n"
            "} // namespace "+obj->name()+"\n\n";
    }
}

string WrapperSpecification::WrapEnum (const md::Ptr<const md::Enum>& obj) {
    return "";
}

string WrapperSpecification::DumpNamespaceNesting () const {
    string dump;
    bool skip = true;
    for (auto module : module_stack_)
        if (skip)
          skip = false;
        else
          dump += module->name+"::";
    return dump;
}

} // namespace lua
} // namespace gen
} // namespace opwig

