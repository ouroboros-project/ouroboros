#ifndef OUROBOROS_SCRIPT_PYTHON_PYTHONMACHINE_H_
#define OUROBOROS_SCRIPT_PYTHON_PYTHONMACHINE_H_

#include <string>
#include <vector>
#include <opa/virtualmachine.h>

namespace opa {

class VirtualObj;

namespace python {

typedef void (*PyInitFunction)(void);
typedef opa::Module<PyInitFunction> PythonModule;
typedef opa::InheritableVirtualMachine<PyInitFunction> PythonMachineBase;
typedef opa::python::PyInitFunction inittype;

#define PYTHON_INIT_FUNCTION_NAME(name) init_##name
#define PYTHON_INIT_FUNCTION_SIGNATURE(name) void PYTHON_INIT_FUNCTION_NAME(name)(void)
#define PYTHON_MODULE_NAME(name) "_" #name
#define PYTHON_NAMESPACE ::opa::python

class PythonMachine : public PythonMachineBase {
  public:
    PythonMachine() : PythonMachineBase("py", LANG(Python), "Python") {}
    virtual ~PythonMachine() {}

    virtual VirtualData::Ptr NewData();

    virtual void ExecuteCode(const std::string& code);
    
    virtual VirtualObj LoadModule(const std::string& name);

    /// Initializes the VirtualMachine (that is, the language's API. Returns bool telling if (true=) no problems occured.
    virtual bool Initialize();
    /// Finalizes the VirtualMachine, finalizing any language specific stuff.
    virtual void Finalize();

    std::string GetPythonExceptionDetails();

  private:
    //std::vector<Module> modules_;
};

}
}

#endif /* OUROBOROS_SCRIPT_PYTHON_PYTHONMACHINE_H_ */
