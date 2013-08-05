
#include <opa/config.h>

#include <opa/script.h>
#include <opa/scriptmanager.h>
#include <opa/virtualobj.h>
#include <languages/lua/luawrapper.h>
#include <languages/python/pythonwrapper.h>

#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>

using opa::VirtualObj;
using opa::ScriptManager;

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::shared_ptr;

namespace {

const string LOGMARK = "[--LOG--] ";

void InitScripts () {
    SCRIPT_MANAGER()->Initialize("./scripts/");
}

bool RunTalker (const string& which, const string& name) {
    VirtualObj  talker = SCRIPT_MANAGER()->LoadModule(which+"talker");
    if (!talker) return false;
    if (!talker["main"]) return false;
    VirtualObj arg(talker.wrapper());
    arg.set_value<const char*>(name.c_str());
    if (!arg) return false;
    VirtualObj::List args;
    args.push_back(arg);
    if (!args.front()) return false;
    VirtualObj result = talker["main"](args);
    if (!result) return false;
    return result.value<bool>();
}

} // unnamed namespace

int main(int argc, char **argv) {
    int success = EXIT_SUCCESS;
    InitScripts();

    if (argc < 2) {
        cout << LOGMARK << "Missing argument." << endl;
        return EXIT_FAILURE;
    }
    
#ifdef OUROBOROS_LUA_BINDINGS
    cout << LOGMARK << "Using Lua wrappings and embedding..." << endl;
    if(!RunTalker("lua", argv[1])) {
        cout << LOGMARK << "Lua wrappings and embedding failed!" << endl;
        success = EXIT_FAILURE;
    } else
        cout << LOGMARK << "Lua wrappings and embedding was successful!" << endl;
#endif

#ifdef OUROBOROS_PYTHON_BINDINGS
    cout << LOGMARK << "Using Python wrappings and embedding..." << endl;
    if(!RunTalker("python", argv[1])) {
        cout << LOGMARK << "Python wrappings and embedding failed!" << endl;
        success = EXIT_FAILURE;
    } else
        cout << LOGMARK << "Python wrappings and embedding was successful!" << endl;
#endif

    SCRIPT_MANAGER()->Finalize();
    delete SCRIPT_MANAGER();
    
    return success;
}
