
#ifndef OPWIG_MDNAMESPACE_H_
#define OPWIG_MDNAMESPACE_H_

#include <opwig/md/scope.h>

#include <string>
#include <map>
#include <memory>

namespace opwig {
namespace md {

/// Metadata class for C++ namespaces.
class Namespace {

  public:

    /// Garbage-collecting smart-pointer type for this class.
    typedef std::shared_ptr<Namespace> Ptr;

    /// Const Garbage-collecting smart-pointer type for this class.
    typedef std::shared_ptr<const Namespace> ConstPtr;

    /// Creates a new Namespace object. Must be used in place of the
    /// constructor.
    static Ptr Create ();

    /// Tells how many nested namespaces exist in this namespace.
    size_t NestedNamespacesNum () const;

    /// Adds a nested namespace.
    bool AddNestedNamespace (const std::string& name, const Ptr& nested);

    /// Gives the nested namespace identified by the given name (const version).
    ConstPtr NestedNamespace (const std::string& name) const;
    
    /// Gives the nested namespace identified by the given name.
    Ptr NestedNamespace (const std::string& name);

  private:

    std::map<std::string, Ptr> nested_namespaces_;

    Namespace () {}

};

inline Namespace::Ptr Namespace::Create () {
  return Ptr(new Namespace);
}

inline size_t Namespace::NestedNamespacesNum () const {
  return nested_namespaces_.size();
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_

