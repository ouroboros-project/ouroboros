
#ifndef OPA_LUA_CONVERTER_H_
#define OPA_LUA_CONVERTER_H_

#include <languages/lua/state.h>
#include <languages/lua/aux/exportmodule.h>

#include <string>
#include <stdexcept>
#include <typeinfo>
#include <typeindex>

namespace opa {
namespace lua {

class Converter final {

  public:

    Converter (lua_State *L)
        : L_(L) {}

    /// [0,+1,-]
    template <typename T>
    void TypeToScript (T value);

    /// [0,0,-]
    template <typename T>
    T ScriptToType (int index);

  private:

    State       L_;

};

template <typename T>
inline void Converter::TypeToScript (T value) {
    L_.pushprimitive<T>(value);
}

template <typename T>
inline T Converter::ScriptToType (int index) {
    if (index < 0 || index > L_.gettop())
        throw std::runtime_error("invalid stack index "+std::to_string(index));
    if (!L_.isprimitive<T>(index)) {
        aux::UserData *udata = static_cast<aux::UserData*>(L_.touserdata(index));
        if (udata->type != typeid(T))
            throw std::runtime_error("type mismatch at index "+std::to_string(index));
        return static_cast<T>(udata->obj);
    }
    return L_.toprimitive<T>(index);
}

template <>
inline bool Converter::ScriptToType<bool> (int index) {
    if (index < 0 || index > L_.gettop())
        throw std::runtime_error("invalid stack index "+std::to_string(index));
    return L_.toboolean(index);
}

template <>
inline void* Converter::ScriptToType<void*> (int index) {
    if (index < 0 || index > L_.gettop())
        throw std::runtime_error("invalid stack index "+std::to_string(index));
    return L_.touserdata(index);
}

}
} // namespace opa

#endif // OPA_LUA_CONVERTER_H_

