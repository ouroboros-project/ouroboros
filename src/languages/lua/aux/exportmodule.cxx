
#include <languages/lua/aux/exportmodule.h>

namespace opa {
namespace lua {
namespace aux {

using std::string;

namespace {

/// This function uses the table on top of the stack to check for the parent module.
/** If it is not there, the function creates it. Recursive. 
 */
void MakeParentModule (State& L, const ModuleInfo* parent) {
    if (!parent) return;
    MakeParentModule(L, parent->parent());
    L.getfield(-1, parent->name());
    if (L.isnil(-1)) {
        L.pop(1);
        L.newtable();
        L.pushvalue(-1);
        L.setfield(-3, parent->name());
    }
    L.remove(-2);
}

} // unnamed namespace

void ExportModule (State& L, const ModuleInfo* info) {
    if (L.gettop() > 1) {
        L.remove(1);
        L.settop(1);
    } else {
        L.settop(0);
        L.pushvalue(Constant::GLOBALSINDEX());
        MakeParentModule(L, info->parent());
    }
    // Stack: [nesting-table]
    L.newtable();
    L.pushvalue(-1);
    // Stack: [nesting-table, module, module]
    L.setfield(1, info->name());
    // Leave only the module table in the stack
    L.remove(1);
    L.settop(1);
    // Stack: [module]
    // Register module's functions.
    luaL_register(L, NULL, info->functions());
}

} // namespace aux
} // namespace lua
} // namespace opa

