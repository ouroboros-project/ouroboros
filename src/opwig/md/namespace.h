
#ifndef OPWIG_MD_NAMESPACE_H_
#define OPWIG_MD_NAMESPACE_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/variable.h>
#include <opwig/md/class.h>

#include <string>
#include <map>

namespace opwig {
namespace md {

// Forward declarations.
class Function;

/// Metadata class for C++ namespaces.
class Namespace final : public Scope {

  public:

    /// Creates a new Namespace object. Must be used in place of the
    /// constructor.
    static Ptr<Namespace> Create ();

    /*** NAMESPACE METHODS ***/
    
    /// @see opwig::md::Scope::NestedNamespacesNum
    size_t NestedNamespacesNum () const override;

    /// @see opwig::md::Scope::AddNestedNamespace
    bool AddNestedNamespace (const std::string& name, Ptr<Namespace> nested) override;

    /// @see opwig::md::Scope::NestedNamespace
    Ptr<const Namespace> NestedNamespace (const std::string& name) const override;

    /// @see opwig::md::Scope::NestedNamespace
    Ptr<Namespace> NestedNamespace (const std::string& name) override;

    /*** VARIABLE METHODS ***/
    
    /// @see opwig::md::Scope::AddGlobalVariable
    bool AddGlobalVariable ( Ptr<Variable> variable) override;

    /// @see opwig::md::Scope::GlobalVariable
    Ptr<const Variable> GlobalVariable (const std::string& name) const override;
    
    /// @see opwig::md::Scope::GlobalVariable
    Ptr<Variable> GlobalVariable (const std::string& name) override;
    
    /*** CLASS METHODS ***/
    
    /// @see opwig::md::Scope::NestedClassesNum
    size_t NestedClassesNum () const override;

    /// @see opwig::md::Scope::AddNestedClass
    bool AddNestedClass (const std::string& name, Ptr<Class> nested) override;

    /// @see opwig::md::Scope::NestedClass
    Ptr<const Class> NestedClass (const std::string& name) const override;
    
    /// @see opwig::md::Scope::NestedClass
    Ptr<Class> NestedClass (const std::string& name) override;

    /// @see opwig::md::Scope::NestedFunctionsNum
    size_t NestedFunctionsNum () const;

    /// @see opwig::md::Scope::AddNestedFunction
    bool AddNestedFunction (Ptr<Function> nested);

    /// @see opwig::md::Scope::NestedFunction
    Ptr<const Function> NestedFunction (const std::string& name) const;

  private:

    std::map<std::string, Ptr<Namespace>> nested_namespaces_;
    std::map<std::string, Ptr<Class>>     nested_classes_;
    std::map<std::string, Ptr<Variable>>  global_variables_;
    std::map<std::string, Ptr<Function>>  nested_functions_;

    Namespace () {}

};

inline Ptr<Namespace> Namespace::Create () {
  return Ptr<Namespace>(new Namespace);
}

inline size_t Namespace::NestedNamespacesNum () const {
  return nested_namespaces_.size();
}

inline size_t Namespace::NestedClassesNum () const {
  return nested_classes_.size();
}

inline size_t Namespace::NestedFunctionsNum() const {
  return nested_functions_.size();
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_NAMESPACE_H_

