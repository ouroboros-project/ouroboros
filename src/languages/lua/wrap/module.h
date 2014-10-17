
#ifndef OPA_LUA_WRAP_MODULE_H_
#define OPA_LUA_WRAP_MODULE_H_

#include <opa/utils/uncopyable.h>

#include <languages/lua/converter.h>
#include <languages/lua/state.h>
#include <languages/lua/wrap/pushable.h>

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <memory>

namespace opa {
namespace lua {
namespace wrap {

/// Represents information about a Lua module to be initialized.
/** Mainly used by opa::lua::wrap::ExportModule.
 ** @see opa::lua::wrap::ExportModule
 */
class ModuleInfo : public utils::Uncopyable {

  public:

    // I hope someday I can use unique_ptrs for the Pushables here.
    using FunctionList = std::vector<std::shared_ptr<Pushable>>;
    using FunctionTable = std::unordered_map<std::string, FunctionList>;

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
    /** @return const FunctionList& The module's functions.
     */
    const FunctionList& functions () const;

    /// Gives the module's getters array.
    /** @return const FunctionList& The module's getters.
     */
    const FunctionList& getters () const;

    /// Gives the module's setter array.
    /** @return const FunctionList& The module's setters.
     */
    const FunctionList& setters () const;

    /// Gives the module's member getters array.
    /** @return const FunctionList& The module's member getters.
     */
    const FunctionList& member_getters () const;

    /// Gives the module's member setters array.
    /** @return const FunctionList& The module's member setters.
     */
    const FunctionList& member_setters () const;

    /// Gives the module's member functions array.
    /** @return const FunctionList& The module's methods.
     */
    const FunctionList& member_functions () const;

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

};

inline ModuleInfo::ModuleInfo (const std::string& the_name, lua_CFunction the_init_function,
                               const FunctionTable& the_function_table,
                               const std::list<ModuleInfo*>& the_children,
                               lua_CFunction the_constructor, lua_CFunction the_desctructor,
                               bool the_class_flag)
    : name_(the_name), init_function_(the_init_function),
      function_table_(std::move(the_function_table)), parent_(nullptr), children_(the_children),
      constructor_(the_constructor), destructor_(the_desctructor), is_class_(the_class_flag) {
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

inline const ModuleInfo::FunctionList& ModuleInfo::functions () const {
    return function_table_.at("functions");
}

inline const ModuleInfo::FunctionList& ModuleInfo::getters () const {
    return function_table_.at("getters");
}

inline const ModuleInfo::FunctionList& ModuleInfo::setters () const {
    return function_table_.at("setters");
}

inline const ModuleInfo::FunctionList& ModuleInfo::member_getters () const {
    return function_table_.at("member_getters");
}

inline const ModuleInfo::FunctionList& ModuleInfo::member_setters () const {
    return function_table_.at("member_setters");
}

inline const ModuleInfo::FunctionList& ModuleInfo::member_functions () const {
    return function_table_.at("member_functions");
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

} // namespace wrap
} // namespace lua
} // namespace opa

#endif // OPA_LUA_WRAP_MODULE_H_

