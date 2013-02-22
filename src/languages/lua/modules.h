#ifndef OUROBOROS_SCRIPT_LUA_MODULES_H_
#define OUROBOROS_SCRIPT_LUA_MODULES_H_

#include <ouroboros/script.h>
#include <languages/lua/defs.h>

namespace ouroboros {
namespace lua {

void AddModule(const Module<inittype> &);

void RegisterModules(LuaWrapper* wrapper);

} /* namespace lua */
} /* namespace ouroboros */


#endif /* OUROBOROS_SCRIPT_LUA_MODULES_H_ */
