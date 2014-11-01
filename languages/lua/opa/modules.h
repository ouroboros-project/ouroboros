#ifndef OUROBOROS_SCRIPT_LUA_MODULES_H_
#define OUROBOROS_SCRIPT_LUA_MODULES_H_

#include <opa/script.h>
#include <lua/opa/defs.h>

namespace opa {
namespace lua {

void AddModule(const Module<inittype> &);

void RegisterModules(LuaMachine* machine);

} /* namespace lua */
} /* namespace opa */


#endif /* OUROBOROS_SCRIPT_LUA_MODULES_H_ */
