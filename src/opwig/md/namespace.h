
#ifndef OPWIG_MD_NAMESPACE_H_
#define OPWIG_MD_NAMESPACE_H_

#include <opwig/md/scope.h>
#include <opwig/md/ptr.h>
#include <opwig/md/class.h>

namespace opwig {
namespace md {

/// Metadata class for C++ namespaces.
class Namespace final : public Scope {

  public:

    /// Creates a new Namespace object. Must be used in place of the
    /// constructor.
    static Ptr<Namespace> Create (const std::string& the_name);

  private:

    Namespace (const std::string& the_name) : Scope::Scope(the_name) { SetAccessSpecifier(AccessSpecifier::PUBLIC); }

};

inline Ptr<Namespace> Namespace::Create (const std::string& the_name) {
    return Ptr<Namespace>(new Namespace(the_name));
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_NAMESPACE_H_

