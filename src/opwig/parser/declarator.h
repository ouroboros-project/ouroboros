
#ifndef OPWIG_DECLARATOR_H_
#define OPWIG_DECLARATOR_H_

#include <opwig/md/ptr.h>
#include <opwig/md/parameter.h>
#include <opwig/md/nestednamespecifier.h>

#include <string>

namespace opwig {
namespace parser {

/// Auxiliar class for keeping declarator information throughout code parsing.
class Declarator final {

  public:

    explicit Declarator(const md::NestedNameSpecifier& the_nested_name = md::NestedNameSpecifier(""));

    md::NestedNameSpecifier nested_name() const;

    void set_nested_name(const md::NestedNameSpecifier& the_nested_name);

    bool has_parameters() const;

    const md::ParameterList& parameters() const;

    void set_parameters(const md::ParameterList& the_parameters);

    void set_parameters(md::Ptr<md::ParameterList> the_parameters);
    
    bool is_pure() const;
    
    void set_pure(bool pure);

  private:

    md::NestedNameSpecifier     nested_name_;
    md::Ptr<md::ParameterList>  parameters_;
    bool pure_;
};

inline Declarator::Declarator(const md::NestedNameSpecifier& the_nested_name)
    : nested_name_(the_nested_name), pure_(false) {}

inline md::NestedNameSpecifier Declarator::nested_name() const {
    return nested_name_;
}

inline void Declarator::set_nested_name(const md::NestedNameSpecifier& the_nested_name) {
    nested_name_ = the_nested_name;
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

