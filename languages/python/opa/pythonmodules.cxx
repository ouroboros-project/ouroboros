#include <python/opa/modules.h>

#include <vector>
#include <cstdio>

#include <opa/module.h>
#include <python/opa/pythonmachine.h>

namespace opa {
namespace python {

typedef std::vector< Module<inittype> > PythonModuleList;

static PythonModuleList& get_module_list() {
    static PythonModuleList lua_modules;
    return lua_modules;
}

void AddModule(const Module<inittype> & module) {
    get_module_list().push_back(module);
}

void RegisterModules(PythonMachine* machine) {
    PythonModuleList& lua_modules = get_module_list();
    for(PythonModuleList::const_iterator it = lua_modules.begin(); it != lua_modules.end(); ++it) {
        if(!machine->RegisterModule(*it)) {
            fprintf(stderr, "[%s] Load module '%s': >>ERROR<<\n", machine->lang_name().c_str(), it->name().c_str());
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%s] Load module '%s': ok\n", machine->lang_name().c_str(), it->name().c_str());
#endif
        }
    }
}

} /* namespace python */
} /* namespace opa */
