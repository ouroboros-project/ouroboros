#include <opwig/gen/python/wrapmodule.h>
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

void WrapModule::AddFunction(const md::Ptr<const md::Function>& func) {
    functions_.push_back(func);
}
void WrapModule::AddVariable(const md::Ptr<const md::Variable>& var) {
    variables_.push_back(var);
}

string WrapModule::GetMethodTableName() const {
    return name_+"Methods";
}

string WrapModule::GenerateMethodTable(const string& base_nspace) const {
    stringstream table;
    table << "static PyMethodDef " << GetMethodTableName() << "[] = {" << endl;
    for (auto func : functions_) {
        string func_name = func->name();
        string full_func_name = GetWrappedNestedName(func);
        table << TAB << "{\"" << func_name << "\", " << base_nspace << "::" << full_func_name;
        table << ", " << METHARGS::ForFunction(func) << ", \"calls C++ wrapped function\" }," << endl;
    }
    for (auto var : variables_) {
        string var_name = var->name();
        string full_var_name = GetWrappedNestedName(var);
        table << TAB << "{\"" << var_name << "\", " << base_nspace << "::" << full_var_name;
        table << ", " << METHARGS::ForVariable(var);
        table << ", \"wraps C++ variable - call() for get, call(newValue) for set [if possible]\" }," << endl;
    }
    table << TAB << "{NULL, NULL, 0, NULL} //sentinel" << endl;
    table << "};" << endl;
    return table.str();
}

string WrapModule::full_dotted_name() const {
    string fullName = name_;
    md::Ptr<WrapModule> mod = parent_;
    while (mod) {
        fullName = mod->name() + "." + fullName;
        mod = mod->parent();
    }
    return fullName;
}


std::list<ScriptModule> WrapModule::ConvertTreeToScriptModuleList() const {
    std::list<ScriptModule> moduleList;
    auxConvertToModuleList(moduleList);
    return moduleList;
}

void WrapModule::auxConvertToModuleList(std::list<ScriptModule>& moduleList) const {
    ScriptModule mod = {name_, GetInitFuncNameForModule(name_)};
    moduleList.push_back(mod);
    for (auto subm : sub_modules_ )
        subm->auxConvertToModuleList(moduleList);
}

} // namespace python
} // namespace gen
} // namespace opwig
