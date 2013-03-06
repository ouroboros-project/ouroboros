#ifndef OUROBOROS_SCRIPT_LUA_MODULES_H_
#define OUROBOROS_SCRIPT_LUA_MODULES_H_

#include <opa/script.h>
#include <languages/lua/defs.h>

namespace opa {
namespace lua {

void AddModule(const Module<inittype> &);

void RegisterModules(LuaWrapper* wrapper);

} /* namespace lua */
} /* namespace opa */


#endif /* OUROBOROS_SCRIPT_LUA_MODULES_H_ */
