
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
#ifdef OUROBOROS_LUA_BINDINGS
    if (SCRIPT_MANAGER()->GetWrapper("Lua") == NULL)
        SCRIPT_MANAGER()->Register(new opa::lua::LuaWrapper());
#endif
#ifdef OUROBOROS_PYTHON_BINDINGS
    if (SCRIPT_MANAGER()->GetWrapper("Python") == NULL)
        SCRIPT_MANAGER()->Register(new opa::python::PythonWrapper());
#endif
    SCRIPT_MANAGER()->Initialize("./scripts/");
}

bool RunTalker (const string& which) {
  string      prompt_token = "$";
  size_t      skip_lines   = 1;
  VirtualObj  talker = SCRIPT_MANAGER()->LoadModule(which+"talker");
  if (!talker) return false;
  if (!talker["respond"]) return false;
  if (talker["prompt_token"])
    prompt_token = talker["prompt_token"].value<const char*>();
  if (talker["skip_lines"])
    skip_lines = static_cast<size_t>(talker["skip_lines"].value<int>());
  cout << "Say something!" << endl;
  cout << prompt_token << " ";
  cout.flush();
  string input;
  getline(cin, input);
  for (size_t i = 0; i < skip_lines; ++i)
    cout << endl;
  VirtualObj arg(talker.wrapper());
  arg.set_value<const char*>(input.c_str());
  if (!arg) return false;
  VirtualObj::List args;
  args.push_back(arg);
  if (!args.front()) return false;
  VirtualObj result = talker["respond"](args);
  if (!result) return false;
  return result.value<bool>();
}

} // unnamed namespace

int main(int argc, char **argv) {
    int success = EXIT_SUCCESS;
    InitScripts();
    
#ifdef OUROBOROS_LUA_BINDINGS
    cout << LOGMARK << "Using Lua wrappings and embedding..." << endl;
    if(!RunTalker("lua")) {
        cout << LOGMARK << "Lua wrappings and embedding failed!" << endl;
        success = EXIT_FAILURE;
    } else
        cout << LOGMARK << "Lua wrappings and embedding was successful!" << endl;
#endif

#ifdef OUROBOROS_PYTHON_BINDINGS
    cout << LOGMARK << "Using Python wrappings and embedding..." << endl;
    if(!RunTalker("python")) {
        cout << LOGMARK << "Python wrappings and embedding failed!" << endl;
        success = EXIT_FAILURE;
    } else
        cout << LOGMARK << "Python wrappings and embedding was successful!" << endl;
#endif

    SCRIPT_MANAGER()->Finalize();
    delete SCRIPT_MANAGER();
    
    return success;
}
