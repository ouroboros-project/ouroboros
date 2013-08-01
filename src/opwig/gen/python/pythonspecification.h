#ifndef OPWIG_GEN_PYTHON_PYTHONSPECIFICATION_H_
#define OPWIG_GEN_PYTHON_PYTHONSPECIFICATION_H_

#include <opwig/gen/wrapperspecification.h>
#include <list>
#include <string>

namespace opwig {
namespace gen {
namespace python {

class PythonSpecification final : public ::opwig::gen::WrapperSpecification {

  public:
    PythonSpecification() {}

    std::string wrapper_name () const;

    std::string FileHeader() const;
    std::string MiddleBlock() const;
    std::string FinishFile() const;

    std::string WrapFunction(const md::Ptr<const md::Function>& obj);
    std::string WrapVariable(const md::Ptr<const md::Variable>& obj);
    std::string WrapClass(const md::Ptr<const md::Class>& obj);
    std::string WrapNamespace(const md::Ptr<const md::Namespace>& obj, bool closing);
    std::string WrapEnum(const md::Ptr<const md::Enum>& obj);

    std::string LoadFuncSignature() const;
    std::string LoadFuncName() const;
    
  private:
    std::list<std::string> wrapped_functions_;
    std::string input_file_;

    std::string generateMethodTable() const;
};

inline std::string PythonSpecification::wrapper_name () const {
    return "Python";
}

inline std::string PythonSpecification::LoadFuncSignature () const {
    return "void (*)(void)";
}

inline std::string PythonSpecification::LoadFuncName () const {
    return "init"+module_name_;
}

} // namespace python
} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_PYTHON_PYTHONSPECIFICATION_H_

