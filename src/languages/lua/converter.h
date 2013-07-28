
#ifndef OPA_LUA_CONVERTER_H_
#define OPA_LUA_CONVERTER_H_

#include <opa/converter.h>

#include <languages/lua/defs.h>
#include <opa/virtualobj.h>

namespace opa {
namespace lua {

struct StackHook {
  lua_State *L;
  int       index;
};

inline StackHook Hook (lua_State *L, int index) {
  StackHook hook = { L, index };
  return hook;
}

class Converter final /* : public ::opa::Converter<const StackHook&> */ {

  public:

    Converter ();

    //bool ScriptToBool (const StackHook& t) override;
    //short ScriptToShort (const StackHook& t) override;
    //int ScriptToInt (const StackHook& t) override;
    //float ScriptToFloat (const StackHook& t) override;
    //double ScriptToDouble (const StackHook& t) override;
    //char ScriptToChar (const StackHook& t) override;
    //const char* ScriptToCStr (const StackHook& t) override;

    //const StackHook& BoolToScript (bool value) override;
    //const StackHook& ShortToScript (short value) override;
    //const StackHook& IntToScript (int value) override;
    //const StackHook& FloatToScript (float value) override;
    //const StackHook& DoubleToScript (double value) override;
    //const StackHook& CharToScript (char value) override;
    //const StackHook& CStrToScript (const char* value) override;

    CONVERTER_TEMPLATE_TO_SCRIPT(const StackHook&);
    CONVERTER_TEMPLATE_FROM_SCRIPT(const StackHook&) {
        return MakeVObj(value).value<T>();
    }

  private:

    LuaWrapper *wrapper_;

    VirtualObj MakeVObj (const StackHook& hook);

};

// No need for now.
// PRIMITIVE_CONVERTER_TEMPLATES(Converter, const StackHook&)

}
} // namespace opa

#endif // OPA_LUA_CONVERTER_H_

