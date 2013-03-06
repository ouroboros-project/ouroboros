
#ifndef OUROBOROS_SCRIPT_LUA_DEFS_H_
#define OUROBOROS_SCRIPT_LUA_DEFS_H_

#ifndef lua_h
extern "C" {
struct lua_State;
typedef struct lua_State lua_State;
typedef int (*lua_CFunction)(lua_State*);
}
#endif

#include <list>
#include <map>

namespace opa {
namespace lua {

typedef int                     DataID;
typedef std::list<DataID>       DataBuffer;
typedef std::map<DataID,DataID> DataMap;
typedef void*                   UData;

template <class T>
UData AsUData(T* p) { return static_cast<UData>(p); }

template <class T>
UData AsUData(const T* p) {
    return AsUData(const_cast<T*>(p));
}

typedef lua_CFunction inittype;

#define LUA_INIT_FUNCTION_NAME(name) luaopen_##name
#define LUA_INIT_FUNCTION_SIGNATURE(name) int LUA_INIT_FUNCTION_NAME(name)(lua_State*)
#define LUA_MODULE_NAME(name) opa::lua::NameConversion(#name)
#define LUA_NAMESPACE ::opa::lua

} /* namespace lua */
} /* namespace opa */


#endif /* OUROBOROS_SCRIPT_LUA_DEFS_H_ */
