
#ifndef OPA_LUA_AUX_EXPORTMODULE_H_
#define OPA_LUA_AUX_EXPORTMODULE_H_

#include <languages/lua/state.h>

#include <string>
#include <list>

namespace opa {
namespace lua {
namespace aux {

/// Represents information about a Lua module to be initialized.
/** Mainly used by opa::lua::aux::ExportModule.
 ** @see opa::lua::aux::ExportModule
 */
class ModuleInfo {

  public:

    /// Constructor.
    /** The luaL_Reg[] parameters are arrays of lua_CFunction's, used by luaL_register().
     ** @param the_name       Module's name (not the absolute path)
     ** @param the_getters    The getters to the module's fields.
     ** @param the_setters    The setters to the module's fields.
     ** @param the_functions  The module's functions.
     ** @param the_children   The module's child modules.
     */
    ModuleInfo (const std::string& the_name, luaL_Reg the_getters[], luaL_Reg the_setters[],
                luaL_Reg the_functions[], const std::list<ModuleInfo>& the_children);

    /// Gives the module's name.
    /** @return std::string The module's name.
     */
    std::string name () const;

    /// Gives the information about the module's supermodule.
    /** @return const ModuleInfo* The module's parent information.
     */
    const ModuleInfo* parent () const;

    /// Gives the module's functions array.
    /** @return const luaL_Reg* The module's functions.
     */
    const luaL_Reg* functions () const;

  private:

    std::string           name_;
    luaL_Reg              *getters_;
    luaL_Reg              *setters_;
    luaL_Reg              *funcions_;
    ModuleInfo            *parent_;
    std::list<ModuleInfo> children_;

};

inline ModuleInfo::ModuleInfo (const std::string& name, luaL_Reg the_getters[], luaL_Reg the_setters[],
                        luaL_Reg the_functions[], const std::list<ModuleInfo>& the_children)
    : getters_(the_getters), setters_(the_setters), funcions_(the_functions), parent_(nullptr),
      children_(the_children) {
    for (auto& child : children_)
        child.parent_ = this;
}

inline std::string ModuleInfo::name () const {
    return name_;
}

inline const ModuleInfo* ModuleInfo::parent () const {
    return parent_;
}

inline const luaL_Reg* ModuleInfo::functions () const {
    return funcions_;
}

void ExportModule (State& L, const ModuleInfo* info);

} // namespace aux
} // namespace lua
} // namespace opa

#endif

