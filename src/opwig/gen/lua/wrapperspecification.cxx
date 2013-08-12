
#include <opwig/gen/lua/wrapperspecification.h>
#include <opwig/gen/lua/codes.h>
#include <opwig/md/function.h>
#include <opwig/md/variable.h>
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
    current_module()->open = false;
    return
        "\n"
        "// This is a generated file.\n\n";
}

string WrapperSpecification::MiddleBlock() const {
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
        "const char *MODULE_NAME = \""+module_name_+"\";\n\n"
        "} // unnamed namespace\n\n";
}

string WrapperSpecification::FinishFile () const {
    string functions_wrap_code =
        "namespace {\n\n"
        "// List of wrapped functions\n";
    for (auto module : modules_) {
        functions_wrap_code += WrapList(module, &Module::functions, "function");
        functions_wrap_code += WrapList(module, &Module::getters, "getter");
    }
    functions_wrap_code +=
        Utilities()+
        "} // unnamed namespace\n\n";
    string init_functions_code =
        // Loader function
        "extern \"C\" {\n\n"
        "// Forward declarations\n\n";
    for (auto module : modules_)
        init_functions_code +=
            "int luaopen_"+module->path+module->name+" (lua_State *L);\n\n";
    for (auto module : modules_) {
        string nesting_modules;
        for (auto parent = module->parent.lock(); parent; parent = parent->parent.lock())
            nesting_modules =
                "        OPWIG_Lua_MakeParentModule(L, \""+parent->name+"\");\n"
                + nesting_modules;
        init_functions_code +=
            "/// [-(1|2),+1,e]\n"
            "int luaopen_"+module->path+module->name+" (lua_State* L_) {\n"
            "    State L(L_);\n"
            "    if (L.gettop() > 1) {\n"
            "        L.remove(1);\n"
            "        L.settop(1);\n"
            "    } else {\n"
            "        L.settop(0);\n"
            "        L.pushvalue(Constant::GLOBALSINDEX());\n"
            + nesting_modules +
            "    }\n"
            "    // Nesting table is at index 1.\n"
            "    L.newtable();\n"
            "    L.pushvalue(-1);\n"
            "    // Module table is at index 2 and 3.\n"
            "    L.setfield(1, MODULE_NAME);\n"
            "    // Leave only the module table in the stack\n"
            "    L.remove(1);\n"
            "    L.settop(1);\n"
            "    // Register module's funcitons.\n"
            "    luaL_register(L, NULL, "+module->path+module->name+"_functions);\n"
            "    // Register module's submodules.\n";
        for (auto submodule : module->children) {
            init_functions_code +=
                "    OPWIG_Lua_ExportSubmodule("
                         "L, "
                         "\""+submodule->name+"\", "
                         "luaopen_"+submodule->path+submodule->name+
                     ");\n"
                ;
        }
        init_functions_code +=
            "    // Set module metatable.\n"
            "    OPWIG_Lua_PrepareMetatable(L, "+module->path+module->name+"_getters);\n"
            "    // Return de module itself\n"
            "    return 1;\n"
            "}\n\n";
    }
    init_functions_code +=
        "} // extern \"C\"\n\n";
    return functions_wrap_code + init_functions_code;
}

