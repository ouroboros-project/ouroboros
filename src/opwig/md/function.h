
#ifndef OPWIG_MD_FUNCTION_H_
#define OPWIG_MD_FUNCTION_H_

#include <opwig/md/ptr.h>

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
    static Ptr<Function> Create(const std::string& name, const std::string& return_type,
                                const std::vector<std::string>& arg_type_list);

    /// Tells the function's name.
    std::string name() const;

    /// Tells the function's return type.
    std::string return_type() const;

    /// Tells the function's nth argument type.
    std::string arg_type(size_t n) const;

  private:

    std::string               name_;
    std::string               return_type_;
    std::vector<std::string>  arg_type_list_;

    Function(const std::string& name, const std::string& return_type,
             const std::vector<std::string>& arg_type_list);

};

Function::Function(const std::string& name, const std::string& return_type,
                   const std::vector<std::string>& arg_type_list)
  : name_(name), return_type_(return_type), arg_type_list_(arg_type_list) {}

Ptr<Function> Function::Create(const std::string& name, const std::string& return_type,
                               const std::vector<std::string>& arg_type_list) {
  return Ptr<Function>(new Function(name, return_type, arg_type_list));
}

std::string Function::name() const {
  return name_;
}

std::string Function::return_type() const {
  return return_type_;
}

std::string Function::arg_type(size_t n) const {
  return arg_type_list_[n];
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_FUNCTION_H_

