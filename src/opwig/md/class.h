
#ifndef OPWIG_MD_CLASS_H_
#define OPWIG_MD_CLASS_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/semanticerror.h>

#include <opwig/parser/basespecifier.h>

#include <string>
#include <list>
#include <vector>

namespace opwig {
namespace md {

/// Metadata class for C++ classes.
class Class final : public Scope {

  public:

    /// Creates a new Class object. Must be used in place of the
    /// constructor.
    static Ptr<Class> Create (const std::string& class_name,
                              const std::list<parser::BaseSpecifier>& the_base_specifiers);
    
    /// Gets the list of BaseSpecifiers (parser struct depicting a base class) of this class.
    const std::list<parser::BaseSpecifier>& base_specifiers() const { return base_specifiers_; }
    
    /// Gets the list of constructors of this class.
    const std::vector<Ptr<Function>> constructors() const { return constructors_; }
    
    /// Gets the destructor of this class.
    const Ptr<Function> destructor() const { return destructor_; }
    

    /*** NAMESPACE METHODS ***/
    
    /// @see opwig::md::Scope::NestedNamespacesNum
    size_t NestedNamespacesNum () const override;

    /// @see opwig::md::Scope::AddNestedNamespace
    bool AddNestedNamespace (Ptr<Namespace> nested) override;

    /// @see opwig::md::Scope::NestedNamespace
    Ptr<const Namespace> NestedNamespace (const std::string& nmspace_id) const override;

    /// @see opwig::md::Scope::NestedNamespace
    Ptr<Namespace> NestedNamespace (const std::string& nmspace_id) override;
   
    /*** FUNCTION METHODS ***/
    
    /// @see opwig::md::Scope::AddNestedFunction
    bool AddNestedFunction (Ptr<Function> nested) override;

  private:
    std::list<parser::BaseSpecifier> base_specifiers_;
    
    std::vector<Ptr<Function>> constructors_;
    Ptr<Function> destructor_;

    Class (const std::string& class_name, const std::list<parser::BaseSpecifier>& the_base_specifiers) : 
        Scope::Scope(class_name), base_specifiers_(the_base_specifiers) {
            SetAccessSpecifier(AccessSpecifier::PRIVATE);
        }

};

inline Ptr<Class> Class::Create (const std::string& class_name, const std::list<parser::BaseSpecifier>& the_base_specifiers) {
    return Ptr<Class>(new Class(class_name, the_base_specifiers));
}

inline size_t Class::NestedNamespacesNum () const {
    return 0;
}

inline bool Class::AddNestedNamespace (Ptr<Namespace> nested) {
    throw SemanticError("Namespaces cannot be added in a class!", __FILE__, __LINE__);
}

inline Ptr<const Namespace> Class::NestedNamespace (const std::string& nmspace_id) const {
    throw SemanticError("Classes do not have nested namespaces!", __FILE__, __LINE__);
}

inline Ptr<Namespace> Class::NestedNamespace (const std::string& nmspace_id) {
    throw SemanticError("Classes do not have nested namespaces!", __FILE__, __LINE__);
}


} // namespace md
} // namespace opwig

#endif // OPWIG_MD_CLASS_H_

