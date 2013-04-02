
#ifndef OPWIG_MD_SCOPE_H_
#define OPWIG_MD_SCOPE_H_

#include <opwig/md/ptr.h>

#include <string>

namespace opwig {
namespace md {

// Forward declarations.
class Namespace;
class Class;
class Variable;
class Function;

/// Metadata interface for C++ scopes.
class Scope {

  public:

    /// Virtual destructor.
    virtual ~Scope () {}

    /*** NAMESPACE METHODS ***/
    
    /// Tells how many nested namespaces exist in this scope.
    virtual size_t NestedNamespacesNum () const = 0;

    /// Adds a nested namespace.
    virtual bool AddNestedNamespace (const std::string& name, Ptr<Namespace> nested) = 0;

    /// Gives the nested namespace identified by the given name (const version).
    virtual Ptr<const Namespace> NestedNamespace (const std::string& name) const = 0;
    
    /// Gives the nested namespace identified by the given name.
    virtual Ptr<Namespace> NestedNamespace (const std::string& name) = 0;

    /*** VARIABLE METHODS ***/
    
     /// Adds a global variable to this scope.
    virtual bool AddGlobalVariable ( Ptr<Variable> variable) = 0;

    /// Gives the global variable identified by the given name (const version).
    virtual Ptr<const Variable> GlobalVariable (const std::string& name) const = 0;
    
    /// Gives the global variable identified by the given name.
    virtual Ptr<Variable> GlobalVariable (const std::string& name) = 0;
    
    /*** CLASS METHODS ***/
    
    /// Tells how many nested classes exist in this scope.
    virtual size_t NestedClassesNum () const = 0;

    /// Adds a nested class.
    virtual bool AddNestedClass (const std::string& name, Ptr<Class> nested) = 0;

    /// Gives the nested class identified by the given name (const version).
    virtual Ptr<const Class> NestedClass (const std::string& name) const = 0;
    
    /// Gives the nested class identified by the given name.
    virtual Ptr<Class> NestedClass (const std::string& name) = 0;

    /*** FUNCTION METHODS ***/
    
    /// Tells how many functions exist in this scope.
    virtual size_t NestedFunctionsNum () const = 0;

    /// Adds a function to the scope. In classes, it will be a method.
    virtual bool AddNestedFunction (Ptr<Function> nested) = 0;

    /// Gives the function identified by the given name.
    virtual Ptr<const Function> NestedFunction (const std::string& name) const = 0;

  protected:

    Scope () {}

  private:

};

} // namespace md
} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_


