
#ifndef OPWIG_MDNAMESPACE_H_
#define OPWIG_MDNAMESPACE_H_

#include <string>
#include <map>
#include <memory>

namespace opwig {

/// Metadata class for C++ namespaces.
class MDNamespace {

  public:

    /// Garbage-collecting smart-pointer type for this class.
    typedef std::shared_ptr<MDNamespace> Ptr;

    /// Const Garbage-collecting smart-pointer type for this class.
    typedef std::shared_ptr<const MDNamespace> ConstPtr;

    /// Creates a new MDNamespace object. Must be used in place of the
    /// constructor.
    static Ptr Create ();

    /// Tells how many nested namespaces exist in this namespace.
    size_t NestedNamespacesNum () const;

  private:

    std::map<std::string, Ptr> nested_namespaces_;

    MDNamespace () {}

};

inline MDNamespace::Ptr MDNamespace::Create () {
  return Ptr(new MDNamespace);
}

inline size_t MDNamespace::NestedNamespacesNum () const {
  return nested_namespaces_.size();
}

} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_

