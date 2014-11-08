
#ifndef OPWIG_MD_FUNCTION_H_
#define OPWIG_MD_FUNCTION_H_

#include <opwig/md/metadataobject.h>
#include <opwig/md/ptr.h>
#include <opwig/md/type.h>
#include <opwig/md/parameter.h>

#include <vector>
#include <string>

namespace opwig {
namespace md {

/// Metadata class for C++ functions.
class Function : public MetadataObject {
  
  public:

    /// Virtual festructor.
    virtual ~Function () {}

    /// Creates a new Function instance.
    /** Creates a new Function instance with given attributes, and returns a smart pointer to it.
    * @param the_name Name of the function.
    * @param the_return_type Smart pointer to the return type of the function.
    * @param the_parameter_list List of Parameters (@see opwig::md::Parameter) of the function.
    * @param the_is_pure_flag Indicates if the function was declared with a pure specifier ('= 0') or not.
    * @param the_virtual_flag Indicates if the function was declared virtual or not.
    * @return A smart pointer containing a new Function instance.
    */
    static Ptr<Function> Create (const std::string& the_name, const Ptr<const Type>& the_return_type,
                                 const ParameterList& the_parameter_list, const bool the_is_pure_flag,
                                 const bool the_virtual_flag);

    /// Gets the signature for a function.
    /** Gets the signature (without return type) for a function, according to its given attributes.
    * @param the_name Name of the function.
    * @param the_parameter_list List of Parameters (@see opwig::md::Parameter) of the function.
    * @param is_const If the method is const.
    * @return The signature without return type of the function.
    */
    static std::string GetSignatureFor (const std::string& the_name, const ParameterList& the_parameter_list, bool is_const = false);

    /// Gets the function ID.
    /** opwig::md::MetadataObject::id overload - a Function ID is not its name,
        rather, its the function's signature. 
    * @see opwig::md::MetadataObject::id
    */
    virtual const std::string& id () const;

    /// Tells the function's return type.
    /** Gets the return type of the function.
    * @return A smart pointer to the return type.
    */
    Ptr<const Type> return_type () const;

    /// Tells the function's nth parameter type.
    /** Gets the type of the n-th parameter of the function.
    * @param n The index of the parameter to check.
    * @return The type of the parameter, if any, at index n.
    */
    Ptr<const Type> parameter_type (size_t n) const;

    /// Tells the function's nth parameter name.
    /** Gets the name of the n-th parameter of the function.
    * @param n The index of the parameter to check.
    * @return The name of the parameter, if any, at index n.
    */
    std::string parameter_name (size_t n) const;
    
    /// Tells the number of parameters of this function.
    /** Gets the number of parameters of this function.
    * @return Number of parameters of this function.
    */
    size_t num_parameters () const { return parameter_list_.size(); }

    /// Tells if the function is pure (normally used for abstract methods).
    bool is_pure () const;

    /// Tells if the function is virtual.
    bool is_virtual () const;

    /// Tells if the function is static.
    /** @return bool Whether the function is static
     */
    bool is_static () const { return is_static_; }
    
    /// Tells if the function was defined as default
    bool is_default () const { return is_default_; }
    
    /// Sets the function to be defined as default
    void set_default (bool _default) { is_default_ = _default; }
    
    /// Tells if the function was defined as delete
    bool is_deleted () const { return is_deleted_; }
    
    /// Sets the function to be defined as default
    void set_deleted (bool _delete) { is_deleted_ = _delete; }
    
    /// Tells if the function was declared
    bool is_declared () const { return is_declared_; }
    
    /// Sets the function as declared
    void set_declared () { is_declared_ = true; }
    
    /// Tells if the function was defined
    bool is_defined () const { return is_defined_; }
    
    /// Sets the function as defined
    void set_defined () { is_defined_ = true; }
    
    /// Tells if the function is a const method
    bool is_const () const { return is_const_; }
    
    /// Sets if the method is const
    void set_const (bool _const) { is_const_ = _const; }

  private:
    std::string   signature_;
    Ptr<const Type>    return_type_;
    ParameterList parameter_list_;
    bool          is_pure_;
    bool          is_virtual_;
    bool          is_static_;
    bool          is_default_;
    bool          is_deleted_;
    bool          is_declared_;
    bool          is_defined_;
    bool          is_const_;

    Function (const std::string& the_name, const Ptr<const Type>& the_return_type,
              const ParameterList& the_parameter_list, const bool the_pure_flag,
              const bool the_virtual_flag);

};

inline Function::Function (const std::string& the_name, const Ptr<const Type>& the_return_type,
                           const ParameterList& the_parameter_list, const bool the_pure_flag,
                           const bool the_virtual_flag)
    : MetadataObject(the_name), signature_(""), return_type_(the_return_type),
      parameter_list_(the_parameter_list), is_pure_(the_pure_flag), is_virtual_(the_virtual_flag),
      is_static_(false), is_default_(false), is_deleted_(false), is_declared_(false),
      is_defined_(false) {

    signature_ = Function::GetSignatureFor(name_, parameter_list_); // FIXME why no init list?
}

inline Ptr<Function> Function::Create (const std::string& the_name,
                                       const Ptr<const Type>& the_return_type,
                                       const ParameterList& the_parameter_list,
                                       const bool the_pure_flag = false,
                                       const bool the_virtual_flag = false) {
    return Ptr<Function>(
        new Function(the_name, the_return_type, the_parameter_list, the_pure_flag, the_virtual_flag)
    );
}

inline std::string Function::GetSignatureFor (const std::string& the_name,
                                              const ParameterList& the_parameter_list,
                                              bool is_const) {
    std::string signature = the_name + "(";
    if (the_parameter_list.size() > 0) {
        for (auto param : the_parameter_list)
            signature += param.type->full_type() + ",";
        signature.pop_back();
    }
    signature += ")";
    if (is_const)
        signature += "const";
    return signature;
}

inline const std::string& Function::id () const {
    return signature_;
}

inline Ptr<const Type> Function::return_type () const {
    return return_type_;
}

inline Ptr<const Type> Function::parameter_type (size_t n) const {
    return parameter_list_.at(n).type; // throw handle exception?
}

inline std::string Function::parameter_name (size_t n) const {
    return parameter_list_.at(n).name; // throw handle exception?
}

inline bool Function::is_pure () const {
    return is_pure_;
}

inline bool Function::is_virtual () const {
    return is_virtual_;
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_FUNCTION_H_

