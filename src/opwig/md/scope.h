
#ifndef OPWIG_MD_SCOPE_H_
#define OPWIG_MD_SCOPE_H_

#include <string>
#include <memory>

namespace opwig {
namespace md {

// Forward declaration.
class Namespace;

/// Metadata interface for C++ scopes.
class Scope {

  public:

    /// Garbage-collecting smart-pointer type for this class.
    typedef std::shared_ptr<Namespace> Ptr;

    /// Const Garbage-collecting smart-pointer type for this class.
    typedef std::shared_ptr<const Namespace> ConstPtr;

    /// Virtual destructor.
    ~Scope () {}

    /// Tells how many nested namespaces exist in this scope.
    virtual size_t NestedNamespacesNum () const = 0;

    /// Adds a nested namespace.
    virtual bool AddNestedNamespace (const std::string& name, const Ptr& nested) = 0;

    /// Gives the nested namespace identified by the given name (const version).
    virtual ConstPtr NestedNamespace (const std::string& name) const = 0;
    
    /// Gives the nested namespace identified by the given name.
    virtual Ptr NestedNamespace (const std::string& name) = 0;

  protected:

    Scope () {}

};

} // namespace md
} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_


