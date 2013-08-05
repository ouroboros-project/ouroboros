#include <opwig/gen/python/utilities.h>
#include <opwig/md/function.h>
#include <string>

namespace opwig {
namespace gen {
namespace python {

using std::string;
using md::Ptr;
using md::Function;

string GetWrappedFunctionNestedName(const Ptr<const Function>& func) {
    string full_name = func->nested_name("::", false);
    full_name.insert(full_name.size()-(func->name()).size(), FUNC_PREFIX);
    return full_name;
}

} // namespace python
} // namespace gen
} // namespace opwig
