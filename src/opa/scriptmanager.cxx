#include <opa/scriptmanager.h>

#include <cstdio>
#include <algorithm>

#include <opa/virtualmachine.h>
#include <opa/virtualobj.h>
#include <opa/exceptions.h>

namespace opa {

using std::string;

ScriptManager* ScriptManager::ref_ = nullptr;

ScriptManager::ScriptManager() {}

bool ScriptManager::Initialize(const std::string& scripts_path) {
    bool is_ok = true;

    scripts_path_ = scripts_path;

    MachineMap::iterator it = machines_.begin();
    while (it != machines_.end()) {
        VirtualMachine* vm = it->second;
        is_ok = is_ok && vm->Initialize();
        ++it;
    }

    return is_ok;
}

void ScriptManager::Finalize() {
    MachineMap::iterator it = machines_.begin();
    while (it != machines_.end()) {
        VirtualMachine* vm = it->second;
        vm->Finalize();
        delete vm;
        ++it;
    }
    machines_.clear();
}

void ScriptManager::Register(VirtualMachine* vm) {
    if (machines_.count(vm->lang_name())) return;
    machines_[vm->lang_name()] = vm;
}

VirtualMachine* ScriptManager::GetMachine(const string& name) {
    // TODO: avoid double search
    if (!machines_.count(name)) return nullptr;
    return machines_[name];
}

void ScriptManager::ExecuteCode(const string& language, const string& code) {
    if (!machines_.count(language)) {
        throw InvalidVMError(language, "this VM does not exist.");
        return;
    }
    
    machines_[language]->ExecuteCode(code);
}


VirtualObj ScriptManager::LoadModule(const string& script) {
    string filepath = scripts_path_ + ConvertDottedNotationToPath(script);

    MachineMap::iterator it = machines_.begin();
    while (it != machines_.end()) {
        VirtualMachine* vm = it->second;
        if ( CheckIfFileExists(filepath + "." + vm->file_extension()) ) {
            return vm->LoadModule( script );
        }
        ++it;
    }
    throw IOError(filepath, "opening");
    return VirtualObj();
}

bool ScriptManager::CheckIfFileExists(const string& filepath) {
    FILE* file = fopen(filepath.c_str(), "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

/// Converts "folder/subfolder/file" (without extension) style paths to "folder.subfolder.file"
std::string ScriptManager::ConvertPathToDottedNotation(const std::string& path) {
    string dotted( path );
    replace(dotted.begin(), dotted.end(), '/', '.');
    return dotted;
}

/// Converts dotted notation strings ("folder.subfolder.file") to "folder/subfolder/file".
std::string ScriptManager::ConvertDottedNotationToPath(const std::string& dotted) {
    string path( dotted );
    replace(path.begin(), path.end(), '.', '/');
    return path;
}

}
