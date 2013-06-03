
#ifndef OPWIG_MD_FUNCTION_H_
#define OPWIG_MD_FUNCTION_H_

#include <opwig/md/metadataobject.h>
#include <opwig/md/ptr.h>
#include <opwig/md/parameter.h>

#include <vector>
#include <string>

namespace opwig {
namespace md {

/// Metadata interface for C++ functions.
class Function : public MetadataObject {
  
  public:

    /// Virtual festructor.
    virtual ~Function() {}

    /// Creates a new Function object. Must be used in place of the constructor.
    static Ptr<Function> Create(const std::string& the_name, const std::string& the_return_type,
                                const ParameterList& the_parameter_list, const bool pure);

    /// Tells the function's return type.
    std::string return_type() const;

    /// Tells the function's nth parameter type.
    std::string parameter_type(size_t n) const;

    /// Tells the function's nth parameter name.
    std::string parameter_name(size_t n) const;

    /// Tells if the function is pure (normally used for abstract methods)
    bool is_pure() const;
    
    /// Tells if the function was defined as default
    bool is_default() const { return default_; }
    
    /// Sets the function to be defined as default
    void set_default(bool _default) { default_ = _default; }
    
    /// Tells if the function was defined as delete
    bool is_delete() const { return delete_; }
    
    /// Sets the function to be defined as default
    void set_delete(bool _delete) { delete_ = _delete; }
    
    /// Tells if the function was declared
    bool is_declared() const { return declared_; }
    
    /// Sets the function as declared
    void set_declared() { declared_ = true; }
    
    /// Tells if the function was defined
    bool is_defined() const { return defined_; }
    
    /// Sets the function as defined
    void set_defined() { defined_ = true; }

  private:

    std::string   return_type_;
    ParameterList parameter_list_;
    bool          pure_;
    bool          default_;
    bool          delete_;
    bool          declared_;
    bool          defined_;

    Function(const std::string& the_name, const std::string& the_return_type,
             const ParameterList& the_parameter_list, const bool pure);

};

inline Function::Function(const std::string& the_name, const std::string& the_return_type,
                          const ParameterList& the_parameter_list, const bool pure)
    : MetadataObject(the_name), return_type_(the_return_type), parameter_list_(the_parameter_list), pure_(pure) { }

inline Ptr<Function> Function::Create(const std::string& the_name,
                                      const std::string& the_return_type,
                                      const ParameterList& the_parameter_list,
                                      const bool pure) {
    return Ptr<Function>(new Function(the_name, the_return_type, the_parameter_list, pure));
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

inline bool Function::is_pure() const {
    return pure_;
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_FUNCTION_H_

