
#ifndef OPWIG_GEN_LUA_WRAPPERSTATE_H_
#define OPWIG_GEN_LUA_WRAPPERSTATE_H_

#include <lua/opwig/wraps.h>
#include <lua/opwig/codes.h>

#include <string>
#include <list>

namespace opwig {
namespace md {
class Function;
class Variable;
} // namespace md
} // namespace opwig

namespace opa {
namespace lua {

struct ModuleWrap;

class WrapperState {

  public:

    /// Constructor.
    /** @param the_module_name Name of the module being wrapped.
     */
    WrapperState (const std::string& the_module_name);
    WrapperState(const WrapperState&) = delete;
    WrapperState& operator=(const WrapperState&) = delete;

    opwig::md::Ptr<ModuleWrap> current_module () const;

    bool is_current_class () const;

    std::string StackAsString (const std::string& sep, size_t skip = 0) const;

    void PushModule (const opwig::md::Ptr<ModuleWrap>& the_module);

    void PushModule (const std::string& module_name, bool is_class_flag = false);

    void PopModule ();

    void AddFunction (const opwig::md::Ptr<const opwig::md::Function>& the_function);

    void AddVariableGetter (const opwig::md::Ptr<const opwig::md::Variable>& the_variable);

    void AddVariableSetter (const opwig::md::Ptr<const opwig::md::Variable>& the_variable);

  private:

    std::list<opwig::md::Ptr<ModuleWrap>> stack_;

    void AddWrap (const std::string& name, WrappedMember member, WrappedMember nonmember);

};

inline opwig::md::Ptr<ModuleWrap> WrapperState::current_module () const {
    return stack_.back();
}

inline bool WrapperState::is_current_class () const {
    return current_module()->is_class();
}

inline void WrapperState::PushModule (const opwig::md::Ptr<ModuleWrap>& the_module) {
    stack_.push_back(the_module);
}

inline void WrapperState::PopModule () {
    stack_.pop_back();
}

} // namespace lua
} // namespace opa

#endif

