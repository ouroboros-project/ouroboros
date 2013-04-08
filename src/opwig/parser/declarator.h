
#ifndef OPWIG_DECLARATOR_H_
#define OPWIG_DECLARATOR_H_

#include <opwig/md/parameter.h>

#include <string>

namespace opwig {
namespace parser {

/// Auxiliar class for keeping declarator information throughout code parsing.
class Declarator final {

  public:

    explicit Declarator(const std::string& the_name = "");

    std::string name() const;

    void set_name(const std::string& the_name);

    bool has_parameters() const;

    const md::ParameterList& parameters() const;

    void set_parameters(const md::ParameterList& the_parameters);

  private:

    std::string       name_;
    md::ParameterList parameters_;

};

inline Declarator::Declarator(const std::string& the_name)
    : name_(the_name) {}

inline std::string Declarator::name() const {
    return name_;
}

inline void Declarator::set_name(const std::string& the_name) {
    name_ = the_name;
}

inline bool Declarator::has_parameters() const {
    return !parameters_.empty();
}

inline const md::ParameterList& Declarator::parameters() const {
    return parameters_;
}

inline void Declarator::set_parameters(const md::ParameterList& the_parameters) {
    parameters_ = the_parameters;
}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLARATOR_H_

