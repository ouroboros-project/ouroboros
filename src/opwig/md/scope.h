
#ifndef OPWIG_MD_SCOPE_H_
#define OPWIG_MD_SCOPE_H_

#include <opwig/md/ptr.h>

#include <string>

namespace opwig {
namespace md {

// Forward declaration.
class Namespace;

/// Metadata interface for C++ scopes.
class Scope {

  public:

    /// Virtual destructor.
    ~Scope () {}

    /// Tells how many nested namespaces exist in this scope.
    virtual size_t NestedNamespacesNum () const = 0;

    /// Adds a nested namespace.
    virtual bool AddNestedNamespace (const std::string& name,
                                     Ptr<Namespace> nested) = 0;

    /// Gives the nested namespace identified by the given name (const version).
    virtual Ptr<const Namespace>
    NestedNamespace (const std::string& name) const = 0;
    
    /// Gives the nested namespace identified by the given name.
    virtual Ptr<Namespace> NestedNamespace (const std::string& name) = 0;

  protected:

    Scope () {}

  private:

};

} // namespace md
} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_


