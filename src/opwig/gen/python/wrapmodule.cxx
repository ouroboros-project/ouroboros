#include <opwig/gen/python/wrapmodule.h>
#include <vector>
#include <string>
#include <sstream>

namespace opwig {
namespace gen {
namespace python {

using std::string;
using std::stringstream;
using std::endl;

void WrapModule::AddFunction(const string& name, const string& wrapped_full_name) {
    function_names_.push_back(name);
    full_wrapped_functions_.push_back(wrapped_full_name);
}

string WrapModule::GetMethodTableName() const {
    return name_+"Methods";
}

string WrapModule::GenerateMethodTable(const string& base_nspace) const {
    stringstream table;
    const char* TAB = "    ";
    table << "static PyMethodDef " << GetMethodTableName() << "[] = {" << endl;
    for (unsigned i=0; i < function_names_.size(); i++) {
        string func_name = function_names_[i];
        string full_func_name = full_wrapped_functions_[i];
        table << TAB << "{\"" << func_name << "\", " << base_nspace << "::" << full_func_name;
        table << ", METH_VARARGS, \"calls C++ wrapped function\" }," << endl;
    }
    table << TAB << "{NULL, NULL, 0, NULL} //sentinel" << endl;
    table << "};" << endl;
    return table.str();
}

} // namespace python
} // namespace gen
} // namespace opwig
