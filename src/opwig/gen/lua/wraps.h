
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

struct ModuleWrap {
    std::string                     name, path;
    std::list<DataWrap>             functions;
    std::list<DataWrap>             getters;
    std::list<DataWrap>             setters;
    std::list<md::Ptr<ModuleWrap>>  children;
    md::WeakPtr<ModuleWrap>         parent;
    bool                            open;
};

class WrapsManager : public opa::utils::Uncopyable {
  public:
    md::Ptr<ModuleWrap> current_module () const;
    std::string StackAsString (const std::string& sep, size_t skip = 0) const;
    void PushModule (const md::Ptr<ModuleWrap>& the_module);
    void PopModule ();
  private:
    std::list<md::Ptr<ModuleWrap>> stack_;
};

inline md::Ptr<ModuleWrap> WrapsManager::current_module () const {
    return stack_.back();
}

inline std::string WrapsManager::StackAsString (const std::string& sep, size_t skip) const {
  std::string result;
  for (auto module : stack_) {
    if (skip > 0) {
      --skip;
      continue;
    }
    result += module->name+sep;
  }
  return result;
}

inline void WrapsManager::PushModule (const md::Ptr<ModuleWrap>& the_module) {
    stack_.push_back(the_module);
}

inline void WrapsManager::PopModule () {
    stack_.pop_back();
}

} // namespace lua
} // namespace gen
} // namespace opwig

#endif

