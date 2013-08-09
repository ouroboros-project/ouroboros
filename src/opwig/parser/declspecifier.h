
#ifndef OPWIG_DECLSPECIFIER_H_
#define OPWIG_DECLSPECIFIER_H_

#include <opwig/md/ptr.h>
#include <opwig/md/type.h>
#include <opwig/md/nestednamespecifier.h>
#include <opwig/md/semanticerror.h>

#include <string>

namespace opwig {
namespace parser {

/// Auxiliar class for keeping declaration specification information throughout code parsing.
class DeclSpecifier final {

  public:

    static DeclSpecifier EMPTY ();
    static DeclSpecifier TYPE (const md::NestedNameSpecifier& the_type);
    static DeclSpecifier VIRTUAL ();
    static DeclSpecifier TYPEDEF ();
    static DeclSpecifier FRIEND ();
    static DeclSpecifier CONST ();

    /// Tells if the declaration is specified as virtual.
    bool is_virtual () const;

    /// Tells if the declaration is specified as a const type.
    bool is_const () const;

    /// Gets the NestedNameSpecifier type associated with this declaration specifier.
    md::NestedNameSpecifier type_specifier () const;

    /// Gets the full type associated with this declaration specifier.
    md::Ptr<md::Type> type () const;

    /// Joins two declaration specifiers according to C++ semantics.
    static DeclSpecifier Join (const DeclSpecifier& lhs, const DeclSpecifier& rhs);

  private:

    bool                    is_virtual_,
                            is_const_,
                            is_volatile_;
    md::NestedNameSpecifier type_specifier_;

    explicit DeclSpecifier (const md::NestedNameSpecifier& the_speficier = md::NestedNameSpecifier(),
                            bool the_virtual_flag = false, bool the_const_flag = false);

};

inline DeclSpecifier::DeclSpecifier (const md::NestedNameSpecifier& the_specifier, bool the_virtual_flag,
                                     bool the_const_flag)
    : is_virtual_(the_virtual_flag), is_const_(the_const_flag), type_specifier_(the_specifier) {}

inline DeclSpecifier DeclSpecifier::EMPTY () {
    return DeclSpecifier();
}

inline DeclSpecifier DeclSpecifier::TYPE (const md::NestedNameSpecifier& the_type) {
    return DeclSpecifier(the_type);
}

inline DeclSpecifier DeclSpecifier::VIRTUAL () {
    return DeclSpecifier(md::NestedNameSpecifier(), true);
}

inline DeclSpecifier DeclSpecifier::TYPEDEF () {
    return EMPTY();
}

inline DeclSpecifier DeclSpecifier::FRIEND () {
    return EMPTY();
}

inline DeclSpecifier DeclSpecifier::CONST () {
    return DeclSpecifier(md::NestedNameSpecifier(), false, true);
}

inline md::NestedNameSpecifier DeclSpecifier::type_specifier () const {
    return type_specifier_;
}

inline md::Ptr<md::Type> DeclSpecifier::type () const {
    return md::Type::Create(type_specifier_.ToString(), is_const_);
}

inline bool DeclSpecifier::is_virtual () const {
    return is_virtual_;
}

inline bool DeclSpecifier::is_const () const {
    return is_const_;
}

inline DeclSpecifier DeclSpecifier::Join (const DeclSpecifier& lhs, const DeclSpecifier& rhs) {
    if (lhs.is_virtual() && rhs.is_virtual())
        throw md::SemanticError("Double virtual specification.", __FILE__, __LINE__);
    bool virtual_flag = lhs.is_virtual() || rhs.is_virtual();

    if (lhs.is_const() && rhs.is_const())
        throw md::SemanticError("Double const type specification.", __FILE__, __LINE__);
    bool const_flag = lhs.is_const() || rhs.is_const();

    bool lhs_typed = !lhs.type_specifier().ToString().empty(),
         rhs_typed = !rhs.type_specifier().ToString().empty();
    if (lhs_typed && rhs_typed)
        throw md::SemanticError("Double type specification.", __FILE__, __LINE__);
    md::NestedNameSpecifier the_type;
    if (lhs_typed)
      the_type = lhs.type_specifier();
    else
      the_type = rhs.type_specifier();
  
    return DeclSpecifier(the_type, virtual_flag, const_flag);
}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLSPECIFIER_H_

