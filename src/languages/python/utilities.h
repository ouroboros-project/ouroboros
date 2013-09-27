#ifndef OPWIG_GEN_PYTHON_UTILITIES_H_
#define OPWIG_GEN_PYTHON_UTILITIES_H_

#include <opwig/md/ptr.h>
#include <string>

namespace opwig {

namespace md{
class Function;
class Variable;
class MetadataObject;
}
}

namespace opa {
namespace python {

const std::string TAB = "    ";

const std::string BASE_NSPACE = "opwig_py_generated";

const std::string FUNC_PREFIX = "OPWIG_wrap_";

std::string GetWrappedNestedName(const opwig::md::Ptr<const opwig::md::MetadataObject>& func);

std::string GetMETHARGSforFunction(const opwig::md::Ptr<const opwig::md::Function>& func);

std::string GetInitFuncNameForModule(const std::string& module_name);

/** Returns the name of the struct representing a object of the type with given
    name. */
std::string GetTypeObjNameForClass(const std::string& class_name);
/// Returns the name of the python type object for the given class name.
std::string GetTypeNameForClass(const std::string& class_name);

namespace METHARGS {
std::string ForFunction(const opwig::md::Ptr<const opwig::md::Function>& func);
std::string ForVariable(const opwig::md::Ptr<const opwig::md::Variable>& var);
}

} // namespace python
} // namespace opa

#endif // OPWIG_GEN_PYTHON_UTILITIES_H_

