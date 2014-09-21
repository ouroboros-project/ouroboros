
#ifndef OPWIG_MD_BASESPECIFIER_H_
#define OPWIG_MD_BASESPECIFIER_H_

#include <opwig/md/accessspecifier.h>
#include <opwig/md/ptr.h>

namespace opwig {
namespace md {

class Class;

///Auxiliar class for passing base_specifier informating thoughout code parsing.
class BaseSpecifier final {
public:
    BaseSpecifier(const std::string& base, bool isVirtual, md::AccessSpecifier base_access) :
        base_(base), virtual_(isVirtual), access_(base_access) {}

    std::string base() const { return base_; }
    bool is_virtual() const { return virtual_; }
    md::AccessSpecifier access_specifier() const { return access_; }
    
private:
    std::string base_;
    bool virtual_;
    md::AccessSpecifier access_;
};

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_BASESPECIFIER_H_

