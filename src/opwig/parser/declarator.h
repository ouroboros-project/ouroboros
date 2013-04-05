
#ifndef OPWIG_DECLARATOR_H_
#define OPWIG_DECLARATOR_H_

#include <string>

namespace opwig {
namespace parser {

class Declarator final {

  public:

    Declarator(const std::string& the_name);

    std::string name() const;

    void set_name(const std::string& the_name);

  private:

    std::string name_;

};

inline Declarator::Declarator(const std::string& the_name)
    : name_(the_name) {}

inline std::string Declarator::name() const {
    return name_;
}

inline void Declarator::set_name(const std::string& the_name) {
    name_ = the_name;
}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLARATOR_H_

