
#ifndef OPWIG_MD_SCOPE_H_
#define OPWIG_MD_SCOPE_H_

#include <opwig/md/container.h>

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

    /// Gets the Namespace container for this scope;
    virtual Container<Namespace>& Namespaces() = 0;
    
    /// Gets the Variable container for this scope;
    virtual Container<Variable>& Variables() = 0;

    /// Gets the Class container for this scope;
    virtual Container<Class>& Classes() = 0;
    
    /// Gets the Function container for this scope;
    virtual Container<Function>& Functions() = 0;
    
    /// Sets the current access specifier of this scope (and its containers) to the given access specifier
    virtual void SetAccessSpecifier(AccessSpecifier access) = 0;

  protected:

    Scope () {}

  private:

};

} // namespace md
} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_


