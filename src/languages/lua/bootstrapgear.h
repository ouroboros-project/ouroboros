
#ifndef OUROBOROS_SCRIPT_LUA_BOOTSTRAPGEAR_H_
#define OUROBOROS_SCRIPT_LUA_BOOTSTRAPGEAR_H_

#include <vector>

#include <languages/lua/header.h>
#include <languages/lua/basegear.h>
#include <languages/lua/state.h>

#include <ouroboros/langwrapper.h>
#include <ouroboros/module.h>

namespace ouroboros {
namespace lua {

class DataGear;

typedef ouroboros::Module<lua_CFunction> LuaModule;
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
} /* namespace ouroboros */

#endif /* OUROBOROS_SCRIPT_LUA_BOOTSTRAPGEAR_H_ */
