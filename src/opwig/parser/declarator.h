
#ifndef OPWIG_DECLARATOR_H_
#define OPWIG_DECLARATOR_H_

#include <opwig/md/ptr.h>
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

    void set_parameters(md::Ptr<md::ParameterList> the_parameters);
    
    bool is_pure() const;
    
    void set_pure(bool pure);

  private:

    std::string                 name_;
    md::Ptr<md::ParameterList>  parameters_;
    bool pure_;
};

inline Declarator::Declarator(const std::string& the_name)
    : name_(the_name), pure_(false) {}

inline std::string Declarator::name() const {
    return name_;
}

inline void Declarator::set_name(const std::string& the_name) {
    name_ = the_name;
}

inline bool Declarator::has_parameters() const {
    return static_cast<bool>(parameters_);
}

inline const md::ParameterList& Declarator::parameters() const {
    return *parameters_;
}

inline void Declarator::set_parameters(const md::ParameterList& the_parameters) {
    parameters_.reset(new md::ParameterList(the_parameters));
}

inline void Declarator::set_parameters(md::Ptr<md::ParameterList> the_parameters) {
  parameters_ = the_parameters;
}

inline bool Declarator::is_pure() const {
    return pure_;
}

inline void Declarator::set_pure(bool pure) {
    pure_ = pure;
}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLARATOR_H_

