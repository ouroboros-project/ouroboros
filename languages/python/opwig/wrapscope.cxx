#include <python/opwig/wrapscope.h>
#include <python/opwig/utilities.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/md/function.h>
#include <opwig/md/variable.h>
#include <vector>
#include <string>
#include <sstream>

namespace opa {
namespace python {

using std::string;
using std::stringstream;
using std::endl;
using opwig::md::Ptr;
using opwig::md::Function;
using opwig::md::Variable;
using opwig::gen::ScriptModule;

WrapScope::WrapScope(const std::string& name, bool is_class) : name_(name), is_class_(is_class) {
    if (is_class_)
        name_ = name_ + "_wrap";
}
WrapScope::WrapScope(const std::string& name, bool is_class, const opwig::md::Ptr<WrapScope>& parent)
     : name_(name), is_class_(is_class), parent_(parent) {
    if (is_class_)
        name_ = name_ + "_wrap";
}

string WrapScope::class_name() const {
    if (is_class_) {
        return name_.substr(0, name_.size()-5); // 5 -> lenght of '_wrap'
    }
    return "NOT_A_CLASS";
}

void WrapScope::AddFunction(const Ptr<const Function>& func) {
    functions_.push_back(func);
}
void WrapScope::AddVariable(const Ptr<const Variable>& var) {
    variables_.push_back(var);
}

string WrapScope::GetMethodTableName() const {
    return name_+"Methods";
}

string WrapScope::GenerateMethodTable(const string& base_nspace) const {
    stringstream table;
    table << "static PyMethodDef " << GetMethodTableName() << "[] = {" << endl;
    for (auto func : functions_) {
        string func_name = func->name();
        string full_func_name = base_nspace + (nested_name().empty() ? "" : "::" + nested_name()) + "::" + FUNC_PREFIX + func_name;
        if (is_class_)
            full_func_name = "(PyCFunction)" + full_func_name;
        table << TAB << "{\"" << func_name << "\", " << full_func_name;
        table << ", " << METHARGS::ForFunction(func) << ", \"calls C++ wrapped function\" }," << endl;
    }
    if (!is_class_) {
        for (auto var : variables_) {
            string var_name = var->name();
            string full_var_name = GetWrappedNestedName(var);
            table << TAB << "{\"" << var_name << "\", " << base_nspace << "::" << full_var_name;
            table << ", " << METHARGS::ForVariable(var);
            table << ", \"wraps C++ variable - call() for get, call(newValue) for set [if possible]\" }," << endl;
        }
    }
    table << TAB << "{NULL, NULL, 0, NULL} //sentinel" << endl;
    table << "};" << endl;
    return table.str();
}

std::string WrapScope::GetGetSetTableName() const {
    return name_+"GetSetters";
}

std::string WrapScope::GenerateGetSetTable(const std::string& base_nspace) const {
    stringstream table;
    table << "static PyGetSetDef " << GetGetSetTableName() << "[] = {" << endl;
    for (auto var : variables_) {
        string var_name = var->name();
        string full_var_name = base_nspace + (nested_name().empty() ? "" : "::" + nested_name()) + "::" + FUNC_PREFIX + var_name;
        table << TAB << "{ const_cast<char*>(\"" << var_name << "\")," << endl;
        table << TAB << "  (getter)" << full_var_name << "_getter," << endl;
        table << TAB << "  (setter)" << full_var_name << "_setter," << endl;
        table << TAB << "  const_cast<char*>(\"C++ class variable wrap\"), NULL }," << endl;
    }
    table << TAB << "{NULL} //sentinel" << endl;
    table << "};" << endl;
    return table.str();
}

string WrapScope::full_dotted_name() const {
    string fullName = name_;
    Ptr<WrapScope> mod = parent_.lock();
    while (mod) {
        fullName = mod->name() + "." + fullName;
        mod = mod->parent();
    }
    return fullName;
}

string WrapScope::nested_name(bool use_class_name) const {
    if (!parent_.expired()) {
        string prefix = parent_.lock()->nested_name();
        if (!prefix.empty()) {
            prefix = prefix + "::";
        }
        return prefix + (use_class_name ? class_name() : name_);
    }
    else
        return "";
}


std::list<ScriptModule> WrapScope::ConvertTreeToScriptModuleList() const {
    std::list<ScriptModule> moduleList;
    auxConvertToModuleList(moduleList);
    return moduleList;
}

void WrapScope::auxConvertToModuleList(std::list<ScriptModule>& moduleList) const {
    ScriptModule mod = {name_, GetInitFuncNameForModule(name_)};
    moduleList.push_back(mod);
    for (auto subm : sub_modules_ )
        if (!subm->is_class())
            subm->auxConvertToModuleList(moduleList);
}

} // namespace python
} // namespace opa
