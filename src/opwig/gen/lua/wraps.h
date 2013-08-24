
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
    std::list<md::Ptr<ModuleWrap>>  children;
    md::WeakPtr<ModuleWrap>         parent;

    /// Tells if the module has any wraps in it.
    /** @return bool Whether there are any wraps in the module.
     */
    bool has_wraps () const;

    /// Tells if the module has any sub-modules in it.
    /** @return Whether there are children modules in the module.
     */
    bool has_children () const;

  private:

    ModuleWrap () {}

    friend class WrapperState;

};

inline bool ModuleWrap::has_wraps () const {
  return !functions.empty() || !getters.empty() || !setters.empty();
}

inline bool ModuleWrap::has_children () const {
  return !children.empty();
}

} // namespace lua
} // namespace gen
} // namespace opwig

#endif

