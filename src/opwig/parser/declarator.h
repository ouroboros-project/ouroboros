
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

    explicit Declarator (const md::NestedNameSpecifier& the_nested_name = md::NestedNameSpecifier(""));

    /// Returns nested_name().ToString().
    std::string name () const;
    
    /// Gets the NestedNameSpecifier associated with this declarator.
    md::NestedNameSpecifier nested_name () const;

    void set_nested_name (const md::NestedNameSpecifier& the_nested_name);

    bool has_parameters () const;

    const md::ParameterList& parameters () const;

    void set_parameters (const md::ParameterList& the_parameters);

    void set_parameters (md::Ptr<md::ParameterList> the_parameters);

    std::string pointer_type () const;

    void set_pointer_type (const std::string& the_pointer_type);
    
    bool is_pure () const;
    
    void set_pure (bool pure);

  private:

    md::NestedNameSpecifier     nested_name_;
    md::Ptr<md::ParameterList>  parameters_;
    std::string                 pointer_type_;
    bool pure_;
};

inline Declarator::Declarator (const md::NestedNameSpecifier& the_nested_name)
    : nested_name_(the_nested_name), pointer_type_(""), pure_(false) {}

inline std::string Declarator::name () const {
    return nested_name_.ToString();
}

inline md::NestedNameSpecifier Declarator::nested_name () const {
    return nested_name_;
}

inline void Declarator::set_nested_name (const md::NestedNameSpecifier& the_nested_name) {
    nested_name_ = the_nested_name;
}

inline bool Declarator::has_parameters () const {
    return static_cast<bool>(parameters_);
}

inline const md::ParameterList& Declarator::parameters () const {
    return *parameters_;
}

inline void Declarator::set_parameters (const md::ParameterList& the_parameters) {
    parameters_.reset(new md::ParameterList(the_parameters));
}

inline void Declarator::set_parameters (md::Ptr<md::ParameterList> the_parameters) {
  parameters_ = the_parameters;
}

inline std::string Declarator::pointer_type () const {
    return pointer_type_;
}

inline void Declarator::set_pointer_type (const std::string& the_pointer_type) {
    pointer_type_ = the_pointer_type;
}

inline bool Declarator::is_pure () const {
    return pure_;
}

inline void Declarator::set_pure (bool pure) {
    pure_ = pure;
}

} // namespace parser
} // namespace opwig

#endif // OPWIG_DECLARATOR_H_

