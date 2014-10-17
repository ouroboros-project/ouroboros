
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

template <typename T, bool IsPrimitive>
class NativeGetter;

template <typename T>
class NativeGetter<T, true> {
  public:
    static int Run (State &&L) {
        auto variable_ptr = static_cast<const T*>(
            L.touserdata(lua_upvalueindex(1)));
        Converter(L).TypeToScript(*variable_ptr);
        return 1;
    }
};

template <typename T>
class NativeGetter<T, false> {
  public:
    static int Run (State &&L) {
        auto variable_ptr = static_cast<const T*>(
            L.touserdata(lua_upvalueindex(1)));
        Converter(L).TypeToScript<const T&>(*variable_ptr);
        return 1;
    }
};

template <typename T>
class VariableGetter final : public Pushable {
  public:
    VariableGetter(const std::string& the_name, const T* the_variable_ptr)
        : Pushable(the_name), variable_ptr_(the_variable_ptr) {}
    void PushOnto(State &L) const override {
        L.pushudata(static_cast<const void*>(variable_ptr_));
        L.pushcfunction(&Wrap, 1);
    }
  private:
    static int Wrap (lua_State *L) {
        return NativeGetter<T, lua_to<T>::PrimitiveType::value>::Run(L);
    }
    const T *variable_ptr_;
};

} // namespace wrap
} // namespace lua
} // namespace opa

#endif // OPA_LUA_WRAP_VARIABLE_H_

