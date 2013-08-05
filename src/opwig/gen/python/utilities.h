#ifndef OPWIG_GEN_PYTHON_UTILITIES_H_
#define OPWIG_GEN_PYTHON_UTILITIES_H_

#include <opwig/md/ptr.h>
#include <string>

namespace opwig {

namespace md{
class Function;
}

namespace gen {
namespace python {

const std::string TAB = "    ";

const std::string BASE_NSPACE = "opwig_py_generated";

const std::string FUNC_PREFIX = "OPWIG_wrap_";

std::string GetWrappedFunctionNestedName(const md::Ptr<const md::Function>& func);

std::string GetMETHARGSforFunction(const md::Ptr<const md::Function>& func);

} // namespace python
} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_PYTHON_UTILITIES_H_

