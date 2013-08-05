#include <opwig/gen/python/pythonspecification.h>
#include <opwig/gen/python/wrapmodule.h>
#include <opwig/gen/python/utilities.h>
#include <opwig/md/function.h>
#include <opwig/md/namespace.h>

#include <sstream>

namespace opwig {
namespace gen {
namespace python {

using std::string;
using md::Ptr;
using std::stringstream;
using std::endl;

PythonSpecification::PythonSpecification() {
    root_module_ = Ptr<WrapModule>(new WrapModule(""));
    current_ = root_module_;
}

// HEADER BLOCK
string PythonSpecification::FileHeader() const {
    root_module_->set_name(module_name_);
    return
        "// FILE GENERATED BY OPWIG - OUROBOROS PROJECT.\n\n"
        "#include <Python.h>\n"
        "#include <languages/python/pythonwrapper.h>\n"
        "#include <languages/python/pythonconverter.h>\n"
        "#include <opa/scriptmanager.h>\n"
        "#include <opa/module.h>\n"
        "#include <iostream>\n"
        "#include <string>\n"
        "\n"
        "using std::string;\n"
        "using std::cout;\n"
        "using std::endl;\n"
        "using opa::Module;\n"
        "using opa::python::PythonWrapper;\n"
        "using opa::python::PythonConverter;\n"
        "\n";
}

string PythonSpecification::MiddleBlock() const {
    return
        "namespace {\n\n"
        "bool NumArgsOk(PyObject* args, int num) {\n"
        "    if (static_cast<int>(PyTuple_Size(args)) != num) {\n"
        "        string msg = \"expected \"+std::to_string(num)+\" parameters, but received \"+std::to_string(PyTuple_Size(args))+\".\";\n"
        "        PyErr_SetString(PyExc_RuntimeError, msg.c_str());\n"
        "        return false;\n"
        "    }\n"
        "    return true;\n"
        "}\n"
        "} // unnamed namespace\n\n"
        "namespace "+BASE_NSPACE+" {\n\n";
}

// FINISH BLOCK
void HandleWrapModuleForInitFunc(stringstream& block, const Ptr<WrapModule>& module) {
    block << TAB << "PyObject* " << module->name() << "_mod = Py_InitModule(\"" << module->name();
    block << "\", " << module->GetMethodTableName() << ");" << endl;
    
    if (module->parent()) {
        block << TAB << "if (PyModule_AddObject(" << module->parent()->name() << "_mod, \"";
        block << module->name() << "\", " << module->name() << "_mod) == -1) {" << endl;
        block << TAB << TAB << "PyErr_SetString(PyExc_RuntimeError, \"could not add submodule ";
        block << module->name() << " to module " << module->parent()->name() << "\");" << endl;
        block << TAB << "}" << endl;
    }

    for (auto subm : module->sub_modules() )
        HandleWrapModuleForInitFunc(block, subm);
}

void HandleModuleMethodTables(stringstream& block, const Ptr<WrapModule>& module) {
    block << "//module " << module->name() << " method table" << endl;
    block << module->GenerateMethodTable(BASE_NSPACE) << endl;
    for (auto subm : module->sub_modules() )
        HandleModuleMethodTables(block, subm);
}

string PythonSpecification::FinishFile() const {
    stringstream block;
    block << endl << "} //namespace " << BASE_NSPACE << endl << endl;
    HandleModuleMethodTables(block, root_module_);
    block << "PyMODINIT_FUNC" << endl;
    block << LoadFuncName() << "(void) {" << endl;
    HandleWrapModuleForInitFunc(block, root_module_);
    block << "}" << endl;
    return block.str();
}

// WRAP FUNCION
string PythonSpecification::WrapFunction(const Ptr<const md::Function>& obj) {
    current_->AddFunction(obj->name(), GetWrappedFunctionNestedName(obj));
        
    stringstream func;
    func << "PyObject* " << FUNC_PREFIX << obj->name() << "(PyObject* self, PyObject* args) {" << std::endl;
    func << TAB << "if (!NumArgsOk(args, " << obj->num_parameters() << ")) return nullptr;" << endl;
    func << TAB << "PythonConverter converter;" << std::endl;
    stringstream args ("");
    for (unsigned i=0; i<obj->num_parameters(); i++) {
        func << TAB << obj->parameter_type(i) <<" fArg"<< i << ";" << endl;
        func << TAB << "try { fArg"<< i <<" = converter.PyArgToType<"<< obj->parameter_type(i) <<">(args, " << i << "); }" << endl;
        func << TAB << "catch (std::exception& e) { cout << e.what() << endl; return nullptr; }" << endl;
        if (i > 0)
            args << ", ";
        args << "fArg" << i;
    }
    if (obj->return_type() == "void") {
        func << TAB << obj->nested_name() << "("<< args.str() << ");" << endl;
        func << TAB << "Py_RETURN_NONE;" << endl;
    }
    else {
        func << TAB << obj->return_type() << " fValue = " << obj->nested_name() << "("<< args.str() << ");" << endl;
        func << TAB << "return converter.TypeToScript<"<< obj->return_type() <<">(fValue);" << endl;
    }
    func << "}";
    return func.str();
}

// WRAP VARIABLE
string PythonSpecification::WrapVariable(const Ptr<const md::Variable>& obj) {
    return "";
}

// WRAP CLASS
string PythonSpecification::WrapClass(const Ptr<const md::Class>& obj) {
    return "";
}

// WRAP NAMESPACE
string PythonSpecification::WrapNamespace(const Ptr<const md::Namespace>& obj, bool closing) {
    if (!closing) {
        Ptr<WrapModule> newm = Ptr<WrapModule>(new WrapModule(obj->name(), current_));
        current_->AddSubModule(newm);
        current_ = newm;
        return "namespace "+obj->name()+" {\n";
    }
    else {
        current_ = current_->parent();
        return "} //closing namespace "+obj->name()+"\n";
    }
}

// WRAP ENUM
string PythonSpecification::WrapEnum(const Ptr<const md::Enum>& obj) {
    return "";
}

} // namespace python
} // namespace gen
} // namespace opwig
