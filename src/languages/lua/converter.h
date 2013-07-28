
#ifndef OPA_LUA_CONVERTER_H_
#define OPA_LUA_CONVERTER_H_

#include <opa/converter.h>

#include <languages/lua/defs.h>
#include <opa/virtualobj.h>

namespace opa {
namespace lua {

class Converter final /* : public ::opa::Converter<const StackHook&> */ {

  public:

    Converter (lua_State *L);

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

    CONVERTER_TEMPLATE_TO_SCRIPT(int) {
        VirtualObj vobj(wrapper_);
        vobj.set_value<T>(value);
        return this->ExportVObj(vobj);
    }

    CONVERTER_TEMPLATE_FROM_SCRIPT(int) {
        return this->ImportVObj(value).value<T>();
    }

  private:

    lua_State   *L_;
    LuaWrapper  *wrapper_;

    VirtualObj ImportVObj (int stack_index);
    int ExportVObj (const VirtualObj& vobj);

};

// No need for now.
// PRIMITIVE_CONVERTER_TEMPLATES(Converter, const StackHook&)

}
} // namespace opa

#endif // OPA_LUA_CONVERTER_H_

