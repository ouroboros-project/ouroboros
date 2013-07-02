
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

    /// Tells if the declaration is specified as virtual.
    bool is_virtual () const;

  private:

    md::NestedNameSpecifier type_;
    bool                    is_virtual_;

};

inline DeclSpecifier::DeclSpecifier (const md::NestedNameSpecifier& the_type,
                                     bool the_virtual_flag) :
    type_(the_type), is_virtual_(the_virtual_flag) {}

inline md::NestedNameSpecifier DeclSpecifier::type () const {
    return type_;
}

inline bool DeclSpecifier::is_virtual () const {
    return is_virtual_;
}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLSPECIFIER_H_

