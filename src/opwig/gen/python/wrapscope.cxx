#include <opwig/gen/python/wrapscope.h>
#include <opwig/gen/python/utilities.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/md/function.h>
#include <opwig/md/variable.h>
#include <vector>
#include <string>
#include <sstream>

namespace opwig {
namespace gen {
namespace python {

using std::string;
using std::stringstream;
using std::endl;

void WrapScope::AddFunction(const md::Ptr<const md::Function>& func) {
    functions_.push_back(func);
}
void WrapScope::AddVariable(const md::Ptr<const md::Variable>& var) {
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
        string full_func_name = base_nspace + "::" + GetWrappedNestedName(func);
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
        string full_var_name = base_nspace + "::" + GetWrappedNestedName(var);
        table << TAB << "{\"" << var_name << "\", (getter)" << full_var_name << "_getter";
        table << ", (setter)" << full_var_name << "_setter";
        table << ", \"C++ class variable wrap\", NULL }," << endl;
    }
    table << TAB << "{NULL} //sentinel" << endl;
    table << "};" << endl;
    return table.str();
}

string WrapScope::full_dotted_name(const std::string& separator) const {
    string fullName = name_;
    md::Ptr<WrapScope> mod = parent_;
    while (mod) {
        fullName = mod->name() + separator + fullName;
        mod = mod->parent();
    }
    return fullName;
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
} // namespace gen
} // namespace opwig
