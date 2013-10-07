#ifndef OUROBOROS_SCRIPT_SCRIPTMANAGER_H_
#define OUROBOROS_SCRIPT_SCRIPTMANAGER_H_

#include <string>
#include <map>

#include <opa/script.h>

#ifdef MODULE_AUTO_LOAD
#define FORCE_LOAD_MODULE(x) void force_link_function_##x(void) { extern int x##_MODULES_HEARTBEAT; x##_MODULES_HEARTBEAT = 1; }
MODULE_AUTO_LOAD(FORCE_LOAD_MODULE)
#endif

namespace opa {

/// Macro to get the ScriptManager singleton instance.
#define SCRIPT_MANAGER() (opa::ScriptManager::ref())

/// OPA Manager - handles the VMs associated with OPA.
/** One of the main classes in OPA, the ScriptManager is a singleton class that
    handles the VMs associated with the system, maintaining a record of them,
    initializing and finalizing them, and so on. */
class ScriptManager {
public:
    /// Gets the manager's singleton instance.
    static ScriptManager* ref() {
        return ref_ ? ref_ : ref_ = new ScriptManager;
    }
    ~ScriptManager() { ref_ = nullptr; }

    /// Gets the path in which scripts are usually loaded from.
    /** Gets the path in which scripts are usually loaded from.
    * @return path in which scripts are usually loaded from. 
    */
    std::string scripts_path() const { return scripts_path_; }

    /// Initializes OPA.
    /** Initializes the manager and all contained VMs.
    * @param scripts_path is the path in which scripts will be usually loaded from.
    */
    bool Initialize(const std::string& scripts_path);
    /// Finalizes OPA.
    /** Finalizes the manager and all contained VMs. */
    void Finalize();

    /// Registers a VM in the manager.
    /** Registers a virtual machine in the manager.
    * @param vm is the virtual machine to be registered.
    * @see opa::VirtualMachine
    */
    void Register(VirtualMachine* vm);
    /// Gets a VM instance with the given name from the manager.
    /** Gets a VM instance with the given name from the manager.
    * @param name is the name of the VM to search.
    * @return the VM instance with given name or null if none was found.
    * @see opa::VirtualMachine
    */
    VirtualMachine* GetMachine(const std::string& name);

    /// Executes a code string in a VM.
    /** Executes a string containing code in a VM. The code should be in the VM's language
        syntax and most likely will have access to anything in the VM - much like a 
        executing a command in an interpreter for the scripting language of the VM.
    * @param language is the name of the VM to execute the code in.
    * @param code is the string to be executed.
    */
    void ExecuteCode(const std::string& language, const std::string& code);

    /// Loads a script module.
    /** Loads a script module from the scripts_path folder, and return a Virtual Object containing it.
    * @param filepath is the path/name of the script to load, without file extensions, using dotted notation.
    * @return a VirtualObj instance containing the module, or an exception if some error occurred.
    * @see opa::VirtualObj
    */
    VirtualObj LoadModule(const std::string& filepath);
    
    /// Checks if a file exists.
    /** Tries to open given file to check if it exists.
    * @param filepath is the file path/name to be checked.
    * @return boolean indicating if file exists (true) or not.
    */
    bool CheckIfFileExists(const std::string& filepath);
    
    /// Converts "folder/subfolder/file" (without extension) style paths to "folder.subfolder.file"
    std::string ConvertPathToDottedNotation(const std::string& path);
    /// Converts dotted notation strings ("folder.subfolder.file") to "folder/subfolder/file".
    std::string ConvertDottedNotationToPath(const std::string& dotted);

private:
    static ScriptManager* ref_;
    ScriptManager();

    std::string scripts_path_;
    typedef std::map<std::string, VirtualMachine*> MachineMap;
    MachineMap machines_;
};

}

#endif /* OUROBOROS_SCRIPT_SCRIPTMANAGER_H_ */
