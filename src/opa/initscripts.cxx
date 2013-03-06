#include <opa/script.h>

#include <opa/config.h>
#include <opa/scriptmanager.h>
#include <languages/lua/luawrapper.h>
#include <languages/python/pythonwrapper.h>

void force_link_function_opa_internal(void) { 
    extern int OUROBOROS_MODULES_HEARTBEAT; 
    OUROBOROS_MODULES_HEARTBEAT = 1;
}

namespace opa {

void InitScripts() {
#ifdef OUROBOROS_LUA_ENABLED
    SCRIPT_MANAGER()->Register(new opa::lua::LuaWrapper());
#endif
#ifdef OUROBOROS_PYTHON_ENABLED
    SCRIPT_MANAGER()->Register(new opa::python::PythonWrapper());
#endif
}

} // namespace opa
