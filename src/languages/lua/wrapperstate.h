
#ifndef OPWIG_GEN_LUA_WRAPPERSTATE_H_
#define OPWIG_GEN_LUA_WRAPPERSTATE_H_

#include <opa/utils/uncopyable.h>
#include <languages/lua/wraps.h>
#include <languages/lua/codes.h>

#include <string>
#include <list>

namespace opwig {

namespace md {
class Function;
class Variable;
} // namespace md

namespace gen {
namespace lua {

struct ModuleWrap;

class WrapperState : public opa::utils::Uncopyable {

  public:

    /// Constructor.
    /** @param the_module_name Name of the module being wrapped.
     */
    WrapperState (const std::string& the_module_name);

    md::Ptr<ModuleWrap> current_module () const;

    bool is_current_class () const;

    std::string StackAsString (const std::string& sep, size_t skip = 0) const;

    void PushModule (const md::Ptr<ModuleWrap>& the_module);

    void PushModule (const std::string& module_name, bool is_class_flag = false);

    void PopModule ();

    void AddFunction (const md::Ptr<const md::Function>& the_function);

    void AddVariableGetter (const md::Ptr<const md::Variable>& the_variable);

    void AddVariableSetter (const md::Ptr<const md::Variable>& the_variable);

  private:

    std::list<md::Ptr<ModuleWrap>> stack_;

    void AddWrap (const std::string& name, WrappedMember member, WrappedMember nonmember);

};

inline md::Ptr<ModuleWrap> WrapperState::current_module () const {
    return stack_.back();
}

inline bool WrapperState::is_current_class () const {
    return current_module()->is_class();
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

