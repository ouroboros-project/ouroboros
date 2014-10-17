
#ifndef OPA_LUA_WRAP_VARIABLE_H_
#define OPA_LUA_WRAP_VARIABLE_H_

#include <languages/lua/wrap/helpers.h>
#include <languages/lua/wrap/pushable.h>

#include <languages/lua/converter.h>
#include <languages/lua/state.h>

#include <type_traits>
#include <utility>
#include <string>

namespace opa {
namespace lua {
namespace wrap {

template <typename T>
class VariableGetter final : public Pushable {
  public:
    VariableGetter(const std::string& the_name, const T* the_variable_ptr)
        : Pushable(the_name), variable_ptr_(the_variable_ptr) {}
    void PushOnto(State &L) const override {
        L.pushudata(reinterpret_cast<const void*>(variable_ptr_));
        L.pushcfunction(&Wrap, 1);
    }
  private:
    static int Wrap (lua_State *L_) {
        State L(L_);
        auto the_variable_ptr = reinterpret_cast<const T*>(
            L.touserdata(lua_upvalueindex(1)));
        Converter(L).TypeToScript<T>(*the_variable_ptr);
        return 1;
    }
    const T *variable_ptr_;
};

} // namespace wrap
} // namespace lua
} // namespace opa

#endif // OPA_LUA_WRAP_VARIABLE_H_

