
#ifndef OPWIG_MD_VARIABLE_H_
#define OPWIG_MD_VARIABLE_H_

#include <opwig/md/metadataobject.h>
#include <opwig/md/ptr.h>
#include <opwig/md/type.h>

#include <string>

namespace opwig {
namespace md {

/// Metadata class for C++ variables.
/** Represents a C++ Variable, containing its metadata values. */
class Variable : public MetadataObject {

  public:
    /// Creates a new Variable instance.
    /** Creates a new Variable instance with given attributes, and returns a smart pointer to it.
    * @param var_name The name of the variable.
    * @param var_type The type of the variable.
    * @return A smart pointer containing a new Variable instance.
    */
    static Ptr<Variable> Create (const std::string& var_name, const Ptr<const Type>& var_type);

    /// Gets the type of the variable.
    /** Gets the type of the variable.
    * @return A smart pointer to the type of the variable.
    */
    Ptr<const Type> type () const { return type_; }

    /// Tells if the variable is static.
    /** @return bool Whether the variable is static (true) or not.
     */
    bool is_static () const { return is_static_; }

  private:

    Ptr<const Type> type_;
    bool            is_static_;

    Variable (const std::string& var_name, const Ptr<const Type>& var_type)
        : MetadataObject(var_name), type_(var_type), is_static_(false) {}

};

inline Ptr<Variable> Variable::Create (const std::string& var_name, const Ptr<const Type>& var_type) {
  return Ptr<Variable>(new Variable(var_name, var_type) );
}


} // namespace md
} // namespace opwig

#endif // OPWIG_MD_VARIABLE_H_

