
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

string WrapperSpecification::FinishFile () const {
    string functions_wrap_code =
        const_cast<WrapperSpecification*>(this)->CloseNamespace(md::Ptr<md::Namespace>());
    string init_functions_code =
        // Loader function
        "extern \"C\" {\n\n";
    for (auto module : modules_) {
        init_functions_code +=
            "/// [-(1|2),+1,e]\n"
            "int luaopen_"+module->path+module->name+" (lua_State* L) {\n"
            "    return "+module->nesting+"init(L);\n"
            "}\n\n";
    }
    init_functions_code +=
        "} // extern \"C\"\n\n";
    return functions_wrap_code + init_functions_code;
}

string WrapperSpecification::WrapFunction (const Ptr<const md::Function>& obj) {
    stringstream    func_code,
                    args_code,
                    call_code;
    bool            is_class = state_.is_current_class();
    size_t          num_params = obj->num_parameters();
    size_t          self_param = (is_class ? 1 : 0);
    string          prefix = is_class ? "member_" : "";
    state_.AddFunction(obj);
    func_code << "int " << GetWrapName(prefix+"function", obj->name()) << " (lua_State* L) {\n";
    if (num_params+self_param > 0)
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
    if (is_class) {
        string type = state_.current_module()->name;
        func_code
              << "    " << type << " *self = static_cast<" << type << "*>(\n"
              << "        static_cast<opa::lua::aux::UserData*>(\n"
              << "            convert.ScriptToType<void*>(1)\n"
              << "        )->obj\n"
              << "    );\n";
    }
    if (num_params > 0)
        args_code
              << "    try {\n";
    call_code << (is_class ? "self->" : "") << obj->name() << "(";
    for (size_t i = 0; i < obj->num_parameters(); ++i) {
        string type = obj->parameter_type(i)->full_type();
        func_code
              << "    " << type << " arg_" << i << ";\n";
        args_code
              << "        arg_" << i << " = "
              <<              "convert.ScriptToType<" << type << ">(" << (i+1+self_param) << ");\n";
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
              << "    lua_settop(L, 0);\n"
              << "    convert.TypeToScript<" << obj->return_type()->full_type() << ">(result);\n"
              << "    int stack = 1;\n";
    }
    func_code << "    return stack;\n"
              << "}\n\n";

    return func_code.str();
}

string WrapperSpecification::WrapVariable (const md::Ptr<const md::Variable>& obj) {
    stringstream  code;
    bool          is_class = state_.is_current_class();
    string        prefix = is_class ? "member_" : "";
    const string  type = obj->type()->full_type();

    state_.AddVariableGetter(obj);
    code  << "int " << GetWrapName(prefix+"getter", obj->name()) << " (lua_State* L) {\n"
          << "    opa::lua::Converter convert(L);\n";
    if (is_class)
        code
          << "    if (lua_gettop(L) < 1)\n"
          << "        return luaL_error(\n"
          << "            L,\n"
          << "            \"Error: '%s' atribute cannot be accessed without an object.\\n\",\n"
          << "            \"" << DumpNamespaceNesting() << obj->name() << "\"\n"
          << "        );\n"
          << "    " << type << " *self = static_cast<" << type << "*>(\n"
          << "        static_cast<opa::lua::aux::UserData*>(\n"
          << "            convert.ScriptToType<void*>(1)\n"
          << "        )->obj\n"
          << "    );\n"
          << "    convert.TypeToScript<" << type << ">(self->" << obj->name() << ");\n";
    else
        code
          << "    convert.TypeToScript<" << type << ">(" << obj->name() << ");\n";
    code  << "    return 1;\n"
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
    bool open   = !state_.current_module()->has_children();
    
    state_.PushModule(obj->name(), true);
    modules_.push_back(state_.current_module());

    return
        string(open ? "} // namespace generated\n\n" : "")+
        "namespace class_"+obj->name()+" {\n\n"+
        "namespace generated {\n\n";
}

string WrapperSpecification::CloseClass (const md::Ptr<const md::Class>& obj) {
    bool    open = !state_.current_module()->has_children();
    string  code = CloseModuleBlock(state_.current_module());

    state_.PopModule();

    return
        CheckAndCloseNamespace(open)+
        code+
        string(obj ? "} // namespace class_"+obj->name()+"\n\n": "");
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
    string code = CloseModuleBlock(state_.current_module());
    state_.PopModule();
    return
        CheckAndCloseNamespace(open)+
        code+
        string(obj ? "} // namespace "+obj->name()+"\n\n": "");
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

