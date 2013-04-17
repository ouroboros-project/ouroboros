
#ifndef OPWIG_MD_CLASS_H_
#define OPWIG_MD_CLASS_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/semanticerror.h>

#include <opwig/parser/basespecifier.h>

#include <string>

namespace opwig {
namespace md {

/// Metadata class for C++ classes.
class Class final : public Scope {

  public:

    /// Creates a new Class object. Must be used in place of the
    /// constructor.
    static Ptr<Class> Create (const std::string& name, const std::list<parser::BaseSpecifier>& base_specifiers);
    
    const std::string& name() const { return name_; }
    const std::list<parser::BaseSpecifier>& base_specifiers() const { return base_specifiers_; }

    /*** NAMESPACE METHODS ***/
    
    /// @see opwig::md::Scope::NestedNamespacesNum
    size_t NestedNamespacesNum () const override;

    /// @see opwig::md::Scope::AddNestedNamespace
    bool AddNestedNamespace (const std::string& name, Ptr<Namespace> nested) override;

    /// @see opwig::md::Scope::NestedNamespace
    Ptr<const Namespace> NestedNamespace (const std::string& name) const override;

    /// @see opwig::md::Scope::NestedNamespace
    Ptr<Namespace> NestedNamespace (const std::string& name) override;

  private:
    std::string name_;
    std::list<parser::BaseSpecifier> base_specifiers_;
    
    Container<Class> classes_;
    Container<Variable> variables_;
    Container<Function>  functions_;

    Class (const std::string& name, const std::list<parser::BaseSpecifier>& base_specifiers) : 
        name_(name), base_specifiers_(base_specifiers) {
            SetAccessSpecifier(AccessSpecifier::PRIVATE);
        }

};

inline Ptr<Class> Class::Create (const std::string& name, const std::list<parser::BaseSpecifier>& base_specifiers) {
    return Ptr<Class>(new Class(name, base_specifiers));
}

inline size_t Class::NestedNamespacesNum () const {
    throw SemanticError("Classes do not have nested namespaces!", __FILE__, __LINE__);
}

bool Class::AddNestedNamespace (const string& name, Ptr<Namespace> nested) {
    throw SemanticError("Namespaces cannot be added in a class!", __FILE__, __LINE__);
}

Ptr<const Namespace> Class::NestedNamespace (const string& name) const {
    throw SemanticError("Classes do not have nested namespaces!", __FILE__, __LINE__);
}

Ptr<Namespace> Class::NestedNamespace (const string& name) {
    throw SemanticError("Classes do not have nested namespaces!", __FILE__, __LINE__);
}


} // namespace md
} // namespace opwig

#endif // OPWIG_MD_CLASS_H_

