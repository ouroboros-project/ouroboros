
#ifndef OPWIG_MD_CLASS_H_
#define OPWIG_MD_CLASS_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/semanticerror.h>
#include <opwig/md/basespecifier.h>

#include <string>
#include <list>
#include <vector>

namespace opwig {
namespace md {

/// Metadata class for C++ classes.
class Class final : public Scope {

  public:

    /// Creates a new Class instance.
    /** Creates a new Class instance with given attributes, and returns a smart pointer to it.
    * @param class_name The name of the class.
    * @param the_base_specifiers List of base specifiers for the class.
    * @return A smart pointer containing a new Class instance.
    */
    static Ptr<Class> Create (const std::string& class_name,
                              const std::list<BaseSpecifier>& the_base_specifiers);
    
    /// Gets the list of BaseSpecifiers (parser struct depicting a base class) of this class.
    /** Gets the list of parser::BaseSpecifier of this class, which are structs used by the parser
        to define the base classes of a class.
    * @return A list of BaseSpecifier.
    */
    const std::list<BaseSpecifier>& base_specifiers() const { return base_specifiers_; }
    
    /// Gets the list of constructors of this class.
    /** Gets the list of constructors of this class.
        Constructors are represented in the metadata as functions of a class, which have the 
        same name as the class and no return type.
    * @return A list of smart pointers of constructors (Function metadata class).
    */
    const std::vector<Ptr<Function>> constructors() const { return constructors_; }
    
    /// Gets the destructor of this class.
    /** Gets the destructor of this class.
        The destructor is represented in the metadata as a function of the class, with the
        same name as the class, plus a '~' prefix, and no return type.
    * @return  A smart pointer to the destructor (Function metadata class).
    */
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
    
    /// @see opwig::md::Scope::IterateNamespaces
    Container<Namespace>::Iterable IterateNamespaces () const override;
   
    /*** FUNCTION METHODS ***/
    
    /// @see opwig::md::Scope::AddNestedFunction
    bool AddNestedFunction (Ptr<Function> nested) override;

    /*****************************************************/
    /*** CLASS METHODS ***/

    /// @see opwig::md::Scope::AddNestedClass
    bool AddNestedClass(Ptr<Class> nested) override;

  private:
    std::list<BaseSpecifier> base_specifiers_;
    
    std::vector<Ptr<Function>> constructors_;
    Ptr<Function> destructor_;

    Class (const std::string& class_name, const std::list<BaseSpecifier>& the_base_specifiers) : 
        Scope(class_name), base_specifiers_(the_base_specifiers) {}

};

inline Ptr<Class> Class::Create (const std::string& class_name, const std::list<BaseSpecifier>& the_base_specifiers) {
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

inline Container<Namespace>::Iterable Class::IterateNamespaces () const {
    throw SemanticError("Can't iterate - classes do not have nested namespaces!", __FILE__, __LINE__); //FIXME: not actually a semantic error
}


} // namespace md
} // namespace opwig

#endif // OPWIG_MD_CLASS_H_

