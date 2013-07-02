
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
    explicit DeclSpecifier (const md::NestedNameSpecifier& the_nested_name,
                            bool is_virtual);
};

inline DeclSpecifier::DeclSpecifier (const md::NestedNameSpecifier& the_nested_name,
                                     bool is_virtual) {}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLSPECIFIER_H_

