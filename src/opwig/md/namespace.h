
#ifndef OPWIG_MD_NAMESPACE_H_
#define OPWIG_MD_NAMESPACE_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/accessspecifier.h>

namespace opwig {
namespace md {

// Forward declarations.
class Namespace;
class Variable;
class Class;
class Function;

/// Metadata class for C++ namespaces.
class Namespace final : public Scope {

  public:

    /// Creates a new Namespace object. Must be used in place of the
    /// constructor.
    static Ptr<Namespace> Create ();

    /// @see opwig::md::Scope::Namespaces
    virtual Container<Namespace>& Namespaces() override { return namespaces_; }
    
    /// @see opwig::md::Scope::Variables
    virtual Container<Variable>& Variables() override { return variables_; }

    /// @see opwig::md::Scope::Classes
    virtual Container<Class>& Classes() override { return classes_; }
    
    /// @see opwig::md::Scope::Functions
    virtual Container<Function>& Functions() override { return functions_; }
    
    /// @see opwig::md::Scope::SetAccessSpecifier
    virtual void SetAccessSpecifier(AccessSpecifier access) override;

  private:

    Container<Namespace> namespaces_;
    Container<Class>     classes_;
    Container<Variable>  variables_;
    Container<Function>  functions_;

    Namespace () { SetAccessSpecifier(AccessSpecifier::PUBLIC); }

};

inline Ptr<Namespace> Namespace::Create () {
    return Ptr<Namespace>(new Namespace);
}

inline void SetAccessSpecifier(AccessSpecifier access) {
    namespaces_.SetAccessSpecifier(access);
    classes_.SetAccessSpecifier(access);
    variables_.SetAccessSpecifier(access);
    functions_.SetAccessSpecifier(access);
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_NAMESPACE_H_

