
#ifndef OPWIG_MD_NAMESPACE_H_
#define OPWIG_MD_NAMESPACE_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/variable.h>

#include <string>
#include <map>

namespace opwig {
namespace md {

/// Metadata class for C++ namespaces.
class Namespace final : public Scope {

  public:

    /// Creates a new Namespace object. Must be used in place of the
    /// constructor.
    static Ptr<Namespace> Create ();

    /// @see opwig::md::Scope::NestedNamespaceNum
    size_t NestedNamespacesNum () const override;

    /// @see opwig::md::Scope::AddNestedNamespace
    bool AddNestedNamespace (const std::string& name, Ptr<Namespace> nested) override;

    /// @see opwig::md::Scope::NestedNamespace
    Ptr<const Namespace> NestedNamespace (const std::string& name) const override;

    /// @see opwig::md::Scope::NestedNamespace
    Ptr<Namespace> NestedNamespace (const std::string& name) override;

    /// Adds a global variable to this namespace.
    bool AddGlobalVariable ( Ptr<Variable> variable);

    /// Gives the global variable identified by the given name (const version).
    Ptr<const Variable> GlobalVariable (const std::string& name) const;
    
    /// Gives the global variable identified by the given name.
    Ptr<Variable> GlobalVariable (const std::string& name);

  private:

    std::map<std::string, Ptr<Namespace>> nested_namespaces_;
    std::map<std::string, Ptr<Variable>> global_variables_;

    Namespace () {}

};

inline Ptr<Namespace> Namespace::Create () {
  return Ptr<Namespace>(new Namespace);
}

inline size_t Namespace::NestedNamespacesNum () const {
  return nested_namespaces_.size();
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_NAMESPACE_H_

