
#ifndef OPA_LUA_CONVERTER_H_
#define OPA_LUA_CONVERTER_H_

#include <languages/lua/state.h>
#include <languages/lua/primitive.h>

#include <string>
#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

namespace opa {
namespace lua {

class Converter final {

  public:

    Converter (lua_State *L)
        : L_(L) {}

    Converter (const State& L)
        : L_(L) {}

    /// [0,+1,-]
    template <typename T>
    void TypeToScript (T value) {
        ConversionToScript<T, typename lua_to<T>::PrimitiveType>().Convert(L_, value);
    }

    /// [0,0,-]
    template <typename T>
    T ScriptToType (int index) {
        return ConversionToType<T, typename lua_to<T>::PrimitiveType>().Convert(L_, index);
    }

  private:

    template <typename T, typename PrimitiveType>
    class ConversionToType;

    template <typename T, typename PrimitiveType>
    class ConversionToScript;

    State L_;

};

template <typename T>
class Converter::ConversionToScript<T, std::true_type> final {
    public:
        void Convert (State &L, T value) {
            L.pushprimitive<T>(value);
        }
};

template <typename T>
class Converter::ConversionToScript<T, std::false_type> final {
    public:
        void Convert (State &L, T value) {
            if (value == nullptr) {
              L.pushnil();
              return;
            }
            L.pushudata(sizeof(UserData));
            UserData *udata = static_cast<UserData*>(L.touserdata(-1));
            udata->obj = static_cast<void*>(value);
            udata->type = typeid(T);
        }
};

template <typename T>
class Converter::ConversionToType<T, std::true_type> final {
  public:
    T Convert (State &L, int index) {
        if (index < 0 || index > L.gettop())
            throw std::runtime_error("invalid stack index "+std::to_string(index));
        if (!L.isprimitive<T>(index))
            throw std::runtime_error("type mismatch at index "+std::to_string(index));
        return L.toprimitive<T>(index);
    }
};

template <typename T>
class Converter::ConversionToType<T, std::false_type> final {
  public:
    T Convert (State &L, int index) {
        if (index < 0 || index > L.gettop())
            throw std::runtime_error("invalid stack index "+std::to_string(index));
        if (!L.isuserdata(index))
            throw std::runtime_error("stack index "+std::to_string(index)+" is not an userdata");
        UserData *udata = static_cast<UserData*>(L.touserdata(index));
        if (!udata || udata->type != typeid(T))
            throw std::runtime_error("type mismatch at index "+std::to_string(index));
        return static_cast<T>(udata->obj);
    }
};

}
} // namespace opa

#endif // OPA_LUA_CONVERTER_H_

