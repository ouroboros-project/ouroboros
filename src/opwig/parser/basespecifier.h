
#ifndef OPWIG_MDPARSER_BASESPECIFIER_H_
#define OPWIG_MDPARSER_BASESPECIFIER_H_

#include <opwig/md/accessspecifier.h>
#include <opwig/md/nestednamespecifier.h>

namespace opwig {
namespace parser {

///Auxiliar class for passing base_specifier informating thoughout code parsing.
class BaseSpecifier final {
public:
    BaseSpecifier(const md::NestedNameSpecifier& nested_base_name = md::NestedNameSpecifier("")) : 
        nested_name_(nested_base_name), virtual_(false), access_(md::AccessSpecifier::PRIVATE) {}
    BaseSpecifier(const md::NestedNameSpecifier& nested_base_name, bool isVirtual) : 
        nested_name_(nested_base_name), virtual_(isVirtual), access_(md::AccessSpecifier::PRIVATE) {}
    BaseSpecifier(const md::NestedNameSpecifier& nested_base_name, md::AccessSpecifier base_access) : 
        nested_name_(nested_base_name), virtual_(false), access_(base_access) {}
    BaseSpecifier(const md::NestedNameSpecifier& nested_base_name, bool isVirtual, md::AccessSpecifier base_access) : 
        nested_name_(nested_base_name), virtual_(isVirtual), access_(base_access) {}

    const md::NestedNameSpecifier& nested_name() const { return nested_name_; } 
    bool is_virtual() const { return virtual_; }
    md::AccessSpecifier access_specifier() const { return access_; }
    
private:
    md::NestedNameSpecifier nested_name_;
    bool virtual_;
    md::AccessSpecifier access_;
};

} // namespace parser
} // namespace opwig

#endif // OPWIG_MDPARSER_BASESPECIFIER_H_

