
#ifndef OPWIG_MD_FUNCTION_H_
#define OPWIG_MD_FUNCTION_H_

#include <opwig/md/ptr.h>
#include <opwig/md/parameter.h>

#include <vector>
#include <string>

namespace opwig {
namespace md {

/// Metadata interface for C++ functions.
class Function {
  
  public:

    /// Virtual festructor.
    virtual ~Function() {}

    /// Creates a new Function object. Must be used in place of the constructor.
    static Ptr<Function> Create(const std::string& the_name, const std::string& the_return_type,
                                const ParameterList& the_parameter_list);

    /// Tells the function's name.
    std::string name() const;

    /// Tells the function's return type.
    std::string return_type() const;

    /// Tells the function's nth parameter type.
    std::string parameter_type(size_t n) const;

    /// Tells the function's nth parameter name.
    std::string parameter_name(size_t n) const;

  private:

    std::string   name_;
    std::string   return_type_;
    ParameterList parameter_list_;

    Function(const std::string& the_name, const std::string& the_return_type,
             const ParameterList& the_parameter_list);

};

inline Function::Function(const std::string& the_name, const std::string& the_return_type,
                          const ParameterList& the_parameter_list)
    : name_(the_name), return_type_(the_return_type), parameter_list_(the_parameter_list) {}

inline Ptr<Function> Function::Create(const std::string& the_name,
                                      const std::string& the_return_type,
                                      const ParameterList& the_parameter_list) {
    return Ptr<Function>(new Function(the_name, the_return_type, the_parameter_list));
}

inline std::string Function::name() const {
    return name_;
}

inline std::string Function::return_type() const {
    return return_type_;
}

inline std::string Function::parameter_type(size_t n) const {
    return parameter_list_.at(n).type; // throw handle exception?
}

inline std::string Function::parameter_name(size_t n) const {
    return parameter_list_.at(n).name; // throw handle exception?
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_FUNCTION_H_

