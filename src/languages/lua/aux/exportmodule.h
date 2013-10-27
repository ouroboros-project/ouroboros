
#ifndef OPA_LUA_AUX_EXPORTMODULE_H_
#define OPA_LUA_AUX_EXPORTMODULE_H_

#include <opa/utils/uncopyable.h>

#include <languages/lua/converter.h>
#include <languages/lua/state.h>

#include <string>
#include <list>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

namespace opa {
namespace lua {
namespace aux {

/// Represents information about a Lua module to be initialized.
/** Mainly used by opa::lua::aux::ExportModule.
 ** @see opa::lua::aux::ExportModule
 */
class ModuleInfo : public utils::Uncopyable {

  public:

    using FunctionTable = std::unordered_map<std::string, luaL_Reg*>;

    /// Constructor.
    /** The luaL_Reg[] parameters are arrays of lua_CFunction's, used by luaL_register().
     ** @param the_name       Module's name (not the absolute path)
     ** @param the_getters    The getters to the module's fields.
     ** @param the_setters    The setters to the module's fields.
     ** @param the_functions  The module's functions.
     ** @param the_children   The module's child modules.
     */
    ModuleInfo (const std::string& the_name, lua_CFunction the_init_function,
                const FunctionTable& the_function_table,
                const std::list<ModuleInfo*>& the_children,
                lua_CFunction the_constructor = nullptr,
                lua_CFunction the_desctructor = nullptr,
                bool the_class_flag = false);

    /// Gives the module's name.
    /** @return std::string The module's name.
     */
    std::string name () const;
    
    /// Tells if the module is a class or not.
    /** @return bool Whether the module is a class or not.
     */
    bool is_class () const;

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

    /// Gives the module's member getters array.
    /** @return const luaL_Reg* The module's member getters.
     */
    const luaL_Reg* member_getters () const;

    /// Gives the module's member setters array.
    /** @return const luaL_Reg* The module's member setters.
     */
    const luaL_Reg* member_setters () const;

    /// Gives the module's member functions array.
    /** @return const luaL_Reg* The module's methods.
     */
    const luaL_Reg* member_functions () const;

    /// Gives the module's constructor function (if there is one).
    /** @return lua_CFunction The module's constructor.
     */
    lua_CFunction constructor () const;

    /// Gives the module's destructor function (if there is one).
    /** @return lua_CFunction The module's destructor.
     */
    lua_CFunction destructor () const;

    /// Gives red-only access toe the module's submodules.
    /** @return const std::list<ModuleInfo>& The module's children.
     */
    const std::list<ModuleInfo*>& children () const;

  private:

    std::string             name_;
    lua_CFunction           init_function_;
    FunctionTable           function_table_;
    ModuleInfo              *parent_;
    std::list<ModuleInfo*>  children_;
    lua_CFunction           constructor_;
    lua_CFunction           destructor_;
    bool                    is_class_;

    const luaL_Reg* VerifyAndGet (const std::string& name) const;

};

inline ModuleInfo::ModuleInfo (const std::string& the_name, lua_CFunction the_init_function,
                               const FunctionTable& the_function_table,
                               const std::list<ModuleInfo*>& the_children,
                               lua_CFunction the_constructor, lua_CFunction the_desctructor,
                               bool the_class_flag)
    : name_(the_name), init_function_(the_init_function), function_table_(the_function_table),
      parent_(nullptr), children_(the_children), constructor_(the_constructor),
      destructor_(the_desctructor), is_class_(the_class_flag) {
    for (auto& child : children_)
        child->parent_ = this;
}

inline std::string ModuleInfo::name () const {
    return name_;
}

inline bool ModuleInfo::is_class () const {
    return is_class_;
}

inline lua_CFunction ModuleInfo::init_function () const {
    return init_function_;
}

inline const ModuleInfo* ModuleInfo::parent () const {
    return parent_;
}

inline const luaL_Reg* ModuleInfo::functions () const {
    return VerifyAndGet("functions");
}

inline const luaL_Reg* ModuleInfo::getters () const {
    return VerifyAndGet("getters");
}

inline const luaL_Reg* ModuleInfo::setters () const {
    return VerifyAndGet("setters");
}

inline const luaL_Reg* ModuleInfo::member_getters () const {
    return VerifyAndGet("member_getters");
}

inline const luaL_Reg* ModuleInfo::member_setters () const {
    return VerifyAndGet("member_setters");
}

inline const luaL_Reg* ModuleInfo::member_functions () const {
    return VerifyAndGet("member_functions");
}

inline lua_CFunction ModuleInfo::constructor () const {
    return constructor_;
}

inline lua_CFunction ModuleInfo::destructor () const {
    return destructor_;
}

inline const std::list<ModuleInfo*>& ModuleInfo::children () const {
    return children_;
}

inline const luaL_Reg* ModuleInfo::VerifyAndGet (const std::string& name) const {
    auto it = function_table_.find(name);
    if (it == function_table_.end())
      return nullptr;
    return it->second;
}

int ExportModule (State&& L, const ModuleInfo* info);

template <typename T>
inline int Construct (lua_State* L_) {
    State L(L_);
    // Stack: [module]
    L.getmetatable(1);
    // Stack: [module, mttable]
    Converter(L_).TypeToScript(new T);
    // Stack: [module, mttable, newobj]
    L.getfield(2, "__vtable");
    // Stack: [module, mttable, newobj, vtable]
    L.setmetatable(3);
    // Stack: [module, mttable, newobj]
    L.insert(1);
    // Stack: [newobj, ...]
    L.settop(1);
    return 1;
}

template <typename T>
inline int Destruct (lua_State* L_) {
    State L(L_);
    UserData *udata = static_cast<UserData*>(L.touserdata(1));
    L.settop(0);
    delete static_cast<T*>(udata->obj);
    return 0;
}

} // namespace aux
} // namespace lua
} // namespace opa

#endif

