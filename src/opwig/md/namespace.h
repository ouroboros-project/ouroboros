
#ifndef OPWIG_MDNAMESPACE_H_
#define OPWIG_MDNAMESPACE_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>

#include <string>
#include <map>

namespace opwig {
namespace md {

/// Metadata class for C++ namespaces.
class Namespace {

  public:

    /// Creates a new Namespace object. Must be used in place of the
    /// constructor.
    static Ptr<Namespace> Create ();

    /// Tells how many nested namespaces exist in this namespace.
    size_t NestedNamespacesNum () const;

    /// Adds a nested namespace.
    bool AddNestedNamespace (const std::string& name, Ptr<Namespace> nested);

    /// Gives the nested namespace identified by the given name (const version).
    Ptr<const Namespace> NestedNamespace (const std::string& name) const;
    
    /// Gives the nested namespace identified by the given name.
    Ptr<Namespace> NestedNamespace (const std::string& name);

  private:

    std::map<std::string, Ptr<Namespace>> nested_namespaces_;

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

#endif // OPWIG_MDNAMESPACE_H_

