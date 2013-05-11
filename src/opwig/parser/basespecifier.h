
#ifndef OPWIG_MDPARSER_BASESPECIFIER_H_
#define OPWIG_MDPARSER_BASESPECIFIER_H_

#include <opwig/md/accessspecifier.h>

namespace opwig {
namespace parser {

///Auxiliar class for passing base_specifier informating thoughout code parsing.
class BaseSpecifier final {
public:
    BaseSpecifier(const std::string& base_name="") : name_(base_name), virtual_(false), access_(md::AccessSpecifier::PRIVATE) {}
    BaseSpecifier(const std::string& base_name, bool isVirtual) : name_(base_name), virtual_(isVirtual), access_(md::AccessSpecifier::PRIVATE) {}
    BaseSpecifier(const std::string& base_name, md::AccessSpecifier base_access) : name_(base_name), virtual_(false), access_(base_access) {}
    BaseSpecifier(const std::string& base_name, bool isVirtual, md::AccessSpecifier base_access) : name_(base_name), virtual_(isVirtual), access_(base_access) {}

    const std::string& name() const { return name_; } 
    bool is_virtual() const { return virtual_; }
    md::AccessSpecifier access_specifier() const { return access_; }
    
private:
    std::string name_;
    bool virtual_;
    md::AccessSpecifier access_;
};

} // namespace parser
} // namespace opwig

#endif // OPWIG_MDPARSER_BASESPECIFIER_H_

