
#ifndef OUROBOROS_SCRIPT_LUA_AUXLIB_H_
#define OUROBOROS_SCRIPT_LUA_AUXLIB_H_

#include <lua/opa/header.h>
#include <lua/opa/primitive.h>

#include <functional>

namespace opa {
namespace lua {

class State;

class AuxLib {

  public:

    explicit AuxLib(lua_State* L) : L_(L) {}

    static lua_State* newstate() {
        return luaL_newstate();
    }

    void openlibs() { luaL_openlibs(L_); }

    const Constant loadfile(const char* filename) {
        return Constant(
            std::bind(luaL_loadfile, L_, filename)
        );
    }

    const Constant loadstring(const char* str) {
        return Constant(
            std::bind(luaL_loadstring, L_, str)
        );
    }

    int checkintteger(int index) { return static_cast<int>(luaL_checkinteger(L_, index)); }
    const char* checkstring(int index) { return luaL_checkstring(L_, index); }

    DataID ref(int index) { return luaL_ref(L_, index); }

    void unref(int index, int n) { luaL_unref(L_, index, n); }

  private:

    lua_State* L_;

};

} /* namespace lua */
} /* namespace opa */


#endif /* OUROBOROS_SCRIPT_LUA_AUXLIB_H_ */
