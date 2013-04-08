
#ifndef OPWIG_MDPARSER_BASESPECIFIER_H_
#define OPWIG_MDPARSER_BASESPECIFIER_H_

namespace opwig {

namespace parser {

enum class AccessSpecifier { PRIVATE, PROTECTED, PUBLIC };

class BaseSpecifier {
public:
    BaseSpecifier(const std::string& name="") : name_(name), virtual_(false), access_(AccessSpecifier::PRIVATE) {}
    BaseSpecifier(const std::string& name, bool isVirtual) : name_(name), virtual_(isVirtual), access_(AccessSpecifier::PRIVATE) {}
    BaseSpecifier(const std::string& name, AccessSpecifier access) : name_(name), virtual_(false), access_(access) {}
    BaseSpecifier(const std::string& name, bool isVirtual, AccessSpecifier access) : name_(name), virtual_(isVirtual), access_(access) {}

    const std::string& name() const { return name_; } 
    bool is_virtual() const { return virtual_; }
    AccessSpecifier access_specifier() const { return access_; }
    
private:
    std::string name_;
    bool virtual_;
    AccessSpecifier access_;
};

} // namespace parser
} // namespace opwig

#endif // OPWIG_MDPARSER_BASESPECIFIER_H_

