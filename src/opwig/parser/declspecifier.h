
#ifndef OPWIG_DECLSPECIFIER_H_
#define OPWIG_DECLSPECIFIER_H_

#include <opwig/md/ptr.h>
#include <opwig/md/nestednamespecifier.h>
#include <opwig/md/semanticerror.h>

#include <string>

namespace opwig {
namespace parser {

/// Auxiliar class for keeping declaration specification information throughout code parsing.
class DeclSpecifier final {

  public:

    explicit DeclSpecifier (bool the_virtual_flag = false,
                            const md::NestedNameSpecifier& the_type = md::NestedNameSpecifier());

    /// Tells if the declaration is specified as virtual.
    bool is_virtual () const;

    /// Gets the NestedNameSpecifier type associated with this declation specifier.
    md::NestedNameSpecifier type () const;

    /// Joins two declaration specifiers according to C++ semantics.
    static DeclSpecifier Join (const DeclSpecifier& lhs, const DeclSpecifier& rhs);

  private:

    bool                    is_virtual_;
    md::NestedNameSpecifier type_;

};

inline DeclSpecifier::DeclSpecifier (bool the_virtual_flag,
                                     const md::NestedNameSpecifier& the_type)
    : is_virtual_(the_virtual_flag), type_(the_type) {}

inline md::NestedNameSpecifier DeclSpecifier::type () const {
    return type_;
}

inline bool DeclSpecifier::is_virtual () const {
    return is_virtual_;
}

inline DeclSpecifier DeclSpecifier::Join (const DeclSpecifier& lhs, const DeclSpecifier& rhs) {
    if (lhs.is_virtual() && rhs.is_virtual())
        throw md::SemanticError("Double virtual specification.", __FILE__, __LINE__);
    bool virtual_flag = lhs.is_virtual() || rhs.is_virtual();

    bool lhs_typed = !lhs.type().ToString().empty(),
         rhs_typed = !rhs.type().ToString().empty();
    if (lhs_typed && rhs_typed)
        throw md::SemanticError("Double type specification.", __FILE__, __LINE__);
    md::NestedNameSpecifier the_type;
    if (lhs_typed)
      the_type = lhs.type();
    else
      the_type = rhs.type();
  
    return DeclSpecifier(virtual_flag, the_type);
}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLSPECIFIER_H_

