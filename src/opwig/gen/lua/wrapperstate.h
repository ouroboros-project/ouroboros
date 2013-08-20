
#ifndef OPWIG_GEN_LUA_WRAPPERSTATE_H_
#define OPWIG_GEN_LUA_WRAPPERSTATE_H_

#include <opa/utils/uncopyable.h>
#include <opwig/gen/lua/wraps.h>

#include <string>
#include <list>

namespace opwig {
namespace gen {
namespace lua {

struct ModuleWrap;

class WrapperState : public opa::utils::Uncopyable {
  public:
    md::Ptr<ModuleWrap> current_module () const;
    std::string StackAsString (const std::string& sep, size_t skip = 0) const;
    void PushModule (const md::Ptr<ModuleWrap>& the_module);
    void PopModule ();
  private:
    std::list<md::Ptr<ModuleWrap>> stack_;
};

inline md::Ptr<ModuleWrap> WrapperState::current_module () const {
    return stack_.back();
}

inline void WrapperState::PushModule (const md::Ptr<ModuleWrap>& the_module) {
    stack_.push_back(the_module);
}

inline void WrapperState::PopModule () {
    stack_.pop_back();
}

} // namespace lua
} // namespace gen
} // namespace opwig

#endif

