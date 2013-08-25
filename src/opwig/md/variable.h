
#ifndef OPWIG_MD_VARIABLE_H_
#define OPWIG_MD_VARIABLE_H_

#include <opwig/md/metadataobject.h>
#include <opwig/md/ptr.h>
#include <opwig/md/type.h>

#include <string>

namespace opwig {
namespace md {

/// Metadata class for C++ variables.
class Variable : public MetadataObject {

  public:
    /// Creates a new Variable object. Must be used in place of the
    /// constructor.
    static Ptr<Variable> Create (const std::string& var_name, const Ptr<const Type>& var_type);

    Ptr<const Type> type () const { return type_; }

    /// Tells if the variable is static.
    /** @return bool Whether the variable is static
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

