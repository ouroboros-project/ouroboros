#include <languages/python/utilities.h>
#include <opwig/md/function.h>
#include <opwig/md/variable.h>
#include <string>

namespace opwig {
namespace gen {
namespace python {

using std::string;
using md::Ptr;
using md::MetadataObject;
using md::Function;
using md::Variable;

string GetWrappedNestedName(const Ptr<const MetadataObject>& func) {
    string full_name = func->nested_name("::", false);
    full_name.insert(full_name.size()-(func->name()).size(), FUNC_PREFIX);
    return full_name;
}

std::string GetInitFuncNameForModule(const std::string& module_name) {
    return "init"+module_name;
}

std::string GetTypeObjNameForClass(const std::string& class_name) {
    return "Py" + class_name + "Object";
}

std::string GetTypeNameForClass(const std::string& class_name) {
    return "Py" + class_name + "Type";
}

namespace METHARGS {
string ForFunction(const Ptr<const Function>& func) {
    if (func->num_parameters() == 0)
        return "METH_NOARGS";
    return "METH_VARARGS";
}

string ForVariable(const Ptr<const Variable>& var) {
    if (var->type()->is_const())
        return "METH_NOARGS";
    return "METH_VARARGS";
}
    
} // namespace METHARGS

} // namespace python
} // namespace gen
} // namespace opwig
