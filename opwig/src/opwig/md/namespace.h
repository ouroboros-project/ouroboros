
#ifndef OPWIG_MD_NAMESPACE_H_
#define OPWIG_MD_NAMESPACE_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/class.h>

namespace opwig {
namespace md {

/// Metadata class for C++ namespaces.
/** Represents a C++ namespace, containing all of its relevant metadata. */
class Namespace final : public Scope {

  public:

    /// Creates a new Namespace instance.
    /** Creates a new Namespace instance with given attributes, and returns a smart pointer to it.
    * @param the_name The name of the namespace.
    * @return A smart pointer containing a new Namespace instance.
    */
    static Ptr<Namespace> Create (const std::string& the_name);

  private:

    Namespace (const std::string& the_name) : Scope(the_name) {}

};

inline Ptr<Namespace> Namespace::Create (const std::string& the_name) {
    return Ptr<Namespace>(new Namespace(the_name));
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_NAMESPACE_H_

