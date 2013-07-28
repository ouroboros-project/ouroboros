
#include <languages/lua/converter.h>
#include <languages/lua/luadata.h>
#include <languages/lua/luawrapper.h>
#include <opa/scriptmanager.h>

#include <lua5.1/lauxlib.h>

#include <stdexcept>

namespace opa {
namespace lua {

Converter::Converter () : wrapper_(nullptr) {
    wrapper_ = dynamic_cast<LuaWrapper*>(SCRIPT_MANAGER()->GetWrapper("Lua"));
    if (!wrapper_) throw std::runtime_error("NO LUA WRAPPER DETECTED");
}

VirtualObj Converter::MakeVObj (const StackHook& t) {
    LuaData *data = wrapper_->NewLuaData();
    lua_pushvalue(t.L, t.index);
    data->UnsafePopValue();
    return VirtualObj(VirtualData::Ptr(data));
}

} // namespace lua
} // namespace opa

