
#ifndef OUROBOROS_SCRIPT_VIRTUALMACHINE_H_
#define OUROBOROS_SCRIPT_VIRTUALMACHINE_H_

#include <string>

#include <opa/script.h>

#include <opa/defs.h>
#include <opa/type.h>
#include <opa/virtualdata.h>
#include <opa/module.h>

namespace opa {

/// Abstract class representing a Virtual Machine in OPA.
/** This abstract class represents a Virtual Machine in OPA, which is the VM that
    executes a particular scripting language (and is basically the language itself).
    This class should be implemented for a scripting language in order for OPA to support the language.
    However, when implementing it, derive the implementation from InheritableVirtualMachine.
* @see opa::InheritableVirtualMachine
*/
class VirtualMachine {

  public:

    virtual ~VirtualMachine() {}

    /// Gets the file extension of this VM.
    /** Gets the file extension (without the '.') of script files for this scripting language.
    * @return the file extension, without the '.' at the start.
    */
    const std::string& file_extension() { return file_extension_; }

    /// Initializes the VirtualMachine.
    /** This is used to initialize the script language's API, if needed.
     ** @return bool : informing whether the initialization was successful.
     */
    virtual bool Initialize() = 0;

    /// Finalizes the VirtualMachine.
    /** Finalizes the VirtualMachine, releasing the resources used by the language. */
    virtual void Finalize() = 0;

    /// Gets a new, empty VirtualData instance associated with this VM.
    /** Gets a new, empty VirtualData instance associated with this VM.
    * @return a new VirtualData instance.
    */
    virtual VirtualData::Ptr NewData() = 0;

    /// Executes code in the VM.
    /** Executes a code string inside the VM. It will be executed at a default
        scope inside the VM and should have access to all features of the language.
    * @param code is the string containing the code to be executed - it should follow
            the script language syntax.
    */
    virtual void ExecuteCode(const std::string& code) = 0;

    /// Loads a script module for this VM.
    /** Loads a script module for this VM. 
    * @param name is the path/name of the script to load, without file extensions, using dotted notation.
    * @return A VirtualObj containing the module.
    * @see opa::VirtualObj
    */
    virtual VirtualObj LoadModule(const std::string& name) = 0;

    /// Gets the language ID for this VM.
    /** Gets the language ID for this VM.
    * @see opa::LangID
    */
    const LangID lang_id () { return lang_id_; }
    /// Gets the name of this VM.
    /** Gets the name of this VM, which is the name of the scripting language it executes.
    * @return the name of this VM.
    */
    const std::string& lang_name() const { return lang_name_; }

  protected:

  private:

    template <class loader_t>
    friend class InheritableVirtualMachine;

    const std::string file_extension_;
    const LangID      lang_id_;
    const std::string lang_name_;

    VirtualMachine(const std::string& file_extension, const LangID id, const std::string& name) :
        file_extension_(file_extension),
        lang_id_(id),
        lang_name_(name) {}

    VirtualMachine& operator=(const VirtualMachine& rhs);

};

/// Inheritable abstract templatized class that represents a VirtualMachine.
/** This abstract templatized class derives from VirtualMachine, extending it
    to contain a record of registered modules. When implementing a VirtualMachine
    to a scripting language, the implementation should derive from a specialization
    of this abstract templatized class, where the template argument is the signature
    of the initialization function for modules in this language.

    Wrapper generators, such as OPWIG, generate wrapper modules for a given language,
    and these wrapper modules usually contain a 'initialization function' to create
    the module in the VM. These generated modules should be registered in the VM 
    so that they are initialized along with it. However, since the signature of this
    initialization function varies from language to language, we have this templatized
    class to solve this.

    Note that OPWIG generated modules register themselves automatically.
* @tparam loader_t signature of the initialization function for modules in this language.
* @see opa::VirtualMachine
*/
template <class loader_t>
class InheritableVirtualMachine : public VirtualMachine {
  public:
    /// Registers a (generated) module with the VM.
    /** Registers a module with this VM, so that its initialization function
        is executed when the VM initializes. 
    * @param module is the module to be registered
    * @return boolean indicating if the module was registered successfully (true) or not.
    * @see opa::Module
    */
    bool RegisterModule(const Module<loader_t>& module) {
        if (module.name().empty())
            return false;
        // TODO: check if name is valid.
        modules_.push_back(module);
        return true;
    }

  protected:

    std::vector<Module<loader_t>> modules_;

    InheritableVirtualMachine(const std::string& file_extension, const LangID id,
                           const std::string& name)
        : VirtualMachine(file_extension, id, name) {}

};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_VIRTUALMACHINE_H_ */
