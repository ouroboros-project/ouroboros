
#ifndef OPA_LUA_WRAP_FUNCTION_H_
#define OPA_LUA_WRAP_FUNCTION_H_

#include <languages/lua/wrap/helpers.h>

#include <languages/lua/converter.h>
#include <languages/lua/state.h>

#include <type_traits>
#include <utility>
#include <string>

namespace opa {
namespace lua {
namespace wrap {

template <typename R, typename... Args>
class NativeCall {
  private:
    template <typename Ind, bool void_flag>
    struct Helper;
  public:
    using FunctionPtr = R (*) (Args...);
    static int Run (State&& L) {
      FunctionPtr func = reinterpret_cast<FunctionPtr>(L.touserdata(lua_upvalueindex(1)));
      Helper<typename MakeIndexes<sizeof...(Args)>::type, std::is_void<R>::value>::Call(L, func);
      return 1;
    }
  private:
    template <size_t... I, template<size_t...> class Ind>
    struct Helper<Ind<I...>, false> {
      static void Call (State& L, FunctionPtr func) {
        Converter conv(L);
        conv.TypeToScript<R>(func(conv.ScriptToType<typename TypeAt<I, Args...>::type>(I+1)...));
      }
    };
    
    template <size_t... I, template<size_t...> class Ind>
    struct Helper<Ind<I...>, true> {
      static void Call (State& L, FunctionPtr func) {
        Converter conv(L);
        func(conv.ScriptToType<typename TypeAt<I, Args...>::type>(I+1)...);
      }
    };
};

template <typename Signature>
class Function;

template <typename R, typename... Args>
class Function<R (Args...)> {
  public:
    static int Wrap (lua_State *L) {
      return NativeCall<R, Args...>::Run(L);
    }
};

template <typename Signature>
inline void PushNativeFunction (State& L, Signature* func) {
  L.pushudata(reinterpret_cast<void*>(func));
  L.pushcfunction(&Function<Signature>::Wrap, 1);
}

template <typename... Entries>
class FunctionList;

template <typename Signature, typename... Entries>
class FunctionList<std::pair<std::string, Signature>, Entries...> {
  public:
    using Entry = std::pair<std::string, Signature>;
    static void Insert (State& L, int index, const Entry& entry, Entries... entries) {
      PushNativeFunction(L, entry.second);
      L.setfield(index, entry.first);
      FunctionList<Entries...>::Insert(L, index, entries...);
    }
};

template <>
class FunctionList<> {
  public:
    static void Insert (State& L, int index) {}
};

template <typename... Entries>
inline void InsertNativeFunctions (State& L, int index, Entries... entries) {
  FunctionList<Entries...>::Insert(L, index, entries...);
}

} // namespace wrap
} // namespace lua
} // namespace opa

#endif // OPA_LUA_WRAP_FUNCTION_H_

