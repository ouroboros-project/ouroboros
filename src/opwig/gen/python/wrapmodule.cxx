#include <opwig/gen/python/wrapmodule.h>
#include <opwig/gen/python/utilities.h>
#include <opwig/md/function.h>
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

string WrapModule::GetMethodTableName() const {
    return name_+"Methods";
}

string WrapModule::GenerateMethodTable(const string& base_nspace) const {
    stringstream table;
    table << "static PyMethodDef " << GetMethodTableName() << "[] = {" << endl;
    for (auto func : functions_) {
        string func_name = func->name();
        string full_func_name = GetWrappedFunctionNestedName(func);
        table << TAB << "{\"" << func_name << "\", " << base_nspace << "::" << full_func_name;
        table << ", " << GetMETHARGSforFunction(func) << ", \"calls C++ wrapped function\" }," << endl;
    }
    table << TAB << "{NULL, NULL, 0, NULL} //sentinel" << endl;
    table << "};" << endl;
    return table.str();
}

} // namespace python
} // namespace gen
} // namespace opwig
