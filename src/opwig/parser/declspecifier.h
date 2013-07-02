
#ifndef OPWIG_DECLSPECIFIER_H_
#define OPWIG_DECLSPECIFIER_H_

#include <opwig/md/ptr.h>
#include <opwig/md/nestednamespecifier.h>

#include <string>

namespace opwig {
namespace parser {

/// Auxiliar class for keeping declaration specification information throughout code parsing.
class DeclSpecifier final {

  public:

    explicit DeclSpecifier (const md::NestedNameSpecifier& the_type,
                            bool the_virtual_flag);

    /// Gets the NestedNameSpecifier type associated with this declation specifier.
    md::NestedNameSpecifier type () const;

  private:

    md::NestedNameSpecifier type_;

};

inline DeclSpecifier::DeclSpecifier (const md::NestedNameSpecifier& the_type,
                                     bool the_virtual_flag) :
    type_(the_type) {}

inline md::NestedNameSpecifier DeclSpecifier::type () const {
    return type_;
}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLSPECIFIER_H_

