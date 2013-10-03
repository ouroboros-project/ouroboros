
#ifndef OUROBOROS_SCRIPT_LUA_BOOTSTRAPGEAR_H_
#define OUROBOROS_SCRIPT_LUA_BOOTSTRAPGEAR_H_

#include <vector>

#include <languages/lua/header.h>
#include <languages/lua/basegear.h>
#include <languages/lua/state.h>

#include <opa/virtualmachine.h>
#include <opa/module.h>

namespace opa {
namespace lua {

class DataGear;

typedef opa::Module<lua_CFunction> LuaModule;
typedef const std::vector<LuaModule> ModuleList;

class BootstrapGear : public BaseGear {

  public:

    BootstrapGear() :
        BaseGear(nullptr) {}

    ~BootstrapGear() {}

    bool Initialize(const ModuleList& modules);

    DataGear* NextGear();

    void Abort();

  private:

    static int SafeInitialize(lua_State* L);

    void LoadLibs();

    void PreloadModules(const ModuleList& modules);

};

} /* namespace lua */
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_LUA_BOOTSTRAPGEAR_H_ */