string WrapperSpecification::WrapFunction (const md::Ptr<const md::Function>& obj) {
    current_module()->functions.push_back({obj->name(), DumpNamespaceNesting()+"generated::"});
    stringstream func_code, args_code, call_code;
    size_t       num_params = obj->num_parameters();
    CheckAndOpenNamespace(func_code);
    func_code << "int " << GetWrapName("function", obj->name()) << " (lua_State* L) {\n";
    if (num_params > 0)
        func_code
              << "    int args = 0;\n"
              << "    if ((args = lua_gettop(L)) < " << num_params << ")\n"
              << "        return luaL_error(\n"
              << "            L,\n"
              << "            \"Error: %s expected %d arguments but received only %d.\\n\",\n"
              << "            \"" << DumpNamespaceNesting() << obj->name() << "\",\n"
              << "            " << static_cast<int>(num_params) << ",\n"
              << "            args\n"
              << "        );\n";
    func_code << "    opa::lua::Converter convert(L);\n";
    if (num_params > 0)
        args_code
              << "    try {\n";
    call_code << obj->name() << "(";
    for (size_t i = 0; i < obj->num_parameters(); ++i) {
        string type = obj->parameter_type(i)->full_type();
        func_code
              << "    " << type << " arg_" << i << ";\n";
        args_code
              << "        arg_" << i << " = "
              <<              "convert.ScriptToType<" << type << ">(" << (i+1) << ");\n";
        call_code << "arg_" << i;
        if (i+1 < obj->num_parameters())
            call_code << ", ";
    }
    if (num_params > 0) {
        args_code
              << "    } catch (runtime_error e) {\n"
              << "        return luaL_error(\n"
              << "            L,\n"
              << "            \"Error: could not convert %s's arguments (%s).\\n\",\n"
              << "            \"" << DumpNamespaceNesting() << obj->name() << "\",\n"
              << "            e.what()\n"
              << "        );\n"
              << "    }\n";
        func_code << args_code.str();
    }
    call_code << ")";
    if (obj->return_type()->full_type() == "void") {
        func_code
              << "    " << call_code.str() << ";\n"
              << "    int stack = 0;\n";
    } else {
        func_code
              << "    " << obj->return_type()->full_type() << " result = "
                  << call_code.str() << ";\n"
              << "    convert.TypeToScript<" << obj->return_type()->full_type() << ">(result);\n"
              << "    int stack = 1;\n";
    }
    func_code << "    return stack;\n"
              << "}\n\n";

    return func_code.str();
}

string WrapperSpecification::WrapVariable (const md::Ptr<const md::Variable>& obj) {
    current_module()->getters.push_back({obj->name(), DumpNamespaceNesting()+"generated::"});
    stringstream code;
    CheckAndOpenNamespace(code);
    code  << "int " << GetWrapName("getter", obj->name()) << " (lua_State* L) {\n"
          << "    opa::lua::Converter convert(L);\n"
          << "    convert.TypeToScript<" << obj->type()->full_type() << ">("
              << obj->name()
          << ");\n"
          << "    return 1;\n"
          << "}\n\n";
    return code.str();
}

string WrapperSpecification::WrapClass (const md::Ptr<const md::Class>& obj) {
    return "";
}

string WrapperSpecification::WrapNamespace (const md::Ptr<const md::Namespace>& obj, bool closing) {
    bool open = current_module()->open;
    if (!closing) {
        if (open) current_module()->open = false;
        Ptr<Module> new_module(new Module);

        new_module->name = obj->name();
        new_module->open = false;
        new_module->parent = current_module();
        for (auto module : module_stack_)
          new_module->path += module->name+"_";

        current_module()->children.push_back(new_module);
        modules_.push_back(new_module);
        module_stack_.push_back(new_module);

        return
            string(open ? "} // namespace generated\n\n" : "")+
            "namespace "+new_module->name+" {\n";
    } else {
        module_stack_.pop_back();

        return
            string(open ? "} // namespace generated\n\n" : "")+
            "} // namespace "+obj->name()+"\n\n";
    }
}

string WrapperSpecification::WrapEnum (const md::Ptr<const md::Enum>& obj) {
    return "";
}

std::list<ScriptModule> WrapperSpecification::GetGeneratedModules () const {
    std::list<ScriptModule> the_list;
    for (auto module : modules_) {
        string modname = module->name;
        for (auto parent = module->parent.lock(); parent; parent = parent->parent.lock())
          modname = parent->name+"."+modname;
        the_list.push_back(ScriptModule(modname, "luaopen_"+module->path+module->name));
    }
    return the_list;
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

