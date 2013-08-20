
#include <opwig/gen/lua/wrapperstate.h>

namespace opwig {
namespace gen {
namespace lua {

std::string WrapperState::StackAsString (const std::string& sep, size_t skip) const {
  std::string result;
  for (auto module : stack_) {
    if (skip > 0) {
      --skip;
      continue;
    }
    result += module->name+sep;
  }
  return result;
}

} // namespace lua
} // namespace gen
} // namespace opwig

