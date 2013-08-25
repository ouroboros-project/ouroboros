
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
using md::Namespace;

WrapperSpecification::WrapperSpecification (const string& the_module_name)
    : state_(the_module_name) {
    modules_.push_back(state_.current_module());
}

string WrapperSpecification::FileHeader () const {
    return
        "\n"
        "// This is a generated file.\n\n";
}

string WrapperSpecification::MiddleBlock() const {
    return MiddleBlockCode(module_name_);
}

namespace {

string CheckAndCloseNamespace (const Ptr<const Namespace>& obj, bool open) {
    return
        string(open ? "} // namespace generated\n\n" : "")+
        string(obj ? "} // namespace "+obj->name()+"\n\n": "");
}

} // unnamed namespace

string WrapperSpecification::FinishFile () const {
    string functions_wrap_code =
        CheckAndCloseNamespace(Ptr<const Namespace>(), !state_.current_module()->has_children())+
        "namespace {\n\n"
        "// List of wrapped functions\n";
    for (auto module : modules_) {
        functions_wrap_code += WrapList(module, &ModuleWrap::functions, "function");
        functions_wrap_code += WrapList(module, &ModuleWrap::getters, "getter");
        functions_wrap_code += WrapList(module, &ModuleWrap::setters, "setter");
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
        for (auto parent = module->parent(); parent; parent = parent->parent())
            nesting_modules =
                "        OPWIG_Lua_MakeParentModule(L, \""+parent->name+"\");\n"
                + nesting_modules;
        init_functions_code +=
            "/// [-(1|2),+1,e]\n"
            "int luaopen_"+module->path+module->name+" (lua_State* L_) {\n"
            "    State L(L_);\n"
            "    //OPWIG_Lua_PrepareModule(L, ...);\n"
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
            "    // Register module's functions.\n"
            "    luaL_register(L, NULL, "+module->path+module->name+"_functions);\n"
            "    // Register module's submodules.\n";
        for (auto submodule : module->children()) {
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
            "    OPWIG_Lua_PrepareMetatable(\n"
            "        L,\n"
            "        "+module->path+module->name+"_getters,\n"
            "        "+module->path+module->name+"_setters\n"
            "    );\n"
            "    // Return de module itself\n"
            "    return 1;\n"
            "}\n\n";
    }
    init_functions_code +=
        "} // extern \"C\"\n\n";
    return functions_wrap_code + init_functions_code;
}

string WrapperSpecification::WrapFunction (const md::Ptr<const md::Function>& obj) {
    if (state_.current_module()->is_class() && !obj->is_static())
        return "";
    stringstream func_code, args_code, call_code;
    size_t       num_params = obj->num_parameters();
    //CheckAndOpenNamespace(func_code);
    state_.current_module()->functions.push_back({obj->name(), DumpNamespaceNesting()+"generated::"});
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
    if (state_.current_module()->is_class() && !obj->is_static())
        return "";
    stringstream code;
    const string type = obj->type()->full_type();
    //CheckAndOpenNamespace(code);
    state_.current_module()->getters.push_back({obj->name(), DumpNamespaceNesting()+"generated::"});
    code  << "int " << GetWrapName("getter", obj->name()) << " (lua_State* L) {\n"
          << "    opa::lua::Converter convert(L);\n"
          << "    convert.TypeToScript<" << type << ">(" << obj->name() << ");\n"
          << "    return 1;\n"
          << "}\n\n";
    if (!obj->type()->is_const()) {
        state_.current_module()->setters.push_back({obj->name(), DumpNamespaceNesting()+"generated::"});
        code
          << "int " << GetWrapName("setter", obj->name()) << " (lua_State* L) {\n"
          << "    opa::lua::Converter convert(L);\n"
          << "    " << type << " value;\n"
          << "    try {\n"
          << "        value = convert.ScriptToType<" << type << ">(1);\n"
          << "    } catch (runtime_error e) {\n"
          << "        return luaL_error(\n"
          << "            L,\n"
          << "            \"Error: could not convert value to %s's type (%s).\\n\",\n"
          << "            \"" << DumpNamespaceNesting() << obj->name() << "\",\n"
          << "            e.what()\n"
          << "        );\n"
          << "    }\n"
          << "    " << obj->name() << " = value;\n"
          << "    return 0;\n"
          << "}\n\n";
    }
    return code.str();
}

string WrapperSpecification::WrapEnum (const md::Ptr<const md::Enum>& obj) {
    return "";
}

string WrapperSpecification::OpenClass (const md::Ptr<const md::Class>& obj) {
    bool    open = state_.current_module()->has_wraps();
    string  last_name = state_.current_module()->name;
    state_.PushModule(obj->name(), true);
    modules_.push_back(state_.current_module());

    return
        string(open ? "} // namespace generated for class "+last_name+"\n\n" : "")+
        "namespace /*"+obj->name()+"*/ {\n";
}

string WrapperSpecification::CloseClass (const md::Ptr<const md::Class>& obj) {
    bool open = !state_.current_module()->has_children() && state_.current_module()->has_wraps();
    state_.PopModule();

    return
        string(open ? "} // namespace generated for class "+obj->name()+"\n\n" : "")+
        string(obj ? "} // namespace for "+obj->name()+"\n\n": "");
}

string WrapperSpecification::OpenNamespace (const Ptr<const Namespace>& obj) {
    bool open = !state_.current_module()->has_children();
    state_.PushModule(obj->name());
    modules_.push_back(state_.current_module());

    return
        string(open ? "} // namespace generated\n\n" : "")+
        "namespace "+obj->name()+" {\n\n"+
        "namespace generated {\n\n";
}

string WrapperSpecification::CloseNamespace (const Ptr<const Namespace>& obj) {
    bool open = !state_.current_module()->has_children();
    state_.PopModule();

    return
        CheckAndCloseNamespace(obj,  open);
}

std::list<ScriptModule> WrapperSpecification::GetGeneratedModules () const {
    std::list<ScriptModule> the_list;
    for (auto module : modules_) {
        string modname = module->name;
        for (auto parent = module->parent(); parent; parent = parent->parent())
          modname = parent->name+"."+modname;
        the_list.push_back(ScriptModule(modname, "luaopen_"+module->path+module->name));
    }
    return the_list;
}

string WrapperSpecification::DumpNamespaceNesting () const {
    return state_.StackAsString("::", 1);
}

} // namespace lua
} // namespace gen
} // namespace opwig

