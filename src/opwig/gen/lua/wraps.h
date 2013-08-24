
#ifndef OPWIG_GEN_LUA_WRAPS_H_
#define OPWIG_GEN_LUA_WRAPS_H_

#include <opwig/md/ptr.h>
#include <opa/utils/uncopyable.h>

#include <string>
#include <list>

namespace opwig {
namespace gen {
namespace lua {

struct DataWrap {
    std::string name;
    std::string nesting;
};

struct ModuleWrap : public opa::utils::Uncopyable {

    std::string                     name, path;
    std::list<DataWrap>             functions;
    std::list<DataWrap>             getters;
    std::list<DataWrap>             setters;
    md::WeakPtr<ModuleWrap>         parent;

    /// Tells if the module has any wraps in it.
    /** @return bool Whether there are any wrap.
     */
    bool has_wraps () const;

    /// Tells if the module has any sub-modules in it.
    /** @return Whether there are children modules.
     */
    bool has_children () const;

    /// Gives a read-only access to the module's submodules.
    /** @return const std::list<md::Ptr<ModuleWrap>>&
     **         A read-only reference to the list of submodules.
     */
    const std::list<md::Ptr<ModuleWrap>>& children () const;

    /// Adds a submodule to the module.
    /** @param the_child The new child module.
     */
    void AddChild (const md::Ptr<ModuleWrap>& the_child);

  private:

    friend class WrapperState;

    std::list<md::Ptr<ModuleWrap>>  children_;

    ModuleWrap () {}

};

inline bool ModuleWrap::has_wraps () const {
    return !functions.empty() || !getters.empty() || !setters.empty();
}

inline bool ModuleWrap::has_children () const {
    return !children_.empty();
}

inline const std::list<md::Ptr<ModuleWrap>>& ModuleWrap::children () const {
    return children_;
}

inline void ModuleWrap::AddChild (const md::Ptr<ModuleWrap>& the_child) {
    children_.push_back(the_child);
}

} // namespace lua
} // namespace gen
} // namespace opwig

#endif

