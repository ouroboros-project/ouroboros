#ifndef OUROBOROS_SCRIPT_PYTHON_PYTHONWRAPPER_H_
#define OUROBOROS_SCRIPT_PYTHON_PYTHONWRAPPER_H_

#include <vector>
#include <opa/langwrapper.h>

namespace ouroboros {

class VirtualObj;

namespace python {

typedef void (*PyInitFunction)(void);
typedef ouroboros::Module<PyInitFunction> PythonModule;
typedef ouroboros::InheritableLangWrapper<PyInitFunction> PythonWrapperBase;
typedef ouroboros::python::PyInitFunction inittype;

#define PYTHON_INIT_FUNCTION_NAME(name) init_##name
#define PYTHON_INIT_FUNCTION_SIGNATURE(name) void PYTHON_INIT_FUNCTION_NAME(name)(void)
#define PYTHON_MODULE_NAME(name) "_" #name
#define PYTHON_NAMESPACE ::ouroboros::python

class PythonWrapper : public PythonWrapperBase {
  public:
    PythonWrapper() : PythonWrapperBase("py", LANG(Python), "Python") {}
    virtual ~PythonWrapper() {}

    virtual VirtualData::Ptr NewData();

    virtual void ExecuteCode(const std::string& code);
    
    virtual VirtualObj LoadModule(const std::string& name);

    /// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
    virtual bool Initialize();
    /// Finalizes the LangWrapper, finalizing any language specific stuff.
    virtual void Finalize();

    void PrintPythonExceptionDetails();

  private:
    //std::vector<Module> modules_;
};

}
}

#endif /* OUROBOROS_SCRIPT_PYTHON_PYTHONWRAPPER_H_ */
