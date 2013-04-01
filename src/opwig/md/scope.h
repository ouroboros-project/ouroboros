
#ifndef OPWIG_MD_SCOPE_H_
#define OPWIG_MD_SCOPE_H_

#include <opwig/md/ptr.h>

#include <string>

namespace opwig {
namespace md {

// Forward declaration.
class Namespace;
class Variable;

/// Metadata interface for C++ scopes.
class Scope {

  public:

    /// Virtual destructor.
    ~Scope () {}

    /* NAMESPACE METHODS */
    
    /// Tells how many nested namespaces exist in this scope.
    virtual size_t NestedNamespacesNum () const = 0;

    /// Adds a nested namespace.
    virtual bool AddNestedNamespace (const std::string& name, Ptr<Namespace> nested) = 0;

    /// Gives the nested namespace identified by the given name (const version).
    virtual Ptr<const Namespace> NestedNamespace (const std::string& name) const = 0;
    
    /// Gives the nested namespace identified by the given name.
    virtual Ptr<Namespace> NestedNamespace (const std::string& name) = 0;

    /* VARIABLE METHODS */
    
     /// Adds a global variable to this scope.
    virtual bool AddGlobalVariable ( Ptr<Variable> variable) = 0;

    /// Gives the global variable identified by the given name (const version).
    virtual Ptr<const Variable> GlobalVariable (const std::string& name) const = 0;
    
    /// Gives the global variable identified by the given name.
    virtual Ptr<Variable> GlobalVariable (const std::string& name) = 0;
    
    /* CLASS METHODS */
    
  protected:

    Scope () {}

  private:

};

} // namespace md
} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_


