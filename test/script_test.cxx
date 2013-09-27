
#include <opa/config.h>

#include <opa/script.h>
#include <opa/scriptmanager.h>
#include <opa/virtualobj.h>
#include <languages/lua/luamachine.h>
#include <languages/python/pythonmachine.h>

#include <cstdlib>

#include <memory>

using opa::VirtualObj;
using opa::ScriptManager;

using std::shared_ptr;

static void InitScripts () {
#ifdef OUROBOROS_LUA_BINDINGS
    SCRIPT_MANAGER()->Register(new opa::lua::LuaMachine());
#endif
#ifdef OUROBOROS_PYTHON_BINDINGS
    SCRIPT_MANAGER()->Register(new opa::python::PythonMachine());
#endif
    SCRIPT_MANAGER()->Initialize("./data/scripts/");
}

static bool LuaTests() {
    return SCRIPT_MANAGER()->LoadModule("main").valid();
}

static bool PythonTests() {
    VirtualObj wassup = SCRIPT_MANAGER()->LoadModule("wassup");
    if(!wassup) return false;
    if(!wassup["supimpa"]) return false;

    VirtualObj::List args;
    return wassup["supimpa"](args).valid();
}

int main(int argc, char **argv) {

    InitScripts();
    
#ifdef OUROBOROS_LUA_BINDINGS
    if(!LuaTests()) {
        puts("LUA FAILED!");
        return EXIT_FAILURE;
    };
#endif

#ifdef OUROBOROS_PYTHON_BINDINGS
    if(!PythonTests()) {
        puts("PYTHON FAILED!");
        return EXIT_FAILURE;
    }
#endif

    SCRIPT_MANAGER()->Finalize();
    delete SCRIPT_MANAGER();
    
    return EXIT_SUCCESS;
}
