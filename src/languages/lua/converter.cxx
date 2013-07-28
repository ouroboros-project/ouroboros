
#include <languages/lua/converter.h>
#include <languages/lua/luadata.h>
#include <languages/lua/luawrapper.h>
#include <opa/scriptmanager.h>

#include <lua5.1/lauxlib.h>

#include <stdexcept>

namespace opa {
namespace lua {

Converter::Converter (lua_State *L) : L_(L), wrapper_(nullptr) {
    wrapper_ = dynamic_cast<LuaWrapper*>(SCRIPT_MANAGER()->GetWrapper("Lua"));
    if (!wrapper_) throw std::runtime_error("NO LUA WRAPPER DETECTED");
}

VirtualObj Converter::ImportVObj (int stack_index) {
    LuaData *data = wrapper_->NewLuaData();
    lua_pushvalue(L_, stack_index);
    data->UnsafePopValue();
    return VirtualObj(VirtualData::Ptr(data));
}

int Converter::ExportVObj (const VirtualObj& vobj) {
    LuaData *data = static_cast<LuaData*>(vobj.unsafe_data());
    lua_settop(L_, 0);
    data->UnsafePushValue();
    return 1;
}

} // namespace lua
} // namespace opa

