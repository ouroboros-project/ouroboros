
#ifndef OPWIG_MDPARSER_BASESPECIFIER_H_
#define OPWIG_MDPARSER_BASESPECIFIER_H_

#include <opwig/md/accessspecifier.h>

namespace opwig {
namespace parser {

///Auxiliar class for passing base_specifier informating thoughout code parsing.
class BaseSpecifier final {
public:
    BaseSpecifier(const std::string& name="") : name_(name), virtual_(false), access_(md::AccessSpecifier::PRIVATE) {}
    BaseSpecifier(const std::string& name, bool isVirtual) : name_(name), virtual_(isVirtual), access_(md::AccessSpecifier::PRIVATE) {}
    BaseSpecifier(const std::string& name, md::AccessSpecifier access) : name_(name), virtual_(false), access_(access) {}
    BaseSpecifier(const std::string& name, bool isVirtual, md::AccessSpecifier access) : name_(name), virtual_(isVirtual), access_(access) {}

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

