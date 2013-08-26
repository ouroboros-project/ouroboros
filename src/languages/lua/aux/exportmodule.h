
#ifndef OPA_LUA_AUX_EXPORTMODULE_H_
#define OPA_LUA_AUX_EXPORTMODULE_H_

#include <opa/utils/uncopyable.h>

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
class ModuleInfo : public utils::Uncopyable {

  public:

    /// Constructor.
    /** The luaL_Reg[] parameters are arrays of lua_CFunction's, used by luaL_register().
     ** @param the_name       Module's name (not the absolute path)
     ** @param the_getters    The getters to the module's fields.
     ** @param the_setters    The setters to the module's fields.
     ** @param the_functions  The module's functions.
     ** @param the_children   The module's child modules.
     */
    ModuleInfo (const std::string& the_name, lua_CFunction the_init_function,
                luaL_Reg the_getters[], luaL_Reg the_setters[], luaL_Reg the_functions[],
                const std::list<ModuleInfo*>& the_children);

    /// Gives the module's name.
    /** @return std::string The module's name.
     */
    std::string name () const;

    /// Gives the module's Lua init function.
    /** @return lua_CFunction The module's init function.
     */
    lua_CFunction init_function () const;

    /// Gives the information about the module's supermodule.
    /** @return const ModuleInfo* The module's parent information.
     */
    const ModuleInfo* parent () const;

    /// Gives the module's functions array.
    /** @return const luaL_Reg* The module's functions.
     */
    const luaL_Reg* functions () const;

    /// Gives the module's getters array.
    /** @return const luaL_Reg* The module's getters.
     */
    const luaL_Reg* getters () const;

    /// Gives the module's setter array.
    /** @return const luaL_Reg* The module's setters.
     */
    const luaL_Reg* setters () const;

    /// Gives the module's constructor function (if there is one).
    /** @return lua_CFunction The module's constructor.
     */
    lua_CFunction constructor () const;

    /// Gives red-only access toe the module's submodules.
    /** @return const std::list<ModuleInfo>& The module's children.
     */
    const std::list<ModuleInfo*>& children () const;

  private:

    std::string             name_;
    lua_CFunction           init_function_;
    luaL_Reg                *getters_;
    luaL_Reg                *setters_;
    luaL_Reg                *funcions_;
    ModuleInfo              *parent_;
    std::list<ModuleInfo*>  children_;

};

inline ModuleInfo::ModuleInfo (const std::string& the_name, lua_CFunction the_init_function,
                               luaL_Reg the_getters[], luaL_Reg the_setters[],
                               luaL_Reg the_functions[], const std::list<ModuleInfo*>& the_children)
    : name_(the_name), init_function_(the_init_function), getters_(the_getters),
      setters_(the_setters), funcions_(the_functions), parent_(nullptr), children_(the_children) {
    for (auto& child : children_)
        child->parent_ = this;
}

inline std::string ModuleInfo::name () const {
    return name_;
}

inline lua_CFunction ModuleInfo::init_function () const {
    return init_function_;
}

inline const ModuleInfo* ModuleInfo::parent () const {
    return parent_;
}

inline const luaL_Reg* ModuleInfo::functions () const {
    return funcions_;
}

inline const luaL_Reg* ModuleInfo::getters () const {
    return getters_;
}

inline const luaL_Reg* ModuleInfo::setters () const {
    return setters_;
}

inline lua_CFunction ModuleInfo::constructor () const {
    return nullptr;
}

inline const std::list<ModuleInfo*>& ModuleInfo::children () const {
    return children_;
}

int ExportModule (State&& L, const ModuleInfo* info);

} // namespace aux
} // namespace lua
} // namespace opa

#endif

