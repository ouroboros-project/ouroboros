#ifndef OPWIG_GEN_PYTHON_PYTHONSPECIFICATION_H_
#define OPWIG_GEN_PYTHON_PYTHONSPECIFICATION_H_

#include <opwig/gen/wrapperspecification.h>
#include <list>
#include <string>

namespace opa {
namespace python {

class WrapScope;

class PythonSpecification final : public ::opwig::gen::WrapperSpecification {

  public:
    PythonSpecification();

    std::string wrapper_name () const;

    std::string FileHeader() const;
    std::string MiddleBlock() const;
    std::string FinishFile() const;

    std::string WrapFunction(const opwig::md::Ptr<const opwig::md::Function>& obj);
    std::string WrapVariable(const opwig::md::Ptr<const opwig::md::Variable>& obj);
    std::string WrapEnum(const opwig::md::Ptr<const opwig::md::Enum>& obj);

    std::string OpenClass(const opwig::md::Ptr<const opwig::md::Class>& obj);
    std::string CloseClass(const opwig::md::Ptr<const opwig::md::Class>& obj);
    std::string OpenNamespace(const opwig::md::Ptr<const opwig::md::Namespace>& obj);
    std::string CloseNamespace(const opwig::md::Ptr<const opwig::md::Namespace>& obj);

    std::string LoadFuncSignature() const;
    std::list<opwig::gen::ScriptModule> GetGeneratedModules() const;
    
  private:
    opwig::md::Ptr<WrapScope> root_module_;
    opwig::md::Ptr<WrapScope> current_;

    void PushScope(const std::string& name, bool is_class);
    void PopScope();
};

inline std::string PythonSpecification::wrapper_name () const {
    return "Python";
}

inline std::string PythonSpecification::LoadFuncSignature () const {
    return "void (*)(void)";
}

} // namespace python
} // namespace opa

#endif // OPWIG_GEN_PYTHON_PYTHONSPECIFICATION_H_

