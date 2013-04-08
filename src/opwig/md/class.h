
#ifndef OPWIG_MD_CLASS_H_
#define OPWIG_MD_CLASS_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/variable.h>
#include <opwig/md/semanticerror.h>

#include <opwig/parser/basespecifier.h>

#include <string>
#include <map>
#include <list>

namespace opwig {
namespace md {

/// Metadata class for C++ classes.
class Class final : public Scope {

  public:

    /// Creates a new Class object. Must be used in place of the
    /// constructor.
    static Ptr<Class> Create (const std::string& name, const std::list<parser::BaseSpecifier>& base_specifiers);
    
    const std::string& name() const { return name_; }
    

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

  private:
    std::string name_;
    std::list<parser::BaseSpecifier> base_specifiers_;
    
    std::map<std::string, Ptr<Class>> nested_classes_;
    std::map<std::string, Ptr<Variable>> global_variables_;

    Class (const std::string& name, const std::list<parser::BaseSpecifier>& base_specifiers) : name_(name), base_specifiers_(base_specifiers) {}

};

inline Ptr<Class> Class::Create (const std::string& name, const std::list<parser::BaseSpecifier>& base_specifiers) {
  return Ptr<Class>(new Class(name, base_specifiers));
}

inline size_t Class::NestedNamespacesNum () const {
    throw SemanticError("Classes do not have nested namespaces!");
}

inline size_t Class::NestedClassesNum () const {
  return nested_classes_.size();
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_CLASS_H_

