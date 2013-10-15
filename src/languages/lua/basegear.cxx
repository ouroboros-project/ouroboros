
#include <languages/lua/basegear.h>
#include <languages/lua/auxlib.h>

#include <string>

namespace opa {
namespace lua {

using std::string;

static int traceback (lua_State *L);

/// Protected:

const Constant BaseGear::Report (const Constant& c) {
  if (c != Constant::OK() && !L_.isnil(-1)) {
    string msg = L_.tostring(-1);
    if (msg.empty()) msg = "(error object is not a string)";
    L_.pop(1);
    /* force a complete garbage collection in case of errors */
    L_.gc(Constant::gc::COLLECT(), 0);
    return Constant::AddInfo(c, msg);
  }
  return c;
}

const Constant BaseGear::TracedCall (int nargs, int nres) {
  int base = L_.gettop() - nargs;
  L_.pushcfunction(traceback);
  L_.insert(base);
  Constant result = L_.pcall(nargs, nres, base);
  L_.remove(base);
  return Report(result);
}

static int traceback (lua_State *L) {
  lua_getglobal(L, "debug");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and traceback */
  lua_call(L, 2, 1);  /* call debug.traceback */
  return 1;
}

} /* namespace lua */
} /* namespace opa */
