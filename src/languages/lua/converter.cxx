
#include <languages/lua/converter.h>
#include <lua5.1/lauxlib.h>

namespace opa {
namespace lua {

struct StackHook {
  lua_State *L;
  int       index;
};

} // namespace lua
} // namespace opa

